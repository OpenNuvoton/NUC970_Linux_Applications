/*
 * jcapimin.c
 *
 * Copyright (C) 1994-1998, Thomas G. Lane.
 * Modified 2003-2010 by Guido Vollbeding.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains application interface code for the compression half
 * of the JPEG library.  These are the "minimum" API routines that may be
 * needed in either the normal full-compression case or the transcoding-only
 * case.
 *
 * Most of the routines intended to be called directly by an application
 * are in this file or in jcapistd.c.  But also see jcparam.c for
 * parameter-setup helper routines, jcomapi.c for routines shared by
 * compression and decompression, and jctrans.c for the transcoding case.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"

#include <dirent.h>

/*
 * Initialization of a JPEG compression object.
 * The error manager must already be set up (in case memory manager fails).
 */

extern S_JPEG g_jpeg;

void _demo_uninit_hwjpeg(void)
{
	if ( g_jpeg.jpeg_buff )
	{
		munmap( g_jpeg.jpeg_buff, g_jpeg.total_jpeg_buffer_size );
		g_jpeg.jpeg_buff = NULL;
	}

	if ( g_jpeg.fd_jpeg > 0 )
	{
		close( g_jpeg.fd_jpeg );
		g_jpeg.fd_jpeg = -1;
	}
}

GLOBAL(void)
jpeg_CreateCompress (j_compress_ptr cinfo, int version, size_t structsize)
{
	DIR *dir;
//	int lcm_fd;

	unsigned char device[] = "/dev/video0";

    printf("\n jpeg_CreateCompress \n");

	TRACEMS( cinfo, 1, JTRC_01 );
	memset( (unsigned char *)&g_jpeg, 0, sizeof(g_jpeg) );
	g_jpeg.fd_jpeg = -1;

	/* Check device for jpegcodec "/dev/video0" or "/dev/video1" */
	TRACEMS( cinfo, 1, JTRC_02 );

	/* Try to open folder "/sys/class/video4linux/video1/",
		if the folder exists, jpegcodec is "/dev/video1", otherwises jpegcodec is "/dev/video0" */
	dir = opendir("/sys/class/video4linux/video1/");
	if(dir)
	{
		closedir(dir);
		device[10]++;
	}
	TRACEMSS( cinfo, 1, JTRC_03, device );

	g_jpeg.fd_jpeg = open( device, O_RDWR );
	if ( g_jpeg.fd_jpeg < 0 )
	{
		TRACEMS( cinfo, 1, JTRC_04 );
		g_jpeg.isHWEnabled = 0;
	}
	else
	{
		if ( (ioctl( g_jpeg.fd_jpeg, JPEG_GET_JPEG_BUFFER, &g_jpeg.total_jpeg_buffer_size)) < 0 )
		{
			_demo_uninit_hwjpeg();
			WARNMS( cinfo, JWRN_01 );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}
		TRACEMS1( cinfo, 1, JTRC_05, g_jpeg.total_jpeg_buffer_size );
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
			WARNMS( cinfo, JWRN_02 );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}
	}
#if 0
	if ( (lcm_fd = open( "/dev/fb0", O_RDWR )) < 0 )
	{
		_demo_uninit_hwjpeg();
		close( lcm_fd );
		WARNMS( cinfo, JWRN_03 );
		ERREXIT( cinfo, JERR_BAD_STATE );
	}

	if ( ioctl( lcm_fd, FBIOGET_VSCREENINFO, &(g_jpeg.var) ) < 0 )
	{
		_demo_uninit_hwjpeg();
		close( lcm_fd );
		WARNMS( cinfo, JWRN_04 );
		ERREXIT( cinfo, JERR_BAD_STATE );
	}
	close( lcm_fd );
#endif
	g_jpeg.eparam.encode				= 1;	// 1:encode
