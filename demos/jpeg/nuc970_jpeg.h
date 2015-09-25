/* nuc970_jpeg.h
 *
 * Copyright (c) 2015 Nuvoton technology corporation
 * All rights reserved.
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __NUC970_JPEG__H
#define __NUC970_JPEG__H

//#include <linux/config.h>
#include <linux/videodev.h>

#include <asm/io.h>


typedef enum jpeg_state {
    JPEG_IDLE               = 1,
    JPEG_ENCODING           = 2,
    JPEG_ENCODED_IMAGE      = 3,
    JPEG_DECODING           = 9,
    JPEG_DECODED_HEADER     = 10,
    JPEG_DECODED_IMAGE      = 11,
    JPEG_DECODE_ERROR       = 12,
    JPEG_MEM_SHORTAGE       = 15,
    JPEG_DECODE_PARAM_ERROR = 13,
    JPEG_ENCODE_PARAM_ERROR = 14,
    JPEG_CLOSED             = 20,
} jpeg_state_t;


#define IS_ENCODE(state)	(state >=JPEG_ENCODING && state <JPEG_DECODING)
#define IS_DECODE(state)	(state >=JPEG_DECODING)
#define IS_DECODED(state)	((state == JPEG_DECODED_IMAGE) || (state==JPEG_DECODE_ERROR))
#define IS_ENCODED(state)	(state == JPEG_ENCODED_IMAGE)
#define IS_FINISHED(state)  (IS_DECODED(state) || IS_ENCODED(state))

typedef struct jpeg_param{
	/*for get/set parameters*/
	__u32	vaddr_src;
	__u32	decopw_vaddr;
	__u32	decopw_en;
	__u32	decopw_TargetBuffersize;
	__u32	vaddr_dst;
	__u32	paddr_src;
	__u32	paddr_dst;
	__u32	src_bufsize;
	__u32	dst_bufsize;
	__u32	dec_stride;
	__u32	windec_en;
	__u32	windec_mcux_start;
	__u32	windec_mcux_end;
	__u32	windec_mcuy_start;
	__u32	windec_mcuy_end;
	__u32	windec_stride;
	/*for set only*/
	__u32	decode_output_format;//DRVJPEG_DEC_PRIMARY_PACKET_YUV422,
	__u32	encode;//1 decode, 0 encode	
	__u32	scale;	//1 enable, 0 disable
	/* encode / decode scale */
	__u32	scaled_width;
	__u32	scaled_height;
	__u32	decInWait_buffer_size;	
	/*encode parameters for set only*/
	__u32	encode_width;/*the width that will be encoded image raw data*/
	__u32	encode_height;/*the height that will be encoded image raw data*/
	__u8	qadjust;//the larger the better quality[2-16](0.25Q, 0.5Q, 0.75Q, Q, 1.25Q, 1.5Q, 1.75Q, 2Q, 2.25Q, 2.5Q, 2.75Q, 3Q, 3.25Q, 3.5Q, 3.75Q) 
	__u8	qscaling;//the smaller the better quality[1-16]
	__u32	encode_source_format;
	__u32	encode_image_format;
    __u32	buffersize;/*each encode buffer size*/
    __u32	buffercount;/*total encode buffer count*/
}jpeg_param_t;

//Define for Encode application
#define DRVJPEG_ENC_THUMBNAIL_QVGA	   0
#define DRVJPEG_ENC_THUMBNAIL_QQVGA	   1

typedef struct jpeg_info{
	/*decode information*/
	__u32	yuvformat;	/*for decode*/
	__u32	width;		/*for decode*/
    __u32	height;		/*for decode*/
	__u32	dec_stride;
    /*encode information*/	
    __u32	bufferend;
    __u32	state;
    __u32	image_size[]; /*image size after encoded*/
}jpeg_info_t;

/* Define for Decode Output Format */
#define DRVJPEG_DEC_PRIMARY_PLANAR_YUV      0x8021    // (PLANAR_ON | PDHTAB | DHEND)
#define DRVJPEG_DEC_PRIMARY_PACKET_YUV422   0x0021    // (PDHTAB | DHEND)
#define DRVJPEG_DEC_PRIMARY_PACKET_RGB555   0x004021  // (PDHTAB | DHEND | ORDER)
#define DRVJPEG_DEC_PRIMARY_PACKET_RGB565   0x006021  // (PDHTAB | DHEND | RGB555_565 | ORDER )
#define DRVJPEG_DEC_PRIMARY_PACKET_RGB555R1 0x404021  // (PDHTAB | DHEND | ORDER)
#define DRVJPEG_DEC_PRIMARY_PACKET_RGB565R1 0x406021  // (PDHTAB | DHEND | RGB555_565 | ORDER )
#define DRVJPEG_DEC_PRIMARY_PACKET_RGB565R2 0x806021  // (PDHTAB | DHEND | RGB555_565 | ORDER )
#define DRVJPEG_DEC_PRIMARY_PACKET_RGB555R2 0x804021  // (PDHTAB | DHEND | ORDER)
#define DRVJPEG_DEC_PRIMARY_PACKET_RGB888   0x14021   // (PDHTAB | DHEND | ORDER | ARGB888)

