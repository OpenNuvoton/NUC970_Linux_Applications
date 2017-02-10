/*
 * jdapimin.c
 *
 * Copyright (C) 1994-1998, Thomas G. Lane.
 * Modified 2009-2013 by Guido Vollbeding.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains application interface code for the decompression half
 * of the JPEG library.  These are the "minimum" API routines that may be
 * needed in either the normal full-decompression case or the
 * transcoding-only case.
 *
 * Most of the routines intended to be called directly by an application
 * are in this file or in jdapistd.c.  But also see jcomapi.c for routines
 * shared by compression and decompression, and jdtrans.c for the transcoding
 * case.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"

#include <dirent.h>

/*
 * Initialization of a JPEG decompression object.
 * The error manager must already be set up (in case memory manager fails).
 */

S_JPEG g_jpeg;

GLOBAL(void)
jpeg_CreateDecompress (j_decompress_ptr cinfo, int version, size_t structsize)
{
	DIR *dir;
//	int lcm_fd;

	unsigned char device[] = "/dev/video0";

    //jinit_memory_mgr((j_common_ptr) cinfo);

    //printf("\n jpeg_CreateDecompress \n");

	//TRACEMS( cinfo, 1, JTRC_12 );
	
	memset( (unsigned char *)&g_jpeg, 0, sizeof(g_jpeg) );
	g_jpeg.fd_jpeg = -1;

	/* Check device for jpegcodec "/dev/video0" or "/dev/video1" */
	//TRACEMS( cinfo, 1, JTRC_13 );

	/* Try to open folder "/sys/class/video4linux/video1/",
		if the folder exists, jpegcodec is "/dev/video1", otherwises jpegcodec is "/dev/video0" */
	dir = opendir("/sys/class/video4linux/video1/");
	if(dir)
	{
		closedir(dir);
		device[10]++;
	}
	//TRACEMSS( cinfo, 1, JTRC_14, device );

    //printf("\n open %s \n", device);

	g_jpeg.fd_jpeg = open( device, O_RDWR );
	if ( g_jpeg.fd_jpeg < 0 )
	{
		//TRACEMS( cinfo, 1, JTRC_15 );
		g_jpeg.isHWEnabled = 0;
	}
	else
	{
		if ( (ioctl( g_jpeg.fd_jpeg, JPEG_GET_JPEG_BUFFER, &g_jpeg.total_jpeg_buffer_size)) < 0 )
		{
			_demo_uninit_hwjpeg();
			WARNMS( cinfo, JWRN_12 );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}
		//TRACEMS1( cinfo, 1, JTRC_16, g_jpeg.total_jpeg_buffer_size );
		g_jpeg.jpeg_buff = mmap(	NULL,
									g_jpeg.total_jpeg_buffer_size,
									(PROT_READ|PROT_WRITE),
									MAP_SHARED,
									g_jpeg.fd_jpeg,
									0
									);

		if ( g_jpeg.jpeg_buff == MAP_FAILED )
		{
			_demo_uninit_hwjpeg();
			WARNMS( cinfo, JWRN_13 );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}

		g_jpeg.isHWEnabled = 1;
	}

    //printf("n g_jpeg.isHWEnabled = %d \n ", g_jpeg.isHWEnabled);
	
#if 0
	if ( (lcm_fd = open( "/dev/fb0", O_RDWR )) < 0 )
	{
		_demo_uninit_hwjpeg();
		close( lcm_fd );
		WARNMS( cinfo, JWRN_14 );			/* ### HW JPEG Decode - Error: failed to open fb0! */ 
		ERREXIT( cinfo, JERR_BAD_STATE );	/* Improper call to JPEG library in state %d */
	}

	if ( ioctl( lcm_fd, FBIOGET_VSCREENINFO, &(g_jpeg.var) ) < 0 )
	{
		_demo_uninit_hwjpeg();
		close( lcm_fd );
		WARNMS( cinfo, JWRN_15 );			/* ### HW JPEG Decode - Error: ioctl FBIOGET_VSCREENINFO! */
		ERREXIT( cinfo, JERR_BAD_STATE );	/* Improper call to JPEG library in state %d */
	}
	close( lcm_fd );
#endif
	g_jpeg.dparam.encode				= 0;			//0:decode
	g_jpeg.dparam.src_bufsize			= 50*1024; //100*1024;
///	g_jpeg.dparam.dst_bufsize			= 640*480*2;
	g_jpeg.dparam.decInWait_buffer_size	= 20*1024;//100*1024;
	g_jpeg.dparam.dst_bufsize			= g_jpeg.total_jpeg_buffer_size - g_jpeg.dparam.src_bufsize - g_jpeg.dparam.decInWait_buffer_size;
	g_jpeg.dparam.decopw_en				= 0;
	g_jpeg.dparam.windec_en				= 0;
	g_jpeg.dparam.scale					= 0;
	g_jpeg.dparam.buffersize			= 0;
	g_jpeg.dparam.buffercount			= 1;
	/* Set decode output format: RGB555/RGB565/ARGB8888 (FF)/YUV422 */
	g_jpeg.dparam.decode_output_format	= DRVJPEG_DEC_PRIMARY_PACKET_RGB565;

	g_jpeg.temp_dptr = g_jpeg.jpeg_buff + g_jpeg.dparam.src_bufsize;
	g_jpeg.in_jpeg_buff = g_jpeg.jpeg_buff + g_jpeg.dparam.src_bufsize + g_jpeg.dparam.dst_bufsize;

  int i;

  /* Guard against version mismatches between library and caller. */
  cinfo->mem = NULL;		/* so jpeg_destroy knows mem mgr not called */
  if (version != JPEG_LIB_VERSION)
    ERREXIT2(cinfo, JERR_BAD_LIB_VERSION, JPEG_LIB_VERSION, version);
  if (structsize != SIZEOF(struct jpeg_decompress_struct))
    ERREXIT2(cinfo, JERR_BAD_STRUCT_SIZE, 
	     (int) SIZEOF(struct jpeg_decompress_struct), (int) structsize);

  /* For debugging purposes, we zero the whole master structure.
   * But the application has already set the err pointer, and may have set
   * client_data, so we have to save and restore those fields.
   * Note: if application hasn't set client_data, tools like Purify may
   * complain here.
   */
  {
    struct jpeg_error_mgr * err = cinfo->err;
    void * client_data = cinfo->client_data; /* ignore Purify complaint here */
    MEMZERO(cinfo, SIZEOF(struct jpeg_decompress_struct));
    cinfo->err = err;
    cinfo->client_data = client_data;
  }
  cinfo->is_decompressor = TRUE;

  /* Initialize a memory manager instance for this object */
  jinit_memory_mgr((j_common_ptr) cinfo);

  /* Zero out pointers to permanent structures. */
  cinfo->progress = NULL;
  cinfo->src = NULL;

  for (i = 0; i < NUM_QUANT_TBLS; i++)
    cinfo->quant_tbl_ptrs[i] = NULL;

  for (i = 0; i < NUM_HUFF_TBLS; i++) {
    cinfo->dc_huff_tbl_ptrs[i] = NULL;
    cinfo->ac_huff_tbl_ptrs[i] = NULL;
  }

  /* Initialize marker processor so application can override methods
   * for COM, APPn markers before calling jpeg_read_header.
   */
  cinfo->marker_list = NULL;
  jinit_marker_reader(cinfo);

  //printf("\n jpeg_CreateDecompress 3 \n ");

  /* And initialize the overall input controller. */
  jinit_input_controller(cinfo);

  //printf("\n jpeg_CreateDecompress 4 \n ");

  /* OK, I'm ready */
  cinfo->global_state = DSTATE_START;
}


