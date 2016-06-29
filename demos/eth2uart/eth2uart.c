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
#include     <netinet/in.h>
#include     <sys/select.h>
#include     <sys/socket.h>
#include     <arpa/inet.h>
#include     <unistd.h>
#include     <string.h>
#include     <stdio.h>
#include     <errno.h>
#include     <sys/time.h>
#include     <sys/types.h>
#include     <pthread.h>
#include     <stdarg.h>
#include     <termio.h>
#include     <stdlib.h>
#include     <sys/stat.h>
#include     <fcntl.h>
#include     <signal.h>

#define FALSE 0
#define TRUE  1

#define MAX_PACKAGE 4096

/* the following are legal, implemented events */
#define IN_ACCESS		0x00000001	/* File was accessed */
#define IN_MODIFY		0x00000002	/* File was modified */
#define IN_ATTRIB		0x00000004	/* File changed attributes */
#define IN_CLOSE_WRITE		0x00000008	/* Writtable file was closed */
#define IN_CLOSE_NOWRITE	0x00000010	/* Unwrittable file closed */
#define IN_OPEN			0x00000020	/* File was opened */
#define IN_MOVED_FROM		0x00000040	/* File was moved from X */
#define IN_MOVED_TO		0x00000080	/* File was moved to Y */
#define IN_DELETE_SUBDIR	0x00000100	/* Subdir was deleted */ 
#define IN_DELETE_FILE		0x00000200	/* Subfile was deleted */
#define IN_CREATE_SUBDIR	0x00000400	/* Subdir was created */
#define IN_CREATE_FILE		0x00000800	/* Subfile was created */
#define IN_DELETE_SELF		0x00001000	/* Self was deleted */
#define IN_UNMOUNT		0x00002000	/* Backing fs was unmounted */
#define IN_Q_OVERFLOW		0x00004000	/* Event queued overflowed */
#define IN_IGNORED		0x00008000	/* File was ignored */

/* Driver-specific ioctls: ...\linux-3.10.x\include\uapi\asm-generic\ioctls.h */
#define TIOCGRS485      0x542E
#define TIOCSRS485      0x542F

struct serial_rs485
{
    unsigned long    flags;                  /* RS485 feature flags */
#define SER_RS485_ENABLED        (1 << 0)    /* If enabled */
#define SER_RS485_RTS_ON_SEND    (1 << 1)    /* Logical level for RTS pin when sending */
#define SER_RS485_RTS_AFTER_SEND (1 << 2)    /* Logical level for RTS pin after sent*/
#define SER_RS485_RX_DURING_TX   (1 << 4)
    unsigned long    delay_rts_before_send;  /* Delay before send (milliseconds) */
    unsigned long    delay_rts_after_send;   /* Delay after send (milliseconds) */
    unsigned long    padding[5];             /* Memory is cheap, new structs are a royal PITA .. */
};

struct inotify_event {
    int32_t wd;
    uint32_t mask;
    uint32_t cookie;
    size_t len;
#ifdef __KERNEL__
    char *filename;
#else
    char filename[0];
#endif
};

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

pthread_t threads[10];

char RxBuffer[MAX_PACKAGE];

typedef  enum {ETHERNET,UART,CONNECT_TCP}_PORT_TYPE_e;

#define PORT_BASE 50000

/*UART Table*/
typedef struct {
    uint32_t  baudrate;
    uint32_t  datalen;
    uint32_t  stopbit;
    uint32_t  odd; //parity  0:non-parity  1:odd parity  2:even parity
    uint32_t  enable_autoflow;
    uint32_t  enable_RS485;

}_UART_PARS_s;
_UART_PARS_s serial_conf;


typedef struct tty_info_t
{
    int fd;                 // UART fd
    pthread_mutex_t mt;     // mutex
    char name[24];         // serial name¡A ex¡G"/dev/ttyS0"
    struct termios ntm;    // new serial port
    struct termios otm;   // old serial port
} _TTY_INFO_s;


_TTY_INFO_s *open_tty(uint32_t id);
int set_tty_speed(_TTY_INFO_s *ptty, uint32_t speed);
int set_tty_parity(_TTY_INFO_s *ptty,uint32_t databits,uint32_t parity,uint32_t stopbits, uint32_t u32CTSRTS);