///	g_jpeg.eparam.src_bufsize			= 640*480*2;
///	g_jpeg.eparam.dst_bufsize			= 100*1024;
///	g_jpeg.eparam.decInWait_buffer_size	= 100*1024;
	g_jpeg.eparam.decInWait_buffer_size	= 0;
	g_jpeg.eparam.dst_bufsize			= 200*1024;
	g_jpeg.eparam.src_bufsize			= g_jpeg.total_jpeg_buffer_size - g_jpeg.eparam.dst_bufsize;
	g_jpeg.eparam.buffersize			= 0;
	g_jpeg.eparam.buffercount			= 1;
	g_jpeg.eparam.scale					= 0;
	g_jpeg.eparam.encode_source_format	= 1;
	g_jpeg.eparam.encode_image_format	= DRVJPEG_ENC_PRIMARY_YUV422;
	g_jpeg.eparam.qadjust				= 0xf;
	g_jpeg.eparam.qscaling				= 0x4;

  int i;

  /* Guard against version mismatches between library and caller. */
  cinfo->mem = NULL;		/* so jpeg_destroy knows mem mgr not called */
  if (version != JPEG_LIB_VERSION)
    ERREXIT2(cinfo, JERR_BAD_LIB_VERSION, JPEG_LIB_VERSION, version);
  if (structsize != SIZEOF(struct jpeg_compress_struct))
    ERREXIT2(cinfo, JERR_BAD_STRUCT_SIZE, 
	     (int) SIZEOF(struct jpeg_compress_struct), (int) structsize);

  /* For debugging purposes, we zero the whole master structure.
   * But the application has already set the err pointer, and may have set
   * client_data, so we have to save and restore those fields.
   * Note: if application hasn't set client_data, tools like Purify may
   * complain here.
   */
  {
    struct jpeg_error_mgr * err = cinfo->err;
    void * client_data = cinfo->client_data; /* ignore Purify complaint here */
    MEMZERO(cinfo, SIZEOF(struct jpeg_compress_struct));
    cinfo->err = err;
    cinfo->client_data = client_data;
  }
  cinfo->is_decompressor = FALSE;

  /* Initialize a memory manager instance for this object */
  jinit_memory_mgr((j_common_ptr) cinfo);

  /* Zero out pointers to permanent structures. */
  cinfo->progress = NULL;
  cinfo->dest = NULL;

  cinfo->comp_info = NULL;

  for (i = 0; i < NUM_QUANT_TBLS; i++) {
    cinfo->quant_tbl_ptrs[i] = NULL;
    cinfo->q_scale_factor[i] = 100;
  }

  for (i = 0; i < NUM_HUFF_TBLS; i++) {
    cinfo->dc_huff_tbl_ptrs[i] = NULL;
    cinfo->ac_huff_tbl_ptrs[i] = NULL;
  }

  /* Must do it here for emit_dqt in case jpeg_write_tables is used */
  cinfo->block_size = DCTSIZE;
  cinfo->natural_order = jpeg_natural_order;
  cinfo->lim_Se = DCTSIZE2-1;

  cinfo->script_space = NULL;

  cinfo->input_gamma = 1.0;	/* in case application forgets */

  /* OK, I'm ready */
  cinfo->global_state = CSTATE_START;
}


/*
 * Destruction of a JPEG compression object
 */

GLOBAL(void)
jpeg_destroy_compress (j_compress_ptr cinfo)
{
  jpeg_destroy((j_common_ptr) cinfo); /* use common routine */
}


/*
 * Abort processing of a JPEG compression operation,
 * but don't destroy the object itself.
 */

GLOBAL(void)
jpeg_abort_compress (j_compress_ptr cinfo)
{
  jpeg_abort((j_common_ptr) cinfo); /* use common routine */
}


/*
 * Forcibly suppress or un-suppress all quantization and Huffman tables.
 * Marks all currently defined tables as already written (if suppress)
 * or not written (if !suppress).  This will control whether they get emitted
 * by a subsequent jpeg_start_compress call.
 *
 * This routine is exported for use by applications that want to produce
 * abbreviated JPEG datastreams.  It logically belongs in jcparam.c, but
 * since it is called by jpeg_start_compress, we put it here --- otherwise
 * jcparam.o would be linked whether the application used it or not.
 */

GLOBAL(void)
jpeg_suppress_tables (j_compress_ptr cinfo, boolean suppress)
{
  int i;
  JQUANT_TBL * qtbl;
  JHUFF_TBL * htbl;

  for (i = 0; i < NUM_QUANT_TBLS; i++) {
    if ((qtbl = cinfo->quant_tbl_ptrs[i]) != NULL)
      qtbl->sent_table = suppress;
  }

  for (i = 0; i < NUM_HUFF_TBLS; i++) {
    if ((htbl = cinfo->dc_huff_tbl_ptrs[i]) != NULL)
      htbl->sent_table = suppress;
    if ((htbl = cinfo->ac_huff_tbl_ptrs[i]) != NULL)
      htbl->sent_table = suppress;
  }
}


/*
 * Finish JPEG compression.
 *
 * If a multipass operating mode was selected, this may do a great deal of
 * work including most of the actual output.
 */