/*
 * Destruction of a JPEG decompression object
 */

GLOBAL(void)
jpeg_destroy_decompress (j_decompress_ptr cinfo)
{
  //printf("\n jpeg_destroy_decompress \n");

  jpeg_destroy((j_common_ptr) cinfo); /* use common routine */
}


/*
 * Abort processing of a JPEG decompression operation,
 * but don't destroy the object itself.
 */

GLOBAL(void)
jpeg_abort_decompress (j_decompress_ptr cinfo)
{
  //printf("\n jpeg_abort_decompress \n");

  jpeg_abort((j_common_ptr) cinfo); /* use common routine */
}


/*
 * Set default decompression parameters.
 */

LOCAL(void)
default_decompress_parms (j_decompress_ptr cinfo)
{
  int cid0, cid1, cid2;

  //printf("\n default_decompress_parms %d \n", cinfo->num_components);

  /* Guess the input colorspace, and set output colorspace accordingly. */
  /* Note application may override our guesses. */
  switch (cinfo->num_components) {
  case 1:
    cinfo->jpeg_color_space = JCS_GRAYSCALE;
    cinfo->out_color_space = JCS_GRAYSCALE;
    break;
    
  case 3:
    cid0 = cinfo->comp_info[0].component_id;
    cid1 = cinfo->comp_info[1].component_id;
    cid2 = cinfo->comp_info[2].component_id;

    //printf("\n cid0 = 0x%x ", cid0);
//	printf("\n cid1 = 0x%x ", cid1);
//	printf("\n cid2 = 0x%x ", cid2);
//	printf("\n cinfo->saw_JFIF_marker = 0x%x ", cinfo->saw_JFIF_marker);
//	printf("\n cinfo->saw_Adobe_marker = 0x%x ", cinfo->saw_Adobe_marker);
//	printf("\n cinfo->Adobe_transform = 0x%x ", cinfo->Adobe_transform);

    /* First try to guess from the component IDs */
    if      (cid0 == 0x01 && cid1 == 0x02 && cid2 == 0x03)
      cinfo->jpeg_color_space = JCS_YCbCr;
    else if (cid0 == 0x01 && cid1 == 0x22 && cid2 == 0x23)
      cinfo->jpeg_color_space = JCS_BG_YCC;
    else if (cid0 == 0x52 && cid1 == 0x47 && cid2 == 0x42)
      cinfo->jpeg_color_space = JCS_RGB;	/* ASCII 'R', 'G', 'B' */
    else if (cid0 == 0x72 && cid1 == 0x67 && cid2 == 0x62)
      cinfo->jpeg_color_space = JCS_BG_RGB;	/* ASCII 'r', 'g', 'b' */
    else if (cinfo->saw_JFIF_marker)
      cinfo->jpeg_color_space = JCS_YCbCr;	/* assume it's YCbCr */
    else if (cinfo->saw_Adobe_marker) {
      switch (cinfo->Adobe_transform) {
      case 0:
	cinfo->jpeg_color_space = JCS_RGB;
	break;
      case 1:
	cinfo->jpeg_color_space = JCS_YCbCr;
	break;
      default:
	WARNMS1(cinfo, JWRN_ADOBE_XFORM, cinfo->Adobe_transform);
	cinfo->jpeg_color_space = JCS_YCbCr;	/* assume it's YCbCr */
	break;
      }
    } else {
      TRACEMS3(cinfo, 1, JTRC_UNKNOWN_IDS, cid0, cid1, cid2);
      cinfo->jpeg_color_space = JCS_YCbCr;	/* assume it's YCbCr */
    }
    /* Always guess RGB is proper output colorspace. */
    cinfo->out_color_space = JCS_RGB;
    break;
    
  case 4:
    if (cinfo->saw_Adobe_marker) {
      switch (cinfo->Adobe_transform) {
      case 0:
	cinfo->jpeg_color_space = JCS_CMYK;
	break;
      case 2:
	cinfo->jpeg_color_space = JCS_YCCK;
	break;
      default:
	WARNMS1(cinfo, JWRN_ADOBE_XFORM, cinfo->Adobe_transform);
	cinfo->jpeg_color_space = JCS_YCCK;	/* assume it's YCCK */
	break;
      }
    } else {
      /* No special markers, assume straight CMYK. */
      cinfo->jpeg_color_space = JCS_CMYK;
    }
    cinfo->out_color_space = JCS_CMYK;
    break;
    
  default:
    cinfo->jpeg_color_space = JCS_UNKNOWN;
    cinfo->out_color_space = JCS_UNKNOWN;
    break;
  }

  /* Set defaults for other decompression parameters. */
  cinfo->scale_num = cinfo->block_size;		/* 1:1 scaling */
  cinfo->scale_denom = cinfo->block_size;
  cinfo->output_gamma = 1.0;
  cinfo->buffered_image = FALSE;
  cinfo->raw_data_out = FALSE;
  cinfo->dct_method = JDCT_DEFAULT;
  cinfo->do_fancy_upsampling = TRUE;
  cinfo->do_block_smoothing = TRUE;
  cinfo->quantize_colors = FALSE;
  /* We set these in case application only sets quantize_colors. */
  cinfo->dither_mode = JDITHER_FS;
#ifdef QUANT_2PASS_SUPPORTED
  cinfo->two_pass_quantize = TRUE;
#else
  cinfo->two_pass_quantize = FALSE;
#endif
  cinfo->desired_number_of_colors = 256;
  cinfo->colormap = NULL;
  /* Initialize for no mode change in buffered-image mode. */
  cinfo->enable_1pass_quant = FALSE;
  cinfo->enable_external_quant = FALSE;
  cinfo->enable_2pass_quant = FALSE;
}


