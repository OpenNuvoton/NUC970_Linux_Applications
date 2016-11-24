/*
 * Copyright (c) 2014 Nuvoton technology corporation
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
 
#include     <stdio.h>
#include     <stdlib.h>
#include     <unistd.h> 
#include     <sys/types.h>
#include     <sys/stat.h> 
#include     <fcntl.h> 
#include     <termios.h>  
#include     <errno.h>
#include     <string.h>
#include 	<signal.h>
#include    <pthread.h>
#include <linux/serial.h>



#define FALSE 0
#define TRUE  1

/* Driver-specific ioctls: ...\linux-3.10.x\include\uapi\asm-generic\ioctls.h */
#define TIOCGRS485      0x542E
#define TIOCSRS485      0x542F

int fd[2];

pthread_t threads[10];

char buff[256];

static struct termios newtios,oldtios; /*termianal settings */
static int saved_portfd=-1;            /*serial port fd */


/* Test GCC version, this structure is consistent in GCC 4.8, thus no need to overwrite */
#if (__GNUC__ == 4 && __GNUC_MINOR__ == 3)

struct my_serial_rs485
{
	unsigned long	flags;			/* RS485 feature flags */
#define SER_RS485_ENABLED		(1 << 0)	/* If enabled */
#define SER_RS485_RTS_ON_SEND		(1 << 1)	/* Logical level for
							   RTS pin when
							   sending */
#define SER_RS485_RTS_AFTER_SEND	(1 << 2)	/* Logical level for
							   RTS pin after sent*/
#define SER_RS485_RX_DURING_TX		(1 << 4)
	unsigned long	delay_rts_before_send;	/* Delay before send (milliseconds) */
	unsigned long	delay_rts_after_send;	/* Delay after send (milliseconds) */
	unsigned long	padding[5];		/* Memory is cheap, new structs
					   are a royal PITA .. */
};

#endif


static void reset_tty_atexit(void)
{
	if(saved_portfd != -1)
	{
		tcsetattr(saved_portfd,TCSANOW,&oldtios);
	} 
}

/*cheanup signal handler */
static void reset_tty_handler(int signal)
{
	if(saved_portfd != -1)
	{
		tcsetattr(saved_portfd,TCSANOW,&oldtios);
	}
	_exit(EXIT_FAILURE);
}

static int open_port(const char *portname)
{
	struct sigaction sa;
	int portfd;
#if (__GNUC__ == 4 && __GNUC_MINOR__ == 3)
	struct my_serial_rs485 rs485conf;
	struct my_serial_rs485 rs485conf_bak;
#else
	struct serial_rs485 rs485conf;
	struct serial_rs485 rs485conf_bak;
#endif	
	//printf("opening serial port:%s\n",portname);
	/*open serial port */
	if((portfd=open(portname,O_RDWR | O_NOCTTY, 0)) < 0 )
	{
   		printf("open serial port %s fail \n ",portname);
   		return portfd;
	}

	printf("opening serial port:%s\n",portname);

	/*get serial port parnms,save away */
	tcgetattr(portfd,&newtios);
	memcpy(&oldtios,&newtios,sizeof newtios);
	/* configure new values */
	cfmakeraw(&newtios); /*see man page */
	newtios.c_iflag |=IGNPAR; /*ignore parity on input */
	newtios.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL); 
	newtios.c_cflag = CS8 | CLOCAL | CREAD;
	newtios.c_cc[VMIN]=1; /* block until 1 char received */
	newtios.c_cc[VTIME]=0; /*no inter-character timer */
	/* 115200 bps */
	cfsetospeed(&newtios,B9600);
	cfsetispeed(&newtios,B9600);
	/* register cleanup stuff */
	atexit(reset_tty_atexit);
	memset(&sa,0,sizeof sa);
	sa.sa_handler = reset_tty_handler;
	sigaction(SIGHUP,&sa,NULL);
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGPIPE,&sa,NULL);
	sigaction(SIGTERM,&sa,NULL);
	/*apply modified termios */
	saved_portfd=portfd;
	tcflush(portfd,TCIFLUSH);
	tcsetattr(portfd,TCSADRAIN,&newtios);
	
		
	if (ioctl (portfd, TIOCGRS485, &rs485conf) < 0) 
	{
		/* Error handling.*/ 
		printf("ioctl TIOCGRS485 error.\n");
	}
	/* Enable RS485 mode: */
	rs485conf.flags |= SER_RS485_ENABLED;

	/* Set logical level for RTS pin equal to 1 when sending: */
	rs485conf.flags |= SER_RS485_RTS_ON_SEND;
	//rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;

	/* set logical level for RTS pin equal to 0 after sending: */ 
	rs485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);
	//rs485conf.flags &= ~(SER_RS485_RTS_ON_SEND);

	/* Set rts delay after send, if needed: */
	rs485conf.delay_rts_after_send = 0x80;

	if (ioctl (portfd, TIOCSRS485, &rs485conf) < 0)
	{
		/* Error handling.*/ 
		printf("ioctl TIOCSRS485 error.\n");
	}

	if (ioctl (portfd, TIOCGRS485, &rs485conf_bak) < 0)
	{
		/* Error handling.*/ 
		printf("ioctl TIOCGRS485 error.\n");
	}
	else
	{
		printf("rs485conf_bak.flags 0x%x.\n", rs485conf_bak.flags);
		printf("rs485conf_bak.delay_rts_before_send 0x%x.\n", rs485conf_bak.delay_rts_before_send);
		printf("rs485conf_bak.delay_rts_after_send 0x%x.\n", rs485conf_bak.delay_rts_after_send);
	}

	return portfd;
}

void * process1(void* arg)
{
	int portfd = (int) arg;
	unsigned int i, j;
	int rev1, rev2;
	char RxBuffer[sizeof(buff)];	

	rev1 =0;
	rev2 =0;

	while(rev2 < sizeof(buff))
   	{
		rev1 = write(portfd,(buff+rev2),sizeof(buff));
		rev2 += rev1;
   	}
	printf("\n uart1 send %d byts\n", rev2);


}	

void * process2(void* arg)
{
	int portfd = (int) arg;
	unsigned int i, j;
	int rev1, rev2;
	char RxBuffer[sizeof(buff)];


	rev1 = 0;
	rev2 = 0;

	while(rev2 < sizeof(RxBuffer))
	{
		rev1 = read(portfd,(RxBuffer+rev2),sizeof(RxBuffer) - rev2);
		rev2 += rev1;

		if(rev1 > 0)
			printf("\n uart2 receive %d bytes \n", rev2);
	}
		
	printf("\n uart2 receive %d bytes \n", rev2);

	for(i = 0; i < sizeof(RxBuffer); i++)
	{
		if(i != RxBuffer[i])
		{
			printf("\n uart2 compare Error!!");
			while(1);
		}
	}

	printf("\n uart2 compare correct!!\n");
	printf("\n uart2 test done!!\n");

}

/**
*@breif 	main()
*/
int main(int argc, char **argv)
{
	char *dev[10]={"/dev/ttyS1", "/dev/ttyS2"};
	unsigned int i;

	printf("\n demo uart1/uart2 external loop back function \n");

	for(i = 0; i < sizeof(buff); i++)
	{
		buff[i] = (i & 0xff);
	}


	for(i = 0; i < 2; i++)
	{
		if((fd[i] = open_port(dev[i]))<0)
   			return -1;
	}
	
	pthread_create(&threads[0], NULL, process1, (void*)(fd[0]));
	pthread_create(&threads[1], NULL, process2, (void*)(fd[1]));

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

		   
  return 0;
}

