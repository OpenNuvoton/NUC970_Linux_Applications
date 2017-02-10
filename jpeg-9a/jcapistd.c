/*
 * jcapistd.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * Modified 2013 by Guido Vollbeding.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains application interface code for the compression half
 * of the JPEG library.  These are the "standard" API routines that are
 * used in the normal full-compression case.  They are not used by a
 * transcoding-only application.  Note that if an application links in
 * jpeg_start_compress, it will end up linking in the entire compressor.
 * We thus must separate this file from jcapimin.c to avoid linking the
 * whole compression library into a transcoder.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/*
 * Compression initialization.
 * Before calling this, all parameters and a data destination must be set up.
 *
 * We require a write_all_tables parameter as a failsafe check when writing
 * multiple datastreams from the same compression object.  Since prior runs
 * will have left all the tables marked sent_table=TRUE, a subsequent run
 * would emit an abbreviated stream (no tables) by default.  This may be what
 * is wanted, but for safety's sake it should not be the default behavior:
 * programmers should have to make a deliberate choice to emit abbreviated
 * images.  Therefore the documentation and examples should encourage people
 * to pass write_all_tables=TRUE; then it will take active thought to do the
 * wrong thing.
 */

extern S_JPEG g_jpeg;

GLOBAL(void)
jpeg_start_compress (j_compress_ptr cinfo, boolean write_all_tables)
{

_AGAIN_:

	if ( g_jpeg.isHWEnabled )
	{
		if ( (cinfo->comp_info->h_samp_factor == 2) && (cinfo->comp_info->v_samp_factor == 2) )
		{
			g_jpeg.eparam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;
			goto _AGAIN2_;
		}

		if ( (cinfo->comp_info->h_samp_factor == 2) && (cinfo->comp_info->v_samp_factor == 1) )
		{
			g_jpeg.eparam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;
			goto _AGAIN2_;
		}

		g_jpeg.isHWEnabled = 0;
		TRACEMS( cinfo, 1, JTRC_07 );
		_demo_uninit_hwjpeg();
		TRACEMS( cinfo, 1, JTRC_08 );
		goto _AGAIN_;

_AGAIN2_:

		if ( (cinfo->scale_num == 0) || (cinfo->scale_denom == 0) )
		{
			g_jpeg.eparam.scale = 0;
		}
		else
		{
			if ( cinfo->scale_num > cinfo->scale_denom )
			{
				float num, denom;
				num = cinfo->scale_num;
				denom = cinfo->scale_denom;
				num = num / denom;
				g_jpeg.eparam.scaled_width = (unsigned int)((float)cinfo->image_width * num);
				g_jpeg.eparam.scaled_height = (unsigned int)((float)cinfo->image_height * num);
				if ( (g_jpeg.eparam.scaled_width == 0) || (g_jpeg.eparam.scaled_height == 0) )
				{
					g_jpeg.eparam.scale = 0;
				}
				else
				{
					if ( ((cinfo->image_width << 3) < g_jpeg.eparam.scaled_width) || ((cinfo->image_height << 3) < g_jpeg.eparam.scaled_height) )
					{
						g_jpeg.isHWEnabled = 0;
						TRACEMS( cinfo, 1, JTRC_09 );
						_demo_uninit_hwjpeg();
						TRACEMS( cinfo, 1, JTRC_08 );
						goto _AGAIN_;
					}
					else
					{
						g_jpeg.eparam.scale = 1;
					}
				}
			}
			else if ( cinfo->scale_num == cinfo->scale_denom )
			{
				g_jpeg.eparam.scale = 0;
			}
			else
			{
				g_jpeg.isHWEnabled = 0;
				TRACEMS( cinfo, 1, JTRC_09 );
				_demo_uninit_hwjpeg();
				TRACEMS( cinfo, 1, JTRC_08 );
				goto _AGAIN_;
			}
		}

		g_jpeg.row_stride = cinfo->image_width * 2;
		g_jpeg.temp_eptr = g_jpeg.jpeg_buff;

		if ( (ioctl(g_jpeg.fd_jpeg, JPEG_S_PARAM, &(g_jpeg.eparam))) < 0 )
		{
			_demo_uninit_hwjpeg();
			WARNMS( cinfo, JWRN_09 );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}
	}

  if (cinfo->global_state != CSTATE_START)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);

  if (write_all_tables)
    jpeg_suppress_tables(cinfo, FALSE);	/* mark all tables to be written */

  /* (Re)initialize error mgr and destination modules */
  (*cinfo->err->reset_error_mgr) ((j_common_ptr) cinfo);
  (*cinfo->dest->init_destination) (cinfo);
  /* Perform master selection of active modules */
  jinit_compress_master(cinfo);
  /* Set up for the first pass */
  (*cinfo->master->prepare_for_pass) (cinfo);
  /* Ready for application to drive first pass through jpeg_write_scanlines
   * or jpeg_write_raw_data.
   */
  cinfo->next_scanline = 0;
  cinfo->global_state = (cinfo->raw_data_in ? CSTATE_RAW_OK : CSTATE_SCANNING);
}