/*
 * Decompression startup: read start of JPEG datastream to see what's there.
 * Need only initialize JPEG object and supply a data source before calling.
 *
 * This routine will read as far as the first SOS marker (ie, actual start of
 * compressed data), and will save all tables and parameters in the JPEG
 * object.  It will also initialize the decompression parameters to default
 * values, and finally return JPEG_HEADER_OK.  On return, the application may
 * adjust the decompression parameters and then call jpeg_start_decompress.
 * (Or, if the application only wanted to determine the image parameters,
 * the data need not be decompressed.  In that case, call jpeg_abort or
 * jpeg_destroy to release any temporary space.)
 * If an abbreviated (tables only) datastream is presented, the routine will
 * return JPEG_HEADER_TABLES_ONLY upon reaching EOI.  The application may then
 * re-use the JPEG object to read the abbreviated image datastream(s).
 * It is unnecessary (but OK) to call jpeg_abort in this case.
 * The JPEG_SUSPENDED return code only occurs if the data source module
 * requests suspension of the decompressor.  In this case the application
 * should load more source data and then re-call jpeg_read_header to resume
 * processing.
 * If a non-suspending data source is used and require_image is TRUE, then the
 * return code need not be inspected since only JPEG_HEADER_OK is possible.
 *
 * This routine is now just a front end to jpeg_consume_input, with some
 * extra error checking.
 */

