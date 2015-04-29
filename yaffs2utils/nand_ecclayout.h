/* 
 * yaffs2utils: Utilities to make/extract a YAFFS2/YAFFS1 image.
 * Copyright (C) 2010-2011 Luen-Yung Lin <penguin.lin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __YAFFS2UTILS_NAND_ECCLAYOUT_H__
#define __YAFFS2UTILS_NAND_ECCLAYOUT_H__

#ifndef _HAVE_BROKEN_MTD_H
#include <mtd/mtd-user.h>
#else
#include "mtd-abi.h"
#endif

static nand_ecclayout_t nand_oob_16 = {
	.eccbytes	= 6,
	.eccpos		= {0, 1, 2, 3, 6, 7},
	.oobfree	= {{.offset = 8, .length = 8}},
};

static nand_ecclayout_t nand_oob_64 = {
	.eccbytes	= 24,
	.eccpos		= {40, 41, 42, 43, 44, 45, 46, 47,
			   48, 49, 50, 51, 52, 53, 54, 55,
			   56, 57, 58, 59, 60, 61, 62, 63},
	.oobfree	= {{.offset = 2, .length = 38}},
};


//nuc970 2k page size
static nand_ecclayout_t nuc970_oobTb = {
	.eccbytes	= 32,
	.eccpos		= {32,33,34,35,36,37,38,39,40, 41, 42, 43, 44, 45, 46, 47,
			   48, 49, 50, 51, 52, 53, 54, 55,
			   56, 57, 58, 59, 60, 61, 62, 63},
	.oobfree	= {{.offset = 4, .length = 28}},
	};
typedef enum  {
    eBCH_T4,
    eBCH_T8,
    eBCH_T12,
    eBCH_T15,
    eBCH_T24,
    eBCH_CNT
} E_BCHALGORITHM;

typedef enum {
    ePageSize_512,
    ePageSize_2048,
    ePageSize_4096,
    ePageSize_8192,
    ePageSize_CNT
} E_PAGESIZE;

static const int g_i32ParityNum[ePageSize_CNT][eBCH_CNT] = {
    { 8,    15,     23,     29,     -1  },  // For 512
    { 32,   60,     92,     116,    90  },  // For 2K
    { 64,   120,    184,    232,    180 },  // For 4K
    { 128,  240,    368,    464,    360 },  // For 8K
};

static nand_ecclayout_t yaffs_nand_oob_64 = {
	.eccbytes	= 24,
	.eccpos		= {40, 41, 42, 43, 44, 45, 46, 47,
			   48, 49, 50, 51, 52, 53, 54, 55,
			   56, 57, 58, 59, 60, 61, 62, 63},
	.oobfree	= {{.offset = 0, .length = 40}},
};

static nand_ecclayout_t nand_oob_128 = {
	.eccbytes	= 48,
	.eccpos		= {
			   80, 81, 82, 83, 84, 85, 86, 87,
			   88, 89, 90, 91, 92, 93, 94, 95,
			   96, 97, 98, 99, 100, 101, 102, 103,
			   104, 105, 106, 107, 108, 109, 110, 111,
			   112, 113, 114, 115, 116, 117, 118, 119,
			   120, 121, 122, 123, 124, 125, 126, 127},
	.oobfree	= {{.offset = 2, .length = 78}}
};

static nand_ecclayout_t yaffs_nand_oob_128 = {
	.eccbytes	= 48,
	.eccpos		= {
			   80, 81, 82, 83, 84, 85, 86, 87,
			   88, 89, 90, 91, 92, 93, 94, 95,
			   96, 97, 98, 99, 100, 101, 102, 103,
			   104, 105, 106, 107, 108, 109, 110, 111,
			   112, 113, 114, 115, 116, 117, 118, 119,
			   120, 121, 122, 123, 124, 125, 126, 127},
	.oobfree	= {{.offset = 0, .length = 80}}
};

static nand_ecclayout_t nand_oob_user = {0};

#endif