/*
 * Write some scanlines of data to the JPEG compressor.
 *
 * The return value will be the number of lines actually written.
 * This should be less than the supplied num_lines only in case that
 * the data destination module has requested suspension of the compressor,
 * or if more than image_height scanlines are passed in.
 *
 * Note: we warn about excess calls to jpeg_write_scanlines() since
 * this likely signals an application programmer error.  However,
 * excess scanlines passed in the last valid call are *silently* ignored,
 * so that the application need not adjust num_lines for end-of-image
 * when using a multiple-scanline buffer.
 */

GLOBAL(JDIMENSION)
jpeg_write_scanlines (j_compress_ptr cinfo, JSAMPARRAY scanlines,
		      JDIMENSION num_lines)
{
	if ( g_jpeg.isHWEnabled )
	{
		if ( scanlines[0] == 0 )
		{
			_demo_uninit_hwjpeg();
			WARNMS( cinfo, JWRN_10 );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}

		int i;
		for ( i = 0; i < num_lines; ++i )
		{
			memcpy( g_jpeg.temp_eptr, scanlines[i], g_jpeg.row_stride );
			g_jpeg.temp_eptr += g_jpeg.row_stride;
			if ( ++cinfo->next_scanline >= cinfo->image_height )
			{
				break;
			}
		}

		return cinfo->next_scanline;
	}
	else
	{

  JDIMENSION row_ctr, rows_left;

  if (cinfo->global_state != CSTATE_SCANNING)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  if (cinfo->next_scanline >= cinfo->image_height)
    WARNMS(cinfo, JWRN_TOO_MUCH_DATA);

  /* Call progress monitor hook if present */
  if (cinfo->progress != NULL) {
    cinfo->progress->pass_counter = (long) cinfo->next_scanline;
    cinfo->progress->pass_limit = (long) cinfo->image_height;
    (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
  }

  /* Give master control module another chance if this is first call to
   * jpeg_write_scanlines.  This lets output of the frame/scan headers be
   * delayed so that application can write COM, etc, markers between
   * jpeg_start_compress and jpeg_write_scanlines.
   */
  if (cinfo->master->call_pass_startup)
    (*cinfo->master->pass_startup) (cinfo);

  /* Ignore any extra scanlines at bottom of image. */
  rows_left = cinfo->image_height - cinfo->next_scanline;
  if (num_lines > rows_left)
    num_lines = rows_left;

  row_ctr = 0;
  (*cinfo->main->process_data) (cinfo, scanlines, &row_ctr, num_lines);
  cinfo->next_scanline += row_ctr;
  return row_ctr;

	}
}


/*
 * Alternate entry point to write raw data.
 * Processes exactly one iMCU row per call, unless suspended.
 */

GLOBAL(JDIMENSION)
jpeg_write_raw_data (j_compress_ptr cinfo, JSAMPIMAGE data,
		     JDIMENSION num_lines)
{
  JDIMENSION lines_per_iMCU_row;

  if (cinfo->global_state != CSTATE_RAW_OK)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  if (cinfo->next_scanline >= cinfo->image_height) {
    WARNMS(cinfo, JWRN_TOO_MUCH_DATA);
    return 0;
  }

  /* Call progress monitor hook if present */
  if (cinfo->progress != NULL) {
    cinfo->progress->pass_counter = (long) cinfo->next_scanline;
    cinfo->progress->pass_limit = (long) cinfo->image_height;
    (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
  }

  /* Give master control module another chance if this is first call to
   * jpeg_write_raw_data.  This lets output of the frame/scan headers be
   * delayed so that application can write COM, etc, markers between
   * jpeg_start_compress and jpeg_write_raw_data.
   */
  if (cinfo->master->call_pass_startup)
    (*cinfo->master->pass_startup) (cinfo);

  /* Verify that at least one iMCU row has been passed. */
  lines_per_iMCU_row = cinfo->max_v_samp_factor * cinfo->min_DCT_v_scaled_size;
  if (num_lines < lines_per_iMCU_row)
    ERREXIT(cinfo, JERR_BUFFER_SIZE);

  /* Directly compress the row. */
  if (! (*cinfo->coef->compress_data) (cinfo, data)) {
    /* If compressor did not consume the whole row, suspend processing. */
    return 0;
  }

  /* OK, we processed one iMCU row. */
  cinfo->next_scanline += lines_per_iMCU_row;
  return lines_per_iMCU_row;
}