/***************************TCP**************************************************/
struct Links_Objects *tcp_link_head = NULL;
struct Links_Objects *tcp_link_tail = NULL;
static pthread_t tcp_thread_id;
static int tcp_max_fd = 0;
static unsigned char tcp_buf[MAX_PACKAGE];

/************************************************************************************/
_TTY_INFO_s *serial_info;

typedef void(*PFunc)(unsigned short ,uint8_t*,uint32_t,void*);

int fd[10];

/*****************************************************************************************/
struct Objects_Opt{
    int fd;
    _PORT_TYPE_e port_type;//ETHERNET or UART
    unsigned short port;  // tcp ip address port number
    PFunc deal;  //function point
};


struct Links_Objects{
    struct Objects_Opt opt;
    struct Links_Objects *next;
};


int add_link(struct Objects_Opt* obj,int *maxfd,struct Links_Objects **start, struct Links_Objects **last)
{
    struct Links_Objects *obj_to_add;

    obj_to_add = (struct Links_Objects*)malloc(sizeof(struct Links_Objects));
    if(!obj_to_add)
        return 0;

    memset(obj_to_add,0,sizeof(struct Links_Objects));

    (obj_to_add->opt).fd = obj->fd;
    (obj_to_add->opt).port = obj->port;
    (obj_to_add->opt).port_type = obj->port_type;
    (obj_to_add->opt).deal = obj->deal;
    obj_to_add->next = NULL;

    if((obj->fd) > *maxfd)
        *maxfd = obj->fd;

    if (!*start)
    {
       *start = obj_to_add;
       *last = obj_to_add;
       return 1;
    }

    (*last)->next = obj_to_add;
    *last = obj_to_add;

    return 1;
}


void del_link(int fd,struct Links_Objects **start, struct Links_Objects **last)
{
    struct Links_Objects *p = *start;
    struct Links_Objects *p_prev = NULL;

    for(;p != NULL;p = p->next)
    {
        if((p->opt).fd == fd)
        {
            if(p_prev)
                p_prev->next = p->next;
            else
                *start = p->next;

            if(!(p->next))
                *last = p_prev;

            free(p);
            break;
        }
        p_prev = p;
    }

}


_TTY_INFO_s *open_tty(uint32_t id)
{
    static _TTY_INFO_s *ptty;

    ptty = (_TTY_INFO_s *)malloc(sizeof(_TTY_INFO_s));
    if(ptty == NULL)
    {
        return NULL;
    }


    memset(ptty,0,sizeof(_TTY_INFO_s));


    pthread_mutex_init(&ptty->mt,NULL);


    sprintf(ptty->name,"/dev/ttyS%d",id);

    ptty->fd = open(ptty->name, O_RDWR | O_NOCTTY,0);
    if (ptty->fd <0)
    {
        free(ptty);
        printf("open tty%d fail\n",id);
        return NULL;
    }


    fcntl(ptty->fd, F_SETFL, 0);


    tcgetattr(ptty->fd,&ptty->otm);

    return ptty;
}


int set_tty_speed(_TTY_INFO_s *ptty, uint32_t baudrate)
{
    uint32_t speed;

    if( tcgetattr(ptty->fd,&ptty->ntm) != 0)
    {
        printf("SetupSerial [%s] speed\n",ptty->name);
        return 1;
    }
    bzero(&ptty->ntm, sizeof(ptty->ntm));


    switch(baudrate)
    {
        case 300:
            speed= B300;
        break;
        case 1200:
            speed= B1200;
        break;
        case 2400:
            speed= B2400;
        break;
        case 4800:
            speed= B4800;
        break;
        case 9600:
            speed= B9600;
        break;
        case 19200:
            speed= B19200;
        break;
        case 38400:
            speed= B38400;
        break;
        case 115200:
            speed= B115200;
        break;
    }

    cfsetispeed(&ptty->ntm, speed);
    cfsetospeed(&ptty->ntm, speed);

    tcflush(ptty->fd, TCIFLUSH);
    tcsetattr(ptty->fd,TCSANOW,&ptty->ntm);

    return 0;
}


