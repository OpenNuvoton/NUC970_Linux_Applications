/*
 * jdapistd.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * Modified 2002-2013 by Guido Vollbeding.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains application interface code for the decompression half
 * of the JPEG library.  These are the "standard" API routines that are
 * used in the normal full-decompression case.  They are not used by a
 * transcoding-only application.  Note that if an application links in
 * jpeg_start_decompress, it will end up linking in the entire decompressor.
 * We thus must separate this file from jdapimin.c to avoid linking the
 * whole decompression library into a transcoder.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/* Forward declarations */
LOCAL(boolean) output_pass_setup JPP((j_decompress_ptr cinfo));


/*
 * Decompression initialization.
 * jpeg_read_header must be completed before calling this.
 *
 * If a multipass operating mode was selected, this will do all but the
 * last pass, and thus may take a great deal of time.
 *
 * Returns FALSE if suspended.  The return value need be inspected only if
 * a suspending data source is used.
 */

extern S_JPEG g_jpeg;
///static int s_count1;
///static int s_count2;
GLOBAL(boolean)
jpeg_start_decompress (j_decompress_ptr cinfo)
{
	int temp1;///, iii;

    
_AGAIN_:

//	printf("\n jpeg_start_decompress \n");
//	printf("\n g_jpeg.isHWEnabled = %d \n", g_jpeg.isHWEnabled );


	if ( g_jpeg.isHWEnabled )
	{
  //      printf("\n cinfo->out_color_space = %d \n", cinfo->out_color_space);
//		printf("\n cinfo->output_components = %d \n", cinfo->output_components);
	
		switch ( cinfo->out_color_space )
		{
		case JCS_RGB:

  //          printf("\n JCS_RGB \n");
			
			if ( cinfo->output_components == 2 )
			{
				g_jpeg.dparam.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_RGB565;
				//cinfo->output_components = 2;
///printf( "### JCS_RGB 2 cinfo->output_components = %d ###\n", cinfo->output_components );
			}
			//else if ( cinfo->output_components == 4 )
			else if ( cinfo->output_components == 3 )
			{
				g_jpeg.dparam.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_RGB888;
				//cinfo->output_components = 4;
///printf( "### JCS_RGB 4 cinfo->output_components = %d ###\n", cinfo->output_components );
			}
			else
			{
//				if ( g_jpeg.var.bits_per_pixel == 16 )
//				{
					g_jpeg.dparam.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_RGB565;
					cinfo->output_components = 2;
///printf( "### JCS_RGB 3 cinfo->output_components = %d ###\n", cinfo->output_components );
//				}
//				else
//				{
//					g_jpeg.dparam.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_RGB888;
//					cinfo->output_components = 4;
///printf( "### JCS_RGB 3 cinfo->output_components = %d ###\n", cinfo->output_components );
//				}
			}	
			break;
		case JCS_YCbCr:
			g_jpeg.dparam.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_YUV422;
			cinfo->output_components = 2;
///printf( "### JCS_YCbCr cinfo->output_components = %d ###\n", cinfo->output_components );
			break;
		case JCS_GRAYSCALE:
			TRACEMS( cinfo, 1, JTRC_18 );
			g_jpeg.isHWEnabled = 0;
			_demo_uninit_hwjpeg();
			TRACEMS( cinfo, 1, JTRC_08 );
			goto _AGAIN_;
		default:
///			g_jpeg.dparam.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_RGB888;
///			cinfo->output_components = 4;
///			break;
			_demo_uninit_hwjpeg();
			WARNMS( cinfo, JWRN_17 );
			ERREXIT( cinfo, JERR_CONVERSION_NOTIMPL );
		}

		cinfo->output_scanline = 0;

///_AGAIN2_:

 //      printf("\n cinfo->scale_num = %d ", cinfo->scale_num);
 //      printf("\n cinfo->scale_denom = %d ", cinfo->scale_denom);

		if ( (cinfo->scale_num == 0) || (cinfo->scale_denom == 0) )
		{
			g_jpeg.dparam.scale = 0;
		}
		else
		{
			if ( cinfo->scale_num < cinfo->scale_denom )
			{
				float num, denom;
				num = cinfo->scale_num;
				denom = cinfo->scale_denom;
				num = num / denom;
				g_jpeg.dparam.scaled_width = (unsigned int)((float)g_jpeg.ori_width * num);
				g_jpeg.dparam.scaled_height = (unsigned int)((float)g_jpeg.ori_height * num);
				g_jpeg.dparam.scaled_width -= (g_jpeg.dparam.scaled_width % 2);
				if ( (g_jpeg.dparam.scaled_width == 0) || (g_jpeg.dparam.scaled_height == 0) )
				{
					g_jpeg.dparam.scale = 0;
				}
				else
				{
					g_jpeg.dparam.scale = 1;
					cinfo->output_width = g_jpeg.dparam.scaled_width;
					cinfo->output_height = g_jpeg.dparam.scaled_height;
					goto _FINISH_;
				}
			}
			if ( cinfo->scale_num == cinfo->scale_denom )
			{
				g_jpeg.dparam.scale = 0;
			}
			else
			{
				g_jpeg.dparam.scale = 0;
				g_jpeg.isHWEnabled = 0;
				TRACEMS( cinfo, 1, JTRC_19 );
				_demo_uninit_hwjpeg();
				TRACEMS( cinfo, 1, JTRC_08 );
				goto _AGAIN_;
			}
		}

		cinfo->output_width = g_jpeg.ori_width;
		cinfo->output_height = g_jpeg.ori_height;

_FINISH_:

///		if ( (cinfo->output_width > g_jpeg.var.xres) || (cinfo->output_height > g_jpeg.var.yres) )
///		{
///			cinfo->scale_denom++;
///			goto _AGAIN2_;
///		}

		temp1 = cinfo->output_width;
		if ( g_jpeg.dparam.scale == 0 )
		{
			if ( (g_jpeg.yuv_format == DRVJPEG_DEC_YUV420) || (g_jpeg.yuv_format == DRVJPEG_DEC_YUV422) )
			{
				if ( temp1 % 16 )
				{
					temp1 = (temp1 & 0xFFFFFFF0) + 16;
				}
///				printf( "420/422 width = %d.\n", temp1 );
			}
			else
			{	// DRVJPEG_DEC_YUV444
				if ( temp1 % 8 )
				{
					temp1 = (temp1 & 0xFFFFFFF8) + 8;
				}
///				printf( "444 width = %d.\n", temp1 );
			}
		}
		temp1 = (temp1 * cinfo->output_height * cinfo->output_components);
		if ( temp1 > g_jpeg.dparam.dst_bufsize )
		{
 //           printf("\n temp1 = %d ", temp1);
//			printf("\n g_jpeg.dparam.dst_bufsize = %d \n", g_jpeg.dparam.dst_bufsize);
		
			g_jpeg.dparam.scale = 0;
			g_jpeg.isHWEnabled = 0;
			cinfo->out_color_space = JCS_RGB;
			TRACEMS2( cinfo, 1, JTRC_20, temp1, g_jpeg.dparam.dst_bufsize );
			_demo_uninit_hwjpeg();
			TRACEMS( cinfo, 1, JTRC_08 );
			goto _AGAIN_;
		}

		unsigned int FactorLimit;
		if ( g_jpeg.yuv_format == DRVJPEG_DEC_YUV444 )
		{
			FactorLimit = 3;
		}
		else
		{
			FactorLimit = 4;
		}

		if ( ((cinfo->output_width << FactorLimit) <= g_jpeg.ori_width) || ((cinfo->output_height << FactorLimit) <= g_jpeg.ori_height) )
		{
  //         printf("\n (cinfo->output_width << FactorLimit) = %d ", (cinfo->output_width << FactorLimit));
  //         printf("\n g_jpeg.ori_width = %d");
//		   printf("\n (cinfo->output_height << FactorLimit) = %d ", (cinfo->output_height << FactorLimit));
//		   printf("\n g_jpeg.ori_height = %d \n", g_jpeg.ori_height);
		
			g_jpeg.isHWEnabled = 0;
			TRACEMS( cinfo, 1, JTRC_21 );
			_demo_uninit_hwjpeg();
			TRACEMS( cinfo, 1, JTRC_08 );
			goto _AGAIN_;
		}

///		if ( g_jpeg.is_read_jpeg_from_mem == 0 )
///			fseek( g_jpeg.dfile_handle, 0, SEEK_SET );
///		else if ( g_jpeg.is_read_jpeg_from_mem == -1 )
			/* Do final cleanup */
///			(*cinfo->src->term_source) (cinfo);
#if 0
		for ( iii = 0; iii < g_jpeg.is_read_jpeg_from_mem; iii++ )
			printf( "%02x\n", g_jpeg.in_jpeg_buff[iii] );
		printf( "### [0] = 0x%02x, [1] = 0x%02x ###\n", cinfo->src->next_input_byte[0], cinfo->src->next_input_byte[1] );
		printf( "### %d ###\n", cinfo->src->bytes_in_buffer );
#endif
///		memcpy( (unsigned char *)&g_jpeg.in_jpeg_buff[g_jpeg.is_read_jpeg_from_mem], (unsigned char *)&cinfo->src->next_input_byte[0], cinfo->src->bytes_in_buffer );
///		g_jpeg.is_read_jpeg_from_mem += cinfo->src->bytes_in_buffer;
///		s_count1 = g_jpeg.is_read_jpeg_from_mem;
///		FILE *tempfile;
///		tempfile = fopen( "test.jpg", "wb" );
///		fwrite( (unsigned char *)&g_jpeg.in_jpeg_buff[0], 1, g_jpeg.is_read_jpeg_from_mem, tempfile );
///		(*cinfo->src->fill_input_buffer) (cinfo);
///		while ( 1 )
///		{
///			fwrite( (unsigned char *)&cinfo->src->next_input_byte[0], 1, cinfo->src->bytes_in_buffer, tempfile );
///			printf( "### %d ###\n", cinfo->src->bytes_in_buffer );
///			for ( iii = 0; iii < cinfo->src->bytes_in_buffer; iii++ )
///				printf( "0x%02x\n", cinfo->src->next_input_byte[iii++] );
///			if ( (cinfo->src->next_input_byte[0] == 0xff) && (cinfo->src->next_input_byte[1] == JPEG_EOI) && (cinfo->src->bytes_in_buffer == 2) )
///				break;
///			else
///				(*cinfo->src->fill_input_buffer) (cinfo);
///		}
///		fclose( tempfile );
///		sync();
		TRACEMS2( cinfo, 1, JTRC_22, cinfo->output_width, cinfo->output_height );
///		for ( iii = 0; iii < g_jpeg.is_read_jpeg_from_mem + cinfo->src->bytes_in_buffer; iii++ )
///			printf( "%02x\n", g_jpeg.in_jpeg_buff[iii] );

        if(g_jpeg.dparam.decode_output_format == DRVJPEG_DEC_PRIMARY_PACKET_RGB888)
		{
            g_jpeg.row_stride = cinfo->output_width * 4;
		}
		else
		{
		    g_jpeg.row_stride = cinfo->output_width * cinfo->output_components;
        }
		
		g_jpeg.offset = 0;

		if ( (ioctl(g_jpeg.fd_jpeg, JPEG_S_PARAM, &(g_jpeg.dparam))) < 0 )
		{
			_demo_uninit_hwjpeg();
			WARNMS( cinfo, JWRN_18 );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}

		return TRUE;
	}
	else
	{

  if (cinfo->global_state == DSTATE_READY) {
    /* First call: initialize master control, select active modules */
    jinit_master_decompress(cinfo);
    if (cinfo->buffered_image) {
      /* No more work here; expecting jpeg_start_output next */
      cinfo->global_state = DSTATE_BUFIMAGE;
      return TRUE;
    }
    cinfo->global_state = DSTATE_PRELOAD;
  }
  if (cinfo->global_state == DSTATE_PRELOAD) {
    /* If file has multiple scans, absorb them all into the coef buffer */
    if (cinfo->inputctl->has_multiple_scans) {
#ifdef D_MULTISCAN_FILES_SUPPORTED
      for (;;) {
	int retcode;
	/* Call progress monitor hook if present */
	if (cinfo->progress != NULL)
	  (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
	/* Absorb some more input */
	retcode = (*cinfo->inputctl->consume_input) (cinfo);
	if (retcode == JPEG_SUSPENDED)
	  return FALSE;
	if (retcode == JPEG_REACHED_EOI)
	  break;
	/* Advance progress counter if appropriate */
	if (cinfo->progress != NULL &&
	    (retcode == JPEG_ROW_COMPLETED || retcode == JPEG_REACHED_SOS)) {
	  if (++cinfo->progress->pass_counter >= cinfo->progress->pass_limit) {
	    /* jdmaster underestimated number of scans; ratchet up one scan */
	    cinfo->progress->pass_limit += (long) cinfo->total_iMCU_rows;
	  }
	}
      }
#else
      ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif /* D_MULTISCAN_FILES_SUPPORTED */
    }
    cinfo->output_scan_number = cinfo->input_scan_number;
  } else if (cinfo->global_state != DSTATE_PRESCAN)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  /* Perform any dummy output passes, and set up for the final pass */
  return output_pass_setup(cinfo);

	}
}


/*
 * Set up for an output pass, and perform any dummy pass(es) needed.
 * Common subroutine for jpeg_start_decompress and jpeg_start_output.
 * Entry: global_state = DSTATE_PRESCAN only if previously suspended.
 * Exit: If done, returns TRUE and sets global_state for proper output mode.
 *       If suspended, returns FALSE and sets global_state = DSTATE_PRESCAN.
 */

LOCAL(boolean)
output_pass_setup (j_decompress_ptr cinfo)
{
  if (cinfo->global_state != DSTATE_PRESCAN) {
    /* First call: do pass setup */
    (*cinfo->master->prepare_for_output_pass) (cinfo);
    cinfo->output_scanline = 0;
    cinfo->global_state = DSTATE_PRESCAN;
  }
  /* Loop over any required dummy passes */
  while (cinfo->master->is_dummy_pass) {
#ifdef QUANT_2PASS_SUPPORTED
    /* Crank through the dummy pass */
    while (cinfo->output_scanline < cinfo->output_height) {
      JDIMENSION last_scanline;
      /* Call progress monitor hook if present */
      if (cinfo->progress != NULL) {
	cinfo->progress->pass_counter = (long) cinfo->output_scanline;
	cinfo->progress->pass_limit = (long) cinfo->output_height;
	(*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
      }
      /* Process some data */
      last_scanline = cinfo->output_scanline;
      (*cinfo->main->process_data) (cinfo, (JSAMPARRAY) NULL,
				    &cinfo->output_scanline, (JDIMENSION) 0);
      if (cinfo->output_scanline == last_scanline)
	return FALSE;		/* No progress made, must suspend */
    }
    /* Finish up dummy pass, and set up for another one */
    (*cinfo->master->finish_output_pass) (cinfo);
    (*cinfo->master->prepare_for_output_pass) (cinfo);
    cinfo->output_scanline = 0;
#else
    ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif /* QUANT_2PASS_SUPPORTED */
  }
  /* Ready for application to drive output pass through
   * jpeg_read_scanlines or jpeg_read_raw_data.
   */
  cinfo->global_state = cinfo->raw_data_out ? DSTATE_RAW_OK : DSTATE_SCANNING;
  return TRUE;
}


/*
 * Read some scanlines of data from the JPEG decompressor.
 *
 * The return value will be the number of lines actually read.
 * This may be less than the number requested in several cases,
 * including bottom of image, data source suspension, and operating
 * modes that emit multiple scanlines at a time.
 *
 * Note: we warn about excess calls to jpeg_read_scanlines() since
 * this likely signals an application programmer error.  However,
 * an oversize buffer (max_lines > scanlines remaining) is not an error.
 */

GLOBAL(JDIMENSION)
jpeg_read_scanlines (j_decompress_ptr cinfo, JSAMPARRAY scanlines,
		     JDIMENSION max_lines)
{
//    printf("\n jpeg_read_scanlines \n");

//	printf("\n g_jpeg.isHWEnabled = %d ", g_jpeg.isHWEnabled);

	if ( g_jpeg.isHWEnabled )
	{
		if ( scanlines[0] == 0 )
		{
			_demo_uninit_hwjpeg();
			WARNMS( cinfo, JWRN_19 );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}

		if ( cinfo->output_scanline == 0 )
		{
			unsigned int jpeg_buffer_state, bTrigger;
			int readSize, jpeg_buffersize, jpeg_buffersize2;

			bTrigger = 0;

			jpeg_state_t	state;

///			printf( "### HW JPEG Decode - Info: JPEG header size = %d.\n", g_jpeg.is_read_jpeg_from_mem );

			while ( (state != JPEG_DECODED_IMAGE) || (state != JPEG_MEM_SHORTAGE) )
			{
				/* Check Buffer status */
				do
				{
					/* Get Decode Input Wait Buffer State */
					ioctl( g_jpeg.fd_jpeg, JPEG_DECIPW_BUFFER_STATE, &jpeg_buffer_state );

					/* Get Decode state */
					ioctl( g_jpeg.fd_jpeg, JPEG_STATE, (unsigned int)&(state) );
				}
				while ( (jpeg_buffer_state != JPEG_DECIPW_BUFFER_EMPTY) && (state != JPEG_DECODED_IMAGE) && (state != JPEG_MEM_SHORTAGE) );

				if ( (state == JPEG_DECODED_IMAGE) || (state == JPEG_MEM_SHORTAGE) )	/* Decode Complete!! */
					break;
#if 0
				if ( s_count2 > 0 )
				{
					memcpy( (unsigned char *)&g_jpeg.in_jpeg_buff[g_jpeg.is_read_jpeg_from_mem], (unsigned char *)&cinfo->src->next_input_byte[cinfo->src->bytes_in_buffer - s_count2], s_count2 );
					g_jpeg.is_read_jpeg_from_mem += s_count2;
					s_count1 = g_jpeg.is_read_jpeg_from_mem;
					s_count2 = -1;
				}
#endif
				/* Get available Buffer size */
				if ( (ioctl(g_jpeg.fd_jpeg, JPEG_G_DECIPW_BUFFER_SIZE, &jpeg_buffersize)) < 0 )
				{
					_demo_uninit_hwjpeg();
					WARNMS( cinfo, JWRN_20 );
					ERREXIT( cinfo, JERR_BAD_STATE );
				}
///				else
///					printf( "### HW JPEG Decode - Info: jpeg_buffersize = %d\n", jpeg_buffersize );
#if 0
				/* Read "jpeg_buffersize" Bytes Bitstream from file to tmp buffer "buffer" */
				readSize = fread(	(g_jpeg.jpeg_buff + g_jpeg.dparam.src_bufsize + g_jpeg.dparam.dst_bufsize),
									1,
									jpeg_buffersize,
									g_jpeg.dfile_handle
									);
#else
///printf( "### 1 ###\n" );
				jpeg_buffersize2 = jpeg_buffersize;
				jpeg_buffersize -= g_jpeg.is_read_jpeg_from_mem;
				while ( jpeg_buffersize > 0 )
				{
				if ( cinfo->src->bytes_in_buffer == 0 )
				{
///printf( "### 2 ###\n" );
					(*cinfo->src->fill_input_buffer) (cinfo);
					if ( (cinfo->src->next_input_byte[0] == 0xff) && (cinfo->src->next_input_byte[1] == JPEG_EOI) && (cinfo->src->bytes_in_buffer == 2) )
						break;
				}
				if ( cinfo->src->bytes_in_buffer <= (jpeg_buffersize2 - g_jpeg.is_read_jpeg_from_mem) )
				{
///printf( "### 4 ###\n" );
					memcpy( (unsigned char *)&g_jpeg.in_jpeg_buff[g_jpeg.is_read_jpeg_from_mem], (unsigned char *)&cinfo->src->next_input_byte[0], cinfo->src->bytes_in_buffer );
					g_jpeg.is_read_jpeg_from_mem += cinfo->src->bytes_in_buffer;
					jpeg_buffersize -= cinfo->src->bytes_in_buffer;
					cinfo->src->next_input_byte += cinfo->src->bytes_in_buffer;
					cinfo->src->bytes_in_buffer -= cinfo->src->bytes_in_buffer;
				}
				else
				{
///printf( "### 5 ###\n" );
					memcpy( (unsigned char *)&g_jpeg.in_jpeg_buff[g_jpeg.is_read_jpeg_from_mem], (unsigned char *)&cinfo->src->next_input_byte[0], (jpeg_buffersize2 - g_jpeg.is_read_jpeg_from_mem) );
					jpeg_buffersize -= (jpeg_buffersize2 - g_jpeg.is_read_jpeg_from_mem);
					cinfo->src->next_input_byte += (jpeg_buffersize2 - g_jpeg.is_read_jpeg_from_mem);
					cinfo->src->bytes_in_buffer -= (jpeg_buffersize2 - g_jpeg.is_read_jpeg_from_mem);
					g_jpeg.is_read_jpeg_from_mem += (jpeg_buffersize2 - g_jpeg.is_read_jpeg_from_mem);
				}
				}
				readSize = g_jpeg.is_read_jpeg_from_mem;
				g_jpeg.is_read_jpeg_from_mem = 0;
#if 0
				readSize = 0;
				while ( jpeg_buffersize > 0 )
				{
					(*cinfo->src->fill_input_buffer) (cinfo);
///					printf( "### cinfo->src->bytes_in_buffer = %d, jpeg_buffersize = %d\n", cinfo->src->bytes_in_buffer, jpeg_buffersize );
					if ( (cinfo->src->next_input_byte[0] == 0xff) && (cinfo->src->next_input_byte[1] == JPEG_EOI) && (cinfo->src->bytes_in_buffer == 2) )
						break;
					if ( cinfo->src->bytes_in_buffer <= jpeg_buffersize )
					{
						memcpy( (unsigned char *)&g_jpeg.in_jpeg_buff[g_jpeg.is_read_jpeg_from_mem], (unsigned char *)&cinfo->src->next_input_byte[0], cinfo->src->bytes_in_buffer );
						g_jpeg.is_read_jpeg_from_mem += cinfo->src->bytes_in_buffer;
						s_count1 += cinfo->src->bytes_in_buffer;
						jpeg_buffersize -= s_count1;
						readSize += s_count1;
					}
					else
					{
						memcpy( (unsigned char *)&g_jpeg.in_jpeg_buff[g_jpeg.is_read_jpeg_from_mem], (unsigned char *)&cinfo->src->next_input_byte[0], jpeg_buffersize );
						g_jpeg.is_read_jpeg_from_mem += jpeg_buffersize;
						s_count1 += jpeg_buffersize;
						jpeg_buffersize -= s_count1;
						readSize += s_count1;
						s_count2 = cinfo->src->bytes_in_buffer - s_count1;
					}
					s_count1 = 0;
				}
				if ( (jpeg_buffersize == g_jpeg.dparam.decInWait_buffer_size) && (readSize == 0) && (g_jpeg.is_read_jpeg_from_mem != 0) )
					readSize = g_jpeg.is_read_jpeg_from_mem;
				g_jpeg.is_read_jpeg_from_mem = 0;
#endif
#if 0
				FILE *tempfile;
				tempfile = fopen( "test.jpg", "wb" );
				fwrite( (unsigned char *)&g_jpeg.in_jpeg_buff[0], 1, readSize, tempfile );
				fclose( tempfile );
				sync();
				while(1);
#endif
#endif
///				printf( "### HW JPEG Decode - Info: readSize = %d\n", readSize );
				if ( readSize != 0 )
				{
					/* Write "jpeg_buffersize" Bytes Bitstream from tmp buffer "buffer" to JPEG engine buffer */
					write(	g_jpeg.fd_jpeg,
							(g_jpeg.jpeg_buff + g_jpeg.dparam.src_bufsize + g_jpeg.dparam.dst_bufsize),
							readSize 
							);
				}

				if ( !bTrigger )
				{
					/* Trigger JPEG engine */
					if ( (ioctl(g_jpeg.fd_jpeg, JPEG_TRIGGER, NULL)) < 0 )
					{
						_demo_uninit_hwjpeg();
						WARNMS( cinfo, JWRN_21 );
						ERREXIT( cinfo, JERR_BAD_STATE );
					}
					bTrigger = 1;
				}
				else
					ioctl( g_jpeg.fd_jpeg, JPEG_DECODE_RESUME, NULL );	/* Resume the JPEG decode operation */
			}

			/* Get JPEG decode information */
			if ( read( g_jpeg.fd_jpeg, &(g_jpeg.jpeginfo), (sizeof(jpeg_info_t) + sizeof(unsigned int)) ) < 0 )
			{
				_demo_uninit_hwjpeg();
				WARNMS( cinfo, JWRN_22 );
				ERREXIT( cinfo, JERR_BAD_STATE );
			}

			if ( g_jpeg.jpeginfo.state == JPEG_DECODED_IMAGE )
			{
				if ( g_jpeg.dparam.scale == 1 )
					g_jpeg.offset = 0;
				else
				{
                    if(g_jpeg.dparam.decode_output_format == DRVJPEG_DEC_PRIMARY_PACKET_RGB888)
					{
						g_jpeg.offset = (g_jpeg.jpeginfo.width - cinfo->output_width) * 4;
					}
					else
					    g_jpeg.offset = (g_jpeg.jpeginfo.width - cinfo->output_width) * cinfo->output_components;

				}
				TRACEMS2( cinfo, 1, JTRC_23, g_jpeg.jpeginfo.width, g_jpeg.jpeginfo.height );
				TRACEMS2( cinfo, 1, JTRC_24, g_jpeg.jpeginfo.image_size[0], g_jpeg.dparam.dst_bufsize );
			}
			else if ( g_jpeg.jpeginfo.state == JPEG_DECODE_ERROR )
			{
				_demo_uninit_hwjpeg();
				WARNMS( cinfo, JWRN_23 );
				ERREXIT( cinfo, JERR_BAD_STATE );
			}
			else if ( g_jpeg.jpeginfo.state == JPEG_MEM_SHORTAGE )
			{
				_demo_uninit_hwjpeg();
				WARNMS( cinfo, JWRN_24 );
				ERREXIT( cinfo, JERR_BAD_STATE );
			}
		}

       // printf("\n g_jpeg.offset = %d ", g_jpeg.offset);
//		printf("\n g_jpeg.row_stride = %d ", g_jpeg.row_stride);

		int i;
		int j;

        if(g_jpeg.dparam.decode_output_format == DRVJPEG_DEC_PRIMARY_PACKET_RGB888)
		{
			for ( i = 0; i < max_lines; ++i )
			{
            	unsigned char *in = g_jpeg.temp_dptr;
            	unsigned char *out = scanlines[i];
            	for (int j = 0; j < (g_jpeg.row_stride/4); ++j)
				{
                	*out ++ = in[2];
					*out ++ = in[1];
					*out ++ = in[0];
                	in += 4;
            	}

				g_jpeg.temp_dptr += (g_jpeg.row_stride + g_jpeg.offset);
				if ( ++cinfo->output_scanline >= cinfo->image_height )
				{
					break;
				}
			}
		}
		else
		{
			for ( i = 0; i < max_lines; ++i )
			{
				memcpy( scanlines[i], g_jpeg.temp_dptr, g_jpeg.row_stride );
				g_jpeg.temp_dptr += (g_jpeg.row_stride + g_jpeg.offset);
				if ( ++cinfo->output_scanline >= cinfo->image_height )
				{
					break;
				}					
			}
		}
		
		return cinfo->output_scanline;
	}
	else
	{

  JDIMENSION row_ctr;

  if (cinfo->global_state != DSTATE_SCANNING)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  if (cinfo->output_scanline >= cinfo->output_height) {
    WARNMS(cinfo, JWRN_TOO_MUCH_DATA);
    return 0;
  }

  /* Call progress monitor hook if present */
  if (cinfo->progress != NULL) {
    cinfo->progress->pass_counter = (long) cinfo->output_scanline;
    cinfo->progress->pass_limit = (long) cinfo->output_height;
    (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
  }

  /* Process some data */
  row_ctr = 0;
  (*cinfo->main->process_data) (cinfo, scanlines, &row_ctr, max_lines);
  cinfo->output_scanline += row_ctr;
  return row_ctr;

	}
}


/*
 * Alternate entry point to read raw data.
 * Processes exactly one iMCU row per call, unless suspended.
 */

GLOBAL(JDIMENSION)
jpeg_read_raw_data (j_decompress_ptr cinfo, JSAMPIMAGE data,
		    JDIMENSION max_lines)
{
  JDIMENSION lines_per_iMCU_row;

  if (cinfo->global_state != DSTATE_RAW_OK)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  if (cinfo->output_scanline >= cinfo->output_height) {
    WARNMS(cinfo, JWRN_TOO_MUCH_DATA);
    return 0;
  }

  /* Call progress monitor hook if present */
  if (cinfo->progress != NULL) {
    cinfo->progress->pass_counter = (long) cinfo->output_scanline;
    cinfo->progress->pass_limit = (long) cinfo->output_height;
    (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
  }

  /* Verify that at least one iMCU row can be returned. */
  lines_per_iMCU_row = cinfo->max_v_samp_factor * cinfo->min_DCT_v_scaled_size;
  if (max_lines < lines_per_iMCU_row)
    ERREXIT(cinfo, JERR_BUFFER_SIZE);

  /* Decompress directly into user's buffer. */
  if (! (*cinfo->coef->decompress_data) (cinfo, data))
    return 0;			/* suspension forced, can do nothing more */

  /* OK, we processed one iMCU row. */
  cinfo->output_scanline += lines_per_iMCU_row;
  return lines_per_iMCU_row;
}


/* Additional entry points for buffered-image mode. */

#ifdef D_MULTISCAN_FILES_SUPPORTED

/*
 * Initialize for an output pass in buffered-image mode.
 */

GLOBAL(boolean)
jpeg_start_output (j_decompress_ptr cinfo, int scan_number)
{
  if (cinfo->global_state != DSTATE_BUFIMAGE &&
      cinfo->global_state != DSTATE_PRESCAN)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  /* Limit scan number to valid range */
  if (scan_number <= 0)
    scan_number = 1;
  if (cinfo->inputctl->eoi_reached &&
      scan_number > cinfo->input_scan_number)
    scan_number = cinfo->input_scan_number;
  cinfo->output_scan_number = scan_number;
  /* Perform any dummy output passes, and set up for the real pass */
  return output_pass_setup(cinfo);
}


/*
 * Finish up after an output pass in buffered-image mode.
 *
 * Returns FALSE if suspended.  The return value need be inspected only if
 * a suspending data source is used.
 */

GLOBAL(boolean)
jpeg_finish_output (j_decompress_ptr cinfo)
{
  if ((cinfo->global_state == DSTATE_SCANNING ||
       cinfo->global_state == DSTATE_RAW_OK) && cinfo->buffered_image) {
    /* Terminate this pass. */
    /* We do not require the whole pass to have been completed. */
    (*cinfo->master->finish_output_pass) (cinfo);
    cinfo->global_state = DSTATE_BUFPOST;
  } else if (cinfo->global_state != DSTATE_BUFPOST) {
    /* BUFPOST = repeat call after a suspension, anything else is error */
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  }
  /* Read markers looking for SOS or EOI */
  while (cinfo->input_scan_number <= cinfo->output_scan_number &&
	 ! cinfo->inputctl->eoi_reached) {
    if ((*cinfo->inputctl->consume_input) (cinfo) == JPEG_SUSPENDED)
      return FALSE;		/* Suspend, come back later */
  }
  cinfo->global_state = DSTATE_BUFIMAGE;
  return TRUE;
}

#endif /* D_MULTISCAN_FILES_SUPPORTED */