GLOBAL(void)
jpeg_finish_compress (j_compress_ptr cinfo)
{
	if ( g_jpeg.isHWEnabled )
	{
		if ( (ioctl(g_jpeg.fd_jpeg, JPEG_TRIGGER, NULL)) < 0 )
		{
			_demo_uninit_hwjpeg();
			WARNMS( cinfo, JWRN_05 );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}

		if ( read( g_jpeg.fd_jpeg, &(g_jpeg.jpeginfo), (sizeof(jpeg_info_t) + sizeof(unsigned int)) ) < 0 )
		{
			WARNMS( cinfo, JWRN_06 );
			if ( g_jpeg.jpeginfo.state == JPEG_MEM_SHORTAGE )
			{
				_demo_uninit_hwjpeg();
				WARNMS( cinfo, JWRN_07 );
				ERREXIT( cinfo, JERR_BAD_STATE );
			}
		}

		g_jpeg.out_jpeg_size = g_jpeg.jpeginfo.image_size[0];

		if ( g_jpeg.out_jpeg_size > g_jpeg.eparam.dst_bufsize )
		{
			_demo_uninit_hwjpeg();
			WARNMS2( cinfo, JWRN_08, g_jpeg.out_jpeg_size, g_jpeg.eparam.dst_bufsize );
			ERREXIT( cinfo, JERR_BAD_STATE );
		}
		else
			TRACEMS1( cinfo, 1, JTRC_06, g_jpeg.out_jpeg_size );

#if 0
		if ( g_jpeg.is_encode_to_mem == 0 )
		{
			fwrite(	(g_jpeg.jpeg_buff + g_jpeg.eparam.src_bufsize),
					1,
					g_jpeg.out_jpeg_size,
					g_jpeg.efile_handle
					);
		}
		else
		{
			memcpy(	g_jpeg.out_jpeg_buff,
					(g_jpeg.jpeg_buff + g_jpeg.eparam.src_bufsize),
					g_jpeg.out_jpeg_size
					);
		}
#else
		unsigned char * pu8jpeg_datbuf = (unsigned char *)(g_jpeg.jpeg_buff + g_jpeg.eparam.src_bufsize);
///		printf( "### %d ###\n", cinfo->dest->free_in_buffer );
		while ( g_jpeg.out_jpeg_size > 0 )
		{
			if ( cinfo->dest->free_in_buffer <= g_jpeg.out_jpeg_size )
			{
///printf( "### 1 ###\n" );
				memcpy( cinfo->dest->next_output_byte, pu8jpeg_datbuf, cinfo->dest->free_in_buffer );
				g_jpeg.out_jpeg_size -= cinfo->dest->free_in_buffer;
				pu8jpeg_datbuf += cinfo->dest->free_in_buffer;
				if ( !(*cinfo->dest->empty_output_buffer) (cinfo) )
				{
///printf( "### 2 ###\n" );
					ERREXIT(cinfo, JERR_CANT_SUSPEND);
				}
			}
			else
			{
///printf( "### 3 ###\n" );
				memcpy( cinfo->dest->next_output_byte, pu8jpeg_datbuf, g_jpeg.out_jpeg_size );
				cinfo->dest->free_in_buffer -= g_jpeg.out_jpeg_size;
				g_jpeg.out_jpeg_size -= g_jpeg.out_jpeg_size;
			}
		}
		(*cinfo->dest->term_destination) (cinfo);
#endif

		/* We can use jpeg_abort to release memory and reset global_state */
		jpeg_abort((j_common_ptr) cinfo);
	}
	else
	{

  JDIMENSION iMCU_row;

  if (cinfo->global_state == CSTATE_SCANNING ||
      cinfo->global_state == CSTATE_RAW_OK) {
    /* Terminate first pass */
    if (cinfo->next_scanline < cinfo->image_height)
      ERREXIT(cinfo, JERR_TOO_LITTLE_DATA);
    (*cinfo->master->finish_pass) (cinfo);
  } else if (cinfo->global_state != CSTATE_WRCOEFS)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  /* Perform any remaining passes */
  while (! cinfo->master->is_last_pass) {
    (*cinfo->master->prepare_for_pass) (cinfo);
    for (iMCU_row = 0; iMCU_row < cinfo->total_iMCU_rows; iMCU_row++) {
      if (cinfo->progress != NULL) {
	cinfo->progress->pass_counter = (long) iMCU_row;
	cinfo->progress->pass_limit = (long) cinfo->total_iMCU_rows;
	(*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
      }
      /* We bypass the main controller and invoke coef controller directly;
       * all work is being done from the coefficient buffer.
       */
      if (! (*cinfo->coef->compress_data) (cinfo, (JSAMPIMAGE) NULL))
	ERREXIT(cinfo, JERR_CANT_SUSPEND);
    }
    (*cinfo->master->finish_pass) (cinfo);
  }
  /* Write EOI, do final cleanup */
  (*cinfo->marker->write_file_trailer) (cinfo);
  (*cinfo->dest->term_destination) (cinfo);
  /* We can use jpeg_abort to release memory and reset global_state */
  jpeg_abort((j_common_ptr) cinfo);

	}
}


/*
 * Write a special marker.
 * This is only recommended for writing COM or APPn markers.
 * Must be called after jpeg_start_compress() and before
 * first call to jpeg_write_scanlines() or jpeg_write_raw_data().
 */

GLOBAL(void)
jpeg_write_marker (j_compress_ptr cinfo, int marker,
		   const JOCTET *dataptr, unsigned int datalen)
{
  JMETHOD(void, write_marker_byte, (j_compress_ptr info, int val));

  if (cinfo->next_scanline != 0 ||
      (cinfo->global_state != CSTATE_SCANNING &&
       cinfo->global_state != CSTATE_RAW_OK &&
       cinfo->global_state != CSTATE_WRCOEFS))
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);

  (*cinfo->marker->write_marker_header) (cinfo, marker, datalen);
  write_marker_byte = cinfo->marker->write_marker_byte;	/* copy for speed */
  while (datalen--) {
    (*write_marker_byte) (cinfo, *dataptr);
    dataptr++;
  }
}

