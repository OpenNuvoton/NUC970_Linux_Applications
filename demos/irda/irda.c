/*********************************************************************
 *                
 * Filename:      irda.c
 *
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/utsname.h>
#include <linux/types.h>
#include <linux/irda.h>

#ifndef N_IRDA
#define N_IRDA 11 
#endif /* N_IRDA */

#define Select_IrDA_Tx 1

static int devfd = -1;		/* File descriptor for the tty device */
static int fdflags = -1;	/* Current file descriptor flags */
static int initfdflags = -1;	/* Initial file descriptor flags */

char *device = "/dev/ttyS1";

static struct termios newtios,oldtios;
static int saved_portfd=-1;            /*serial port fd */

/*cheanup signal handler */
static void reset_tty_handler(int signal)
{
	if(saved_portfd != -1)
	{
		tcsetattr(saved_portfd,TCSANOW,&oldtios);
	}
	_exit(EXIT_FAILURE);
}

static void reset_tty_atexit(void)
{
	if(saved_portfd != -1)
	{
		tcsetattr(saved_portfd,TCSANOW,&oldtios);
	} 
}

static int set_tty(int ttyfd)
{
	struct sigaction sa;

	/*get serial port parnms,save away */
	tcgetattr(ttyfd,&newtios);
	memcpy(&oldtios,&newtios,sizeof newtios);
	/* configure new values */
	cfmakeraw(&newtios); /*see man page */
	//newtios.c_iflag |=IGNPAR; /*ignore parity on input */
	newtios.c_iflag =IGNBRK |IGNPAR;
	//newtios.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL); 
	newtios.c_oflag = 0;
	newtios.c_cflag = CS8 | CLOCAL | CREAD;
	newtios.c_cc[VMIN]=1; /* block until 1 char received */
	newtios.c_cc[VTIME]=0; /*no inter-character timer */
	/* 115200 bps */
	cfsetospeed(&newtios,B115200);
	cfsetispeed(&newtios,B115200);
	/* register cleanup stuff */
	atexit(reset_tty_atexit);
	memset(&sa,0,sizeof sa);
	sa.sa_handler = reset_tty_handler;
	sigaction(SIGHUP,&sa,NULL);
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGPIPE,&sa,NULL);
	sigaction(SIGTERM,&sa,NULL);
	/*apply modified termios */
	saved_portfd=ttyfd;
	tcflush(ttyfd,TCIFLUSH);
	tcsetattr(ttyfd,TCSADRAIN,&newtios);

	if ((fdflags = fcntl(ttyfd, F_GETFL)) == -1) {
		close(ttyfd);
		exit(-1);
	}
	initfdflags = fdflags;


	return 1;
}

static int open_tty(char *dev) 
{
	int ttyfd;
	struct sigaction sa;

	ttyfd = open(dev,O_RDWR | O_NOCTTY);

	if (ttyfd < 0) {
		fprintf(stderr, "Failed to open device %s: %s\n",
			dev, strerror(errno));
		exit(-1);
	}

	set_tty(ttyfd);

	return(ttyfd);
}


int main() 
{
	int 	i;
	char 	buffer[20], buffer2[21];
	int 	rev1, rev2;
	int 	irdadisc = N_IRDA;

	devfd = open_tty(device);

	ioctl(devfd, TIOCSETD, &irdadisc);

	for (i = 0; i < 20; i++) {
		buffer[i] = 'A'+i;
	}

	#ifdef Select_IrDA_Tx
	rev1 =0;
	rev2 =0;

	while(rev2 < 20)
   	{
		rev1 = write(devfd,(buffer+rev2),20);

		if(rev1 > 0)
		{		
			rev2 += rev1;
		}
    }

	#else

	rev1 = 0;
	rev2 = 0;

	while(rev2 < 20)
	{
		rev1 = read(devfd,(buffer2+rev2),20);

		if(rev1 > 0)
		{	
			rev2 += rev1;
		}
	}


	for(i=0; i<20; i++)
	{
		printf("\n buffer2[%d]= 0x%x", i, buffer2[i]);
	}

	#endif

	while (1)
		pause();

	return 0;
}