int set_tty_parity(_TTY_INFO_s *ptty,uint32_t databits,uint32_t parity,uint32_t stopbits, uint32_t u32CTSRTS)
{
     if( tcgetattr(ptty->fd,&ptty->ntm) != 0)
    {
        printf("SetupSerial [%s]\n",ptty->name);
        return 1;
    }

    ptty->ntm.c_cflag &= ~CSIZE;
    switch (databits)
    {
        case 7:
            ptty->ntm.c_cflag |= CS7;
        break;
        case 8:
            ptty->ntm.c_cflag |= CS8;
        break;
        default:
            printf("Unsupported data size\n");
        return 5;
    }


    switch (parity)
    {
        case 0: // non-parity
            ptty->ntm.c_cflag &= ~PARENB;
            ptty->ntm.c_iflag &= ~INPCK;
        break;

        case 1: // odd-parity
            ptty->ntm.c_cflag |= (PARODD|PARENB);
            ptty->ntm.c_iflag |= INPCK;
        break;

        case 2: //even-parity
            ptty->ntm.c_cflag |= PARENB;
            ptty->ntm.c_cflag &= ~PARODD;
            ptty->ntm.c_iflag |= INPCK;
        break;


        default:
            printf("Unsupported parity\n");
        return 2;
    }

    // stop-bits
    switch (stopbits)
    {
        case 1:
            ptty->ntm.c_cflag &= ~CSTOPB;
        break;
        case 2:
            ptty->ntm.c_cflag |= CSTOPB;
        break;
        default:
            printf("Unsupported stop bits\n");
        return 3;
    }

    ptty->ntm.c_cflag |= (CLOCAL | CREAD | u32CTSRTS);
    ptty->ntm.c_lflag &=~(ICANON | ECHO | ECHOE | ISIG);
    ptty->ntm.c_oflag &=~OPOST;
    ptty->ntm.c_iflag &=~(IXON | ICRNL);

    ptty->ntm.c_cc[VTIME] =0;
    ptty->ntm.c_cc[VMIN] = 1;
    tcflush(ptty->fd, TCIFLUSH);
    if (tcsetattr(ptty->fd,TCSANOW,&ptty->ntm) != 0)
    {
        printf("SetupSerial \n");
        return 4;
    }

    return 0;
}

int set_tty_rs485(int portfd, uint32_t u32Enable_RS485)
{
    struct serial_rs485 rs485conf;
    struct serial_rs485 rs485conf_bak;

    if (ioctl (portfd, TIOCGRS485, &rs485conf) < 0)
    {
        /* Error handling.*/
        printf("ioctl TIOCGRS485 error.\n");
        return 1;
    }

    if(u32Enable_RS485 == 1)
    {
        /* Enable RS485 mode: */
        rs485conf.flags |= SER_RS485_ENABLED;
    }
    else
    {
        /* Enable RS485 mode: */
        rs485conf.flags = 0;
    }

    /* Set logical level for RTS pin equal to 1 when sending: */
    rs485conf.flags |= SER_RS485_RTS_ON_SEND;

    /* set logical level for RTS pin equal to 0 after sending: */
    rs485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);

    /* Set rts delay after send, if needed: */
    rs485conf.delay_rts_after_send = 0x80;

    if (ioctl (portfd, TIOCSRS485, &rs485conf) < 0)
    {
        /* Error handling.*/
        printf("ioctl TIOCSRS485 error.\n");
        return 1;
    }

    if (ioctl (portfd, TIOCGRS485, &rs485conf_bak) < 0)
    {
        /* Error handling.*/
        printf("ioctl TIOCGRS485 error.\n");
        return 1;
    }

    return 0;
}