//==============================================================================
// JPEG Decode Header
//==============================================================================
#define NVT_JPEG_SOI			0xD8

#define NVT_JPEG_APP0			0xE0
#define NVT_JPEG_APP1			0xE1
#define NVT_JPEG_APP2			0xE2
#define NVT_JPEG_APP3			0xE3
#define NVT_JPEG_APP4			0xE4
#define NVT_JPEG_APP5			0xE5
#define NVT_JPEG_APP6			0xE6
#define NVT_JPEG_APP7			0xE7
#define NVT_JPEG_APP8			0xE8
#define NVT_JPEG_APP9			0xE9
#define NVT_JPEG_APP10			0xEA
#define NVT_JPEG_APP11			0xEB
#define NVT_JPEG_APP12			0xEC
#define NVT_JPEG_APP13			0xED
#define NVT_JPEG_APP14			0xEE
#define NVT_JPEG_APP15			0xEF

#define NVT_JPEG_COM			0xFE

#define NVT_JPEG_DRI			0xDD

#define NVT_JPEG_DHT			0xC4

#define NVT_JPEG_DQT			0xDB

#define NVT_JPEG_SOF0			0xC0
#define NVT_JPEG_SOF2			0xC2

#define NVT_JPEG_SOS			0xDA

#define NVT_JPEG_EOI			0xD9

