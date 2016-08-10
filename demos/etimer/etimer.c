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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "nuc970-etimer.h"

int main(int argc, char **argv)
{
	int fd[4], i, j, mode = ETMR_CAP_EDGE_FF;
	fd_set rfd;
	struct timeval tv;
	unsigned int cap, cnt;
	int ret;
	char dev[4][14] = {"/dev/etimer0",
                       "/dev/etimer1",
                       "/dev/etimer2",
                       "/dev/etimer3"};
	int period[4] = {1000, 800, 600, 400};	// Period (in us) of 4 timer channels
    int wkperiod[4] = {16384, 32768, 65536, 163840};    
    int timeout = 1000000;  // 1 sec
    
    for(i = 0; i < 4; i++) {        
        fd[i] = open(&dev[i][0], O_RDWR);
        if(fd[i] < 0)
            printf("open etimer %d error\n", i); 
            
        printf("Channel %d Time-out Wakeup testing\n", i);
        // switch clock, power down mode using 32kHz            
        ioctl(fd[i], ETMR_IOC_CLKLXT, NULL);
        
        ret = ioctl(fd[i], ETMR_IOC_PERIODIC_FOR_WKUP, &wkperiod[i]);
        if(ret == -1)
            printf("clock source error\n");   
        else    
        {
            system("echo mem > /sys/power/state");            
            printf("Wake up from ETimer%d.\n",i);
            printf("hit any key to test next channel\n");
            getchar();
            ioctl(fd[i], ETMR_IOC_STOP, NULL);
        }
    }
    printf("hit any key to quit periodic wake-up mode\n");
    getchar();
    
    //switch clock 12MHz
    for(i = 0; i < 4; i++) {        
        ioctl(fd[i], ETMR_IOC_CLKHXT, NULL);
    }
    
    //Periodic demo
    for(i = 0; i < 4; i++) {
        printf("Channel %d Periodic testing\n", i);    
        ioctl(fd[i], ETMR_IOC_PERIODIC, &timeout);        
        for(j = 0; j < 5; j++) {            
            read(fd[i], &cnt, sizeof(cnt));
            printf("%d sec\n", cnt);
		}
        ioctl(fd[i], ETMR_IOC_STOP, NULL);
    }
    printf("hit any key to quit periodic mode\n");
    getchar();    
	// Toggle output demo
	for(i = 0; i < 4; i++) {
		ioctl(fd[i], ETMR_IOC_TOGGLE, &period[i]);
	}

    printf("hit any key to quit toggle mode\n");
	getchar();
	// Free counting mode, block
	for(i = 0; i < 4; i++) {
		printf("Channel %d capture testing\n", i);
		ioctl(fd[i], ETMR_IOC_FREE_COUNTING, &mode);
		for(j = 0; j < 10; j++) {
			read(fd[i], &cap, sizeof(cap));
			printf("cap:%d Hz\n", cap);
		}
		printf("hit any key to test next channel\n");
		getchar();
		ioctl(fd[i], ETMR_IOC_STOP, NULL);
	}
    
	printf("hit any key to quit free counting mode\n");
	getchar();
	// Trigger counting mode, block
	for(i = 0; i < 4; i++) {
		printf("Channel %d capture testing\n", i);
		ioctl(fd[i], ETMR_IOC_TRIGGER_COUNTING, &mode);
		for(j = 0; j < 10; j++) {
			read(fd[i], &cap, sizeof(cap));
			printf("cap:%d us\n", cap);
		}
		printf("hit any key to test next channel\n");
		getchar();
		ioctl(fd[i], ETMR_IOC_STOP, NULL);
	}

	// Trigger counting mode, polling
	for(i = 0; i < 4; i++) {
		FD_ZERO(&rfd);
		FD_SET(fd[i], &rfd);

		printf("Channel %d capture poll testing\n", i);
		ioctl(fd[i], ETMR_IOC_TRIGGER_COUNTING, &mode);

		for(j = 0; j < 10; j++) {

			ret = select(fd[i] + 1, &rfd, NULL, NULL, NULL);
			if(ret == -1) {
				printf("select error\n");
			} else if(FD_ISSET(fd[i], &rfd)){

				read(fd[i], &cap, sizeof(cap));
				printf("cap:%d us\n", cap);
			}
		}
		printf("hit any key to test next channel\n");
		getchar();
		ioctl(fd[i], ETMR_IOC_STOP, NULL);
	}


	printf("hit any key to quit\n");
	for(i = 0; i < 4; i++) {
		close(fd[i]);
	}

	return 0;
}