int create_socket(char *ip, unsigned short port)
{
    int fd;
    int Client_Sockfd;
    struct sockaddr_in m_tcpsocket_addr;
    struct sockaddr_in m_tcpsocket_addr1;
    struct sockaddr_in Client_addr;
    struct Objects_Opt obj;

    memset(&m_tcpsocket_addr,0,sizeof(m_tcpsocket_addr));

    m_tcpsocket_addr.sin_family = AF_INET;

    if(ip != NULL)
    {
        m_tcpsocket_addr.sin_addr.s_addr = inet_addr(ip);
    }
    else
    {
        m_tcpsocket_addr.sin_addr.s_addr = INADDR_ANY;
    }

    fd = socket(AF_INET,SOCK_STREAM,0);

    if(-1 == fd)
    {
        printf("create tcp socket failed,errno = %d\n",errno);
        return -1;
    }

    m_tcpsocket_addr.sin_port = htons(port);
    if(bind(fd,(struct sockaddr *)&m_tcpsocket_addr,sizeof(m_tcpsocket_addr))==-1)
    {
        printf("Bind1 failed!\n");
        return -1;
    }

    if(listen(fd,10)==-1)
    {
        printf("Listen is failed!\n");
        return -1;
    }

    obj.port = port;
    obj.fd   = fd;
    obj.port_type = ETHERNET;

    if(!add_link(&obj,&tcp_max_fd,&tcp_link_head,&tcp_link_tail))
    {
        printf("\nadd_link occure an error\n");
        while(1);
    }

    return fd;
}

uint32_t set_uart_config_struct(_UART_PARS_s *pUART_Conf, FILE *fp)
{
    char buf_[2048];
    uint32_t Port_Number = 0;
    uint32_t Enable_AutoFlow = 0;
    uint32_t u32TmpBaudrate = 0;
    size_t read_size;
    uint32_t i;
    uint32_t index = 0;

    fseek(fp,0,0);

    read_size = fread(&buf_[0],sizeof(char),sizeof(buf_),fp);
    if(read_size < 0)
    {
        if(!feof(fp))
        {
            printf("\n uart.ini file size too large \n");
            while(1);
        }
    }

    for(i = 0; i < read_size; i++)
    {
      if(buf_[i] == ',')
      {
          index++;
          continue;
      }

      if(index == 0)
      {
          Port_Number = (buf_[i] - 0x30);
      }

      if(index == 1)
      {
          u32TmpBaudrate = (u32TmpBaudrate*10) + (buf_[i]-0x30);
          pUART_Conf->baudrate = u32TmpBaudrate;
      }

      if(index == 2)
      {
          pUART_Conf->datalen = (buf_[i]-0x30);
      }

      if(index == 3)
      {
          pUART_Conf->odd = (buf_[i]-0x30);
      }

      if(index == 4)
      {
          pUART_Conf->stopbit = (buf_[i]-0x30);
      }

      if(index == 5)
      {
         pUART_Conf->enable_autoflow = (buf_[i]-0x30);
      }
      
      if(index == 6)
      {
          pUART_Conf->enable_RS485 = (buf_[i]-0x30);
          break;
      }

    }

    return 0;
}


void process_inotify_events(void)
{
    FILE *fp;
    size_t read_size;
    char buf_[2048];
    uint32_t i, ret;
    uint32_t Port_Number = 0;
    uint32_t Enable_AutoFlow = 0;
    uint32_t index = 0;
    struct Links_Objects *pp;
    _UART_PARS_s UART_Conf;

    UART_Conf.baudrate = 0;
    UART_Conf.datalen = 0;
    UART_Conf.stopbit = 0;
    UART_Conf.odd = 0;  //0: non-parity  //1: odd-parity  //2: even-parity

    //file formate: UART port number, baudrate 115200, data 8 bits, no parity, stop bits 1, no flow control, disable RS485: 1,115200,8,0,1,0,0

    for(i = 0; i < 2048; i++)
    {
      buf_[i] = 0;
    }

    fp=fopen("/mnt/mtdblock0/uart.ini","r");

    fseek(fp,0,0);

    read_size = fread(&buf_[0],sizeof(char),sizeof(buf_),fp);

    if(read_size < 0)
    {
        if(!feof(fp))
        {
            printf("\n uart.ini file size too large \n");
            while(1);
        }
    }

    for(i = 0; i < read_size; i++)
    {

      if(buf_[i] == ',')
      {
          index++;
          continue;
      }

      if(index == 0)
      {
          Port_Number = (buf_[i] - 0x30);
      }

      if(index == 1)
      {
          UART_Conf.baudrate = (UART_Conf.baudrate*10) + (buf_[i]-0x30);
      }

      if(index == 2)
      {
          UART_Conf.datalen = (buf_[i]-0x30);
      }

      if(index == 3)
      {
          UART_Conf.odd = (buf_[i]-0x30);
      }

      if(index == 4)
      {
          UART_Conf.stopbit = (buf_[i]-0x30);
      }

      if(index == 5)
      {
         Enable_AutoFlow = (buf_[i]-0x30);
      }
      
      if(index == 6)
      {
          UART_Conf.enable_RS485 = (buf_[i]-0x30);
          break;
      }

    }
    
    pp =  tcp_link_head;
    for(;pp != NULL;pp = pp->next)
    {
        if((pp->opt).port_type == UART)
        {
            if((pp->opt).port == (Port_Number+PORT_BASE))
            {
                serial_info->fd = (pp->opt).fd;
                break;
            }
        }
    }

    set_tty_speed(serial_info, UART_Conf.baudrate);
    if((ret=set_tty_parity(serial_info,UART_Conf.datalen,UART_Conf.odd,UART_Conf.stopbit, Enable_AutoFlow))==0)
    {
        if((Port_Number == 1) || (Port_Number == 2))
        {
            if(set_tty_rs485(serial_info->fd, UART_Conf.enable_RS485) == 0)
            {
            }
            else
            {
                printf("\n RS485 set fail!!");
            }
        }
    }

    fclose(fp);
}

