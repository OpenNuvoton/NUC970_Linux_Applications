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
	rt_config.h

	Abstract:
	Central header file to maintain all include files for all NDIS
	miniport driver routines.

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
	Paul Lin    08-01-2002    created

*/
#ifndef	__RT_CONFIG_H__
#define	__RT_CONFIG_H__

#define VENDOR_FEATURE1_SUPPORT
//#define VENDOR_FEATURE2_SUPPORT
//#define VENDOR_FEATURE3_SUPPORT

//#define MONITOR_FLAG_11N_SNIFFER_SUPPORT

#ifdef VENDOR_FEATURE3_SUPPORT 
#ifdef DOT1X_SUPPORT
#undef DOT1X_SUPPORT
#endif // DOT1X_SUPPORT //
#ifdef SYSTEM_LOG_SUPPORT
#undef SYSTEM_LOG_SUPPORT
#endif // SYSTEM_LOG_SUPPORT //
#ifdef LED_CONTROL_SUPPORT
#undef LED_CONTROL_SUPPORT
#endif // LED_CONTROL_SUPPORT //
#ifdef WSC_LED_SUPPORT
#undef WSC_LED_SUPPORT
#endif // WSC_LED_SUPPORT //
#endif // VENDOR_FEATURE3_SUPPORT //



#include "rtmp_type.h"
#include "rtmp_os.h"

#include "rtmp_def.h"
#include "rtmp_chip.h"
#include "rtmp_timer.h"

#include "oid.h"
#include "mlme.h"
#include "crypt_md5.h"
#include "crypt_sha2.h"
#include "crypt_hmac.h"
#include "crypt_aes.h"
#include "crypt_arc4.h"
#include "rtmp_cmd.h"
#include "rtmp.h"
#include "ap.h"
#include "wpa.h"
#include "dfs.h"
#include "chlist.h"
#include "spectrum.h"

#include "eeprom.h"
#if defined(RTMP_PCI_SUPPORT) || defined(RTMP_USB_SUPPORT)
#include "rtmp_mcu.h"
#endif



#undef AP_WSC_INCLUDED
#undef STA_WSC_INCLUDED
#undef WSC_INCLUDED



#ifdef CONFIG_STA_SUPPORT
#endif // CONFIG_STA_SUPPORT //

#ifdef BLOCK_NET_IF
#include "netif_block.h"
#endif // BLOCK_NET_IF //

#ifdef IGMP_SNOOP_SUPPORT
#include "igmp_snoop.h"
#endif // IGMP_SNOOP_SUPPORT //

#ifdef RALINK_ATE
#include "rt_ate.h"
#endif // RALINK_ATE //

#ifdef RALINK_28xx_QA
#ifndef RALINK_ATE
#error "For supporting QA GUI, please set HAS_ATE=y and HAS_QA_SUPPORT=y."
#endif // RALINK_ATE //
#endif // RALINK_28xx_QA //





#if defined(AP_WSC_INCLUDED) || defined(STA_WSC_INCLUDED)
#define WSC_INCLUDED
#endif


#ifdef CONFIG_STA_SUPPORT
#ifdef NATIVE_WPA_SUPPLICANT_SUPPORT
#ifndef WPA_SUPPLICANT_SUPPORT
#error "Build for being controlled by NetworkManager or wext, please set HAS_WPA_SUPPLICANT=y and HAS_NATIVE_WPA_SUPPLICANT_SUPPORT=y"
#endif // WPA_SUPPLICANT_SUPPORT //
#endif // NATIVE_WPA_SUPPLICANT_SUPPORT //

#endif // CONFIG_STA_SUPPORT //



#ifdef IKANOS_VX_1X0
#include "vr_ikans.h"
#endif // IKANOS_VX_1X0 //






#ifdef CONFIG_STA_SUPPORT
#include "sta_cfg.h"
#endif // CONFIG_STA_SUPPORT //

#ifdef RT_CFG80211_SUPPORT
#include "cfg80211.h"
#endif // RT_CFG80211_SUPPORT //

#endif	// __RT_CONFIG_H__

