/*
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2007, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  * 
 * it under the terms of the GNU General Public License as published by  * 
 * the Free Software Foundation; either version 2 of the License, or     * 
 * (at your option) any later version.                                   * 
 *                                                                       * 
 * This program is distributed in the hope that it will be useful,       * 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        * 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         * 
 * GNU General Public License for more details.                          * 
 *                                                                       * 
 * You should have received a copy of the GNU General Public License     * 
 * along with this program; if not, write to the                         * 
 * Free Software Foundation, Inc.,                                       * 
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             * 
 *                                                                       * 
 *************************************************************************

    Module Name:
	rt3593.h
 
    Abstract:
	3*3 Wireless Chip PCIe

    Revision History:
    Who			When			What
    ---------	----------		----------------------------------------------
	SampleLin	20091105		Initial version
 */

#ifndef __RT3593_H__
#define __RT3593_H__

#ifdef RT3593

/*
	MCS 16 ~ 23 Test Note:
	Use fix rate mode, HT_MCS = 23, and set bit 30 of MAC Reg 134C to 0
	(disable auto-fallback mode).
*/

#error "For RT3593, you should define the compile flag -DRTMP_PCI_SUPPORT"

#error "For RT3593, you should define the compile flag -DRTMP_MAC_PCI"

#ifndef RTMP_RF_RW_SUPPORT
#error "For RT3593, you should define the compile flag -DRTMP_RF_RW_SUPPORT"
#endif

#ifndef RT30xx
#error "For RT3593, you should define the compile flag -DRT30xx"
#endif

#error "For RT3593, you should define the compile flag -DRT35xx"

/* General definition */

/* if you want to support PCIe power save function */
/* 2009/11/06, if open the function, the signal will be bad and sometimes crush */
//#define PCIE_PS_SUPPORT

//
// Device ID & Vendor ID, these values should match EEPROM value
//
#define NIC3593_PCI_OR_PCIe_DEVICE_ID	0x3593

extern REG_PAIR RF3053RegTable[];
extern UCHAR NUM_RF_3053_REG_PARMS;


/* MACRO definition */

/* for 3*3 related */
#define RTMP_RF33_SHARED_MEM_SELECT(__pAd)							\
{																	\
	if (IS_RT3593(__pAd))											\
	{																\
		PBF_SYS_CTRL_STRUC __PbfSysCtrl = {{0}};					\
		RTMP_IO_READ32(__pAd, PBF_SYS_CTRL, &__PbfSysCtrl.word);	\
		__PbfSysCtrl.field.SHR_MSEL = 1;							\
		RTMP_IO_WRITE32(__pAd, PBF_SYS_CTRL, __PbfSysCtrl.word);	\
	}																\
}

#define RTMP_RF33_SHARED_MEM_DESELECT(__pAd)						\
{																	\
	if (IS_RT3593(__pAd))											\
	{																\
		PBF_SYS_CTRL_STRUC __PbfSysCtrl = {{0}};					\
		RTMP_IO_WRITE32(pAd, BCN_OFFSET0, 0x18100800); /* 0x0000(00), 0x0200(08), 0x0400(10), 0x0600(18), 512B for each beacon */ \
		RTMP_IO_WRITE32(pAd, BCN_OFFSET1, 0x38302820); /* 0x0800(20), 0x0A00(28), 0x0C00(30), 0x0E00(38), 512B for each beacon */ \
		RTMP_IO_READ32(pAd, PBF_SYS_CTRL, &__PbfSysCtrl.word);		\
		__PbfSysCtrl.field.SHR_MSEL = 0;							\
		RTMP_IO_WRITE32(pAd, PBF_SYS_CTRL, __PbfSysCtrl.word);		\
	}																\
}


#endif // RT3593 //
#endif //__RT3593_H__ //

/* End of rt3593.h */
