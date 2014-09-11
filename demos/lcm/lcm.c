/****************************************************************************
 *                                                                          *
 * Copyright (c) 2008 Nuvoton Technology Corp. All rights reserved.         *
 *                                                                          *
 ***************************************************************************/

/****************************************************************************
 *
 * FILENAME
 *     lcm.c
 *
 * VERSION
 *     1.0
 *
 * DESCRIPTION
 *     This file is a LCM sample program
 *
 * DATA STRUCTURES
 *     None
 *
 * FUNCTIONS
 *     None
 *
 * HISTORY
 *     2004/09/06		 Ver 1.0 Created by PC34 MCLi
 *
 * REMARK
 *     None
 **************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <asm/ioctl.h>
#include <sys/mman.h>

#define LCD_WIDTH  320
#define LCD_HEIGHT 240
int main()
{
        int fd, ret;
        int i;
        FILE *fpVideoImg;
        unsigned char *pVideoBuffer;
        unsigned long uVideoSize;

        fd = open("/dev/fb0", O_RDWR);
        if (fd == -1) {
                printf("Cannot open fb0!\n");
                return -1;
        }

        uVideoSize = LCD_WIDTH * LCD_HEIGHT * 2;

        pVideoBuffer = mmap(NULL, uVideoSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

        if (pVideoBuffer == MAP_FAILED) {
                printf("mmap() failed\n");
                exit(0);
        }

	for(i = 0; i <  LCD_WIDTH * LCD_HEIGHT; i++) {
		*(unsigned short *)(pVideoBuffer + i * 2) = 0xF800;
	}

	sleep(2);

	for(i = 0; i <  LCD_WIDTH * LCD_HEIGHT; i++) {
		*(unsigned short *)(pVideoBuffer + i * 2) = 0x07E0;
	}

	sleep(2);

	for(i = 0; i <  LCD_WIDTH * LCD_HEIGHT; i++) {
		*(unsigned short *)(pVideoBuffer + i * 2) = 0x001F;
	}

        sleep(2);

        memset(pVideoBuffer, 0, uVideoSize);
        close(fd);
        return 0;
}