void *tcp_process (void *arg)
{
    int ret, uart_rev, tcp_rev, Client_Sockfd, eth_connect_fd;
    uint32_t i, r;
    struct Objects_Opt obj;
    struct sockaddr_in Client_addr;
    struct timeval tcp_timeval;
    struct Links_Objects *p;
    socklen_t sin_size;
    fd_set net_fds;
    int wd;
    int file_fd;
    char buffer[BUF_LEN];

    file_fd = inotify_init();

    wd = inotify_add_watch (file_fd, "/mnt/mtdblock0/uart.ini",IN_MODIFY);

    if(file_fd > tcp_max_fd)
    {
        tcp_max_fd = file_fd;
    }


    while(1)
    {
        FD_ZERO(&net_fds);

        p =  tcp_link_head;

        for(;p != NULL;p = p->next)
        {
            if((p->opt).fd > 0)
            {
                 FD_SET((p->opt).fd,&net_fds);
            }
        }

        FD_SET(file_fd, &net_fds);

        tcp_timeval.tv_sec   =  0 ;
        tcp_timeval.tv_usec   = 0;
        ret = select(tcp_max_fd +1,&net_fds,NULL,NULL,&tcp_timeval);

        if (ret < 0) {

         } else if (ret == 0) {

         }
         else if(ret > 0)
         {
              if (FD_ISSET(file_fd, &net_fds))
              {
                  read (file_fd, buffer, BUF_LEN);
                  process_inotify_events();
              }

              p =  tcp_link_head;

              for(;p != NULL;p = p->next)
              {
                  if((p->opt).fd > 0)
                  {
                      if (FD_ISSET((p->opt).fd, &net_fds))
                      {
                          if((p->opt).port_type == ETHERNET)
                          {
                               eth_connect_fd = accept((p->opt).fd, (struct sockaddr *)&Client_addr, &sin_size);

                               if (eth_connect_fd <= 0)
                               {
                                   printf("fd=%d accept error! \n",(p->opt).fd);
                                   continue;
                               }

                               obj.fd = eth_connect_fd;
                               obj.port_type = CONNECT_TCP;
                               obj.port = (p->opt).port;

                               if(!add_link(&obj,&tcp_max_fd,&tcp_link_head,&tcp_link_tail))
                               {
                                   printf("\nconnect tcp add_link occure an error\n");
                                   while(1);
                               }

                          }
                          else if((p->opt).port_type == UART)
                          {
                              uart_rev = read((p->opt).fd,RxBuffer,MAX_PACKAGE);

                              if(uart_rev > 0)
                              {
                                  struct Links_Objects *pp;
                                  pp =  tcp_link_head;
                                  for(;pp != NULL;pp = pp->next)
                                  {
                                      if((pp->opt).port_type == CONNECT_TCP)
                                      {
                                          if((pp->opt).port == (p->opt).port)
                                          {
                                              send((pp->opt).fd, RxBuffer, uart_rev, 0);
                                          }
                                      }
                                  }
                              }

                          }
                          else if((p->opt).port_type == CONNECT_TCP)
                          {
                              tcp_rev = recv((p->opt).fd,tcp_buf,sizeof(tcp_buf),0);

                              if (tcp_rev == -1)
                              {
                                  printf("tcp receive error!\n");
                                  while(1);
                              }
                              else if (tcp_rev == 0)
                              {
                                  close((p->opt).fd);
                                  FD_CLR((p->opt).fd, &net_fds);
                                  del_link((p->opt).fd,&tcp_link_head,&tcp_link_tail);
                               }
                              else
                              {
                                  struct Links_Objects *pp;

                                  pp =  tcp_link_head;
                                  for(;pp != NULL;pp = pp->next)
                                  {
                                      if((pp->opt).port_type == UART)
                                      {
                                          if((pp->opt).port == (p->opt).port)
                                          {
                                              write((pp->opt).fd,tcp_buf,tcp_rev);
                                          }
                                      }
                                  }
                              }
                          }
                      }

                  }
              }


         }


    }
    printf("tcp read thread is end:\n");

    inotify_rm_watch(file_fd, wd);
    close(file_fd);

    pthread_exit(NULL);
}