#define DRVJPEG_DEC_SOFTWARE_RGB555         0xFFF0    // (PDHTAB | DHEND | ORDER)
#define DRVJPEG_DEC_SOFTWARE_RGB565         0xFFF1    // (PDHTAB | DHEND | ORDER)

#define DRVJPEG_DEC_THUMBNAIL_PLANAR_YUV    0x8011    // (PLANAR_ON | DTHB | PDHTAB)
#define DRVJPEG_DEC_THUMBNAIL_PACKET_YUV422 0x0031    // (DTHB | PDHTAB | DHEND)
#define DRVJPEG_DEC_THUMBNAIL_PACKET_RGB555 0x4031    // (DTHB | PDHTAB | DHEND | ORDER)

//Define for Encode Image Format
#define DRVJPEG_ENC_PRIMARY_YUV420     0xA0
#define DRVJPEG_ENC_PRIMARY_YUV422     0xA8
#define DRVJPEG_ENC_PRIMARY_GRAY       0xA1
#define DRVJPEG_ENC_THUMBNAIL_YUV420   0x90
#define DRVJPEG_ENC_THUMBNAIL_YUV422   0x98
#define DRVJPEG_ENC_THUMBNAIL_GRAY     0x91

//Define for Decode Image Format
#define DRVJPEG_DEC_YUV420             0x000
#define DRVJPEG_DEC_YUV422             0x100
#define DRVJPEG_DEC_YUV444             0x200
#define DRVJPEG_DEC_YUV411             0x300
#define DRVJPEG_DEC_GRAY               0x400
#define DRVJPEG_DEC_YUV422T            0x500

//Define for Encode input Format
#define DRVJPEG_ENC_SRC_PLANAR         0
#define DRVJPEG_ENC_SRC_PACKET         1

//Define for Encode Image Header
#define DRVJPEG_ENC_PRIMARY_DRI        P_DRI
#define DRVJPEG_ENC_PRIMARY_QTAB       P_QTAB
#define DRVJPEG_ENC_PRIMARY_HTAB       P_HTAB
#define DRVJPEG_ENC_PRIMARY_JFIF       P_JFIF
#define DRVJPEG_ENC_THUMBNAIL_DRI      T_DRI
#define DRVJPEG_ENC_THUMBNAIL_QTAB     T_QTAB
#define DRVJPEG_ENC_THUMBNAIL_HTAB     T_HTAB
#define DRVJPEG_ENC_THUMBNAIL_JFIF     T_JFIF

#define JPEG_DECIPW_BUFFER_EMPTY 	   1
#define JPEG_DECIPW_BUFFER_NOT_EMPTY   0

#define JPEG_TRIGGER                   _IOW('v',120, struct jpeg_param)
#define JPEG_S_PARAM                   _IOW('v',121, struct jpeg_param)
#define JPEG_G_PARAM                   _IOW('v',122, struct jpeg_param)
#define JPEG_STATE                     _IOR('v',123, struct jpeg_param)
#define JPEG_G_INFO                    _IOR('v',124, struct jpeg_info)
#define JPEG_DECIPW_BUFFER_STATE       _IOR('v',125, __u32)
#define JPEG_G_DECIPW_BUFFER_SIZE      _IOR('v',126, __u32)
#define JPEG_DECODE_RESUME             _IOR('v',127, __u32)        
#define JPEG_S_DECOPW                  _IOW('v',128,  __u32)   
#define JPEG_GET_JPEG_BUFFER           _IOR('v',129, __u32)
#define JPEG_SET_ENCOCDE_RESERVED      _IOW('v',130,  __u32)
//#define JPEG_SET_ENC_SRC_FROM_VIN _IOW('v',131,  __u32)
#define JPEG_FLUSH_CACHE               _IOW('v',132,  __u32)
#define JPEG_SET_ENC_THUMBNAIL         _IOW('v',133, __u32)
#define JPEG_GET_ENC_THUMBNAIL_SIZE    _IOW('v',134, __u32)
#define JPEG_GET_ENC_THUMBNAIL_OFFSET  _IOW('v',135, __u32)
#define JPEG_SET_ENC_STRIDE            _IOW('v',136, __u32)
#define JPEG_SET_ENC_USER_QTABLE0      _IOW('v',137, __u32)
#define JPEG_SET_ENC_USER_QTABLE1      _IOW('v',138, __u32)
#define JPEG_ACTIVE_ENC_DEFAULTQTABLE  _IOW('v',139, __u32)
#define JPEG_ACTIVE_ENC_USER_QTABLE    _IOW('v',140, __u32)
#define JPEG_SET_ENC_USER_YADDRESS     _IOW('v',141,  __u32)
#define JPEG_SET_ENC_USER_UADDRESS     _IOW('v',142,  __u32)
#define JPEG_SET_ENC_USER_VADDRESS     _IOW('v',143,  __u32)
#define JPEG_GET_JPEG_BUFFER_PADDR     _IOR('v',144, __u32)
#define JPEG_DECODE_TO_FRAME_BUFFER    _IOR('v',145, __u32)


#endif  //__NUC970_JPEG__H
