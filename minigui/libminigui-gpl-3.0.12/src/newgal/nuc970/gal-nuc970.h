/*
**  $Id: nullvideo.h 12768 2010-04-22 05:28:30Z wanzheng $
**  
**  Copyright (C) 2003 ~ 2007 Feynman Software.
**  Copyright (C) 2001 ~ 2002 Wei Yongming.
*/

#ifndef _GAL_nuc970video_h
#define _GAL_nuc970video_h

#include "sysvideo.h"
#include "videomem-bucket.h"

/* Hidden "this" pointer for the video functions */
#define _THIS	GAL_VideoDevice *this

/* Private display data */

struct GAL_PrivateVideoData { /* for 6410 only */
    int fd_g2d;

    int fd_fb;
    unsigned int smem_start; /* physics address */
    unsigned int smem_len;

    unsigned char *pixels;
    int width;
    int height;
    int pitch;

    gal_vmbucket_t vmbucket;
};

struct private_hwdata { /* for GAL_SURFACE */
    gal_vmblock_t *vmblock;
    unsigned int addr_phy;
    int fd_g2d;
};

extern int nuc970_size_table[48];

/******************************************************************
 * copy from g2d driver
 */
typedef enum
{
	PAL1, PAL2, PAL4, PAL8,
	RGB8, ARGB8, RGB16, ARGB16, RGB18, RGB24, RGB30, ARGB24,RGBA16,RGBX24,RGBA24,
	YC420, YC422, // Non-interleave
	CRYCBY, CBYCRY, YCRYCB, YCBYCR, YUV444 // Interleave
} G2D_COLOR_SPACE;

typedef struct
{
    unsigned int	src_base_addr;			//Base address of the source image
	unsigned int	src_full_width;			//source image full width
	unsigned int	src_full_height;		//source image full height
	unsigned int	src_start_x;			//coordinate start x of source image
	unsigned int	src_start_y;			//coordinate start y of source image
	unsigned int	src_work_width;			//source image width for work
	unsigned int    src_work_height;		//source image height for work
    unsigned int    src_colormode;

	unsigned int	dst_base_addr;			//Base address of the destination image	
	unsigned int	dst_full_width;			//destination screen full width
	unsigned int	dst_full_height;		//destination screen full width
	unsigned int	dst_start_x;			//coordinate start x of destination screen
	unsigned int	dst_start_y;			//coordinate start y of destination screen
	unsigned int	dst_work_width;			//destination screen width for work
	unsigned int    dst_work_height;		//destination screen height for work
    unsigned int    dst_colormode;

	// Coordinate (X, Y) of clipping window
	unsigned int    cw_x1, cw_y1;
	unsigned int    cw_x2, cw_y2;
    
    // Line address
    unsigned int    line_x1, line_y1;
	unsigned int    line_x2, line_y2;
    
	unsigned char   color_val[8];

	// Alpha blending
    unsigned int	alpha_mode;			//true : enable, false : disable
	unsigned int	alpha_val;
    
    // Transparent
	unsigned int	color_key_mode;			//true : enable, false : disable
	unsigned int	color_key_val;			//transparent color value    
    
	unsigned char   bpp_src;
    unsigned char   rop;        // rop code
    unsigned char   rotate;     // rotate option
    
    // Scale up/down
    unsigned char   scale_mode;
    unsigned int    scale_vfn, scale_vfm, scale_hfn, scale_hfm;
} nuc970_g2d_params;

typedef enum
{   
    G2D_BLACK = 0, G2D_RED = 1, G2D_GREEN = 2, G2D_BLUE = 3, G2D_WHITE = 4, 
    G2D_YELLOW = 5, G2D_CYAN = 6, G2D_MAGENTA = 7
} G2D_COLOR;

#define G2D_MINOR  220                     // Just some number
#define G2D_IOCTL_MAGIC 'G'

#define NUC970_G2D_COLOR_RGB_565               (0x0<<0)
#define NUC970_G2D_COLOR_RGBA_5551             (0x1<<0)
#define NUC970_G2D_COLOR_ARGB_1555             (0x2<<0)
#define NUC970_G2D_COLOR_RGBA_8888             (0x3<<0)
#define NUC970_G2D_COLOR_ARGB_8888             (0x4<<0)
#define NUC970_G2D_COLOR_XRGB_8888             (0x5<<0)
#define NUC970_G2D_COLOR_RGBX_8888             (0x6<<0)

/* Alpha Blending Mode */
#define NUC970_G2D_ROP_REG_ABM_NO_BLENDING         (0<<10)
#define NUC970_G2D_ROP_REG_ABM_SRC_BITMAP          (1<<10)
#define NUC970_G2D_ROP_REG_ABM_REGISTER            (2<<10)
#define NUC970_G2D_ROP_REG_ABM_FADING          (4<<10)

#define NUC970_GE2D_START_BITBLT			_IO(G2D_IOCTL_MAGIC,0)
#define NUC970_GE2D_START_BITBLT_ROP		_IO(G2D_IOCTL_MAGIC,1)
#define NUC970_GE2D_FILL_RECTANGLE		    _IO(G2D_IOCTL_MAGIC,2)
#define NUC970_GE2D_ROTATION		        _IO(G2D_IOCTL_MAGIC,3)
#define NUC970_GE2D_LINE		            _IO(G2D_IOCTL_MAGIC,4)
#define NUC970_GE2D_STRETCH		            _IO(G2D_IOCTL_MAGIC,5)

#endif /* _GAL_nuc970video_h */