/**
*@breif 	main()
*/
int main(int argc, char **argv)
{
    unsigned int i;

    char file_name[50];
    int tcp_fd;

    int32_t ret;
    uint32_t uart_port_num;

    struct Objects_Opt obj;
    FILE *fp;

    _UART_PARS_s UART_Conf;

    UART_Conf.baudrate = 115200;
    UART_Conf.datalen = 8;
    UART_Conf.stopbit = 1;
    UART_Conf.odd = 0;  //0: non-parity  //1: odd-parity  //2: even-parity
    UART_Conf.enable_autoflow = 0;
    UART_Conf.enable_RS485 = 0;

    for(i = 1; i < 11; i++)
    {
        uart_port_num = i;

        sprintf(&file_name[0],"/mnt/mtdblock0/uart%d.ini",uart_port_num);

        fp=fopen(&file_name[0],"r");

        if(fp == 0)
        {
            UART_Conf.baudrate = 115200;
            UART_Conf.datalen = 8;
            UART_Conf.stopbit = 1;
            UART_Conf.odd = 0;  //0: non-parity  //1: odd-parity  //2: even-parity
            UART_Conf.enable_autoflow = 0;
            UART_Conf.enable_RS485 = 0;
        }
        else
        {
            set_uart_config_struct(&UART_Conf, fp);

            fclose(fp);
        }

        serial_info=open_tty(uart_port_num);
        set_tty_speed(serial_info, UART_Conf.baudrate);
        if((ret=set_tty_parity(serial_info,UART_Conf.datalen,UART_Conf.odd,UART_Conf.stopbit, UART_Conf.enable_autoflow))==0)
        {
            obj.fd = serial_info->fd;
            obj.port_type = UART;
            obj.port = (PORT_BASE+uart_port_num);

            if(!add_link(&obj,&tcp_max_fd,&tcp_link_head,&tcp_link_tail))
            {
                printf("\nuart add_link occure an error\n");
                while(1);
            }

            if((uart_port_num == 1) || (uart_port_num == 2))
            {
                if(set_tty_rs485(serial_info->fd, UART_Conf.enable_RS485) == 0)
                {
                }
                else
                {
                    printf("\n RS485 set fail!!");
                }
            }
        }
        else
        {
            printf("set uart fail! \n");
            while(1);
        }
    }

    for(i = 0; i < 10; i++)
    {
        fd[i] = create_socket(NULL, PORT_BASE+1+i);
    }

    printf("\n\n");

    if(pthread_create(&tcp_thread_id,NULL,tcp_process,0) !=0 )
    {
        printf("UserTimer thread (create) error\n");
        while(1);
    }

    pthread_join(tcp_thread_id, NULL);

    return 0;
}