int
NvtJpeg_GetRealWidthRealHeightYUVFormat(
	j_decompress_ptr cinfo,
	unsigned char	*pbyImagebuf,
	unsigned short	*pu16RealWidth,
	unsigned short	*pu16RealHeight,
	unsigned short	*pu16YUVFormat
)
{
#if 0
	if ( pbyImagebuf == 0 )
	{
		printf( "### Decode - Error: parser buffer is null!\n" );
		return -1;
	}

	unsigned char	u8Data, u8Nf, u8c, u8i, au8HV[4]; // just for 4 bytes align, last element is dummy
	unsigned int	u32l, u32V, u32H, u32Index, u32FailCount;

	*pu16YUVFormat = *pu16RealWidth = *pu16RealHeight = 0xFFFF;
	u32Index = u32FailCount = 0;

	while ( 1 )
	{
		if ( u32FailCount++ >= 0x100000 )
		{
			u32FailCount = 0;
			printf( "### Decode - Error: parser time out!\n" );
			return -1;
		}

		u8Data = *(pbyImagebuf + u32Index);
		u32Index++;
		//Search Marker
		if ( u8Data != 0xFF)
			continue;
		while ( u8Data == 0xFF )
		{
			u8Data = *(pbyImagebuf + u32Index);
			u32Index++;
		}

		//Parsing Marker
		switch ( u8Data )
		{
		//--------------------
		//SOI
		//--------------------
		case NVT_JPEG_SOI:
			break;
		//--------------------
		//APPn, COM, DRI
		//--------------------
		case NVT_JPEG_APP0:
		case NVT_JPEG_APP1:
		case NVT_JPEG_APP2:
		case NVT_JPEG_APP3:
		case NVT_JPEG_APP4:
		case NVT_JPEG_APP5:
		case NVT_JPEG_APP6:
		case NVT_JPEG_APP7:
		case NVT_JPEG_APP8:
		case NVT_JPEG_APP9:
		case NVT_JPEG_APP10:
		case NVT_JPEG_APP11:
		case NVT_JPEG_APP12:
		case NVT_JPEG_APP13:
		case NVT_JPEG_APP14:
		case NVT_JPEG_APP15:
		case NVT_JPEG_COM:
		case NVT_JPEG_DRI:
			u32l = *(pbyImagebuf + u32Index) << 8;
			u32Index++;
			u32l += *(pbyImagebuf + u32Index) - 2;
			u32Index++;
			u32Index += u32l;
			break;
		//--------------------
		//DHT
		//--------------------
		case NVT_JPEG_DHT:
			u32l = *(pbyImagebuf + u32Index) << 8;
			u32Index++;
			u32l += *(pbyImagebuf + u32Index) - 2;
			u32Index++;
			u32Index += u32l;
			break;
		//--------------------
		//DQT
		//--------------------
		case NVT_JPEG_DQT:
			u32l = *(pbyImagebuf + u32Index) << 8;
			u32Index++;
			u32l += *(pbyImagebuf + u32Index) - 2;
			u32Index++;
			u32Index += u32l;
			break;
		//--------------------
		//SOF0
		//--------------------
		case NVT_JPEG_SOF0:
			u32Index += 2;
			if ( *(pbyImagebuf + u32Index) != 8 )
			{
				printf( "### Decode - Error: parser SOF0!\n" );
				return -1;
			}
			u32Index++;

			u32V = *(pbyImagebuf + u32Index) << 8;
			u32Index++;
			u32V += *(pbyImagebuf + u32Index);
			u32Index++;
			*pu16RealHeight = u32V;

			u32H = *(pbyImagebuf + u32Index) << 8;
			u32Index++;
			u32H += *(pbyImagebuf + u32Index);
			u32Index++;
			*pu16RealWidth = u32H;

			au8HV[0] = 0;
			au8HV[1] = 0;
			au8HV[2] = 0;

			u8Nf = *(pbyImagebuf + u32Index);
			u32Index++;
			for ( u8c = 0; u8c < u8Nf; u8c++ )
			{
				u8i = *(pbyImagebuf + u32Index) - 1;
				u32Index++;
				au8HV[u8c] = *(pbyImagebuf + u32Index);
				u32Index++;
				u32Index++;
			}

			switch ( au8HV[0] )
			{
			case 0x11:	//YUV444
				if ( (au8HV[1] == 0) && (au8HV[2] == 0) )
				{	//Y
					*pu16YUVFormat = DRVJPEG_DEC_GRAY;
				}
				else
				{	// YUV444
					*pu16YUVFormat = DRVJPEG_DEC_YUV444;
				}
				break;
			case 0x12:	//YUV422v
				*pu16YUVFormat = DRVJPEG_DEC_YUV422T;
				break;
			case 0x21:	//YUV422
				*pu16YUVFormat = DRVJPEG_DEC_YUV422;
				break;
			case 0x22:	//YUV420
				if ( au8HV[1] == 0x22 )
				{	//YUV444
					*pu16YUVFormat = DRVJPEG_DEC_YUV444;
				}
				else if ( au8HV[1] == 0x21 )
				{	//YUV422v
					*pu16YUVFormat = DRVJPEG_DEC_YUV422T;
				}
				else if ( au8HV[1] == 0x12 )
				{	//YUV422
					*pu16YUVFormat = DRVJPEG_DEC_YUV422;
				}
				else
				{	//YUV420
					*pu16YUVFormat = DRVJPEG_DEC_YUV420;
				}
				break;
			case 0x14:	//YUV411
			case 0x41:	//YUV411
				*pu16YUVFormat = DRVJPEG_DEC_YUV411;
				break;
			}

			return 0;
		//--------------------
		//SOF2
		//--------------------
		case NVT_JPEG_SOF2:
			u32Index += 2;
			if ( *(pbyImagebuf + u32Index) != 8 )
			{
				printf( "### Decode - Error: parser SOF2!\n" );
				return -1;
			}
			u32Index++;

			u32V = *(pbyImagebuf + u32Index) << 8;
			u32Index++;
			u32V += *(pbyImagebuf + u32Index);
			u32Index++;
			*pu16RealHeight = u32V;

			u32H = *(pbyImagebuf + u32Index) << 8;
			u32Index++;
			u32H += *(pbyImagebuf + u32Index);
			u32Index++;
			*pu16RealWidth = u32H;

			*pu16YUVFormat = NVT_JPEG_SOF2;
			return 0;
		//--------------------
		//SOS
		//--------------------
		case NVT_JPEG_SOS:
			if ( (*pu16YUVFormat == 0xFFFF) || (*pu16RealWidth == 0xFFFF) || (*pu16RealHeight == 0xFFFF) )
			{
				printf( "### Decode - Error: parser SOS!\n" );
				return -1;
			}
			else
			{
				return 0;
			}
		//--------------------
		//EOI
		//--------------------
		case NVT_JPEG_EOI:
			printf( "### Decode - Error: parser EOI!\n" );
			return -1;
		//--------------------
		//Default
		//--------------------
		default:
			break;
		}
	}
#else
	_demo_uninit_hwjpeg();
	WARNMS( cinfo, JWRN_16 );
	ERREXIT( cinfo, JERR_BAD_STATE );
#endif
}

