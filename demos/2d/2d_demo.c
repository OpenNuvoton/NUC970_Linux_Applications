/* demos/2d/2d_demo.c
 *
 * Copyright (c) 2015 Nuvoton technology corporation
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <pthread.h>
#include <linux/fb.h>
#include <sched.h>
#include <unistd.h>
#include <linux/vt.h>
#include <linux/ioctl.h>

#include "2d_demo.h"

#define SRC_IMAGE_FILE_NAME_0 "/mnt/sda1/image/image_800x480_rgb888_1.dat"
#define SRC_IMAGE_FILE_NAME_1 "/mnt/sda1/image/image_320x240_rgb888_1.dat"
#define SRC_IMAGE_FILE_NAME_2 "/mnt/sda1/image/image_100x100_rgb888_1.dat"

image_info_t image_info[3] = {
                                { SRC_IMAGE_FILE_NAME_0, 800, 480},
                                { SRC_IMAGE_FILE_NAME_1, 320, 240},
                                { SRC_IMAGE_FILE_NAME_2, 100, 100},
                             };
                             
unsigned short get_random_num(unsigned short mask)
{
    unsigned char buf[2];
    unsigned short v;
    FILE *fin;
        
    if ((fin = fopen(RANDOM_PATH, "r")) == NULL) {
            fprintf(stderr, "%s: unable to open file\n", RANDOM_PATH);
            return EXIT_FAILURE;
    }
    if (fread(buf, 1, sizeof buf, fin) != sizeof buf) {
            fprintf(stderr, "%s: not enough bytes (expected %u)\n",
                    RANDOM_PATH, (unsigned) sizeof buf);
            return EXIT_FAILURE;
    }
    fclose(fin);
    
    v = buf[0] | (buf[1] << 8UL);
     
    return (v%mask); 
}

static void print_menu(void)
{
    printf("\n===== NUC970 2D test Application =======\n");
    printf("=                                        =\n");
    printf("=  r.    reset frame buffer              =\n");
    printf("=  q.    exit                            =\n");
    printf("=  1.    bitblt                          =\n");
    printf("=  2.    bitblt with rop                 =\n");
    printf("=  3.    fill rectangle                  =\n");
    printf("=  4.    rotate                          =\n");
    printf("=  5.    line                            =\n");
    printf("=  6.    scale up/down                   =\n");
    printf("=                                        =\n");
    printf("=========================================\n");
    printf("Select number -->: ");
}
                        
int file_open_and_mmap ( char *file_name, char **mmap_addr, int *size )
{
    int     in_fd;

    struct stat    s;

    in_fd = open(file_name, O_RDONLY);
    if (in_fd < 0) {
        printf("input file open error\n");
        return -1;
    }

    // get input file size
    fstat(in_fd, &s);
    *size = s.st_size;

    // mapping input file to memory
    *mmap_addr = (char *) mmap(0, *size, PROT_READ, MAP_SHARED, in_fd, 0);
    if(mmap_addr == NULL) {
        printf("input file memory mapping failed\n");
        return -1;
    }

    return in_fd;
}

void select_image(int image, unsigned int copyto)
{
    char *image_ptr;
    int image_fd;    
    int file_size;
    
    printf("copy %dx%d image to off-screen buffer ...\n", image_info[image].width, image_info[image].height);
    
    if  ( (image_fd = file_open_and_mmap ( image_info[image].file_name, &image_ptr, &file_size ) ) < 0 )
        return;    
    memcpy((unsigned char *)copyto , image_ptr, file_size); 
    close(image_fd);
    munmap(image_ptr, file_size);
}
                   
int main(int argc, char *argv[])
{
    int fb_fd, ge2d_fd, ret, strip;
    int i, key;
    unsigned char *pVideoBuffer;
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    nuc970_g2d_params params;
    struct nuc970_mem_alloc mem_alloc;
    
    fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd == -1) {
            printf("Cannot open fb0!\n");
            return -1;
    }
    
    ge2d_fd = open("/dev/ge2d", O_RDWR);
    if (ge2d_fd == -1) {
            printf("Cannot open ge2d!\n");
            return -1;
    }
    
    if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) < 0) {
        perror("ioctl(FBIOGET_FSCREENINFO)");
        return -1;
    }
    
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("ioctl(FBIOGET_VSCREENINFO)");
        return -1;
    }
    
    switch(vinfo.bits_per_pixel)
    {
        case 16:
            strip = 2;
            break;
        
        case 24:
        case 32:
            strip = 4;
            break;
    }
    pVideoBuffer = mmap(NULL, vinfo.xres * vinfo.yres * strip, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
    
    if (pVideoBuffer == MAP_FAILED) {
            printf("mmap() failed\n");
            exit(0);
    }
    
    g_mem_fd = open("/dev/nuc970-mem", O_RDWR);
    if (g_mem_fd == -1) {
        printf("Cannot open mem!\n");
        return -1;
    }
    
    mem_alloc.size = vinfo.xres * vinfo.yres * strip;
    if ( ioctl(g_mem_fd, NUC970_MEM_ALLOC, &mem_alloc) )
    {
        printf("OOPS!!!! line=%d, size=%d\n", __LINE__, (int)mem_alloc.size );
        return -1;
     }
    printf("Memory ==> vir_addr = 0x%08x, phy_addr = 0x%08x\n", 
            mem_alloc.vir_addr, mem_alloc.phy_addr);
    
    while(1)
    {
        print_menu();
        key = getc(stdin);
        
        switch(key)
        {
            case 'r':
                memset(pVideoBuffer, 0x0, vinfo.xres * vinfo.yres * 4);
                break;
                
            case 'q':
                goto exit;
                
            case '1':   //bitblt
                select_image(0, mem_alloc.vir_addr);
                params.src_start_x = 0;
                params.src_start_y = 0;
                params.src_work_width = image_info[0].width;
                params.src_work_height = image_info[0].height;
                params.dst_start_x = 0;
                params.dst_start_y = 0;
                params.bpp_src = vinfo.bits_per_pixel;                
                params.src_full_width = image_info[0].width;
                params.dst_full_width = vinfo.xres;
                params.src_base_addr = mem_alloc.phy_addr;
                params.dst_base_addr = finfo.smem_start;
                
                ioctl(ge2d_fd, NUC970_GE2D_START_BITBLT, &params);
                break;

            case '2':   //bitblt with rop
                select_image(1, mem_alloc.vir_addr);
                params.src_start_x = 0;
                params.src_start_y = 0;
                params.src_work_width = image_info[1].width;
                params.src_work_height = image_info[1].height;
                params.dst_start_x = 0;
                params.dst_start_y = 0;
                params.bpp_src = vinfo.bits_per_pixel;                
                params.src_full_width = image_info[1].width;
                params.dst_full_width = vinfo.xres;
                params.src_base_addr = mem_alloc.phy_addr;
                params.dst_base_addr = finfo.smem_start;
                
                for(i=1;i<10;i++) {
                    params.rop = i;                
                    ioctl(ge2d_fd, NUC970_GE2D_START_BITBLT_ROP, &params);
                    
                    printf("Press a key to continue ...\n");
                    getc(stdin);
                    memset(pVideoBuffer, 0x0, vinfo.xres * vinfo.yres * 4);
                }
                break;
            
            case '3':   //fill rectangle
                for(i=0;i<10;i++) {
                    params.dst_start_x = (unsigned int)get_random_num(700);
                    params.dst_start_y = (unsigned int)get_random_num(350);
                    params.dst_work_width =  (unsigned int)get_random_num(500);
                    params.dst_work_height =  (unsigned int)get_random_num(200);
                    
                    if (params.dst_work_width + params.dst_start_x > vinfo.xres)
                        params.dst_work_width = vinfo.xres - params.dst_start_x;
                    if (params.dst_work_width == 0)
                        params.dst_work_width = 1;
                    
                    if (params.dst_work_height + params.dst_start_y > vinfo.yres)
                        params.dst_work_height = vinfo.yres - params.dst_start_y;
                    if (params.dst_work_height == 0)
                        params.dst_work_height = 1;
                    
                    params.bpp_src = vinfo.bits_per_pixel;                
                    params.dst_full_width = vinfo.xres;
                    
                    params.color_val[G2D_RED] = get_random_num(255);
                    params.color_val[G2D_GREEN] = get_random_num(255);
                    params.color_val[G2D_BLUE] = get_random_num(255);
                    
                    params.src_base_addr = mem_alloc.phy_addr;
                    params.dst_base_addr = finfo.smem_start;
                         
                    ioctl(ge2d_fd, NUC970_GE2D_FILL_RECTANGLE, &params);                 
                }
                break;
            
            case '4':   //rotation
                select_image(2, mem_alloc.vir_addr);
                for(i=1;i<8;i++) {
                    params.src_start_x = 0;
                    params.src_start_y = 0;
                    params.src_work_width = image_info[2].width;
                    params.src_work_height = image_info[2].height;
                    params.dst_start_x = 300;
                    params.dst_start_y = 200;
                    params.bpp_src = vinfo.bits_per_pixel;                
                    params.src_full_width = image_info[2].width;
                    params.dst_full_width = vinfo.xres;
                    
                    params.rotate = i;
                                        
                    params.src_base_addr = mem_alloc.phy_addr;
                    params.dst_base_addr = finfo.smem_start;
                         
                    ioctl(ge2d_fd, NUC970_GE2D_ROTATION, &params);
                    sleep(1);
                }               
                break;  
            
            case '5':   //draw line
                params.bpp_src = vinfo.bits_per_pixel;                
                params.dst_full_width = vinfo.xres;
                params.dst_full_height = vinfo.yres;
                params.src_base_addr = mem_alloc.phy_addr;
                params.dst_base_addr = finfo.smem_start;

                params.line_x1 = 0;
                params.line_y1 = 0;
                params.line_x2 = vinfo.xres-1;
                params.line_y2 = 0+1;
                
                params.color_val[G2D_RED] = get_random_num(255);
                params.color_val[G2D_GREEN] = get_random_num(255);
                params.color_val[G2D_BLUE] = get_random_num(255);
               
                for (i=0; i<80; i++) {                        
                    ioctl(ge2d_fd, NUC970_GE2D_LINE, &params);
                    params.line_x1 += 4;
                    params.line_y2 += 3;
                }

                params.color_val[G2D_RED] = get_random_num(255);
                params.color_val[G2D_GREEN] = get_random_num(255);
                params.color_val[G2D_BLUE] = get_random_num(255);
                
                params.line_x1 = vinfo.xres-1;
                params.line_y1 = 0;
                params.line_x2 = vinfo.xres-1;
                params.line_y2 = vinfo.yres-1;
                for (i=0; i<80; i++) {
                    ioctl(ge2d_fd, NUC970_GE2D_LINE, &params);
                    params.line_y1 += 3;
                    params.line_x2 -= 4;
                }

                params.color_val[G2D_RED] = get_random_num(255);
                params.color_val[G2D_GREEN] = get_random_num(255);
                params.color_val[G2D_BLUE] = get_random_num(255);
                
                params.line_x1 = vinfo.xres-1;
                params.line_y1 = vinfo.yres-1;
                params.line_x2 = 0;
                params.line_y2 = vinfo.yres;
                for (i=0; i<80; i++) {
                    ioctl(ge2d_fd, NUC970_GE2D_LINE, &params);
                    params.line_x1 -= 4;
                    params.line_y2 -= 3;
                }

                params.color_val[G2D_RED] = get_random_num(255);
                params.color_val[G2D_GREEN] = get_random_num(255);
                params.color_val[G2D_BLUE] = get_random_num(255);
                params.line_x1 = 0;
                params.line_y1 = vinfo.yres-1;
                params.line_x2 = 0;
                params.line_y2 = 0;
                for (i=0; i<80; i++) {
                    ioctl(ge2d_fd, NUC970_GE2D_LINE, &params);
                    params.line_y1 -= 3;
                    params.line_x2 += 4;
                }
                break;
                
            case '6':   //scale up/down
                select_image(1, mem_alloc.vir_addr);
                params.src_start_x = 0;
                params.src_start_y = 0;
                params.src_work_width = image_info[1].width;
                params.src_work_height = image_info[1].height;
                params.dst_start_x = 240;
                params.dst_start_y = 100;
                params.bpp_src = vinfo.bits_per_pixel;                
                params.src_full_width = image_info[1].width;
                params.dst_full_width = vinfo.xres;
                params.src_base_addr = mem_alloc.phy_addr;
                params.dst_base_addr = finfo.smem_start;
                
                for(i=2;i<10;i+=2)
                {
                    params.scale_mode = G2D_SCALE_DOWN;
                    params.scale_vfn = 1;
                    params.scale_vfm = i;
                    params.scale_hfn = 1;
                    params.scale_hfm = i;                
                    ioctl(ge2d_fd, NUC970_GE2D_STRETCH, &params);
                    sleep(1);
                }
                
                memset(pVideoBuffer, 0x0, vinfo.xres * vinfo.yres * 4);
                
                for(i=2;i<10;i+=2)
                {
                    params.scale_mode = G2D_SCALE_UP;
                    params.scale_vfn = 1;
                    params.scale_vfm = i;
                    params.scale_hfn = 1;
                    params.scale_hfm = i;                
                    ioctl(ge2d_fd, NUC970_GE2D_STRETCH, &params);
                    sleep(1);
                }
                break;
                
            default:
                printf("no exist input command, please enter the command number \n");
                break;
        }
        getc(stdin);
    }

exit:
    munmap(pVideoBuffer, vinfo.xres * vinfo.yres * strip);
    if ( ioctl(g_mem_fd, NUC970_MEM_FREE, &mem_alloc) )
        return;
    close(g_mem_fd);
    
    printf("exit!!\n");
}