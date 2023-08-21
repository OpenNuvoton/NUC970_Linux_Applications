/* demos/2d/2d_demo.h
 *
 * Copyright (c) 2015 Nuvoton technology corporation
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
*/
#ifndef GE2D_DEMO_H
#define GE2D_DEMO_H

typedef enum {
	G2D_BLACK = 0,
	G2D_RED = 1,
	G2D_GREEN = 2,
	G2D_BLUE = 3,
	G2D_WHITE = 4,
	G2D_YELLOW = 5,
	G2D_CYAN = 6,
	G2D_MAGENTA = 7
} G2D_COLOR;

/* LCD Display Description */
struct nuc970fb_display {
	/* LCD Image type */
	unsigned type;

	/* LCD Screen Size */
	unsigned short width;
	unsigned short height;
    
	/* LCD Screen Info */
	unsigned short xres;
	unsigned short yres;
	unsigned short bpp;

	unsigned long pixclock;
	unsigned short left_margin;
	unsigned short right_margin;
	unsigned short hsync_len;
	unsigned short upper_margin;
	unsigned short lower_margin;
	unsigned short vsync_len;

	/* hardware special register value */
	unsigned int dccs;
	unsigned int devctl;
	unsigned int fbctrl;
	unsigned int scale;
    unsigned int phy_addr;
};

typedef enum {
	G2D_SCALE_DOWN,
	G2D_SCALE_UP
} G2D_SCALE_MODE;

typedef struct {
	unsigned int src_base_addr;	//Base address of the source image
	unsigned int src_full_width;	//source image full width
	unsigned int src_full_height;	//source image full height
	unsigned int src_start_x;	//coordinate start x of source image
	unsigned int src_start_y;	//coordinate start y of source image
	unsigned int src_work_width;	//source image width for work
	unsigned int src_work_height;	//source image height for work
	unsigned int src_colormode;

	unsigned int dst_base_addr;	//Base address of the destination image
	unsigned int dst_full_width;	//destination screen full width
	unsigned int dst_full_height;	//destination screen full width
	unsigned int dst_start_x;	//coordinate start x of destination screen
	unsigned int dst_start_y;	//coordinate start y of destination screen
	unsigned int dst_work_width;	//destination screen width for work
	unsigned int dst_work_height;	//destination screen height for work
	unsigned int dst_colormode;

	// Coordinate (X, Y) of clipping window
	unsigned int cw_x1, cw_y1;
	unsigned int cw_x2, cw_y2;

	// Line address
	unsigned int line_x1, line_y1;
	unsigned int line_x2, line_y2;

	unsigned char color_val[8];

	// Alpha blending
	unsigned int alpha_mode;	//true : enable, false : disable
	unsigned int alpha_val;

	// Transparent
	unsigned int color_key_mode;	//true : enable, false : disable
	unsigned int color_key_val;	//transparent color value

	unsigned char bpp_src;
	unsigned char rop;		// rop code
	unsigned char rotate;		// rotate option

	// Scale up/down
	unsigned char scale_mode;
	unsigned int scale_vfn, scale_vfm, scale_hfn, scale_hfm;
} nuc970_g2d_params;

struct nuc970_mem_alloc {
	int size;
	unsigned int vir_addr;
	unsigned int phy_addr;
	unsigned int kvir_addr;
};

typedef enum {
	ROT_R_45=1,
	ROT_L_45,
	ROT_L_90,
	ROT_UP_DOWN,
	ROT_R_90, ROT_180
} G2D_ROT_DEG;

typedef struct {
	char *file_name;
	int width;
	int height;
} image_info_t;

int g_mem_fd;
struct nuc970fb_display g_working_fb;

#define NUC970FB_GET_FB_INFO		_IOW('F', 203, struct nuc970fb_display)

#define NUC970_GE2D_START_BITBLT	_IO('G', 0)
#define NUC970_GE2D_START_BITBLT_ROP	_IO('G', 1)
#define NUC970_GE2D_FILL_RECTANGLE	_IO('G', 2)
#define NUC970_GE2D_ROTATION		_IO('G', 3)
#define NUC970_GE2D_LINE		_IO('G', 4)
#define NUC970_GE2D_STRETCH		_IO('G', 5)
#define NUC970_GE2D_GET_MEM_PHYADDR	_IO('G', 20)
#define NUC970_GE2D_GET_MEM_SIZE	_IO('G', 21)

#define NUC970_MEM_ALLOC		_IOWR('M', 310,	struct nuc970_mem_alloc)
#define NUC970_MEM_FREE			_IOWR('M', 311, struct nuc970_mem_alloc)

#define RANDOM_PATH "/dev/urandom"

#endif