GLOBAL(int)
jpeg_read_header (j_decompress_ptr cinfo, boolean require_image)
{
//    printf("\n jpeg_read_header \n");

	g_jpeg.isHWEnabled = 0;
	TRACEMS2( cinfo, 1, JTRC_17, g_jpeg.total_jpeg_buffer_size, g_jpeg.isHWEnabled );
#if 0
	if ( g_jpeg.isHWEnabled )
	{
	int total_len = 0;

	if ( g_jpeg.is_read_jpeg_from_mem == 0 )
	{
		int len;
		unsigned char *temp_ptr;
		temp_ptr = g_jpeg.jpeg_buff;
		fseek( g_jpeg.dfile_handle, 0, SEEK_SET );
		while ( !feof(g_jpeg.dfile_handle) )
		{
			len = fread( temp_ptr, 1, 4096, g_jpeg.dfile_handle );
			temp_ptr += len;
			total_len += len;
			if ( total_len >= g_jpeg.total_jpeg_buffer_size )
			{
				break;
			}
		}
		fseek( g_jpeg.dfile_handle, 0, SEEK_SET );
	}
	else
	{
		if ( g_jpeg.in_jpeg_buff_size > g_jpeg.total_jpeg_buffer_size )
			total_len = g_jpeg.total_jpeg_buffer_size;
		else
			total_len = g_jpeg.in_jpeg_buff_size;
		memcpy( g_jpeg.jpeg_buff, g_jpeg.in_jpeg_buff, total_len );
	}
	printf( "### Decode - Info: read JPEG len = %d, max is %d.\n", total_len, g_jpeg.total_jpeg_buffer_size );

	if ( NvtJpeg_GetRealWidthRealHeightYUVFormat( g_jpeg.jpeg_buff, &(g_jpeg.ori_width), &(g_jpeg.ori_height), &(g_jpeg.yuv_format) ) == 0 )
	{
		printf( "### Decode - Info: original width = %d, height = %d.\n", g_jpeg.ori_width, g_jpeg.ori_height );

		switch ( g_jpeg.yuv_format )
		{
		case DRVJPEG_DEC_GRAY:
			printf( "### Decode - Info: YUV format gray, changed to software JPEG decoder.\n" );
			g_jpeg.isHWEnabled = 0;	// sw enable
			if ( g_jpeg.fd_jpeg < 0 )
			{
				;
			}
			else
			{
///				if ( g_jpeg.jpeg_buff )
///					munmap( g_jpeg.jpeg_buff, g_jpeg.total_jpeg_buffer_size );
				close( g_jpeg.fd_jpeg );
				printf( "### HW JPEG Codec - Info: close JPEGCodec device.\n" );
			}
			break;
		case DRVJPEG_DEC_YUV444:
			printf( "### Decode - Info: YUV format yuv444.\n" );
			g_jpeg.isHWEnabled = 1;	// sw disable
			break;
		case DRVJPEG_DEC_YUV422T:
			printf( "### Decode - Info: YUV format yuv422t, changed to software JPEG decoder.\n" );
			g_jpeg.isHWEnabled = 0;	// sw enable
			if ( g_jpeg.fd_jpeg < 0 )
			{
				;
			}
			else
			{
///				if ( g_jpeg.jpeg_buff )
///					munmap( g_jpeg.jpeg_buff, g_jpeg.total_jpeg_buffer_size );
				close( g_jpeg.fd_jpeg );
				printf( "### HW JPEG Codec - Info: close JPEGCodec device.\n" );
			}
			break;
		case DRVJPEG_DEC_YUV422:
			printf( "### Decode - Info: YUV format yuv422.\n" );
			g_jpeg.isHWEnabled = 1;	// sw disable
			break;
		case DRVJPEG_DEC_YUV420:
			printf( "### Decode - Info: YUV format yuv420.\n" );
			g_jpeg.isHWEnabled = 1;	// sw disable
			break;
		case DRVJPEG_DEC_YUV411:
			printf( "### Decode - Info: YUV format yuv411, changed to software JPEG decoder.\n" );
			g_jpeg.isHWEnabled = 0;	// sw enable
			if ( g_jpeg.fd_jpeg < 0 )
			{
				;
			}
			else
			{
///				if ( g_jpeg.jpeg_buff )
///					munmap( g_jpeg.jpeg_buff, g_jpeg.total_jpeg_buffer_size );
				close( g_jpeg.fd_jpeg );
				printf( "### HW JPEG Codec - Info: close JPEGCodec device.\n" );
			}
			break;
		case NVT_JPEG_SOF2:
			printf( "### Decode - Info: progressive JPEG detected, changed to software JPEG decoder.\n" );
			g_jpeg.isHWEnabled = 0;	// sw enable
			if ( g_jpeg.fd_jpeg < 0 )
			{
				;
			}
			else
			{
///				if ( g_jpeg.jpeg_buff )
///					munmap( g_jpeg.jpeg_buff, g_jpeg.total_jpeg_buffer_size );
				close( g_jpeg.fd_jpeg );
				printf( "### HW JPEG Codec - Info: close JPEGCodec device.\n" );
			}
			break;
		default:
			printf( "### Decode - Error: YUV format unknown!\n" );
			g_jpeg.isHWEnabled = 0;	// sw enable
			if ( g_jpeg.fd_jpeg < 0 )
			{
				;
			}
			else
			{
///				if ( g_jpeg.jpeg_buff )
///					munmap( g_jpeg.jpeg_buff, g_jpeg.total_jpeg_buffer_size );
				close( g_jpeg.fd_jpeg );
				printf( "### HW JPEG Codec - Info: close JPEGCodec device.\n" );
			}
			return -1;
		}
	}
	else
	{
		printf( "### Decode - Error: can't get image siz in %5d byte bistream.\n", total_len );
		g_jpeg.isHWEnabled = 0;	// sw enable
		if ( g_jpeg.fd_jpeg < 0 )
		{
			;
		}
		else
		{
///			if ( g_jpeg.jpeg_buff )
///				munmap( g_jpeg.jpeg_buff, g_jpeg.total_jpeg_buffer_size );
			close( g_jpeg.fd_jpeg );
			printf( "### HW JPEG Codec - Info: close JPEGCodec device.\n" );
		}
		return -1;
	}
	}
#endif
///	if ( g_jpeg.isHWEnabled )
	{
///		cinfo->output_width = 0;
///		cinfo->output_height = 0;
///		cinfo->output_components = 0;
///		cinfo->scale_num = 0;
///		cinfo->scale_denom = 0;
///		cinfo->out_color_space = JCS_RGB;
	}
///	else
	{

  int retcode;

  if (cinfo->global_state != DSTATE_START &&
      cinfo->global_state != DSTATE_INHEADER)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);

  retcode = jpeg_consume_input(cinfo);

  switch (retcode) {
  case JPEG_REACHED_SOS:
//  	printf("\n JPEG_REACHED_SOS ");
//	printf("\n cinfo->output_components = 0x%x ", cinfo->output_components);
//	printf("\n cinfo->out_color_components = 0x%x ", cinfo->out_color_components);
//	printf("\n cinfo->out_color_space = 0x%x ", cinfo->out_color_space);
    retcode = JPEG_HEADER_OK;
    break;
  case JPEG_REACHED_EOI:
    if (require_image)		/* Complain if application wanted an image */
      ERREXIT(cinfo, JERR_NO_IMAGE);
    /* Reset to start state; it would be safer to require the application to
     * call jpeg_abort, but we can't change it now for compatibility reasons.
     * A side effect is to free any temporary memory (there shouldn't be any).
     */

//    printf("\n JPEG_REACHED_EOI ");
	
    jpeg_abort((j_common_ptr) cinfo); /* sets state = DSTATE_START */
    retcode = JPEG_HEADER_TABLES_ONLY;
    break;
  case JPEG_SUSPENDED:
    /* no work */
    break;
  }

  return retcode;

	}
}


