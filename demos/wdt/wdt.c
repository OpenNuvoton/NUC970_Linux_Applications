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
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/watchdog.h>

int main(void)
{

        int ii = 10, j;
        int fd = open("/dev/watchdog", O_RDWR);
        if (fd == -1) {
                perror("watchdog");
                exit(EXIT_FAILURE);
        }

        printf("Open watchdog ok\n");

	// Ping WDT for 10 times, then let system reset
        while (1) {
        	printf("ii = %d\n", ii);
                if(ii-- > 0)
                        ioctl(fd, WDIOC_KEEPALIVE, 0);
                sleep(1);
        }
        close(fd);
        return 0;
}