/* Same, but piecemeal. */

GLOBAL(void)
jpeg_write_m_header (j_compress_ptr cinfo, int marker, unsigned int datalen)
{
  if (cinfo->next_scanline != 0 ||
      (cinfo->global_state != CSTATE_SCANNING &&
       cinfo->global_state != CSTATE_RAW_OK &&
       cinfo->global_state != CSTATE_WRCOEFS))
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);

  (*cinfo->marker->write_marker_header) (cinfo, marker, datalen);
}

GLOBAL(void)
jpeg_write_m_byte (j_compress_ptr cinfo, int val)
{
  (*cinfo->marker->write_marker_byte) (cinfo, val);
}


/*
 * Alternate compression function: just write an abbreviated table file.
 * Before calling this, all parameters and a data destination must be set up.
 *
 * To produce a pair of files containing abbreviated tables and abbreviated
 * image data, one would proceed as follows:
 *
 *		initialize JPEG object
 *		set JPEG parameters
 *		set destination to table file
 *		jpeg_write_tables(cinfo);
 *		set destination to image file
 *		jpeg_start_compress(cinfo, FALSE);
 *		write data...
 *		jpeg_finish_compress(cinfo);
 *
 * jpeg_write_tables has the side effect of marking all tables written
 * (same as jpeg_suppress_tables(..., TRUE)).  Thus a subsequent start_compress
 * will not re-emit the tables unless it is passed write_all_tables=TRUE.
 */

GLOBAL(void)
jpeg_write_tables (j_compress_ptr cinfo)
{
  if (cinfo->global_state != CSTATE_START)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);

  /* (Re)initialize error mgr and destination modules */
  (*cinfo->err->reset_error_mgr) ((j_common_ptr) cinfo);
  (*cinfo->dest->init_destination) (cinfo);
  /* Initialize the marker writer ... bit of a crock to do it here. */
  jinit_marker_writer(cinfo);
  /* Write them tables! */
  (*cinfo->marker->write_tables_only) (cinfo);
  /* And clean up. */
  (*cinfo->dest->term_destination) (cinfo);
  /*
   * In library releases up through v6a, we called jpeg_abort() here to free
   * any working memory allocated by the destination manager and marker
   * writer.  Some applications had a problem with that: they allocated space
   * of their own from the library memory manager, and didn't want it to go
   * away during write_tables.  So now we do nothing.  This will cause a
   * memory leak if an app calls write_tables repeatedly without doing a full
   * compression cycle or otherwise resetting the JPEG object.  However, that
   * seems less bad than unexpectedly freeing memory in the normal case.
   * An app that prefers the old behavior can call jpeg_abort for itself after
   * each call to jpeg_write_tables().
   */
}