/*
 * Consume data in advance of what the decompressor requires.
 * This can be called at any time once the decompressor object has
 * been created and a data source has been set up.
 *
 * This routine is essentially a state machine that handles a couple
 * of critical state-transition actions, namely initial setup and
 * transition from header scanning to ready-for-start_decompress.
 * All the actual input is done via the input controller's consume_input
 * method.
 */

GLOBAL(int)
jpeg_consume_input (j_decompress_ptr cinfo)
{
  int retcode = JPEG_SUSPENDED;

 // printf("\n jpeg_consume_input %d \n", cinfo->global_state);

  /* NB: every possible DSTATE value should be listed in this switch */
  switch (cinfo->global_state) {
  case DSTATE_START:
    /* Start-of-datastream actions: reset appropriate modules */
    (*cinfo->inputctl->reset_input_controller) (cinfo);
    /* Initialize application's data source module */
    (*cinfo->src->init_source) (cinfo);
#if 0
	printf( "### jpeg_consume_input - fill_input_buffer ###\n" );
	(*cinfo->src->fill_input_buffer) (cinfo);
	printf( "### [0] = 0x%x, [1] = 0x%x ###\n", cinfo->src->next_input_byte[0], cinfo->src->next_input_byte[1] );
	printf( "### %d ###\n", cinfo->src->bytes_in_buffer );
#endif
    cinfo->global_state = DSTATE_INHEADER;
    /*FALLTHROUGH*/
  case DSTATE_INHEADER:
    retcode = (*cinfo->inputctl->consume_input) (cinfo);
    if (retcode == JPEG_REACHED_SOS) { /* Found SOS, prepare to decompress */
      /* Set up default parameters based on header data */
      default_decompress_parms(cinfo);
      /* Set global state: ready for start_decompress */
      cinfo->global_state = DSTATE_READY;
    }
    break;
  case DSTATE_READY:
    /* Can't advance past first SOS until start_decompress is called */
    retcode = JPEG_REACHED_SOS;
    break;
  case DSTATE_PRELOAD:
  case DSTATE_PRESCAN:
  case DSTATE_SCANNING:
  case DSTATE_RAW_OK:
  case DSTATE_BUFIMAGE:
  case DSTATE_BUFPOST:
  case DSTATE_STOPPING:
    retcode = (*cinfo->inputctl->consume_input) (cinfo);
    break;
  default:
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  }
  return retcode;
}


