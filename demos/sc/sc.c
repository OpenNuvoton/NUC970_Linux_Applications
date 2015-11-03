/*
 * Copyright (c) 2015 Nuvoton technology corporation
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
#include "nuc970-sc.h"

#define DEV "/dev/sc0"
//#define DEV "/dev/sc1"


int fd;
// get challenge command. see iso 7816-4
unsigned char cmd[] = {0x00, 0x84, 0x00, 0x00, 0x08};
unsigned char resp[10];


int main(int argc, char **argv)
{
	int i, len, j;
	char atr[33];
	int param = ICC_ABSENT;
	struct sc_transact sc_t;

	fd = open(DEV, O_RDWR);
	if(fd < 0) {
		printf("open sc error\n");
		goto out;
	} else {
		printf("open successed\n");
        }

	do {
		ioctl(fd, SC_IOC_GETSTATUS, &param);

	} while(param == ICC_ABSENT);

	// activate
	len = ioctl(fd, SC_IOC_ACTIVATE);

	if(len < 0) {
		printf("activate failed\n");
		goto out1;
	}

	ioctl(fd, SC_IOC_READATR, &atr[0]);

	printf("ATR: ");
	for(i = 0; i < len; i++)
		printf("%02x ", atr[i]);
	printf("\n");

	sc_t.tx_buf = &cmd[0];
	sc_t.rx_buf = resp;
	sc_t.tx_len = 5;
	if(ioctl(fd, SC_IOC_TRANSACT, &sc_t) < 0) {
		printf("read/write error\n");
		goto out2;

	}

	printf("response length %d\n", sc_t.rx_len);
	for(j = 0; j < sc_t.rx_len; j++) {
		printf("%02x ", resp[j]);
	}
	printf("\n");


out2:
	ioctl(fd, SC_IOC_DEACTIVATE);
out1:
	close(fd);
out:
	return 0;
}