/*
 * Have we finished reading the input file?
 */

GLOBAL(boolean)
jpeg_input_complete (j_decompress_ptr cinfo)
{
//  printf("\n jpeg_consume_input \n");

  /* Check for valid jpeg object */
  if (cinfo->global_state < DSTATE_START ||
      cinfo->global_state > DSTATE_STOPPING)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  return cinfo->inputctl->eoi_reached;
}


/*
 * Is there more than one scan?
 */

GLOBAL(boolean)
jpeg_has_multiple_scans (j_decompress_ptr cinfo)
{
//  printf("\n jpeg_has_multiple_scans \n");

  /* Only valid after jpeg_read_header completes */
  if (cinfo->global_state < DSTATE_READY ||
      cinfo->global_state > DSTATE_STOPPING)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  return cinfo->inputctl->has_multiple_scans;
}


/*
 * Finish JPEG decompression.
 *
 * This will normally just verify the file trailer and release temp storage.
 *
 * Returns FALSE if suspended.  The return value need be inspected only if
 * a suspending data source is used.
 */

GLOBAL(boolean)
jpeg_finish_decompress (j_decompress_ptr cinfo)
{
//    printf("\n jpeg_finish_decompress \n");

	if ( g_jpeg.isHWEnabled == 0 )
	{

  if ((cinfo->global_state == DSTATE_SCANNING ||
       cinfo->global_state == DSTATE_RAW_OK) && ! cinfo->buffered_image) {
    /* Terminate final pass of non-buffered mode */
    if (cinfo->output_scanline < cinfo->output_height)
      ERREXIT(cinfo, JERR_TOO_LITTLE_DATA);
    (*cinfo->master->finish_output_pass) (cinfo);
    cinfo->global_state = DSTATE_STOPPING;
  } else if (cinfo->global_state == DSTATE_BUFIMAGE) {
    /* Finishing after a buffered-image operation */
    cinfo->global_state = DSTATE_STOPPING;
  } else if (cinfo->global_state != DSTATE_STOPPING) {
    /* STOPPING = repeat call after a suspension, anything else is error */
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  }
  /* Read until EOI */
  while (! cinfo->inputctl->eoi_reached) {
    if ((*cinfo->inputctl->consume_input) (cinfo) == JPEG_SUSPENDED)
      return FALSE;		/* Suspend, come back later */
  }

	}

  /* Do final cleanup */
  (*cinfo->src->term_source) (cinfo);
  /* We can use jpeg_abort to release memory and reset global_state */
  jpeg_abort((j_common_ptr) cinfo);
  return TRUE;
}
