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
	rtmp_init.c

	Abstract:
	Miniport generic portion header file

	Revision History:
	Who         When          What
	--------    ----------    ----------------------------------------------
*/
#include	"rt_config.h"
#define RT3090A_DEFAULT_INTERNAL_LNA_GAIN	0x0A
UCHAR    BIT8[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
#ifdef DBG
char*   CipherName[] = {"none","wep64","wep128","TKIP","AES","CKIP64","CKIP128","CKIP152","SMS4"};
#endif

//
// BBP register initialization set
//
REG_PAIR   BBPRegTable[] = {

	{BBP_R65,		0x2C},		// fix rssi issue
	{BBP_R66,		0x38},	// Also set this default value to pAd->BbpTuning.R66CurrentValue at initial
	{BBP_R69,		0x12},
	{BBP_R70,		0xa},	// BBP_R70 will change to 0x8 in ApStartUp and LinkUp for rt2860C, otherwise value is 0xa
	{BBP_R73,		0x10},
	{BBP_R81,		0x37},
	{BBP_R82,		0x62},
	{BBP_R83,		0x6A},
	{BBP_R84,		0x99},	// 0x19 is for rt2860E and after. This is for extension channel overlapping IOT. 0x99 is for rt2860D and before
	{BBP_R86,		0x00},	// middle range issue, Rory @2008-01-28 	
	{BBP_R91,		0x04},	// middle range issue, Rory @2008-01-28
	{BBP_R92,		0x00},	// middle range issue, Rory @2008-01-28
	{BBP_R103,		0x00}, 	// near range high-power issue, requested from Gary @2008-0528
	{BBP_R105,		0x05},	// 0x05 is for rt2860E to turn on FEQ control. It is safe for rt2860D and before, because Bit 7:2 are reserved in rt2860D and before.
	{BBP_R106,		0x35},	// Optimizing the Short GI sampling request from Gray @2009-0409
};
#define	NUM_BBP_REG_PARMS	(sizeof(BBPRegTable) / sizeof(REG_PAIR))


//
// ASIC register initialization sets
//

RTMP_REG_PAIR	MACRegTable[] =	{
#if defined(HW_BEACON_OFFSET) && (HW_BEACON_OFFSET == 0x200)
	{BCN_OFFSET0,			0xf8f0e8e0}, /* 0x3800(e0), 0x3A00(e8), 0x3C00(f0), 0x3E00(f8), 512B for each beacon */
	{BCN_OFFSET1,			0x6f77d0c8}, /* 0x3200(c8), 0x3400(d0), 0x1DC0(77), 0x1BC0(6f), 512B for each beacon */
#elif defined(HW_BEACON_OFFSET) && (HW_BEACON_OFFSET == 0x100)
	{BCN_OFFSET0,			0xece8e4e0}, /* 0x3800, 0x3A00, 0x3C00, 0x3E00, 512B for each beacon */
	{BCN_OFFSET1,			0xfcf8f4f0}, /* 0x3800, 0x3A00, 0x3C00, 0x3E00, 512B for each beacon */
#else
    #error You must re-calculate new value for BCN_OFFSET0 & BCN_OFFSET1 in MACRegTable[]!!!
#endif // HW_BEACON_OFFSET //

	{LEGACY_BASIC_RATE,		0x0000013f}, //  Basic rate set bitmap
	{HT_BASIC_RATE,		0x00008003}, // Basic HT rate set , 20M, MCS=3, MM. Format is the same as in TXWI.
	{MAC_SYS_CTRL,		0x00}, // 0x1004, , default Disable RX
	{RX_FILTR_CFG,		0x17f97}, //0x1400  , RX filter control,  
	{BKOFF_SLOT_CFG,	0x209}, // default set short slot time, CC_DELAY_TIME should be 2	 
	//{TX_SW_CFG0,		0x40a06}, // Gary,2006-08-23 
	{TX_SW_CFG0,		0x0}, 		// Gary,2008-05-21 for CWC test 
	{TX_SW_CFG1,		0x80606}, // Gary,2006-08-23 
	{TX_LINK_CFG,		0x1020},		// Gary,2006-08-23 
	//{TX_TIMEOUT_CFG,	0x00182090},	// CCK has some problem. So increase timieout value. 2006-10-09// MArvek RT
	{TX_TIMEOUT_CFG,	0x000a2090},	// CCK has some problem. So increase timieout value. 2006-10-09// MArvek RT , Modify for 2860E ,2007-08-01
	{MAX_LEN_CFG,		MAX_AGGREGATION_SIZE | 0x00001000},	// 0x3018, MAX frame length. Max PSDU = 16kbytes.
	{LED_CFG,		0x7f031e46}, // Gary, 2006-08-23

//#ifdef CONFIG_AP_SUPPORT
//	{WMM_AIFSN_CFG,		0x00001173},
//	{WMM_CWMIN_CFG,		0x00002344},
//	{WMM_CWMAX_CFG,		0x000034a6},
//	{WMM_TXOP0_CFG,		0x00100020},
//	{WMM_TXOP1_CFG,		0x002F0038},
//#endif // CONFIG_AP_SUPPORT //

//#ifdef CONFIG_STA_SUPPORT
//	{WMM_AIFSN_CFG,		0x00002273},
//	{WMM_CWMIN_CFG,		0x00002344},
//	{WMM_CWMAX_CFG,		0x000034aa},
//#endif // CONFIG_STA_SUPPORT //
#ifdef INF_AMAZON_SE
	{PBF_MAX_PCNT,			0x1F3F6F6F}, 	//iverson modify for usb issue, 2008/09/19
											// 6F + 6F < total page count FE
											// so that RX doesn't occupy TX's buffer space when WMM congestion.
#else
	{PBF_MAX_PCNT,			0x1F3FBF9F}, 	//0x1F3f7f9f},		//Jan, 2006/04/20
#endif // INF_AMAZON_SE //
	//{TX_RTY_CFG,			0x6bb80408},	// Jan, 2006/11/16
// WMM_ACM_SUPPORT
//	{TX_RTY_CFG,			0x6bb80101},	// sample
	{TX_RTY_CFG,			0x47d01f0f},	// Jan, 2006/11/16, Set TxWI->ACK =0 in Probe Rsp Modify for 2860E ,2007-08-03
	
	{AUTO_RSP_CFG,			0x00000013},	// Initial Auto_Responder, because QA will turn off Auto-Responder
	{CCK_PROT_CFG,			0x05740003 /*0x01740003*/},	// Initial Auto_Responder, because QA will turn off Auto-Responder. And RTS threshold is enabled. 
	{OFDM_PROT_CFG,			0x05740003 /*0x01740003*/},	// Initial Auto_Responder, because QA will turn off Auto-Responder. And RTS threshold is enabled. 
#ifdef RTMP_MAC_USB
	{PBF_CFG, 				0xf40006}, 		// Only enable Queue 2
	{MM40_PROT_CFG,			0x3F44084},		// Initial Auto_Responder, because QA will turn off Auto-Responder
	{WPDMA_GLO_CFG,			0x00000030},
#endif // RTMP_MAC_USB //
	{GF20_PROT_CFG,			0x01744004},    // set 19:18 --> Short NAV for MIMO PS
	{GF40_PROT_CFG,			0x03F44084},    
	{MM20_PROT_CFG,			0x01744004},    
	{TXOP_CTRL_CFG,			0x0000583f, /*0x0000243f*/ /*0x000024bf*/},	//Extension channel backoff.
	{TX_RTS_CFG,			0x00092b20},	
//#ifdef WIFI_TEST
	{EXP_ACK_TIME,			0x002400ca},	// default value
//#else
//	{EXP_ACK_TIME,			0x005400ca},	// suggested by Gray @ 20070323 for 11n intel-sta throughput
//#endif // end - WIFI_TEST //
//#ifdef CONFIG_AP_SUPPORT
//	{TBTT_SYNC_CFG,			0x00422000},	// TBTT_ADJUST(7:0) == 0	
//	{TBTT_SYNC_CFG,			0x00012000},	// TBTT_ADJUST(7:0) == 0	
//#endif // CONFIG_AP_SUPPORT //
	{TXOP_HLDR_ET, 			0x00000002},

	/* Jerry comments 2008/01/16: we use SIFS = 10us in CCK defaultly, but it seems that 10us
		is too small for INTEL 2200bg card, so in MBSS mode, the delta time between beacon0
		and beacon1 is SIFS (10us), so if INTEL 2200bg card connects to BSS0, the ping
		will always lost. So we change the SIFS of CCK from 10us to 16us. */
	{XIFS_TIME_CFG,			0x33a41010},
	{PWR_PIN_CFG,			0x00000003},	// patch for 2880-E
};


#ifdef CONFIG_STA_SUPPORT
RTMP_REG_PAIR	STAMACRegTable[] =	{
	{WMM_AIFSN_CFG,		0x00002273},
	{WMM_CWMIN_CFG,	0x00002344},
	{WMM_CWMAX_CFG,	0x000034aa},
};
#endif // CONFIG_STA_SUPPORT //

#define	NUM_MAC_REG_PARMS		(sizeof(MACRegTable) / sizeof(RTMP_REG_PAIR))
#ifdef CONFIG_STA_SUPPORT
#define	NUM_STA_MAC_REG_PARMS	(sizeof(STAMACRegTable) / sizeof(RTMP_REG_PAIR))
#endif // CONFIG_STA_SUPPORT //


/*
	========================================================================
	
	Routine Description:
		Allocate RTMP_ADAPTER data block and do some initialization

	Arguments:
		Adapter		Pointer to our adapter

	Return Value:
		NDIS_STATUS_SUCCESS
		NDIS_STATUS_FAILURE

	IRQL = PASSIVE_LEVEL

	Note:
	
	========================================================================
*/
NDIS_STATUS	RTMPAllocAdapterBlock(
	IN  PVOID	handle,
	OUT	PRTMP_ADAPTER	*ppAdapter)
{
	PRTMP_ADAPTER	pAd = NULL;
	NDIS_STATUS		Status;
	INT 			index;
	UCHAR			*pBeaconBuf = NULL;

	DBGPRINT(RT_DEBUG_TRACE, ("--> RTMPAllocAdapterBlock\n"));

	*ppAdapter = NULL;

	do
	{
		// Allocate RTMP_ADAPTER memory block
		pBeaconBuf = kmalloc(MAX_BEACON_SIZE, MEM_ALLOC_FLAG);
		if (pBeaconBuf == NULL)
		{
			Status = NDIS_STATUS_FAILURE;
			DBGPRINT_ERR(("Failed to allocate memory - BeaconBuf!\n"));
			break;
		}
		NdisZeroMemory(pBeaconBuf, MAX_BEACON_SIZE);

		Status = AdapterBlockAllocateMemory(handle, (PVOID *)&pAd);
		if (Status != NDIS_STATUS_SUCCESS)
		{
			DBGPRINT_ERR(("Failed to allocate memory - ADAPTER\n"));
			break;
		}
		pAd->BeaconBuf = pBeaconBuf;
		DBGPRINT(RT_DEBUG_OFF, ("\n\n=== pAd = %p, size = %d ===\n\n", pAd, (UINT32)sizeof(RTMP_ADAPTER)));


		// Init spin locks
		NdisAllocateSpinLock(&pAd->MgmtRingLock);

		for (index =0 ; index < NUM_OF_TX_RING; index++)
		{
			NdisAllocateSpinLock(&pAd->TxSwQueueLock[index]);
			NdisAllocateSpinLock(&pAd->DeQueueLock[index]);
			pAd->DeQueueRunning[index] = FALSE;
		}

		NdisAllocateSpinLock(&pAd->irq_lock);



		// assign function pointers
	} while (FALSE);

	if ((Status != NDIS_STATUS_SUCCESS) && (pBeaconBuf))
		kfree(pBeaconBuf);
	
	*ppAdapter = pAd;


	/*
		Init ProbeRespIE Table
	*/
	for (index = 0; index < MAX_LEN_OF_BSS_TABLE; index++) 
	{
		if (os_alloc_mem(pAd,&pAd->ProbeRespIE[index].pIe, MAX_VIE_LEN) == NDIS_STATUS_SUCCESS)
			RTMPZeroMemory(pAd->ProbeRespIE[index].pIe, MAX_VIE_LEN);
		else
			pAd->ProbeRespIE[index].pIe = NULL;
	}	

	DBGPRINT_S(Status, ("<-- RTMPAllocAdapterBlock, Status=%x\n", Status));
	return Status;
}


#ifdef RT3593
//
// The verification of the Tx power per rate
//
// Parameters
//	pAd: The adapter data structure
//	pTxPwr: Point to a Tx power per rate
//
// Return Value
//	None
//
VOID RTMPVerifyTxPwrPerRateExt(
	IN PRTMP_ADAPTER pAd, 
	IN OUT PUCHAR pTxPwr)
{
	DBGPRINT(RT_DEBUG_INFO, ("--> %s\n", __FUNCTION__));

	if (*pTxPwr > 0xC)
	{
		*pTxPwr = 0xC;
	}
	
	DBGPRINT(RT_DEBUG_INFO, ("<-- %s\n", __FUNCTION__));
}

//
// Read the Tx power (per data rate) based on the extended EEPROM format
//
// Parameters
//	pAd: The adapter data structure
//
// Return Value
//	None
//
VOID RTMPReadTxPwrPerRateExt(
	IN PRTMP_ADAPTER pAd)
{
	USHORT value = 0;
	ULONG TxPwrOverMAC = 0;
	UCHAR t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;
	UCHAR t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0;
	UCHAR t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0;
	UCHAR t25 = 0, t26 = 0, t27 = 0, t28 = 0, t29 = 0, t30 = 0, t31 = 0, t32 = 0;
	
	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __FUNCTION__));

	//
	// (a) Default Tx power for BW20 over 2.4G 
	//
	RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_2DOT4G, value);
	t1 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t1);
	t2 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t2);
	t3 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t3);
	t4 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t4);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_2DOT4G + 2), value);
	t5 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t5);
	t6 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t6);
	t7 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t7);
	t8 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t8);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_2DOT4G + 4), value);
	t9 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t9);
	t10 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t10);
	t11 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t11);
	t12 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t12);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_2DOT4G + 6), value);
	t13 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t13);
	t14 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t14);
	t15 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t15);
	t16 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t16);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_2DOT4G + 8), value);
	t17 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t17);
	t18 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t18);
	t19 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t19);
	t20 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t20);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_2DOT4G + 10), value);
	t21 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t21);
	t22 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t22);
	t23 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t23);
	t24 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t24);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_2DOT4G + 12), value);
	t25 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t25);
	t26 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t26);
	t27 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t27);
	t28 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t28);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_2DOT4G + 14), value);
	t29 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t29);
	t30 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t30);
	t31 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t31);
	t32 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t32);

	TxPwrOverMAC = ((t4 << 28) | (t4 << 24) | (t3 << 20) | (t3 << 16) | 
	                             (t2 << 12) | (t2 << 8) | (t1 << 4) | (t1));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg0 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg0 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg0));

	TxPwrOverMAC = ((t4 << 24) | (t3 << 16) | (t2 << 8) | (t1));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg0Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg0Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg0Ext));

	TxPwrOverMAC = ((t10 << 28) | (t10 << 24) | (t9 << 20) | (t9 << 16) | 
	                             (t6 << 12) | (t6 << 8) | (t5 << 4) | (t5));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg1 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg1 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg1));

	TxPwrOverMAC = ((t10 << 24) | (t9 << 16) | (t6 << 8) | (t5));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg1Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg1Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg1Ext));

	TxPwrOverMAC = ((t15 << 28) | (t15 << 24) | (t14 << 20) | (t14 << 16) | 
	                             (t12 << 12) | (t12 << 8) | (t11 << 4) | (t11));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg2 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg2 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg2));

	TxPwrOverMAC = ((t15 << 24) | (t14 << 16) | (t12 << 8) | (t11));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg2Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg2Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg2Ext));

	TxPwrOverMAC = ((t26 << 28) | (t26 << 24) | (t25 << 20) | (t25 << 16) | 
	                             (t17 << 12) | (t17 << 8) | (t16 << 4) | (t16));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg3 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg3 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg3));

	TxPwrOverMAC = ((t26 << 24) | (t25 << 16) | (t17 << 8) | (t16));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg3Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg3Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg3Ext));

	TxPwrOverMAC = ((t28 << 12) | (t28 << 8) | (t27 << 4) | (t27));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg4 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg4 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg4));

	TxPwrOverMAC = ((t28 << 8) | (t27));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg4Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg4Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg4Ext));

	TxPwrOverMAC = ((t20 << 24) | (t20 << 20) | (t20 << 16) | 
	                            (t19 << 8) | (t19 << 4) | (t19));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg5 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg5 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg5));

	TxPwrOverMAC = ((t22 << 24) | (t22 << 20) | (t22 << 16) | 
	                            (t21 << 8) | (t21 << 4) | (t21));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg6 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg6 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg6));

	TxPwrOverMAC = ((t13 << 24) | (t13 << 20) | (t13 << 16) | 
	                            (t7 << 8) | (t7 << 4) | (t7));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg7 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg7 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg7));

	TxPwrOverMAC = ((t23 << 24) | (t23 << 20) | (t23 << 16) | 
	                            (t18 << 8) | (t18 << 4) | (t18));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg8 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg8 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg8));

	TxPwrOverMAC = ((t29 << 8) | (t29 << 4) | (t29));
	pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg9 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg9 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over2Dot4G.TxPwrCfg9));

	//
	// (b) Default Tx power for BW40 over 2.4G 
	//
	RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_2DOT4G, value);
	t1 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t1);
	t2 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t2);
	t3 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t3);
	t4 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t4);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_2DOT4G + 2), value);
	t5 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t5);
	t6 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t6);
	t7 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t7);
	t8 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t8);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_2DOT4G + 4), value);
	t9 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t9);
	t10 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t10);
	t11 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t11);
	t12 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t12);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_2DOT4G + 6), value);
	t13 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t13);
	t14 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t14);
	t15 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t15);
	t16 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t16);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_2DOT4G + 8), value);
	t17 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t17);
	t18 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t18);
	t19 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t19);
	t20 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t20);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_2DOT4G + 10), value);
	t21 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t21);
	t22 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t22);
	t23 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t23);
	t24 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t24);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_2DOT4G + 12), value);
	t25 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t25);
	t26 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t26);
	t27 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t27);
	t28 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t28);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_2DOT4G + 14), value);
	t29 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t29);
	t30 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t30);
	t31 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t31);
	t32 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t32);

	TxPwrOverMAC = ((t4 << 28) | (t4 << 24) | (t3 << 20) | (t3 << 16) | 
	                             (DEFAULT_TX_POWER << 12) | (DEFAULT_TX_POWER << 8) | (DEFAULT_TX_POWER << 4) | (DEFAULT_TX_POWER));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg0 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("\n%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg0 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg0));

	TxPwrOverMAC = ((t4 << 24) | (t3 << 16) | (DEFAULT_TX_POWER << 8) | (DEFAULT_TX_POWER));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg0Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg0Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg0Ext));

	TxPwrOverMAC = ((t10 << 28) | (t10 << 24) | (t9 << 20) | (t9 << 16) | 
	                             (t6 << 12) | (t6 << 8) | (t5 << 4) | (t5));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg1 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg1 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg1));

	TxPwrOverMAC = ((t10 << 24) | (t9 << 16) | (t6 << 8) | (t5));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg1Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg1Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg1Ext));

	TxPwrOverMAC = ((t15 << 28) | (t15 << 24) | (t14 << 20) | (t14 << 16) | 
	                             (t12 << 12) | (t12 << 8) | (t11 << 4) | (t11));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg2 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg2 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg2));

	TxPwrOverMAC = ((t15 << 24) | (t14 << 16) | (t12 << 8) | (t11));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg2Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg2Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg2Ext));

	TxPwrOverMAC = ((t26 << 28) | (t26 << 24) | (t25 << 20) | (t25 << 16) | 
	                             (t17 << 12) | (t17 << 8) | (t16 << 4) | (t16));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg3 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg3 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg3));

	TxPwrOverMAC = ((t26 << 24) | (t25 << 16) | (t17 << 8) | (t16));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg3Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg3Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg3Ext));

	TxPwrOverMAC = ((t28 << 12) | (t28 << 8) | (t27 << 4) | (t27));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg4 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg4 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg4));

	TxPwrOverMAC = ((t28 << 8) | (t27));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg4Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg4Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg4Ext));

	TxPwrOverMAC = ((t20 << 24) | (t20 << 20) | (t20 << 16) | 
	                            (t19 << 8) | (t19 << 4) | (t19));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg5 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg5 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg5));

	TxPwrOverMAC = ((t22 << 24) | (t22 << 20) | (t22 << 16) | 
	                            (t21 << 8) | (t21 << 4) | (t21));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg6 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg6 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg6));

	TxPwrOverMAC = ((t13 << 24) | (t13 << 20) | (t13 << 16) | 
	                            (t7 << 8) | (t7 << 4) | (t7));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg7 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg7 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg7));

	TxPwrOverMAC = ((t23 << 24) | (t23 << 20) | (t23 << 16) | 
	                            (t18 << 8) | (t18 << 4) | (t18));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg8 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg8 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg8));

	TxPwrOverMAC = ((t29 << 8) | (t29 << 4) | (t29));
	pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg9 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg9 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over2Dot4G.TxPwrCfg9));

	//
	// (c) Default Tx power for BW20 over 5G 
	//
	RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_5G, value);
	t1 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t1);
	t2 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t2);
	t3 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t3);
	t4 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t4);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_5G + 2), value);
	t5 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t5);
	t6 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t6);
	t7 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t7);
	t8 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t8);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_5G + 4), value);
	t9 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t9);
	t10 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t10);
	t11 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t11);
	t12 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t12);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_5G + 6), value);
	t13 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t13);
	t14 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t14);
	t15 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t15);
	t16 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t16);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_5G + 8), value);
	t17 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t17);
	t18 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t18);
	t19 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t19);
	t20 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t20);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_5G + 10), value);
	t21 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t21);
	t22 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t22);
	t23 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t23);
	t24 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t24);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_5G + 12), value);
	t25 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t25);
	t26 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t26);
	t27 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t27);
	t28 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t28);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW20_5G + 14), value);
	t29 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t29);
	t30 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t30);
	t31 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t31);
	t32 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t32);

	TxPwrOverMAC = ((t4 << 28) | (t4 << 24) | (t3 << 20) | (t3 << 16) | 
	                             (DEFAULT_TX_POWER << 12) | (DEFAULT_TX_POWER << 8) | (DEFAULT_TX_POWER << 4) | (DEFAULT_TX_POWER));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg0 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("\n%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg0 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg0));

	TxPwrOverMAC = ((t4 << 24) | (t3 << 16) | (DEFAULT_TX_POWER << 8) | (DEFAULT_TX_POWER));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg0Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg0Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg0Ext));

	TxPwrOverMAC = ((t10 << 28) | (t10 << 24) | (t9 << 20) | (t9 << 16) | 
	                             (t6 << 12) | (t6 << 8) | (t5 << 4) | (t5));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg1 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg1 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg1));

	TxPwrOverMAC = ((t10 << 24) | (t9 << 16) | (t6 << 8) | (t5));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg1Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg1Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg1Ext));

	TxPwrOverMAC = ((t15 << 28) | (t15 << 24) | (t14 << 20) | (t14 << 16) | 
	                             (t12 << 12) | (t12 << 8) | (t11 << 4) | (t11));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg2 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg2 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg2));

	TxPwrOverMAC = ((t15 << 24) | (t14 << 16) | (t12 << 8) | (t11));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg2Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg2Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg2Ext));

	TxPwrOverMAC = ((t26 << 28) | (t26 << 24) | (t25 << 20) | (t25 << 16) | 
	                             (t17 << 12) | (t17 << 8) | (t16 << 4) | (t16));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg3 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg3 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg3));

	TxPwrOverMAC = ((t26 << 24) | (t25 << 16) | (t17 << 8) | (t16));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg3Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg3Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg3Ext));

	TxPwrOverMAC = ((t28 << 12) | (t28 << 8) | (t27 << 4) | (t27));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg4 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg4 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg4));

	TxPwrOverMAC = ((t28 << 8) | (t27));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg4Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg4Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg4Ext));

	TxPwrOverMAC = ((t20 << 24) | (t20 << 20) | (t20 << 16) | 
	                            (t19 << 8) | (t19 << 4) | (t19));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg5 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg5 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg5));

	TxPwrOverMAC = ((t22 << 24) | (t22 << 20) | (t22 << 16) | 
	                            (t21 << 8) | (t21 << 4) | (t21));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg6 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg6 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg6));

	TxPwrOverMAC = ((t13 << 24) | (t13 << 20) | (t13 << 16) | 
	                            (t7 << 8) | (t7 << 4) | (t7));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg7 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg7 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg7));

	TxPwrOverMAC = ((t23 << 24) | (t23 << 20) | (t23 << 16) | 
	                            (t18 << 8) | (t18 << 4) | (t18));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg8 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg8 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg8));

	TxPwrOverMAC = ((t29 << 8) | (t29 << 4) | (t29));
	pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg9 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg9 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW20Over5G.TxPwrCfg9));

	//
	// (d) Default Tx power for BW40 over 5G 
	//
	RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_5G, value);
	t1 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t1);
	t2 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t2);
	t3 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t3);
	t4 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t4);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_5G + 2), value);
	t5 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t5);
	t6 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t6);
	t7 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t7);
	t8 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t8);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_5G + 4), value);
	t9 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t9);
	t10 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t10);
	t11 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t11);
	t12 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t12);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_5G + 6), value);
	t13 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t13);
	t14 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t14);
	t15 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t15);
	t16 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t16);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_5G + 8), value);
	t17 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t17);
	t18 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t18);
	t19 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t19);
	t20 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t20);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_5G + 10), value);
	t21 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t21);
	t22 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t22);
	t23 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t23);
	t24 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t24);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_5G + 12), value);
	t25 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t25);
	t26 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t26);
	t27 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t27);
	t28 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t28);
	RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_PWR_CTRL_OVER_BW40_5G + 14), value);
	t29 = (value & 0x000F);
	RTMPVerifyTxPwrPerRateExt(pAd, &t29);
	t30 = ((value & 0x00F0) >> 4);
	RTMPVerifyTxPwrPerRateExt(pAd, &t30);
	t31 = ((value & 0x0F00) >> 8);
	RTMPVerifyTxPwrPerRateExt(pAd, &t31);
	t32 = ((value & 0xF000) >> 12);
	RTMPVerifyTxPwrPerRateExt(pAd, &t32);

	TxPwrOverMAC = ((t4 << 28) | (t4 << 24) | (t3 << 20) | (t3 << 16) | 
	                             (DEFAULT_TX_POWER << 12) | (DEFAULT_TX_POWER << 8) | (DEFAULT_TX_POWER << 4) | (DEFAULT_TX_POWER));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg0 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("\n%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg0 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg0));

	TxPwrOverMAC = ((t4 << 24) | (t3 << 16) | (DEFAULT_TX_POWER << 8) | (DEFAULT_TX_POWER));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg0Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg0Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg0Ext));

	TxPwrOverMAC = ((t10 << 28) | (t10 << 24) | (t9 << 20) | (t9 << 16) | 
	                             (t6 << 12) | (t6 << 8) | (t5 << 4) | (t5));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg1 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg1 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg1));

	TxPwrOverMAC = ((t10 << 24) | (t9 << 16) | (t6 << 8) | (t5));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg1Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg1Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg1Ext));

	TxPwrOverMAC = ((t15 << 28) | (t15 << 24) | (t14 << 20) | (t14 << 16) | 
	                             (t12 << 12) | (t12 << 8) | (t11 << 4) | (t11));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg2 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg2 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg2));

	TxPwrOverMAC = ((t15 << 24) | (t14 << 16) | (t12 << 8) | (t11));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg2Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg2Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg2Ext));

	TxPwrOverMAC = ((t26 << 28) | (t26 << 24) | (t25 << 20) | (t25 << 16) | 
	                             (t17 << 12) | (t17 << 8) | (t16 << 4) | (t16));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg3 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg3 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg3));

	TxPwrOverMAC = ((t26 << 24) | (t25 << 16) | (t17 << 8) | (t16));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg3Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg3Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg3Ext));

	TxPwrOverMAC = ((t28 << 12) | (t28 << 8) | (t27 << 4) | (t27));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg4 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg4 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg4));

	TxPwrOverMAC = ((t28 << 8) | (t27));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg4Ext = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg4Ext = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg4Ext));

	TxPwrOverMAC = ((t20 << 24) | (t20 << 20) | (t20 << 16) | 
	                            (t19 << 8) | (t19 << 4) | (t19));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg5 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg5 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg5));

	TxPwrOverMAC = ((t22 << 24) | (t22 << 20) | (t22 << 16) | 
	                            (t21 << 8) | (t21 << 4) | (t21));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg6 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg6 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg6));

	TxPwrOverMAC = ((t13 << 24) | (t13 << 20) | (t13 << 16) | 
	                            (t7 << 8) | (t7 << 4) | (t7));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg7 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg7 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg7));

	TxPwrOverMAC = ((t23 << 24) | (t23 << 20) | (t23 << 16) | 
	                            (t18 << 8) | (t18 << 4) | (t18));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg8 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg8 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg8));

	TxPwrOverMAC = ((t29 << 8) | (t29 << 4) | (t29));
	pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg9 = TxPwrOverMAC;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg9 = 0x%08lX\n", 
		__FUNCTION__, 
		pAd->TxPwrCtrlExtOverMAC.BW40Over5G.TxPwrCfg9));


	DBGPRINT(RT_DEBUG_TRACE, ("<-- %s\n", __FUNCTION__));
}
#endif // RT3593 //


/*
	========================================================================
	
	Routine Description:
		Read initial Tx power per MCS and BW from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID	RTMPReadTxPwrPerRate(
	IN	PRTMP_ADAPTER	pAd)
{
	ULONG		data, Adata, Gdata;
	USHORT		i, value, value2;
	USHORT		value_1, value_2, value_3, value_4;
	INT			Apwrdelta, Gpwrdelta;
	UCHAR		t1,t2,t3,t4;
	BOOLEAN		bApwrdeltaMinus = TRUE, bGpwrdeltaMinus = TRUE;
	
#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		RTMPReadTxPwrPerRateExt(pAd);
	}
	else
#endif // RT3593 //
	{	
		//
		// Get power delta for 20MHz and 40MHz.
		//
		DBGPRINT(RT_DEBUG_TRACE, ("Txpower per Rate\n"));
		RT28xx_EEPROM_READ16(pAd, EEPROM_TXPOWER_DELTA, value2);
		Apwrdelta = 0;
		Gpwrdelta = 0;

		if ((value2 & 0xff) != 0xff)
		{
			if ((value2 & 0x80))
				Gpwrdelta = (value2&0xf);
			
			if ((value2 & 0x40))
				bGpwrdeltaMinus = FALSE;
			else
				bGpwrdeltaMinus = TRUE;
		}
		if ((value2 & 0xff00) != 0xff00)
		{
			if ((value2 & 0x8000))
				Apwrdelta = ((value2&0xf00)>>8);

			if ((value2 & 0x4000))
				bApwrdeltaMinus = FALSE;
			else
				bApwrdeltaMinus = TRUE;
		}	
		DBGPRINT(RT_DEBUG_TRACE, ("Gpwrdelta = %x, Apwrdelta = %x .\n", Gpwrdelta, Apwrdelta));

		//
		// Get Txpower per MCS for 20MHz in 2.4G.
		//
		for (i=0; i<5; i++)
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_TXPOWER_BYRATE_20MHZ_2_4G + i*4, value);
			data = value;

			/* use value_1 ~ value_4 for code size reduce */
			value_1 = value&0xf;
			value_2 = (value&0xf0)>>4;
			value_3 = (value&0xf00)>>8;
			value_4 = (value&0xf000)>>12;

			if (bApwrdeltaMinus == FALSE)
			{
				t1 = value_1+(Apwrdelta);
				if (t1 > 0xf)
					t1 = 0xf;
				t2 = value_2+(Apwrdelta);
				if (t2 > 0xf)
					t2 = 0xf;
				t3 = value_3+(Apwrdelta);
				if (t3 > 0xf)
					t3 = 0xf;
				t4 = value_4+(Apwrdelta);
				if (t4 > 0xf)
					t4 = 0xf;
			}
			else
			{
				if (value_1 > Apwrdelta)
					t1 = value_1-(Apwrdelta);
				else
					t1 = 0;
				if (value_2 > Apwrdelta)
					t2 = value_2-(Apwrdelta);
				else
					t2 = 0;
				if (value_3 > Apwrdelta)
					t3 = value_3-(Apwrdelta);
				else
					t3 = 0;
				if (value_4 > Apwrdelta)
					t4 = value_4-(Apwrdelta);
				else
					t4 = 0;
			}				
			Adata = t1 + (t2<<4) + (t3<<8) + (t4<<12);
			if (bGpwrdeltaMinus == FALSE)
			{
				t1 = value_1+(Gpwrdelta);
				if (t1 > 0xf)
					t1 = 0xf;
				t2 = value_2+(Gpwrdelta);
				if (t2 > 0xf)
					t2 = 0xf;
				t3 = value_3+(Gpwrdelta);
				if (t3 > 0xf)
					t3 = 0xf;
				t4 = value_4+(Gpwrdelta);
				if (t4 > 0xf)
					t4 = 0xf;
			}
			else
			{
				if (value_1 > Gpwrdelta)
					t1 = value_1-(Gpwrdelta);
				else
					t1 = 0;
				if (value_2 > Gpwrdelta)
					t2 = value_2-(Gpwrdelta);
				else
					t2 = 0;
				if (value_3 > Gpwrdelta)
					t3 = value_3-(Gpwrdelta);
				else
					t3 = 0;
				if (value_4 > Gpwrdelta)
					t4 = value_4-(Gpwrdelta);
				else
					t4 = 0;
			}				
			Gdata = t1 + (t2<<4) + (t3<<8) + (t4<<12);
			
			RT28xx_EEPROM_READ16(pAd, EEPROM_TXPOWER_BYRATE_20MHZ_2_4G + i*4 + 2, value);

			/* use value_1 ~ value_4 for code size reduce */
			value_1 = value&0xf;
			value_2 = (value&0xf0)>>4;
			value_3 = (value&0xf00)>>8;
			value_4 = (value&0xf000)>>12;

			if (bApwrdeltaMinus == FALSE)
			{
				t1 = value_1+(Apwrdelta);
				if (t1 > 0xf)
					t1 = 0xf;
				t2 = value_2+(Apwrdelta);
				if (t2 > 0xf)
					t2 = 0xf;
				t3 = value_3+(Apwrdelta);
				if (t3 > 0xf)
					t3 = 0xf;
				t4 = value_4+(Apwrdelta);
				if (t4 > 0xf)
					t4 = 0xf;
			}
			else
			{
				if (value_1 > Apwrdelta)
					t1 = value_1-(Apwrdelta);
				else
					t1 = 0;
				if (value_2 > Apwrdelta)
					t2 = value_2-(Apwrdelta);
				else
					t2 = 0;
				if (value_3 > Apwrdelta)
					t3 = value_3-(Apwrdelta);
				else
					t3 = 0;
				if (value_4 > Apwrdelta)
					t4 = value_4-(Apwrdelta);
				else
					t4 = 0;
			}				
			Adata |= ((t1<<16) + (t2<<20) + (t3<<24) + (t4<<28));
			if (bGpwrdeltaMinus == FALSE)
			{
				t1 = value_1+(Gpwrdelta);
				if (t1 > 0xf)
					t1 = 0xf;
				t2 = value_2+(Gpwrdelta);
				if (t2 > 0xf)
					t2 = 0xf;
				t3 = value_3+(Gpwrdelta);
				if (t3 > 0xf)
					t3 = 0xf;
				t4 = value_4+(Gpwrdelta);
				if (t4 > 0xf)
					t4 = 0xf;
			}
			else
			{
				if (value_1 > Gpwrdelta)
					t1 = value_1-(Gpwrdelta);
				else
					t1 = 0;
				if (value_2 > Gpwrdelta)
					t2 = value_2-(Gpwrdelta);
				else
					t2 = 0;
				if (value_3 > Gpwrdelta)
					t3 = value_3-(Gpwrdelta);
				else
					t3 = 0;
				if (value_4 > Gpwrdelta)
					t4 = value_4-(Gpwrdelta);
				else
					t4 = 0;
			}				
			Gdata |= ((t1<<16) + (t2<<20) + (t3<<24) + (t4<<28));
			data |= (value<<16);

			/* For 20M/40M Power Delta issue */		
			pAd->Tx20MPwrCfgABand[i] = data;
			pAd->Tx20MPwrCfgGBand[i] = data;
			pAd->Tx40MPwrCfgABand[i] = Adata;
			pAd->Tx40MPwrCfgGBand[i] = Gdata;
			
			if (data != 0xffffffff)
				RTMP_IO_WRITE32(pAd, TX_PWR_CFG_0 + i*4, data);
			DBGPRINT_RAW(RT_DEBUG_TRACE, ("20MHz BW, 2.4G band-%lx,  Adata = %lx,  Gdata = %lx \n", data, Adata, Gdata));
		}
	}
}


/*
	========================================================================
	
	Routine Description:
		Read initial channel power parameters from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID	RTMPReadChannelPwr(
	IN	PRTMP_ADAPTER	pAd)
{
	UINT32					i, choffset;
	EEPROM_TX_PWR_STRUC	    Power;
	EEPROM_TX_PWR_STRUC	    Power2;
#ifdef RT3593
	EEPROM_TX_PWR_STRUC	    Power3;
	UCHAR Tx0ALC = 0, Tx1ALC = 0, Tx2ALC = 0, Tx0FinePowerCtrl = 0, Tx1FinePowerCtrl = 0, Tx2FinePowerCtrl = 0;
	EEPROM_ANTENNA_STRUC NICConfig0 = {{ 0 }};

	RT28xx_EEPROM_READ16(pAd, EEPROM_NIC1_OFFSET, NICConfig0.word);

	DBGPRINT(RT_DEBUG_TRACE, ("%s: NICConfig0.field.TxPath = %d, NICConfig0.field.RxPath = %d\n", 
		__FUNCTION__, 
		NICConfig0.field.TxPath, 
		NICConfig0.field.RxPath));
#endif // RT3593 //

	// Read Tx power value for all channels
	// Value from 1 - 0x7f. Default value is 24.
	// Power value : 2.4G 0x00 (0) ~ 0x1F (31)
	//             : 5.5G 0xF9 (-7) ~ 0x0F (15)

	// 0. 11b/g, ch1 - ch 14
	for (i = 0; i < 7; i++)
	{
#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			if (NICConfig0.field.TxPath == 3)
			{
				RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX0_OVER_2DOT4G + (i * 2)), Power.word);
				RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX1_OVER_2DOT4G + (i * 2)), Power2.word);
				RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX2_OVER_2DOT4G + (i * 2)), Power3.word);
			}
			else
			{
				RT28xx_EEPROM_READ16(pAd, (EEPROM_G_TX_PWR_OFFSET + i * 2), Power.word);
				RT28xx_EEPROM_READ16(pAd, (EEPROM_G_TX2_PWR_OFFSET + i * 2), Power2.word);
			}
			
			pAd->TxPower[i * 2].Channel = i * 2 + 1;
			pAd->TxPower[i * 2 + 1].Channel = i * 2 + 2;

			//
			// Tx0 power control
			//
			Tx0ALC = GET_TX_ALC_BIT_MASK & Power.field.Byte0;
			Tx0FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power.field.Byte0) >> NUMBER_OF_BITS_FOR_TX_ALC);

			if (Tx0ALC > 31)
			{
				pAd->TxPower[i * 2].Power = DEFAULT_RF_TX_POWER;
			}
			else
			{
				pAd->TxPower[i * 2].Power = Tx0ALC;
			}

			if (Tx0FinePowerCtrl > 4)
			{
				pAd->TxPower[i * 2].Tx0FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
			}
			else
			{
				pAd->TxPower[i * 2].Tx0FinePowerCtrl = Tx0FinePowerCtrl;
			}

			Tx0ALC = GET_TX_ALC_BIT_MASK & Power.field.Byte1;
			Tx0FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power.field.Byte1) >> NUMBER_OF_BITS_FOR_TX_ALC);

			if (Tx0ALC > 31)
			{
				pAd->TxPower[i * 2 + 1].Power = DEFAULT_RF_TX_POWER;
			}
			else
			{
				pAd->TxPower[i * 2 + 1].Power = Tx0ALC;
			}

			if (Tx0FinePowerCtrl > 4)
			{
				pAd->TxPower[i * 2 + 1].Tx0FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
			}
			else
			{
				pAd->TxPower[i * 2 + 1].Tx0FinePowerCtrl = Tx0FinePowerCtrl;
			}

			//
			// Tx1 power control
			//
			if (NICConfig0.field.TxPath >= 2)
			{
				Tx1ALC = GET_TX_ALC_BIT_MASK & Power2.field.Byte0;
				Tx1FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power2.field.Byte0) >> NUMBER_OF_BITS_FOR_TX_ALC);

				if (Tx1ALC > 31)
				{
					pAd->TxPower[i * 2].Power2 = DEFAULT_RF_TX_POWER;
				}
				else
				{
					pAd->TxPower[i * 2].Power2 = Tx1ALC;
				}

				if (Tx1FinePowerCtrl > 4)
				{
					pAd->TxPower[i * 2].Tx1FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
				}
				else
				{
					pAd->TxPower[i * 2].Tx1FinePowerCtrl = Tx1FinePowerCtrl;
				}

				Tx1ALC = GET_TX_ALC_BIT_MASK & Power2.field.Byte1;
				Tx1FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power2.field.Byte1) >> NUMBER_OF_BITS_FOR_TX_ALC);

				if (Tx1ALC > 31)
				{
					pAd->TxPower[i * 2 + 1].Power2 = DEFAULT_RF_TX_POWER;
				}
				else
				{
					pAd->TxPower[i * 2 + 1].Power2 = Tx1ALC;
				}

				if (Tx1FinePowerCtrl > 4)
				{
					pAd->TxPower[i * 2 + 1].Tx1FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
				}
				else
				{
					pAd->TxPower[i * 2 + 1].Tx1FinePowerCtrl = Tx1FinePowerCtrl;
				}
			}

			DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPower[%d].Power = 0x%02X, pAd->TxPower[%d].Tx0FinePowerCtrl = 0x%02X, pAd->TxPower[%d].Power = 0x%02X, pAd->TxPower[%d].Tx0FinePowerCtrl = 0x%02X, pAd->TxPower[%d].Power2 = 0x%02X, pAd->TxPower[%d].Tx1FinePowerCtrl = 0x%02X, pAd->TxPower[%d].Power2 = 0x%02X, pAd->TxPower[%d].Tx1FinePowerCtrl = 0x%02X, \n", 
				__FUNCTION__, 
				i * 2, 
				pAd->TxPower[i * 2].Power, 
				i * 2, 
				pAd->TxPower[i * 2].Tx0FinePowerCtrl, 
				i * 2 + 1, 
				pAd->TxPower[i * 2 + 1].Power, 
				i * 2 + 1, 
				pAd->TxPower[i * 2 + 1].Tx0FinePowerCtrl, 
				i * 2, 
				pAd->TxPower[i * 2].Power2, 
				i * 2, 
				pAd->TxPower[i * 2].Tx1FinePowerCtrl, 
				i * 2 + 1, 
				pAd->TxPower[i * 2 + 1].Power2, 
				i * 2 + 1, 
				pAd->TxPower[i * 2 + 1].Tx1FinePowerCtrl));

			//
			// Tx2 power control
			//
			if (NICConfig0.field.TxPath == 3)
			{
				Tx2ALC = GET_TX_ALC_BIT_MASK & Power3.field.Byte0;
				Tx2FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power3.field.Byte0) >> NUMBER_OF_BITS_FOR_TX_ALC);

				if (Tx2ALC > 31)
				{
					pAd->TxPower[i * 2].Power3 = DEFAULT_RF_TX_POWER;
				}
				else
				{
					pAd->TxPower[i * 2].Power3 = Tx2ALC;
				}

				if (Tx2FinePowerCtrl > 4)
				{
					pAd->TxPower[i * 2].Tx2FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
				}
				else
				{
					pAd->TxPower[i * 2].Tx2FinePowerCtrl = Tx2FinePowerCtrl;
				}

				Tx2ALC = GET_TX_ALC_BIT_MASK & Power3.field.Byte1;
				Tx2FinePowerCtrl = ((GET_TX_FINE_POWER_CTRL_BIT_MASK & Power3.field.Byte1) >> NUMBER_OF_BITS_FOR_TX_ALC);

				if (Tx2ALC > 31)
				{
					pAd->TxPower[i * 2 + 1].Power3 = DEFAULT_RF_TX_POWER;
				}
				else
				{
					pAd->TxPower[i * 2 + 1].Power3 = Tx2ALC;
				}

				if (Tx2FinePowerCtrl > 4)
				{
					pAd->TxPower[i * 2 + 1].Tx2FinePowerCtrl = DEFAULT_BBP_TX_FINE_POWER_CTRL;
				}
				else
				{
					pAd->TxPower[i * 2 + 1].Tx2FinePowerCtrl = Tx2FinePowerCtrl;
				}

				DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPower[%d].Power3 = 0x%02X, pAd->TxPower[%d].Tx2FinePowerCtrl = 0x%02X, pAd->TxPower[%d].Power3 = 0x%02X, pAd->TxPower[%d].Tx2FinePowerCtrl = 0x%02X\n", 
					__FUNCTION__, 
					i * 2, 
					pAd->TxPower[i * 2].Power3, 
					i * 2, 
					pAd->TxPower[i * 2].Tx2FinePowerCtrl, 
					i * 2 + 1, 
					pAd->TxPower[i * 2 + 1].Power3, 
					i * 2 + 1, 
					pAd->TxPower[i * 2 + 1].Tx2FinePowerCtrl));
			}
		}
		else // Tx power control over RF R12 and RF R13
#endif // RT3593 //
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TX_PWR_OFFSET + i * 2, Power.word);
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TX2_PWR_OFFSET + i * 2, Power2.word);
			pAd->TxPower[i * 2].Channel = i * 2 + 1;
			pAd->TxPower[i * 2 + 1].Channel = i * 2 + 2;

			if ((Power.field.Byte0 > 31) || (Power.field.Byte0 < 0))
				pAd->TxPower[i * 2].Power = DEFAULT_RF_TX_POWER;
			else
				pAd->TxPower[i * 2].Power = Power.field.Byte0;

			if ((Power.field.Byte1 > 31) || (Power.field.Byte1 < 0))
				pAd->TxPower[i * 2 + 1].Power = DEFAULT_RF_TX_POWER;
			else
				pAd->TxPower[i * 2 + 1].Power = Power.field.Byte1;

			if ((Power2.field.Byte0 > 31) || (Power2.field.Byte0 < 0))
				pAd->TxPower[i * 2].Power2 = DEFAULT_RF_TX_POWER;
			else
				pAd->TxPower[i * 2].Power2 = Power2.field.Byte0;

			if ((Power2.field.Byte1 > 31) || (Power2.field.Byte1 < 0))
				pAd->TxPower[i * 2 + 1].Power2 = DEFAULT_RF_TX_POWER;
			else
				pAd->TxPower[i * 2 + 1].Power2 = Power2.field.Byte1;
		}
	}
	
	// 1. U-NII lower/middle band: 36, 38, 40; 44, 46, 48; 52, 54, 56; 60, 62, 64 (including central frequency in BW 40MHz)
	// 1.1 Fill up channel
	choffset = 14;
	for (i = 0; i < 4; i++)
	{
		pAd->TxPower[3 * i + choffset + 0].Channel	= 36 + i * 8 + 0;
		pAd->TxPower[3 * i + choffset + 0].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 0].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 1].Channel	= 36 + i * 8 + 2;
		pAd->TxPower[3 * i + choffset + 1].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 1].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 2].Channel	= 36 + i * 8 + 4;
		pAd->TxPower[3 * i + choffset + 2].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 2].Power2	= DEFAULT_RF_TX_POWER;
	}

	// 1.2 Fill up power
	for (i = 0; i < 6; i++)
	{
#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			if (NICConfig0.field.TxPath == 3)
			{
				RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX0_OVER_5G + i * 2, Power.word);
				RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX1_OVER_5G + i * 2, Power2.word);
				RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX2_OVER_5G + i * 2, Power3.word);
			}
		}
		else
#endif // RT3593 //
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX_PWR_OFFSET + i * 2, Power.word);
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX2_PWR_OFFSET + i * 2, Power2.word);
		}

		if ((Power.field.Byte0 < 16) && (Power.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power = Power.field.Byte0;

		if ((Power.field.Byte1 < 16) && (Power.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power = Power.field.Byte1;			

		if ((Power2.field.Byte0 < 16) && (Power2.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power2 = Power2.field.Byte0;

		if ((Power2.field.Byte1 < 16) && (Power2.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power2 = Power2.field.Byte1;			

#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			if (NICConfig0.field.TxPath == 3)
			{
				if ((Power3.field.Byte0 < 16) && (Power3.field.Byte0 >= -7))
				{
					pAd->TxPower[i * 2 + choffset + 0].Power3 = Power3.field.Byte0;
				}

				if ((Power3.field.Byte1 < 16) && (Power3.field.Byte1 >= -7))
				{
					pAd->TxPower[i * 2 + choffset + 1].Power3 = Power3.field.Byte1;
				}
			}
		}
#endif // RT3593 //
	}
	
	// 2. HipperLAN 2 100, 102 ,104; 108, 110, 112; 116, 118, 120; 124, 126, 128; 132, 134, 136; 140 (including central frequency in BW 40MHz)
	// 2.1 Fill up channel
	choffset = 14 + 12;
	for (i = 0; i < 5; i++)
	{
		pAd->TxPower[3 * i + choffset + 0].Channel	= 100 + i * 8 + 0;
		pAd->TxPower[3 * i + choffset + 0].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 0].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 1].Channel	= 100 + i * 8 + 2;
		pAd->TxPower[3 * i + choffset + 1].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 1].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 2].Channel	= 100 + i * 8 + 4;
		pAd->TxPower[3 * i + choffset + 2].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 2].Power2	= DEFAULT_RF_TX_POWER;
	}
	pAd->TxPower[3 * 5 + choffset + 0].Channel		= 140;
	pAd->TxPower[3 * 5 + choffset + 0].Power		= DEFAULT_RF_TX_POWER;
	pAd->TxPower[3 * 5 + choffset + 0].Power2		= DEFAULT_RF_TX_POWER;
#ifdef RT3593
	pAd->TxPower[3 * 5 + choffset + 0].Power3		= DEFAULT_RF_TX_POWER;
#endif // RT3593 //

	// 2.2 Fill up power
	for (i = 0; i < 8; i++)
	{
#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			if (NICConfig0.field.TxPath == 3)
			{
				RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX0_OVER_5G + (choffset - 14) + i * 2, Power.word);
				RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX1_OVER_5G + (choffset - 14) + i * 2, Power2.word);
				RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX2_OVER_5G + (choffset - 14) + i * 2, Power3.word);
			}
		}
		else
#endif // RT3593 //
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX_PWR_OFFSET + (choffset - 14) + i * 2, Power.word);
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX2_PWR_OFFSET + (choffset - 14) + i * 2, Power2.word);
		}

		if ((Power.field.Byte0 < 16) && (Power.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power = Power.field.Byte0;

		if ((Power.field.Byte1 < 16) && (Power.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power = Power.field.Byte1;			

		if ((Power2.field.Byte0 < 16) && (Power2.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power2 = Power2.field.Byte0;

		if ((Power2.field.Byte1 < 16) && (Power2.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power2 = Power2.field.Byte1;			

#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			if (NICConfig0.field.TxPath == 3)
			{
				if ((Power3.field.Byte0 < 16) && (Power3.field.Byte0 >= -7))
				{
					pAd->TxPower[i * 2 + choffset + 0].Power3 = Power3.field.Byte0;
				}

				if ((Power3.field.Byte1 < 16) && (Power3.field.Byte1 >= -7))
				{
					pAd->TxPower[i * 2 + choffset + 1].Power3 = Power3.field.Byte1;
				}
			}
		}
#endif // RT3593 //
	}

	// 3. U-NII upper band: 149, 151, 153; 157, 159, 161; 165, 167, 169; 171, 173 (including central frequency in BW 40MHz)
	// 3.1 Fill up channel
	choffset = 14 + 12 + 16;
	/*for (i = 0; i < 2; i++)*/
	for (i = 0; i < 3; i++)
	{
		pAd->TxPower[3 * i + choffset + 0].Channel	= 149 + i * 8 + 0;
		pAd->TxPower[3 * i + choffset + 0].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 0].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 1].Channel	= 149 + i * 8 + 2;
		pAd->TxPower[3 * i + choffset + 1].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 1].Power2	= DEFAULT_RF_TX_POWER;

		pAd->TxPower[3 * i + choffset + 2].Channel	= 149 + i * 8 + 4;
		pAd->TxPower[3 * i + choffset + 2].Power	= DEFAULT_RF_TX_POWER;
		pAd->TxPower[3 * i + choffset + 2].Power2	= DEFAULT_RF_TX_POWER;
	}
	pAd->TxPower[3 * 3 + choffset + 0].Channel		= 171;
	pAd->TxPower[3 * 3 + choffset + 0].Power		= DEFAULT_RF_TX_POWER;
	pAd->TxPower[3 * 3 + choffset + 0].Power2		= DEFAULT_RF_TX_POWER;
#ifdef RT3593
	pAd->TxPower[3 * 3 + choffset + 0].Power3		= DEFAULT_RF_TX_POWER;
#endif // RT3593 //

	pAd->TxPower[3 * 3 + choffset + 1].Channel		= 173;
	pAd->TxPower[3 * 3 + choffset + 1].Power		= DEFAULT_RF_TX_POWER;
	pAd->TxPower[3 * 3 + choffset + 1].Power2		= DEFAULT_RF_TX_POWER;
#ifdef RT3593
	pAd->TxPower[3 * 3 + choffset + 1].Power3		= DEFAULT_RF_TX_POWER;
#endif // RT3593 //

	// 3.2 Fill up power
	/*for (i = 0; i < 4; i++)*/
	for (i = 0; i < 6; i++)
	{
#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			if (NICConfig0.field.TxPath == 3)
			{
				RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX0_OVER_5G + (choffset - 14) + i * 2, Power.word);
				RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX1_OVER_5G + (choffset - 14) + i * 2, Power2.word);
				RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_TX2_OVER_5G + (choffset - 14) + i * 2, Power3.word);
			}
		}
		else
#endif // RT3593 //
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX_PWR_OFFSET + (choffset - 14) + i * 2, Power.word);
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TX2_PWR_OFFSET + (choffset - 14) + i * 2, Power2.word);
		}

		if ((Power.field.Byte0 < 16) && (Power.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power = Power.field.Byte0;

		if ((Power.field.Byte1 < 16) && (Power.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power = Power.field.Byte1;			

		if ((Power2.field.Byte0 < 16) && (Power2.field.Byte0 >= -7))
			pAd->TxPower[i * 2 + choffset + 0].Power2 = Power2.field.Byte0;

		if ((Power2.field.Byte1 < 16) && (Power2.field.Byte1 >= -7))
			pAd->TxPower[i * 2 + choffset + 1].Power2 = Power2.field.Byte1;			

#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			if (NICConfig0.field.TxPath == 3)
			{
				if ((Power3.field.Byte0 < 16) && (Power3.field.Byte0 >= -7))
				{
					pAd->TxPower[i * 2 + choffset + 0].Power3 = Power3.field.Byte0;
				}

				if ((Power3.field.Byte1 < 16) && (Power3.field.Byte1 >= -7))
				{
					pAd->TxPower[i * 2 + choffset + 1].Power3 = Power3.field.Byte1;
				}
			}
		}
#endif // RT3593 //
	}

	// 4. Print and Debug
	/*choffset = 14 + 12 + 16 + 7;*/
	choffset = 14 + 12 + 16 + 11;
	

}

/*
	========================================================================
	
	Routine Description:
		Read initial parameters from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID	NICReadEEPROMParameters(
	IN	PRTMP_ADAPTER	pAd,
	IN	PSTRING			mac_addr)
{
	UINT32			data = 0;
	USHORT			i, value, value2;
	UCHAR			TmpPhy;
	EEPROM_TX_PWR_STRUC	    Power;
	EEPROM_VERSION_STRUC    Version;
	EEPROM_ANTENNA_STRUC	Antenna;
	EEPROM_NIC_CONFIG2_STRUC    NicConfig2;
	USHORT  Addr01,Addr23,Addr45 ;
	MAC_DW0_STRUC csr2;
	MAC_DW1_STRUC csr3;


	DBGPRINT(RT_DEBUG_TRACE, ("--> NICReadEEPROMParameters\n"));	

	if (pAd->chipOps.eeinit)
	{
		pAd->chipOps.eeinit(pAd);
#ifdef RTMP_EFUSE_SUPPORT
#ifdef RT30xx
#ifdef RALINK_ATE
		if(!pAd->bFroceEEPROMBuffer && pAd->bEEPROMFile)
		{
			DBGPRINT(RT_DEBUG_TRACE, ("--> NICReadEEPROMParameters::(Efuse)Load to EEPROM Buffer Mode\n"));	
			eFuseLoadEEPROM(pAd);
		}
#endif // RALINK_ATE //
#endif // RT30xx //
#endif // RTMP_EFUSE_SUPPORT //
	}

	// Init EEPROM Address Number, before access EEPROM; if 93c46, EEPROMAddressNum=6, else if 93c66, EEPROMAddressNum=8
	RTMP_IO_READ32(pAd, E2PROM_CSR, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("--> E2PROM_CSR = 0x%x\n", data));

	if((data & 0x30) == 0)
		pAd->EEPROMAddressNum = 6;		// 93C46
	else if((data & 0x30) == 0x10)
		pAd->EEPROMAddressNum = 8;     // 93C66
	else
		pAd->EEPROMAddressNum = 8;     // 93C86
	DBGPRINT(RT_DEBUG_TRACE, ("--> EEPROMAddressNum = %d\n", pAd->EEPROMAddressNum ));

	/* Read MAC setting from EEPROM and record as permanent MAC address */
	DBGPRINT(RT_DEBUG_TRACE, ("Initialize MAC Address from E2PROM \n"));

	RT28xx_EEPROM_READ16(pAd, 0x04, Addr01);
	RT28xx_EEPROM_READ16(pAd, 0x06, Addr23);
	RT28xx_EEPROM_READ16(pAd, 0x08, Addr45);

	pAd->PermanentAddress[0] = (UCHAR)(Addr01 & 0xff);
	pAd->PermanentAddress[1] = (UCHAR)(Addr01 >> 8);
	pAd->PermanentAddress[2] = (UCHAR)(Addr23 & 0xff);
	pAd->PermanentAddress[3] = (UCHAR)(Addr23 >> 8);
	pAd->PermanentAddress[4] = (UCHAR)(Addr45 & 0xff);
	pAd->PermanentAddress[5] = (UCHAR)(Addr45 >> 8);

	//more conveninet to test mbssid, so ap's bssid &0xf1
	if (pAd->PermanentAddress[0] == 0xff)
		pAd->PermanentAddress[0] = RandomByte(pAd)&0xf8;
			
	DBGPRINT(RT_DEBUG_TRACE, ("E2PROM MAC: =%02x:%02x:%02x:%02x:%02x:%02x\n",
								PRINT_MAC(pAd->PermanentAddress)));

	/* Assign the actually working MAC Address */
	if (pAd->bLocalAdminMAC)
	{		
		DBGPRINT(RT_DEBUG_TRACE, ("Use the MAC address what is assigned from Configuration file(.dat). \n"));
	}
	else if (mac_addr && 
			 strlen((PSTRING)mac_addr) == 17 &&
			 (strcmp(mac_addr, "00:00:00:00:00:00") != 0))
	{
		INT		j;
		PSTRING	macptr;

		macptr = (PSTRING) mac_addr;

		for (j=0; j<MAC_ADDR_LEN; j++)
		{
			AtoH(macptr, &pAd->CurrentAddress[j], 1);
			macptr=macptr+3;
		}	
		
		DBGPRINT(RT_DEBUG_TRACE, ("Use the MAC address what is assigned from Moudle Parameter. \n"));
	}
	else
	{
		COPY_MAC_ADDR(pAd->CurrentAddress, pAd->PermanentAddress);
		DBGPRINT(RT_DEBUG_TRACE, ("Use the MAC address what is assigned from EEPROM. \n"));
	}

	/* Set the current MAC to ASIC */	
	csr2.field.Byte0 = pAd->CurrentAddress[0];
	csr2.field.Byte1 = pAd->CurrentAddress[1];
	csr2.field.Byte2 = pAd->CurrentAddress[2];
	csr2.field.Byte3 = pAd->CurrentAddress[3];
	RTMP_IO_WRITE32(pAd, MAC_ADDR_DW0, csr2.word);
	csr3.word = 0;
	csr3.field.Byte4 = pAd->CurrentAddress[4];
	csr3.field.Byte5 = pAd->CurrentAddress[5];
	csr3.field.U2MeMask = 0xff;
	RTMP_IO_WRITE32(pAd, MAC_ADDR_DW1, csr3.word);
	DBGPRINT_RAW(RT_DEBUG_TRACE,("Current MAC: =%02x:%02x:%02x:%02x:%02x:%02x\n",
					PRINT_MAC(pAd->CurrentAddress)));

	// if not return early. cause fail at emulation.
	// Init the channel number for TX channel power	
	RTMPReadChannelPwr(pAd);

	// if E2PROM version mismatch with driver's expectation, then skip
	// all subsequent E2RPOM retieval and set a system error bit to notify GUI
	RT28xx_EEPROM_READ16(pAd, EEPROM_VERSION_OFFSET, Version.word);
	pAd->EepromVersion = Version.field.Version + Version.field.FaeReleaseNumber * 256;
	DBGPRINT(RT_DEBUG_TRACE, ("E2PROM: Version = %d, FAE release #%d\n", Version.field.Version, Version.field.FaeReleaseNumber));

	if (Version.field.Version > VALID_EEPROM_VERSION)
	{
		DBGPRINT_ERR(("E2PROM: WRONG VERSION 0x%x, should be %d\n",Version.field.Version, VALID_EEPROM_VERSION));
		/*pAd->SystemErrorBitmap |= 0x00000001;

		// hard-code default value when no proper E2PROM installed
		pAd->bAutoTxAgcA = FALSE;
		pAd->bAutoTxAgcG = FALSE;

		// Default the channel power
		for (i = 0; i < MAX_NUM_OF_CHANNELS; i++)
			pAd->TxPower[i].Power = DEFAULT_RF_TX_POWER;

		// Default the channel power
		for (i = 0; i < MAX_NUM_OF_11JCHANNELS; i++)
			pAd->TxPower11J[i].Power = DEFAULT_RF_TX_POWER;
		
		for(i = 0; i < NUM_EEPROM_BBP_PARMS; i++)
			pAd->EEPROMDefaultValue[i] = 0xffff;
		return;  */
	}

	// Read BBP default value from EEPROM and store to array(EEPROMDefaultValue) in pAd
	RT28xx_EEPROM_READ16(pAd, EEPROM_NIC1_OFFSET, value);
	pAd->EEPROMDefaultValue[EEPROM_NIC_CFG1_OFFSET] = value;

	RT28xx_EEPROM_READ16(pAd, EEPROM_NIC2_OFFSET, value);
	pAd->EEPROMDefaultValue[EEPROM_NIC_CFG2_OFFSET] = value;

#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		UCHAR CountryRegion5G = 0, CountryRegion2Dot4G = 0;

		RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_COUNTRY_REGION_CODE_FOR_5G_BAND - 1), value);
				//
		// Swap
		//
		CountryRegion5G = ((value & 0xFF00) >> 8);
		CountryRegion2Dot4G = (value & 0x00FF);

		value = ((CountryRegion2Dot4G << 8) | CountryRegion5G);
				pAd->EEPROMDefaultValue[EEPROM_COUNTRY_REG_OFFSET] = value;
	}
	else
#endif // RT3593 //
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_COUNTRY_REGION, value);	// Country Region
		pAd->EEPROMDefaultValue[EEPROM_COUNTRY_REG_OFFSET] = value;
	}


	for(i = 0; i < 8; i++)
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_BBP_BASE_OFFSET + i*2, value);
		pAd->EEPROMDefaultValue[i+EEPROM_BBP_ARRAY_OFFSET] = value;
	}

	// We have to parse NIC configuration 0 at here.
	// If TSSI did not have preloaded value, it should reset the TxAutoAgc to false
	// Therefore, we have to read TxAutoAgc control beforehand.
	// Read Tx AGC control bit
	Antenna.word = pAd->EEPROMDefaultValue[0];
	if (Antenna.word == 0xFFFF)
	{
#ifdef RT30xx
		if(IS_RT3090(pAd))
		{
			Antenna.word = 0;
			Antenna.field.RfIcType = RFIC_3020;
			Antenna.field.TxPath = 1;
			Antenna.field.RxPath = 1;		
		}
		else
#endif // RT30xx //
#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			Antenna.word = 0;
			Antenna.field.RfIcType = RFIC_3053;
			Antenna.field.TxPath = 3;
			Antenna.field.RxPath = 3;		
		}
		else
#endif // RT3593 //
		{

			Antenna.word = 0;
			Antenna.field.RfIcType = RFIC_2820;
			Antenna.field.TxPath = 1;
			Antenna.field.RxPath = 2;
			DBGPRINT(RT_DEBUG_WARN, ("E2PROM error, hard code as 0x%04x\n", Antenna.word));
		}
	}

	// Choose the desired Tx&Rx stream.
	if ((pAd->CommonCfg.TxStream == 0) || (pAd->CommonCfg.TxStream > Antenna.field.TxPath))
		pAd->CommonCfg.TxStream = Antenna.field.TxPath;

	if ((pAd->CommonCfg.RxStream == 0) || (pAd->CommonCfg.RxStream > Antenna.field.RxPath))
	{
		pAd->CommonCfg.RxStream = Antenna.field.RxPath;
	
		if ((pAd->MACVersion != RALINK_2883_VERSION) &&
			(pAd->CommonCfg.RxStream > 2))
		{
			// only 2 Rx streams for RT2860 series
			pAd->CommonCfg.RxStream = 2;
		}
	}


	/* EEPROM offset 0x36 - NIC Configuration 1 */
	NicConfig2.word = pAd->EEPROMDefaultValue[1];



#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		if ((NicConfig2.word & 0x00ff) == 0xff)
		{
			NicConfig2.word &= 0xff00;
		}

		if ((NicConfig2.word >> 8) == 0xff)
		{
			NicConfig2.word &= 0x00ff;
		}
	}
#endif // CONFIG_STA_SUPPORT //

	if (NicConfig2.field.DynamicTxAgcControl == 1)
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = TRUE;
	else
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = FALSE;
	
	/* Save value for future using */
	pAd->NicConfig2.word = NicConfig2.word;
	
	DBGPRINT_RAW(RT_DEBUG_TRACE, ("NICReadEEPROMParameters: RxPath = %d, TxPath = %d\n", Antenna.field.RxPath, Antenna.field.TxPath));

	// Save the antenna for future use
	pAd->Antenna.word = Antenna.word;

	// Set the RfICType here, then we can initialize RFIC related operation callbacks
	pAd->Mlme.RealRxPath = (UCHAR) Antenna.field.RxPath;

	pAd->RfIcType = (UCHAR) Antenna.field.RfIcType;

#ifdef CONFIG_STA_SUPPORT
#ifdef RTMP_MAC_USB
		sprintf((PSTRING) pAd->nickname, "RT2870STA");
#endif // RTMP_MAC_USB //
#endif // CONFIG_STA_SUPPORT //

#ifdef RTMP_RF_RW_SUPPORT
	RtmpChipOpsRFHook(pAd);
#endif // RTMP_RF_RW_SUPPORT //

	//
	// Reset PhyMode if we don't support 802.11a
	// Only RFIC_2850 & RFIC_2750 support 802.11a
	//
	if ((Antenna.field.RfIcType != RFIC_2850)
		&& (Antenna.field.RfIcType != RFIC_2750)
		&& (Antenna.field.RfIcType != RFIC_3052)
#ifdef RT3593
		&& (Antenna.field.RfIcType != RFIC_3053)
#endif // RT3593
		)
	{
		if ((pAd->CommonCfg.PhyMode == PHY_11ABG_MIXED) || 
			(pAd->CommonCfg.PhyMode == PHY_11A))
			pAd->CommonCfg.PhyMode = PHY_11BG_MIXED;
#ifdef DOT11_N_SUPPORT
		else if ((pAd->CommonCfg.PhyMode == PHY_11ABGN_MIXED)	|| 
				 (pAd->CommonCfg.PhyMode == PHY_11AN_MIXED) 	|| 
				 (pAd->CommonCfg.PhyMode == PHY_11AGN_MIXED) 	||
				 (pAd->CommonCfg.PhyMode == PHY_11N_5G))
			pAd->CommonCfg.PhyMode = PHY_11BGN_MIXED;
#endif // DOT11_N_SUPPORT //

		pAd->RFICType = RFIC_24GHZ; // CRDA
	}
	else
	{
		pAd->RFICType = RFIC_24GHZ | RFIC_5GHZ; // CRDA
	}
	
	// Read TSSI reference and TSSI boundary for temperature compensation. This is ugly
	// 0. 11b/g
	{
		/* these are tempature reference value (0x00 ~ 0xFE)
		   ex: 0x00 0x15 0x25 0x45 0x88 0xA0 0xB5 0xD0 0xF0
		   TssiPlusBoundaryG [4] [3] [2] [1] [0] (smaller) +
		   TssiMinusBoundaryG[0] [1] [2] [3] [4] (larger) */
#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_2DOT4G, Power.word);
			pAd->TssiMinusBoundaryG[4] = Power.field.Byte0;
			pAd->TssiMinusBoundaryG[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_2DOT4G + 2), Power.word);
			pAd->TssiMinusBoundaryG[2] = Power.field.Byte0;
			pAd->TssiMinusBoundaryG[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_2DOT4G + 4), Power.word);
			pAd->TssiRefG = Power.field.Byte0;
			pAd->TssiPlusBoundaryG[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_2DOT4G + 6), Power.word);
			pAd->TssiPlusBoundaryG[2] = Power.field.Byte0;
			pAd->TssiPlusBoundaryG[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_2DOT4G + 8), Power.word);
			pAd->TssiPlusBoundaryG[4] = Power.field.Byte0;

			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_ALC_STEP_VALUE_OVER_2DOT4G - 1), value);
			pAd->TxAgcStepG = ((value & 0xFF00) >> 8);

			pAd->TxAgcCompensateG = 0;
			pAd->TssiMinusBoundaryG[0] = pAd->TssiRefG;
			pAd->TssiPlusBoundaryG[0]  = pAd->TssiRefG;

			// Disable TxAgc if the based value is not right
			if (pAd->TssiRefG == 0xFF)
			{
				pAd->bAutoTxAgcG = FALSE;
			}
		}
		else
#endif // RT3593 //
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND1, Power.word);
			pAd->TssiMinusBoundaryG[4] = Power.field.Byte0;
			pAd->TssiMinusBoundaryG[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND2, Power.word);
			pAd->TssiMinusBoundaryG[2] = Power.field.Byte0;
			pAd->TssiMinusBoundaryG[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND3, Power.word);
			pAd->TssiRefG   = Power.field.Byte0; /* reference value [0] */
			pAd->TssiPlusBoundaryG[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND4, Power.word);
			pAd->TssiPlusBoundaryG[2] = Power.field.Byte0;
			pAd->TssiPlusBoundaryG[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_G_TSSI_BOUND5, Power.word);
			pAd->TssiPlusBoundaryG[4] = Power.field.Byte0;
			pAd->TxAgcStepG = Power.field.Byte1;    
			pAd->TxAgcCompensateG = 0;
			pAd->TssiMinusBoundaryG[0] = pAd->TssiRefG;
			pAd->TssiPlusBoundaryG[0]  = pAd->TssiRefG;

			// Disable TxAgc if the based value is not right
			if (pAd->TssiRefG == 0xff)
				pAd->bAutoTxAgcG = FALSE;
		}

		DBGPRINT(RT_DEBUG_TRACE,("E2PROM: G Tssi[-4 .. +4] = %d %d %d %d - %d -%d %d %d %d, step=%d, tuning=%d\n",
			pAd->TssiMinusBoundaryG[4], pAd->TssiMinusBoundaryG[3], pAd->TssiMinusBoundaryG[2], pAd->TssiMinusBoundaryG[1],
			pAd->TssiRefG,
			pAd->TssiPlusBoundaryG[1], pAd->TssiPlusBoundaryG[2], pAd->TssiPlusBoundaryG[3], pAd->TssiPlusBoundaryG[4],
			pAd->TxAgcStepG, pAd->bAutoTxAgcG));
	}	
	// 1. 11a
	{
#ifdef RT3593
		if (IS_RT3593(pAd))
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_5G, Power.word);
			pAd->TssiMinusBoundaryA[4] = Power.field.Byte0;
			pAd->TssiMinusBoundaryA[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_5G + 2), Power.word);
			pAd->TssiMinusBoundaryA[2] = Power.field.Byte0;
			pAd->TssiMinusBoundaryA[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_5G + 4), Power.word);
			pAd->TssiRefA = Power.field.Byte0;
			pAd->TssiPlusBoundaryA[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_5G + 6), Power.word);
			pAd->TssiPlusBoundaryA[2] = Power.field.Byte0;
			pAd->TssiPlusBoundaryA[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_DELTA_TSSI_BOUNDARY_OVER_5G + 8), Power.word);
			pAd->TssiPlusBoundaryA[4] = Power.field.Byte0;

			RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_TX_ALC_STEP_VALUE_OVER_5G - 1), value);
			pAd->TxAgcStepA = ((value & 0xFF00) >> 8);

			pAd->TxAgcCompensateA = 0;
			pAd->TssiMinusBoundaryA[0] = pAd->TssiRefA;
			pAd->TssiPlusBoundaryA[0]  = pAd->TssiRefA;

			// Disable TxAgc if the based value is not right
			if (pAd->TssiRefA == 0xFF)
			{
				pAd->bAutoTxAgcA = FALSE;
			}
		}
		else
#endif // RT3593 //
		{
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND1, Power.word);
			pAd->TssiMinusBoundaryA[4] = Power.field.Byte0;
			pAd->TssiMinusBoundaryA[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND2, Power.word);
			pAd->TssiMinusBoundaryA[2] = Power.field.Byte0;
			pAd->TssiMinusBoundaryA[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND3, Power.word);
			pAd->TssiRefA = Power.field.Byte0;
			pAd->TssiPlusBoundaryA[1] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND4, Power.word);
			pAd->TssiPlusBoundaryA[2] = Power.field.Byte0;
			pAd->TssiPlusBoundaryA[3] = Power.field.Byte1;
			RT28xx_EEPROM_READ16(pAd, EEPROM_A_TSSI_BOUND5, Power.word);
			pAd->TssiPlusBoundaryA[4] = Power.field.Byte0;
			pAd->TxAgcStepA = Power.field.Byte1;    
			pAd->TxAgcCompensateA = 0;
			pAd->TssiMinusBoundaryA[0] = pAd->TssiRefA;
			pAd->TssiPlusBoundaryA[0]  = pAd->TssiRefA;

			// Disable TxAgc if the based value is not right
			if (pAd->TssiRefA == 0xff)
				pAd->bAutoTxAgcA = FALSE;
		}

		DBGPRINT(RT_DEBUG_TRACE,("E2PROM: A Tssi[-4 .. +4] = %d %d %d %d - %d -%d %d %d %d, step=%d, tuning=%d\n",
			pAd->TssiMinusBoundaryA[4], pAd->TssiMinusBoundaryA[3], pAd->TssiMinusBoundaryA[2], pAd->TssiMinusBoundaryA[1],
			pAd->TssiRefA,
			pAd->TssiPlusBoundaryA[1], pAd->TssiPlusBoundaryA[2], pAd->TssiPlusBoundaryA[3], pAd->TssiPlusBoundaryA[4],
			pAd->TxAgcStepA, pAd->bAutoTxAgcA));
	}	
	pAd->BbpRssiToDbmDelta = 0x0;
	
	// Read frequency offset setting for RF
#ifdef RT3593
	if (IS_RT3593(pAd))
		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_FREQUENCY_OFFSET, value);
	else
#endif // RT3593 //
		RT28xx_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET, value);

	if ((value & 0x00FF) != 0x00FF)
		pAd->RfFreqOffset = (ULONG) (value & 0x00FF);
	else
		pAd->RfFreqOffset = 0;
	DBGPRINT(RT_DEBUG_TRACE, ("E2PROM: RF FreqOffset=0x%lx \n", pAd->RfFreqOffset));

	//CountryRegion byte offset (38h)
	value = pAd->EEPROMDefaultValue[EEPROM_COUNTRY_REG_OFFSET] >> 8;		// 2.4G band
	value2 = pAd->EEPROMDefaultValue[EEPROM_COUNTRY_REG_OFFSET] & 0x00FF;	// 5G band
	
	if ((value <= REGION_MAXIMUM_BG_BAND) && (value2 <= REGION_MAXIMUM_A_BAND))
	{
		pAd->CommonCfg.CountryRegion = ((UCHAR) value) | 0x80;
		pAd->CommonCfg.CountryRegionForABand = ((UCHAR) value2) | 0x80;
		TmpPhy = pAd->CommonCfg.PhyMode;
		pAd->CommonCfg.PhyMode = 0xff;
		RTMPSetPhyMode(pAd, TmpPhy);
#ifdef DOT11_N_SUPPORT
		SetCommonHT(pAd);
#endif // DOT11_N_SUPPORT //
	}

	//
	// Get RSSI Offset on EEPROM 0x9Ah & 0x9Ch.
	// The valid value are (-10 ~ 10) 
	// 
#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_RSSI0_OVER_2DOT4G, value);
		pAd->BGRssiOffset0 = (value & 0x00FF);
		pAd->BGRssiOffset1 = ((value >> 8) & 0x00FF);
	}
	else
#endif // RT3593 //
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_BG_OFFSET, value);
		pAd->BGRssiOffset0 = value & 0x00ff;
		pAd->BGRssiOffset1 = (value >> 8);
	}

#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_RSSI2_OVER_2DOT4G, value);
		pAd->BGRssiOffset2 = (value & 0x00FF);

		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_EXTERNAL_LNA_GAIN_FOR_5G_OVER_CH100_TO_CH128, value);
		pAd->ALNAGain1 = (value & 0x00FF);
	}
	else
#endif // RT3593 //
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_BG_OFFSET+2, value);
//		if (IS_RT2860(pAd)) // RT2860 supports 3 Rx and the 2.4 GHz RSSI #2 offset is in the EEPROM 0x48
			pAd->BGRssiOffset2 = value & 0x00ff;
		pAd->ALNAGain1 = (value >> 8);
	}

#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_EXTERNAL_LNA_GAIN_FOR_2DOT4G, value);
		pAd->BLNAGain = (value & 0x00FF);
		pAd->ALNAGain0 = (value >> 8);
	}
	else
#endif // RT3593 //
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_LNA_OFFSET, value);
		pAd->BLNAGain = value & 0x00ff;
		pAd->ALNAGain0 = (value >> 8);
	}
	
	// Validate 11b/g RSSI_0 offset.
	if ((pAd->BGRssiOffset0 < -10) || (pAd->BGRssiOffset0 > 10))
		pAd->BGRssiOffset0 = 0;

	// Validate 11b/g RSSI_1 offset.
	if ((pAd->BGRssiOffset1 < -10) || (pAd->BGRssiOffset1 > 10))
		pAd->BGRssiOffset1 = 0;

	// Validate 11b/g RSSI_2 offset.
	if ((pAd->BGRssiOffset2 < -10) || (pAd->BGRssiOffset2 > 10))
		pAd->BGRssiOffset2 = 0;

#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_RSSI0_OVER_5G, value);
		pAd->ARssiOffset0 = (value & 0x00FF);
		pAd->ARssiOffset1 = ((value >> 8) & 0x00FF);
	}
	else
#endif // RT3593 //
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_RSSI_A_OFFSET, value);
		pAd->ARssiOffset0 = value & 0x00ff;
		pAd->ARssiOffset1 = (value >> 8);
	}

#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_RSSI2_OVER_5G, value);
		pAd->ARssiOffset2 = (value & 0x00FF);

		RT28xx_EEPROM_READ16(pAd, (EEPROM_EXT_EXTERNAL_LNA_GAIN_FOR_5G_OVER_CH132_TO_CH165 - 1), value);
		pAd->ALNAGain2 = ((value >> 8) & 0x00FF);
	}
	else
#endif // RT3593 //
	{
		RT28xx_EEPROM_READ16(pAd, (EEPROM_RSSI_A_OFFSET+2), value);
		pAd->ARssiOffset2 = value & 0x00ff;
		pAd->ALNAGain2 = (value >> 8);
	}


	if (((UCHAR)pAd->ALNAGain1 == 0xFF) || (pAd->ALNAGain1 == 0x00))
		pAd->ALNAGain1 = pAd->ALNAGain0;
	if (((UCHAR)pAd->ALNAGain2 == 0xFF) || (pAd->ALNAGain2 == 0x00))
		pAd->ALNAGain2 = pAd->ALNAGain0;

	// Validate 11a RSSI_0 offset.
	if ((pAd->ARssiOffset0 < -10) || (pAd->ARssiOffset0 > 10))
		pAd->ARssiOffset0 = 0;

	// Validate 11a RSSI_1 offset.
	if ((pAd->ARssiOffset1 < -10) || (pAd->ARssiOffset1 > 10))
		pAd->ARssiOffset1 = 0;

	//Validate 11a RSSI_2 offset.
	if ((pAd->ARssiOffset2 < -10) || (pAd->ARssiOffset2 > 10))
		pAd->ARssiOffset2 = 0;

#ifdef RT30xx
	//
	// Get TX mixer gain setting
	// 0xff are invalid value
	// Note: RT30xX default value is 0x00 and will program to RF_R17 only when this value is not zero.
	//       RT359X default value is 0x02
	//
	if (IS_RT30xx(pAd) || IS_RT3572(pAd) || IS_RT3390(pAd) || IS_RT3593(pAd))
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_TXMIXER_GAIN_2_4G, value);
		pAd->TxMixerGain24G = 0;
		value &= 0x00ff;
		if (value != 0xff)
		{
			value &= 0x07;
			pAd->TxMixerGain24G = (UCHAR)value;
		}

#ifdef RT3593
		//2 TODO: test only!!!
		//2 TODO: Where are the Tx mixer gain for 2.4 GHz and 5 GHz?
		pAd->TxMixerGain24G = 0;
		pAd->TxMixerGain5G = 0;
#endif // RT3593 //
	}
#endif // RT30xx //
	
	//
	// Get LED Setting.
	//
#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_LED_MODE_SETTING - 1, value);
		pAd->LedCntl.word = ((value & 0xFF00) >> 8);

		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_LED_AG_CONFIGURATION, value);
		pAd->Led1 = value;

		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_LED_ACT_CONFIGURATION, value);
		pAd->Led2 = value;

		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_LED_AG_ACT_POLARITY, value);
		pAd->Led3 = value;
	}
	else
#endif // RT3593 //
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_FREQ_OFFSET, value);
		pAd->LedCntl.word = (value>>8);
		RT28xx_EEPROM_READ16(pAd, EEPROM_LED1_OFFSET, value);
		pAd->Led1 = value;
		RT28xx_EEPROM_READ16(pAd, EEPROM_LED2_OFFSET, value);
		pAd->Led2 = value;
		RT28xx_EEPROM_READ16(pAd, EEPROM_LED3_OFFSET, value);
		pAd->Led3 = value;
	}
		
	RTMPReadTxPwrPerRate(pAd);

#ifdef SINGLE_SKU
#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_EXT_MAX_TX_POWER_OVER_2DOT4G_AND_5G, pAd->CommonCfg.DefineMaxTxPwr);
	}
	else
#endif // RT3593 //
	{
		RT28xx_EEPROM_READ16(pAd, EEPROM_DEFINE_MAX_TXPWR, pAd->CommonCfg.DefineMaxTxPwr);
	}

	if ((pAd->CommonCfg.DefineMaxTxPwr & 0xFF) <= 0x50 && pAd->CommonCfg.AntGain > 0 && pAd->CommonCfg.BandedgeDelta >= 0)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Single SKU Mode is enabled\n"));
		pAd->CommonCfg.bSKUMode = TRUE;
	}
	else
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Single SKU Mode is disabled\n"));
		pAd->CommonCfg.bSKUMode = FALSE;
	}
#endif // SINGLE_SKU //

#ifdef RT30xx
#ifdef RTMP_EFUSE_SUPPORT
	RtmpEfuseSupportCheck(pAd);
#endif // RTMP_EFUSE_SUPPORT //
#endif // RT30xx //

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICReadEEPROMParameters\n"));
}


/*
	========================================================================
	
	Routine Description:
		Set default value from EEPROM
		
	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	
	Note:
		
	========================================================================
*/
VOID	NICInitAsicFromEEPROM(
	IN	PRTMP_ADAPTER	pAd)
{
#ifdef CONFIG_STA_SUPPORT
	UINT32					data = 0;
	UCHAR	BBPR1 = 0; 
#endif // CONFIG_STA_SUPPORT //
	USHORT					i;
	EEPROM_NIC_CONFIG2_STRUC    NicConfig2;
	UCHAR	BBPR3 = 0;
#ifdef RT30xx
	UCHAR			bbpreg = 0;
	UCHAR			RFValue = 0;
#endif // RT30xx //
	
	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitAsicFromEEPROM\n"));
	for(i = 3; i < NUM_EEPROM_BBP_PARMS; i++)
	{
		UCHAR BbpRegIdx, BbpValue;
	
		if ((pAd->EEPROMDefaultValue[i] != 0xFFFF) && (pAd->EEPROMDefaultValue[i] != 0))
		{
			BbpRegIdx = (UCHAR)(pAd->EEPROMDefaultValue[i] >> 8);
			BbpValue  = (UCHAR)(pAd->EEPROMDefaultValue[i] & 0xff);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BbpRegIdx, BbpValue);
		}
	}

	NicConfig2.word = pAd->NicConfig2.word;

#ifdef RT30xx
	// set default antenna as main
	if ((pAd->RfIcType == RFIC_2020) || (pAd->RfIcType == RFIC_3020) || (pAd->RfIcType == RFIC_3320))
		AsicSetRxAnt(pAd, pAd->RxAnt.Pair1PrimaryRxAnt);
#endif // RT30xx //

#ifdef LED_CONTROL_SUPPORT
	//
	// Send LED Setting to MCU.
	//
	if (pAd->LedCntl.word == 0xFF)
	{
		pAd->LedCntl.word = 0x01;
		pAd->Led1 = 0x5555;
		pAd->Led2 = 0x2221;

#ifdef RTMP_MAC_USB
		pAd->Led3 = 0x5627;
#endif // RTMP_MAC_USB //
	}

	AsicSendCommandToMcu(pAd, 0x52, 0xff, (UCHAR)pAd->Led1, (UCHAR)(pAd->Led1 >> 8));
	AsicSendCommandToMcu(pAd, 0x53, 0xff, (UCHAR)pAd->Led2, (UCHAR)(pAd->Led2 >> 8));
	AsicSendCommandToMcu(pAd, 0x54, 0xff, (UCHAR)pAd->Led3, (UCHAR)(pAd->Led3 >> 8));
	AsicSendCommandToMcu(pAd, 0x51, 0xff, 0, pAd->LedCntl.field.Polarity);
	
	pAd->LedIndicatorStrength = 0xFF;
	RTMPSetSignalLED(pAd, -100);	// Force signal strength Led to be turned off, before link up
#endif // LED_CONTROL_SUPPORT //

#ifdef RTMP_RF_RW_SUPPORT
	//Init RT30xx RFRegisters after read RFIC type from EEPROM
	NICInitRFRegisters(pAd);
#endif // RTMP_RF_RW_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		// Read Hardware controlled Radio state enable bit
		if (NicConfig2.field.HardwareRadioControl == 1)
		{
			pAd->StaCfg.bHardwareRadio = TRUE;

			// Read GPIO pin2 as Hardware controlled radio state
			RTMP_IO_READ32(pAd, GPIO_CTRL_CFG, &data);
			if ((data & 0x04) == 0)
			{
				pAd->StaCfg.bHwRadio = FALSE;
				pAd->StaCfg.bRadio = FALSE;
//				RTMP_IO_WRITE32(pAd, PWR_PIN_CFG, 0x00001818);
				RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
			}
		}
		else
			pAd->StaCfg.bHardwareRadio = FALSE;		

#ifdef LED_CONTROL_SUPPORT
		if (pAd->StaCfg.bRadio == FALSE)
		{
			RTMPSetLED(pAd, LED_RADIO_OFF);
		}
		else
		{
			RTMPSetLED(pAd, LED_RADIO_ON);
		}
#endif // LED_CONTROL_SUPPORT //

	}
#ifdef PCIE_PS_SUPPORT
#if defined(RT3090) || defined(RT3592) || defined(RT3390) || defined(RT3593)
		if (IS_RT3090(pAd)|| IS_RT3572(pAd) || IS_RT3390(pAd) || IS_RT3593(pAd))
		{
			RTMP_CHIP_OP *pChipOps = &pAd->chipOps;
			if (pChipOps->AsicReverseRfFromSleepMode)
				pChipOps->AsicReverseRfFromSleepMode(pAd);
		}
		// 3090 MCU Wakeup command needs more time to be stable. 
		// Before stable, don't issue other MCU command to prevent from firmware error.

		if ((IS_RT3090(pAd)|| IS_RT3572(pAd) ||
			IS_RT3390(pAd) || IS_RT3593(pAd)) && IS_VERSION_AFTER_F(pAd)
			&& (pAd->StaCfg.PSControl.field.rt30xxPowerMode == 3) 
			&& (pAd->StaCfg.PSControl.field.EnableNewPS == TRUE))
		{
			DBGPRINT(RT_DEBUG_TRACE,("%s::%d,release Mcu Lock\n",__FUNCTION__,__LINE__));
			RTMP_SEM_LOCK(&pAd->McuCmdLock);
			pAd->brt30xxBanMcuCmd = FALSE;
			RTMP_SEM_UNLOCK(&pAd->McuCmdLock);
		}
#endif // defined(RT3090) || defined(RT3592) || defined(RT3390) || defined(RT3593) //
#endif // PCIE_PS_SUPPORT //
#endif // CONFIG_STA_SUPPORT //
#ifdef RTMP_MAC_USB
		if (IS_RT3070(pAd) || IS_RT3090(pAd)|| IS_RT3572(pAd) || IS_RT3390(pAd))
		{
			RTMP_CHIP_OP *pChipOps = &pAd->chipOps;
			if (pChipOps->AsicReverseRfFromSleepMode)
				pChipOps->AsicReverseRfFromSleepMode(pAd);
		}
#endif // RTMP_MAC_USB //
	// Turn off patching for cardbus controller
	if (NicConfig2.field.CardbusAcceleration == 1)
	{
//		pAd->bTest1 = TRUE;
	}

	if (NicConfig2.field.DynamicTxAgcControl == 1)
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = TRUE;
	else
		pAd->bAutoTxAgcA = pAd->bAutoTxAgcG = FALSE;

#ifdef RTMP_INTERNAL_TX_ALC
/*
    Internal Tx ALC support is starting from RT3370 / RT3390, which combine PA / LNA in single chip.
    The old chipset don't have this, add new feature flag RTMP_INTERNAL_ALC.
 */
	// Internal Tx ALC
	if (((NicConfig2.field.DynamicTxAgcControl == 1) && 
            (NicConfig2.field.bInternalTxALC == 1)) || (!IS_RT3390(pAd)))
	{
		pAd->TxPowerCtrl.bInternalTxALC = FALSE;
	}
	else
	{
		if (NicConfig2.field.bInternalTxALC == 1)
		{
			pAd->TxPowerCtrl.bInternalTxALC = TRUE;
		}
		else
		{
			pAd->TxPowerCtrl.bInternalTxALC = FALSE;
		}
	}

	DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd->TxPowerCtrl.bInternalTxALC = %d\n", 
		__FUNCTION__, 
		pAd->TxPowerCtrl.bInternalTxALC));
#endif // RTMP_INTERNAL_TX_ALC //

	//
	// Since BBP has been progamed, to make sure BBP setting will be 
	// upate inside of AsicAntennaSelect, so reset to UNKNOWN_BAND!!
	//
	pAd->CommonCfg.BandState = UNKNOWN_BAND;
	
	RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &BBPR3);
	BBPR3 &= (~0x18);
	if(pAd->Antenna.field.RxPath == 3)
	{
		BBPR3 |= (0x10);
	}
	else if(pAd->Antenna.field.RxPath == 2)
	{
		BBPR3 |= (0x8);
	}
	else if(pAd->Antenna.field.RxPath == 1)
	{
		BBPR3 |= (0x0);
	}
	RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, BBPR3);
	
#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		// Handle the difference when 1T
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R1, &BBPR1);

#ifdef RT3593
        if (IS_RT3593(pAd))
        {
			if (pAd->Antenna.field.TxPath == 3)
			{
					BBPR1 = ((BBPR1 & ~0x18) | 0x10); // Number of transmitter chains availabe (N_TXER, 2: N_TXER = 3)
			}
			else if (pAd->Antenna.field.TxPath == 2)
			{
					BBPR1 = ((BBPR1 & ~0x18) | 0x08); // Number of transmitter chains availabe (N_TXER, 1: N_TXER = 2)
			}
			else if (pAd->Antenna.field.TxPath == 1)
			{
					BBPR1 = ((BBPR1 & ~0x18) | 0x00); // Number of transmitter chains availabe (N_TXER, 0: N_TXER = 1)
			}
        }
        else
#endif // RT3593 //
        {
			if(pAd->Antenna.field.TxPath == 1)
			{
			BBPR1 &= (~0x18);
			}
		}

		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R1, BBPR1);
    
		DBGPRINT(RT_DEBUG_TRACE, ("Use Hw Radio Control Pin=%d; if used Pin=%d;\n", 
					pAd->CommonCfg.bHardwareRadio, pAd->CommonCfg.bHardwareRadio));
	}
#endif // CONFIG_STA_SUPPORT //

#ifdef RT30xx
	// Initialize RT3070 serial MAC registers which is different from RT2870 serial
	if (IS_RT3090(pAd) || IS_RT3390(pAd) || IS_RT3593(pAd))
	{
		// enable DC filter
		if ((pAd->MACVersion & 0xffff) >= 0x0211)
		{
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R103, 0xc0);
		}

		// improve power consumption in RT3071 Ver.E 
		if ((pAd->MACVersion & 0xffff) >= 0x0211)
		{
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R31, &bbpreg);
			bbpreg &= (~0x3);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R31, bbpreg);
		}


		RTMP_IO_WRITE32(pAd, TX_SW_CFG1, 0);
		
		// RT3071 version E has fixed this issue
		if ((pAd->MACVersion & 0xffff) < 0x0211)
		{
			if (pAd->NicConfig2.field.DACTestBit == 1)
			{
				RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x2C);	// To fix throughput drop drastically
			}
			else
			{
				RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x0F);	// To fix throughput drop drastically
			}
		}
		else
		{
			RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x0);
		}
	}
	else if (IS_RT3070(pAd))
	{
		if ((pAd->MACVersion & 0xffff) >= 0x0201)
		{
			// enable DC filter
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R103, 0xc0);
			
			// improve power consumption in RT3070 Ver.F
			RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R31, &bbpreg);
			bbpreg &= (~0x3);
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R31, bbpreg);
		}
		/*
		     RT3070(E) Version[0200]
		     RT3070(F) Version[0201]
		 */
		if (((pAd->MACVersion & 0xffff) < 0x0201))
		{
			RTMP_IO_WRITE32(pAd, TX_SW_CFG1, 0);
			RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x2C);	// To fix throughput drop drastically
		}
		else
		{
			RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0);
		}
	}
	else if (IS_RT3071(pAd) || IS_RT3572(pAd))
	{
		RTMP_IO_WRITE32(pAd, TX_SW_CFG1, 0);
		if (((pAd->MACVersion & 0xffff) < 0x0211))
		{
			if (pAd->NicConfig2.field.DACTestBit == 1)
			{
				RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x1F); // To fix throughput drop drastically
			}
			else
			{
				RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x0F); // To fix throughput drop drastically
			}
		}
		else
		{
			RTMP_IO_WRITE32(pAd, TX_SW_CFG2, 0x0);
		}
	}

	// update registers from EEPROM for RT3071 or later(3572/3562/3592).
	if (IS_RT3090(pAd) || IS_RT3572(pAd) || IS_RT3390(pAd))
	{
		UCHAR RegIdx, RegValue;
		USHORT value;

		// after RT3071, write BBP from EEPROM 0xF0 to 0x102
		for (i = 0xF0; i <= 0x102; i = i+2)
		{
			value = 0xFFFF;
			RT28xx_EEPROM_READ16(pAd, i, value);
			if ((value != 0xFFFF) && (value != 0))
			{
				RegIdx = (UCHAR)(value >> 8);
				RegValue  = (UCHAR)(value & 0xff);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, RegIdx, RegValue);
				DBGPRINT(RT_DEBUG_TRACE, ("Update BBP Registers from EEPROM(0x%0x), BBP(0x%x) = 0x%x\n", i, RegIdx, RegValue));
			}
		}

		// after RT3071, write RF from EEPROM 0x104 to 0x116
		for (i = 0x104; i <= 0x116; i = i+2)
		{
			value = 0xFFFF;
			RT28xx_EEPROM_READ16(pAd, i, value);
			if ((value != 0xFFFF) && (value != 0))
			{
				RegIdx = (UCHAR)(value >> 8);
				RegValue  = (UCHAR)(value & 0xff);
				RT30xxWriteRFRegister(pAd, RegIdx, RegValue);
				DBGPRINT(RT_DEBUG_TRACE, ("Update RF Registers from EEPROM0x%x), BBP(0x%x) = 0x%x\n", i, RegIdx, RegValue));
			}
		}
	}
#endif // RT30xx //

	DBGPRINT(RT_DEBUG_TRACE, ("TxPath = %d, RxPath = %d, RFIC=%d, Polar+LED mode=%x\n", 
				pAd->Antenna.field.TxPath, pAd->Antenna.field.RxPath, 
				pAd->RfIcType, pAd->LedCntl.word));
	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitAsicFromEEPROM\n"));
}

/*
	========================================================================
	
	Routine Description:
		Initialize NIC hardware

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
NDIS_STATUS	NICInitializeAdapter(
	IN	PRTMP_ADAPTER	pAd,
	IN   BOOLEAN    bHardReset)
{
	NDIS_STATUS     Status = NDIS_STATUS_SUCCESS;
	WPDMA_GLO_CFG_STRUC	GloCfg;
//	INT_MASK_CSR_STRUC		IntMask;
	ULONG	i =0, j=0;
	//AC_TXOP_CSR0_STRUC	csr0;

	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitializeAdapter\n"));
	
	// 3. Set DMA global configuration except TX_DMA_EN and RX_DMA_EN bits:
retry:
	i = 0;
	do
	{
		RTMP_IO_READ32(pAd, WPDMA_GLO_CFG, &GloCfg.word);
		if ((GloCfg.field.TxDMABusy == 0)  && (GloCfg.field.RxDMABusy == 0))
			break;
		
		RTMPusecDelay(1000);
		i++;
	}while ( i<100);
	DBGPRINT(RT_DEBUG_TRACE, ("<== DMA offset 0x208 = 0x%x\n", GloCfg.word));	
	GloCfg.word &= 0xff0;
	GloCfg.field.EnTXWriteBackDDONE =1;
	RTMP_IO_WRITE32(pAd, WPDMA_GLO_CFG, GloCfg.word);
	
	// Record HW Beacon offset
#ifdef RT3593
	if (IS_RT3593(pAd))
	{
		DBGPRINT(RT_DEBUG_INFO, ("%s: Put the beacon contents in the higher shared memory location\n", __FUNCTION__));

		pAd->BeaconOffset[0] = HW_BEACON_BASE0_REDIRECTION;
		pAd->BeaconOffset[1] = HW_BEACON_BASE1_REDIRECTION;
		pAd->BeaconOffset[2] = HW_BEACON_BASE2_REDIRECTION;
		pAd->BeaconOffset[3] = HW_BEACON_BASE3_REDIRECTION;
		pAd->BeaconOffset[4] = HW_BEACON_BASE4_REDIRECTION;
		pAd->BeaconOffset[5] = HW_BEACON_BASE5_REDIRECTION;
		pAd->BeaconOffset[6] = HW_BEACON_BASE6_REDIRECTION;
		pAd->BeaconOffset[7] = HW_BEACON_BASE7_REDIRECTION;
	}
	else
#endif // RT3593 //
	{
		pAd->BeaconOffset[0] = HW_BEACON_BASE0;
		pAd->BeaconOffset[1] = HW_BEACON_BASE1;
		pAd->BeaconOffset[2] = HW_BEACON_BASE2;
		pAd->BeaconOffset[3] = HW_BEACON_BASE3;
		pAd->BeaconOffset[4] = HW_BEACON_BASE4;
		pAd->BeaconOffset[5] = HW_BEACON_BASE5;
		pAd->BeaconOffset[6] = HW_BEACON_BASE6;
		pAd->BeaconOffset[7] = HW_BEACON_BASE7;
	}
	
	//
	// write all shared Ring's base address into ASIC
	//

	// asic simulation sequence put this ahead before loading firmware.
	// pbf hardware reset

	// Initialze ASIC for TX & Rx operation
	if (NICInitializeAsic(pAd , bHardReset) != NDIS_STATUS_SUCCESS)
	{
		if (j++ == 0)
		{
			NICLoadFirmware(pAd);
			goto retry;
		}
		return NDIS_STATUS_FAILURE;
	}







	// reset action
	// Load firmware
	//  Status = NICLoadFirmware(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitializeAdapter\n"));
	return Status;
}

/*
	========================================================================
	
	Routine Description:
		Initialize ASIC

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
NDIS_STATUS	NICInitializeAsic(
	IN	PRTMP_ADAPTER	pAd,
	IN  BOOLEAN		bHardReset)
{
	ULONG			Index = 0;
	UCHAR			R0 = 0xff;
	UINT32			MacCsr12 = 0, Counter = 0;
#ifdef RTMP_MAC_USB
	UINT32			MacCsr0 = 0;
#endif // RTMP_MAC_USB //
	USHORT			KeyIdx;
	INT				i,apidx;

	DBGPRINT(RT_DEBUG_TRACE, ("--> NICInitializeAsic\n"));



#ifdef RTMP_MAC_USB
	//
	// Make sure MAC gets ready after NICLoadFirmware().
	//
	Index = 0;
	
	//To avoid hang-on issue when interface up in kernel 2.4, 
	//we use a local variable "MacCsr0" instead of using "pAd->MACVersion" directly.
	do 
	{
		RTMP_IO_READ32(pAd, MAC_CSR0, &MacCsr0);

		if ((MacCsr0 != 0x00) && (MacCsr0 != 0xFFFFFFFF))
			break;

		RTMPusecDelay(10);
	} while (Index++ < 100);

	pAd->MACVersion = MacCsr0;
	DBGPRINT(RT_DEBUG_TRACE, ("MAC_CSR0  [ Ver:Rev=0x%08x]\n", pAd->MACVersion));
	// turn on bit13 (set to zero) after rt2860D. This is to solve high-current issue.
	RTMP_IO_READ32(pAd, PBF_SYS_CTRL, &MacCsr12);
	MacCsr12 &= (~0x2000);
	RTMP_IO_WRITE32(pAd, PBF_SYS_CTRL, MacCsr12);
	
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x3);
	RTMP_IO_WRITE32(pAd, USB_DMA_CFG, 0x0);
	RTUSBVenderReset(pAd);
	RTMPusecDelay(1); 
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x0);

	// Initialize MAC register to default value
	for(Index=0; Index<NUM_MAC_REG_PARMS; Index++)
	{
#ifdef RT30xx
		if ((MACRegTable[Index].Register == TX_SW_CFG0) &&
			(IS_RT3070(pAd) || IS_RT3071(pAd) || IS_RT3572(pAd) || IS_RT3390(pAd) || IS_RT3593(pAd)))
		{
			MACRegTable[Index].Value = 0x00000400;
		}
#endif // RT30xx //
		RTMP_IO_WRITE32(pAd, (USHORT)MACRegTable[Index].Register, MACRegTable[Index].Value);
	}


#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		for (Index = 0; Index < NUM_STA_MAC_REG_PARMS; Index++)
		{
			RTMP_IO_WRITE32(pAd, (USHORT)STAMACRegTable[Index].Register, STAMACRegTable[Index].Value);
		}
	}
#endif // CONFIG_STA_SUPPORT //
#endif // RTMP_MAC_USB //

	//
	// Before program BBP, we need to wait BBP/RF get wake up.
	//
	Index = 0;
	do
	{
		RTMP_IO_READ32(pAd, MAC_STATUS_CFG, &MacCsr12);

		if ((MacCsr12 & 0x03) == 0)	// if BB.RF is stable
			break;
		
		DBGPRINT(RT_DEBUG_TRACE, ("Check MAC_STATUS_CFG  = Busy = %x\n", MacCsr12));
		RTMPusecDelay(1000);
	} while (Index++ < 100);


	// Wait to be stable.
	RTMPusecDelay(1000);
	pAd->LastMCUCmd = 0x72;

	// Read BBP register, make sure BBP is up and running before write new data
	Index = 0;
	do 
	{
		RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R0, &R0);
		DBGPRINT(RT_DEBUG_TRACE, ("BBP version = %x\n", R0));
	} while ((++Index < 20) && ((R0 == 0xff) || (R0 == 0x00)));
	//ASSERT(Index < 20); //this will cause BSOD on Check-build driver

	if ((R0 == 0xff) || (R0 == 0x00))
		return NDIS_STATUS_FAILURE;

	// Initialize BBP register to default value
	for (Index = 0; Index < NUM_BBP_REG_PARMS; Index++)
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBPRegTable[Index].Register, BBPRegTable[Index].Value);
	}

	if (pAd->chipOps.pBBPRegTable)
	{
		REG_PAIR *pbbpRegTb = pAd->chipOps.pBBPRegTable;
		
		for (Index = 0; Index < pAd->chipOps.bbpRegTbSize; Index++)
		{
			RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, pbbpRegTb[Index].Register, pbbpRegTb[Index].Value);
			DBGPRINT(RT_DEBUG_TRACE, ("BBP_R%d=%d\n", pbbpRegTb[Index].Register, pbbpRegTb[Index].Value));
		}
	}
	

	// for rt2860E and after, init BBP_R84 with 0x19. This is for extension channel overlapping IOT.
	// RT3090 should not program BBP R84 to 0x19, otherwise TX will block.
	//3070/71/72,3090,3090A( are included in RT30xx),3572,3390
	if (((pAd->MACVersion & 0xffff) != 0x0101) &&
		!(IS_RT30xx(pAd)|| IS_RT3572(pAd) || IS_RT3390(pAd) || IS_RT3593(pAd)))
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R84, 0x19);

#ifdef RT30xx
	// RF power sequence setup
	if (IS_RT30xx(pAd) || IS_RT3572(pAd) || IS_RT3390(pAd) || IS_RT3593(pAd))
	{	//update for RT3070/71/72/90/91/92,3572,3390.
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R79, 0x13);		
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R80, 0x05);	
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R81, 0x33);	
	}
#endif // RT30xx //

	if (pAd->MACVersion == 0x28600100)
	{
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R69, 0x16);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R73, 0x12);
	}
	
	if (pAd->MACVersion >= RALINK_2880E_VERSION && pAd->MACVersion < RALINK_3070_VERSION) // 3*3
	{
		// enlarge MAX_LEN_CFG
		UINT32 csr;
		RTMP_IO_READ32(pAd, MAX_LEN_CFG, &csr);
		{
		csr &= 0xFFF;
		csr |= 0x2000;
		}
		RTMP_IO_WRITE32(pAd, MAX_LEN_CFG, csr);
	}

#ifdef RTMP_MAC_USB
{
	UCHAR	MAC_Value[]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0,0};

	//Initialize WCID table
	for(Index =0 ;Index < 254;Index++)
	{
		RTUSBMultiWrite(pAd, (USHORT)(MAC_WCID_BASE + Index * 8), MAC_Value, 8);
	}
}
#endif // RTMP_MAC_USB //

#ifdef CONFIG_STA_SUPPORT
	// Add radio off control
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		if (pAd->StaCfg.bRadio == FALSE)
		{
//			RTMP_IO_WRITE32(pAd, PWR_PIN_CFG, 0x00001818);
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
			DBGPRINT(RT_DEBUG_TRACE, ("Set Radio Off\n"));
		}
	}
#endif // CONFIG_STA_SUPPORT //	

	// Clear raw counters
	RTMP_IO_READ32(pAd, RX_STA_CNT0, &Counter);
	RTMP_IO_READ32(pAd, RX_STA_CNT1, &Counter);
	RTMP_IO_READ32(pAd, RX_STA_CNT2, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT0, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT1, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT2, &Counter);
	
	// ASIC will keep garbage value after boot
	// Clear all shared key table when initial
	// This routine can be ignored in radio-ON/OFF operation. 
	if (bHardReset)
	{
		for (KeyIdx = 0; KeyIdx < 4; KeyIdx++)
		{
			RTMP_IO_WRITE32(pAd, SHARED_KEY_MODE_BASE + 4*KeyIdx, 0);
		}

		// Clear all pairwise key table when initial
		for (KeyIdx = 0; KeyIdx < 256; KeyIdx++)
		{
			RTMP_IO_WRITE32(pAd, MAC_WCID_ATTRIBUTE_BASE + (KeyIdx * HW_WCID_ATTRI_SIZE), 1);
		}
	}
	
	// assert HOST ready bit
//  RTMP_IO_WRITE32(pAd, MAC_CSR1, 0x0); // 2004-09-14 asked by Mark
//  RTMP_IO_WRITE32(pAd, MAC_CSR1, 0x4);

	// It isn't necessary to clear this space when not hard reset. 	
	if (bHardReset == TRUE)
	{
		// clear all on-chip BEACON frame space			
		for (apidx = 0; apidx < HW_BEACON_MAX_COUNT; apidx++)
		{
			for (i = 0; i < HW_BEACON_OFFSET>>2; i+=4)
				RTMP_IO_WRITE32(pAd, pAd->BeaconOffset[apidx] + i, 0x00); 
		}
	}
	
#ifdef RTMP_MAC_USB
	AsicDisableSync(pAd);
	// Clear raw counters
	RTMP_IO_READ32(pAd, RX_STA_CNT0, &Counter);
	RTMP_IO_READ32(pAd, RX_STA_CNT1, &Counter);
	RTMP_IO_READ32(pAd, RX_STA_CNT2, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT0, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT1, &Counter);
	RTMP_IO_READ32(pAd, TX_STA_CNT2, &Counter);
	// Default PCI clock cycle per ms is different as default setting, which is based on PCI.
	RTMP_IO_READ32(pAd, USB_CYC_CFG, &Counter);
	Counter&=0xffffff00;
	Counter|=0x000001e;
	RTMP_IO_WRITE32(pAd, USB_CYC_CFG, Counter);
#endif // RTMP_MAC_USB //

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		// for rt2860E and after, init TXOP_CTRL_CFG with 0x583f. This is for extension channel overlapping IOT.
		if ((pAd->MACVersion&0xffff) != 0x0101)
			RTMP_IO_WRITE32(pAd, TXOP_CTRL_CFG, 0x583f);
	}
#endif // CONFIG_STA_SUPPORT //

	DBGPRINT(RT_DEBUG_TRACE, ("<-- NICInitializeAsic\n"));
	return NDIS_STATUS_SUCCESS;
}



VOID NICUpdateFifoStaCounters(
	IN PRTMP_ADAPTER pAd)
{
	TX_STA_FIFO_STRUC	StaFifo;
	MAC_TABLE_ENTRY		*pEntry;
	UINT32				i = 0;
	UCHAR				pid = 0, wcid = 0;
	INT32				reTry;
	UCHAR				succMCS;

#ifdef RALINK_ATE		
	/* Nothing to do in ATE mode */
	if (ATE_ON(pAd))
		return;
#endif // RALINK_ATE //

		do
		{
			RTMP_IO_READ32(pAd, TX_STA_FIFO, &StaFifo.word);

			if (StaFifo.field.bValid == 0)
				break;
		
			wcid = (UCHAR)StaFifo.field.wcid;


		/* ignore NoACK and MGMT frame use 0xFF as WCID */
			if ((StaFifo.field.TxAckRequired == 0) || (wcid >= MAX_LEN_OF_MAC_TABLE))
			{
				i++;
				continue;
			}

			/* PID store Tx MCS Rate */
			pid = (UCHAR)StaFifo.field.PidType;

			pEntry = &pAd->MacTab.Content[wcid];

			pEntry->DebugFIFOCount++;

#ifdef DOT11_N_SUPPORT
#ifndef RT3593
			if (StaFifo.field.TxBF) // 3*3
				pEntry->TxBFCount++;
#endif // RT3593 //
#endif // DOT11_N_SUPPORT //

#ifdef UAPSD_AP_SUPPORT
			UAPSD_SP_AUE_Handle(pAd, pEntry, StaFifo.field.TxSuccess);
#endif // UAPSD_AP_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
			if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS))
				continue;
#endif // CONFIG_STA_SUPPORT //

			if (!StaFifo.field.TxSuccess)
			{
				pEntry->FIFOCount++;
				pEntry->OneSecTxFailCount++;
									
				if (pEntry->FIFOCount >= 1)
				{			
					DBGPRINT(RT_DEBUG_TRACE, ("#"));
#ifdef DOT11_N_SUPPORT
					pEntry->NoBADataCountDown = 64;
#endif // DOT11_N_SUPPORT //

//#ifdef CONFIG_STA_SUPPORT
//#ifdef DOT11Z_TDLS_SUPPORT
//					IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
//					{
//						if(IS_ENTRY_TDLS(pEntry))
//							TDLS_LinkTearDown(pAd);
//					}
//#endif // DOT11Z_TDLS_SUPPORT //
//#endif // CONFIG_STA_SUPPORT //

					// Update the continuous transmission counter.
					pEntry->ContinueTxFailCnt++;

					if(pEntry->PsMode == PWR_ACTIVE)
					{
#ifdef DOT11_N_SUPPORT					
						int tid;
						for (tid=0; tid<NUM_OF_TID; tid++)
						{
							BAOriSessionTearDown(pAd, pEntry->Aid,  tid, FALSE, FALSE);
						}
#endif // DOT11_N_SUPPORT //


#ifdef WDS_SUPPORT
						// fix WDS Jam issue
						if(IS_ENTRY_WDS(pEntry)
							&& (pEntry->LockEntryTx == FALSE)
							&& (pEntry->ContinueTxFailCnt >= pAd->ApCfg.EntryLifeCheck))
						{ 
							DBGPRINT(RT_DEBUG_TRACE, ("Entry %02x:%02x:%02x:%02x:%02x:%02x Blocked!! (Fail Cnt = %d)\n",
								pEntry->Addr[0],pEntry->Addr[1],pEntry->Addr[2],pEntry->Addr[3],
								pEntry->Addr[4],pEntry->Addr[5],pEntry->ContinueTxFailCnt ));

							pEntry->LockEntryTx = TRUE;
						}
#endif // WDS_SUPPORT //
					}

					//pEntry->FIFOCount = 0;
				}
				//pEntry->bSendBAR = TRUE;
			}
			else
			{
#ifdef DOT11_N_SUPPORT
				if ((pEntry->PsMode != PWR_SAVE) && (pEntry->NoBADataCountDown > 0))
				{
					pEntry->NoBADataCountDown--;
					if (pEntry->NoBADataCountDown==0)
					{
						DBGPRINT(RT_DEBUG_TRACE, ("@\n"));
					}
				}
#endif // DOT11_N_SUPPORT //
				pEntry->FIFOCount = 0;
				pEntry->OneSecTxNoRetryOkCount++;
				// update NoDataIdleCount when sucessful send packet to STA.
				pEntry->NoDataIdleCount = 0;
				pEntry->ContinueTxFailCnt = 0;
#ifdef WDS_SUPPORT
				pEntry->LockEntryTx = FALSE;
#endif // WDS_SUPPORT //
			}

			succMCS = StaFifo.field.SuccessRate & 0x7F;


			reTry = pid - succMCS;

			if (StaFifo.field.TxSuccess)
			{
				pEntry->TXMCSExpected[pid]++;
				if (pid == succMCS)
				{
					pEntry->TXMCSSuccessful[pid]++;
				}
				else 
				{
					pEntry->TXMCSAutoFallBack[pid][succMCS]++;
				}
			}
			else
			{
				pEntry->TXMCSFailed[pid]++;
			}

			if (reTry > 0)
			{
				if ((pid >= 12) && succMCS <=7)
				{
					reTry -= 4;
				} 
				pEntry->OneSecTxRetryOkCount += reTry;
			}

			i++;
			// ASIC store 16 stack
		} while ( i < (TX_RING_SIZE<<1) );

}

/*
	========================================================================
	
	Routine Description:
		Read statistical counters from hardware registers and record them
		in software variables for later on query

	Arguments:
		pAd					Pointer to our adapter

	Return Value:
		None

	IRQL = DISPATCH_LEVEL
	
	========================================================================
*/
VOID NICUpdateRawCounters(
	IN PRTMP_ADAPTER pAd)
{
	UINT32	OldValue;//, Value2;
	//ULONG	PageSum, OneSecTransmitCount;
	//ULONG	TxErrorRatio, Retry, Fail;
	RX_STA_CNT0_STRUC	 RxStaCnt0;
	RX_STA_CNT1_STRUC   RxStaCnt1;
	RX_STA_CNT2_STRUC   RxStaCnt2;
	TX_STA_CNT0_STRUC 	 TxStaCnt0;
	TX_STA_CNT1_STRUC	 StaTx1;
	TX_STA_CNT2_STRUC	 StaTx2;
#ifdef STATS_COUNT_SUPPORT
	TX_AGG_CNT_STRUC	TxAggCnt;
	TX_AGG_CNT0_STRUC	TxAggCnt0;
	TX_AGG_CNT1_STRUC	TxAggCnt1;
	TX_AGG_CNT2_STRUC	TxAggCnt2;
	TX_AGG_CNT3_STRUC	TxAggCnt3;
	TX_AGG_CNT4_STRUC	TxAggCnt4;
	TX_AGG_CNT5_STRUC	TxAggCnt5;
	TX_AGG_CNT6_STRUC	TxAggCnt6;
	TX_AGG_CNT7_STRUC	TxAggCnt7;
#endif // STATS_COUNT_SUPPORT //
	COUNTER_RALINK		*pRalinkCounters;


	pRalinkCounters = &pAd->RalinkCounters;

	RTMP_IO_READ32(pAd, RX_STA_CNT0, &RxStaCnt0.word);
	RTMP_IO_READ32(pAd, RX_STA_CNT2, &RxStaCnt2.word);

	{
		RTMP_IO_READ32(pAd, RX_STA_CNT1, &RxStaCnt1.word);
	    // Update RX PLCP error counter
	    pAd->PrivateInfo.PhyRxErrCnt += RxStaCnt1.field.PlcpErr;
		// Update False CCA counter
		pAd->RalinkCounters.OneSecFalseCCACnt += RxStaCnt1.field.FalseCca;
	}

#ifdef STATS_COUNT_SUPPORT
	// Update FCS counters
	OldValue= pAd->WlanCounters.FCSErrorCount.u.LowPart;
	pAd->WlanCounters.FCSErrorCount.u.LowPart += (RxStaCnt0.field.CrcErr); // >> 7);
	if (pAd->WlanCounters.FCSErrorCount.u.LowPart < OldValue)
		pAd->WlanCounters.FCSErrorCount.u.HighPart++;
#endif // STATS_COUNT_SUPPORT //

	// Add FCS error count to private counters
	pRalinkCounters->OneSecRxFcsErrCnt += RxStaCnt0.field.CrcErr;
	OldValue = pRalinkCounters->RealFcsErrCount.u.LowPart;
	pRalinkCounters->RealFcsErrCount.u.LowPart += RxStaCnt0.field.CrcErr;
	if (pRalinkCounters->RealFcsErrCount.u.LowPart < OldValue)
		pRalinkCounters->RealFcsErrCount.u.HighPart++;

	// Update Duplicate Rcv check
	pRalinkCounters->DuplicateRcv += RxStaCnt2.field.RxDupliCount;
#ifdef STATS_COUNT_SUPPORT
	pAd->WlanCounters.FrameDuplicateCount.u.LowPart += RxStaCnt2.field.RxDupliCount;
#endif // STATS_COUNT_SUPPORT //
	// Update RX Overflow counter
	pAd->Counters8023.RxNoBuffer += (RxStaCnt2.field.RxFifoOverflowCount);
	
	//pAd->RalinkCounters.RxCount = 0;
#ifdef RTMP_MAC_USB
	if (pRalinkCounters->RxCount != pAd->watchDogRxCnt)
	{
		pAd->watchDogRxCnt = pRalinkCounters->RxCount;
		pAd->watchDogRxOverFlowCnt = 0;
	}
	else
	{
		if (RxStaCnt2.field.RxFifoOverflowCount)
			pAd->watchDogRxOverFlowCnt++;
		else
			pAd->watchDogRxOverFlowCnt = 0;
	}
#endif // RTMP_MAC_USB //

	
	//if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED) || 
	//	(OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_TX_RATE_SWITCH_ENABLED) && (pAd->MacTab.Size != 1)))
	if (!pAd->bUpdateBcnCntDone)
	{
	// Update BEACON sent count
	RTMP_IO_READ32(pAd, TX_STA_CNT0, &TxStaCnt0.word);
	RTMP_IO_READ32(pAd, TX_STA_CNT1, &StaTx1.word);
	RTMP_IO_READ32(pAd, TX_STA_CNT2, &StaTx2.word);
	pRalinkCounters->OneSecBeaconSentCnt += TxStaCnt0.field.TxBeaconCount;
	pRalinkCounters->OneSecTxRetryOkCount += StaTx1.field.TxRetransmit;
	pRalinkCounters->OneSecTxNoRetryOkCount += StaTx1.field.TxSuccess;
	pRalinkCounters->OneSecTxFailCount += TxStaCnt0.field.TxFailCount;

#ifdef STATS_COUNT_SUPPORT
	pAd->WlanCounters.TransmittedFragmentCount.u.LowPart += StaTx1.field.TxSuccess;
	pAd->WlanCounters.RetryCount.u.LowPart += StaTx1.field.TxRetransmit;
	pAd->WlanCounters.FailedCount.u.LowPart += TxStaCnt0.field.TxFailCount;
#endif // STATS_COUNT_SUPPORT //
	}


	//if (pAd->bStaFifoTest == TRUE)
#ifdef STATS_COUNT_SUPPORT
	{
		RTMP_IO_READ32(pAd, TX_AGG_CNT, &TxAggCnt.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT0, &TxAggCnt0.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT1, &TxAggCnt1.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT2, &TxAggCnt2.word);
	RTMP_IO_READ32(pAd, TX_AGG_CNT3, &TxAggCnt3.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT4, &TxAggCnt4.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT5, &TxAggCnt5.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT6, &TxAggCnt6.word);
		RTMP_IO_READ32(pAd, TX_AGG_CNT7, &TxAggCnt7.word);
		pRalinkCounters->TxAggCount += TxAggCnt.field.AggTxCount;
		pRalinkCounters->TxNonAggCount += TxAggCnt.field.NonAggTxCount;
		pRalinkCounters->TxAgg1MPDUCount += TxAggCnt0.field.AggSize1Count;
		pRalinkCounters->TxAgg2MPDUCount += TxAggCnt0.field.AggSize2Count;
		
		pRalinkCounters->TxAgg3MPDUCount += TxAggCnt1.field.AggSize3Count;
		pRalinkCounters->TxAgg4MPDUCount += TxAggCnt1.field.AggSize4Count;
		pRalinkCounters->TxAgg5MPDUCount += TxAggCnt2.field.AggSize5Count;
		pRalinkCounters->TxAgg6MPDUCount += TxAggCnt2.field.AggSize6Count;
	
		pRalinkCounters->TxAgg7MPDUCount += TxAggCnt3.field.AggSize7Count;
		pRalinkCounters->TxAgg8MPDUCount += TxAggCnt3.field.AggSize8Count;
		pRalinkCounters->TxAgg9MPDUCount += TxAggCnt4.field.AggSize9Count;
		pRalinkCounters->TxAgg10MPDUCount += TxAggCnt4.field.AggSize10Count;

		pRalinkCounters->TxAgg11MPDUCount += TxAggCnt5.field.AggSize11Count;
		pRalinkCounters->TxAgg12MPDUCount += TxAggCnt5.field.AggSize12Count;
		pRalinkCounters->TxAgg13MPDUCount += TxAggCnt6.field.AggSize13Count;
		pRalinkCounters->TxAgg14MPDUCount += TxAggCnt6.field.AggSize14Count;

		pRalinkCounters->TxAgg15MPDUCount += TxAggCnt7.field.AggSize15Count;
		pRalinkCounters->TxAgg16MPDUCount += TxAggCnt7.field.AggSize16Count;

		// Calculate the transmitted A-MPDU count
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += TxAggCnt0.field.AggSize1Count;
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt0.field.AggSize2Count >> 1);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt1.field.AggSize3Count / 3);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt1.field.AggSize4Count >> 2);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt2.field.AggSize5Count / 5);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt2.field.AggSize6Count / 6);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt3.field.AggSize7Count / 7);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt3.field.AggSize8Count >> 3);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt4.field.AggSize9Count / 9);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt4.field.AggSize10Count / 10);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt5.field.AggSize11Count / 11);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt5.field.AggSize12Count / 12);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt6.field.AggSize13Count / 13);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt6.field.AggSize14Count / 14);

		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt7.field.AggSize15Count / 15);
		pRalinkCounters->TransmittedAMPDUCount.u.LowPart += (TxAggCnt7.field.AggSize16Count >> 4);	
	}
#endif // STATS_COUNT_SUPPORT //			

#ifdef DBG_DIAGNOSE
	{
		RtmpDiagStruct	*pDiag;
		UCHAR			ArrayCurIdx, i;
		
		pDiag = &pAd->DiagStruct;
		ArrayCurIdx = pDiag->ArrayCurIdx;
		
		if (pDiag->inited == 0)
		{
			NdisZeroMemory(pDiag, sizeof(struct _RtmpDiagStrcut_));
			pDiag->ArrayStartIdx = pDiag->ArrayCurIdx = 0;
			pDiag->inited = 1;
		}
		else
		{
			// Tx
			pDiag->TxFailCnt[ArrayCurIdx] = TxStaCnt0.field.TxFailCount;
			pDiag->TxAggCnt[ArrayCurIdx] = TxAggCnt.field.AggTxCount;
			pDiag->TxNonAggCnt[ArrayCurIdx] = TxAggCnt.field.NonAggTxCount;

			pDiag->TxAMPDUCnt[ArrayCurIdx][0] = TxAggCnt0.field.AggSize1Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][1] = TxAggCnt0.field.AggSize2Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][2] = TxAggCnt1.field.AggSize3Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][3] = TxAggCnt1.field.AggSize4Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][4] = TxAggCnt2.field.AggSize5Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][5] = TxAggCnt2.field.AggSize6Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][6] = TxAggCnt3.field.AggSize7Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][7] = TxAggCnt3.field.AggSize8Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][8] = TxAggCnt4.field.AggSize9Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][9] = TxAggCnt4.field.AggSize10Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][10] = TxAggCnt5.field.AggSize11Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][11] = TxAggCnt5.field.AggSize12Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][12] = TxAggCnt6.field.AggSize13Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][13] = TxAggCnt6.field.AggSize14Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][14] = TxAggCnt7.field.AggSize15Count;
			pDiag->TxAMPDUCnt[ArrayCurIdx][15] = TxAggCnt7.field.AggSize16Count;

			pDiag->RxCrcErrCnt[ArrayCurIdx] = RxStaCnt0.field.CrcErr;
			
			INC_RING_INDEX(pDiag->ArrayCurIdx,  DIAGNOSE_TIME);
			ArrayCurIdx = pDiag->ArrayCurIdx;
			for (i =0; i < 9; i++)
			{
				pDiag->TxDescCnt[ArrayCurIdx][i]= 0;
				pDiag->TxSWQueCnt[ArrayCurIdx][i] =0;
				pDiag->TxMcsCnt[ArrayCurIdx][i] = 0;
				pDiag->RxMcsCnt[ArrayCurIdx][i] = 0;
			}
			pDiag->TxDataCnt[ArrayCurIdx] = 0;
			pDiag->TxFailCnt[ArrayCurIdx] = 0;
			pDiag->RxDataCnt[ArrayCurIdx] = 0;
			pDiag->RxCrcErrCnt[ArrayCurIdx]  = 0;
//			for (i = 9; i < 16; i++)
			for (i = 9; i < 24; i++) // 3*3
			{
				pDiag->TxDescCnt[ArrayCurIdx][i] = 0;
				pDiag->TxMcsCnt[ArrayCurIdx][i] = 0;
				pDiag->RxMcsCnt[ArrayCurIdx][i] = 0;
}

			if (pDiag->ArrayCurIdx == pDiag->ArrayStartIdx)
				INC_RING_INDEX(pDiag->ArrayStartIdx,  DIAGNOSE_TIME);
		}
		
	}
#endif // DBG_DIAGNOSE //


}

NDIS_STATUS NICLoadFirmware(
	IN PRTMP_ADAPTER pAd)
{
	NDIS_STATUS	 status = NDIS_STATUS_SUCCESS;
	if (pAd->chipOps.loadFirmware)
		status = pAd->chipOps.loadFirmware(pAd);

	return status;
}


/*
	========================================================================
	
	Routine Description:
		erase 8051 firmware image in MAC ASIC

	Arguments:
		Adapter						Pointer to our adapter

	IRQL = PASSIVE_LEVEL
		
	========================================================================
*/
VOID NICEraseFirmware(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->chipOps.eraseFirmware)
		pAd->chipOps.eraseFirmware(pAd);
	
}/* End of NICEraseFirmware */


/*
	========================================================================
	
	Routine Description:
		Load Tx rate switching parameters

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		NDIS_STATUS_SUCCESS         firmware image load ok
		NDIS_STATUS_FAILURE         image not found

	IRQL = PASSIVE_LEVEL

	Rate Table Format:
		1. (B0: Valid Item number) (B1:Initial item from zero)
		2. Item Number(Dec)      Mode(Hex)     Current MCS(Dec)    TrainUp(Dec)    TrainDown(Dec)
		
	========================================================================
*/
NDIS_STATUS NICLoadRateSwitchingParams(
	IN PRTMP_ADAPTER pAd)
{
	return NDIS_STATUS_SUCCESS;
}


/*
	========================================================================
	
	Routine Description:
		Compare two memory block

	Arguments:
		pSrc1		Pointer to first memory address
		pSrc2		Pointer to second memory address
		
	Return Value:
		0:			memory is equal
		1:			pSrc1 memory is larger
		2:			pSrc2 memory is larger

	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
ULONG	RTMPCompareMemory(
	IN	PVOID	pSrc1,
	IN	PVOID	pSrc2,
	IN	ULONG	Length)
{
	PUCHAR	pMem1;
	PUCHAR	pMem2;
	ULONG	Index = 0;

	pMem1 = (PUCHAR) pSrc1;
	pMem2 = (PUCHAR) pSrc2;

	for (Index = 0; Index < Length; Index++)
	{
		if (pMem1[Index] > pMem2[Index])
			return (1);
		else if (pMem1[Index] < pMem2[Index])
			return (2);
	}

	// Equal
	return (0);
}


/*
	========================================================================
	
	Routine Description:
		Zero out memory block

	Arguments:
		pSrc1		Pointer to memory address
		Length		Size

	Return Value:
		None
		
	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
VOID	RTMPZeroMemory(
	IN	PVOID	pSrc,
	IN	ULONG	Length)
{
	PUCHAR	pMem;
	ULONG	Index = 0;

	pMem = (PUCHAR) pSrc;

	for (Index = 0; Index < Length; Index++)
	{
		pMem[Index] = 0x00;
	}
}


/*
	========================================================================
	
	Routine Description:
		Copy data from memory block 1 to memory block 2

	Arguments:
		pDest		Pointer to destination memory address
		pSrc		Pointer to source memory address
		Length		Copy size
		
	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
VOID RTMPMoveMemory(
	OUT	PVOID	pDest,
	IN	PVOID	pSrc,
	IN	ULONG	Length)
{
	PUCHAR	pMem1;
	PUCHAR	pMem2;
	UINT	Index;

	ASSERT((Length==0) || (pDest && pSrc));

	pMem1 = (PUCHAR) pDest;
	pMem2 = (PUCHAR) pSrc;

	for (Index = 0; Index < Length; Index++)
	{
		pMem1[Index] = pMem2[Index];
	}
}


/*
	========================================================================
	
	Routine Description:
		Initialize port configuration structure

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL

	Note:
		
	========================================================================
*/
VOID	UserCfgInit(
	IN	PRTMP_ADAPTER pAd)
{
	UINT i;
//	EDCA_PARM DefaultEdcaParm;
    UINT key_index, bss_index;

	DBGPRINT(RT_DEBUG_TRACE, ("--> UserCfgInit\n"));    
	 

	 
	//
	//  part I. intialize common configuration
	//
#ifdef RTMP_MAC_USB
	pAd->BulkOutReq = 0;
	
	pAd->BulkOutComplete = 0;
	pAd->BulkOutCompleteOther = 0;
	pAd->BulkOutCompleteCancel = 0;
	pAd->BulkInReq = 0;
	pAd->BulkInComplete = 0;
	pAd->BulkInCompleteFail = 0;

	//pAd->QuickTimerP = 100;
	//pAd->TurnAggrBulkInCount = 0;
	pAd->bUsbTxBulkAggre = 0;

	// init as unsed value to ensure driver will set to MCU once.
	pAd->LedIndicatorStrength = 0xFF;

	pAd->CommonCfg.MaxPktOneTxBulk = 2;
	pAd->CommonCfg.TxBulkFactor = 1;
	pAd->CommonCfg.RxBulkFactor =1;

	pAd->CommonCfg.TxPower = 100; //mW

	NdisZeroMemory(&pAd->CommonCfg.IOTestParm, sizeof(pAd->CommonCfg.IOTestParm));
#endif // RTMP_MAC_USB //

	for(key_index=0; key_index<SHARE_KEY_NUM; key_index++)
	{
		for(bss_index = 0; bss_index < MAX_MBSSID_NUM; bss_index++)
		{
			pAd->SharedKey[bss_index][key_index].KeyLen = 0;
			pAd->SharedKey[bss_index][key_index].CipherAlg = CIPHER_NONE;
		}
	}

	pAd->bLocalAdminMAC = FALSE;
	pAd->EepromAccess = FALSE;
	
	pAd->Antenna.word = 0; 
	pAd->CommonCfg.BBPCurrentBW = BW_20;

	pAd->LedCntl.word = 0;

	pAd->bAutoTxAgcA = FALSE;			// Default is OFF
	pAd->bAutoTxAgcG = FALSE;			// Default is OFF
#ifdef RTMP_INTERNAL_TX_ALC
	pAd->TxPowerCtrl.bInternalTxALC = FALSE; // Off by default
#endif // RTMP_INTERNAL_TX_ALC //
	pAd->RfIcType = RFIC_2820;

	// Init timer for reset complete event
	pAd->CommonCfg.CentralChannel = 1;
	pAd->bForcePrintTX = FALSE;
	pAd->bForcePrintRX = FALSE;
	pAd->bStaFifoTest = FALSE;
	pAd->bProtectionTest = FALSE;
	pAd->bHCCATest = FALSE;
	pAd->bGenOneHCCA = FALSE;
	pAd->CommonCfg.Dsifs = 10;      // in units of usec 
	pAd->CommonCfg.TxPower = 100; //mW
	pAd->CommonCfg.TxPowerPercentage = 0xffffffff; // AUTO
	pAd->CommonCfg.TxPowerDefault = 0xffffffff; // AUTO
	pAd->CommonCfg.TxPreamble = Rt802_11PreambleAuto; // use Long preamble on TX by defaut
	pAd->CommonCfg.bUseZeroToDisableFragment = FALSE;
	pAd->CommonCfg.RtsThreshold = 2347;
	pAd->CommonCfg.FragmentThreshold = 2346;
	pAd->CommonCfg.UseBGProtection = 0;    // 0: AUTO
	pAd->CommonCfg.bEnableTxBurst = TRUE; //0;    	
	pAd->CommonCfg.PhyMode = 0xff;     // unknown
	pAd->CommonCfg.BandState = UNKNOWN_BAND;
	pAd->CommonCfg.RadarDetect.CSPeriod = 10;
	pAd->CommonCfg.RadarDetect.CSCount = 0;
	pAd->CommonCfg.RadarDetect.RDMode = RD_NORMAL_MODE;
	


#ifdef TONE_RADAR_DETECT_SUPPORT
#ifdef CARRIER_DETECTION_SUPPORT
	pAd->CommonCfg.CarrierDetect.delta = CARRIER_DETECT_DELTA;
	pAd->CommonCfg.CarrierDetect.div_flag = CARRIER_DETECT_DIV_FLAG;
	pAd->CommonCfg.CarrierDetect.criteria = CARRIER_DETECT_CRITIRIA;
	pAd->CommonCfg.CarrierDetect.threshold = CARRIER_DETECT_THRESHOLD;
#endif // CARRIER_DETECTION_SUPPORT //
#endif // TONE_RADAR_DETECT_SUPPORT //

	pAd->CommonCfg.RadarDetect.ChMovingTime = 65;
#ifdef MERGE_ARCH_TEAM
	pAd->CommonCfg.RadarDetect.LongPulseRadarTh = 2;
	pAd->CommonCfg.RadarDetect.AvgRssiReq = -75;
#else // original rt28xx source code
	pAd->CommonCfg.RadarDetect.LongPulseRadarTh = 3;
#endif // MERGE_ARCH_TEAM //
	pAd->CommonCfg.bAPSDCapable = FALSE;
	pAd->CommonCfg.bNeedSendTriggerFrame = FALSE;
	pAd->CommonCfg.TriggerTimerCount = 0;
	pAd->CommonCfg.bAPSDForcePowerSave = FALSE;
	pAd->CommonCfg.bCountryFlag = FALSE;
	pAd->CommonCfg.TxStream = 0;
	pAd->CommonCfg.RxStream = 0;

	NdisZeroMemory(&pAd->BeaconTxWI, sizeof(pAd->BeaconTxWI));

#ifdef DOT11_N_SUPPORT
	NdisZeroMemory(&pAd->CommonCfg.HtCapability, sizeof(pAd->CommonCfg.HtCapability));
	pAd->HTCEnable = FALSE;
	pAd->bBroadComHT = FALSE;
	pAd->CommonCfg.bRdg = FALSE;
	
#ifdef DOT11N_DRAFT3
	pAd->CommonCfg.Dot11OBssScanPassiveDwell = dot11OBSSScanPassiveDwell;	// Unit : TU. 5~1000
	pAd->CommonCfg.Dot11OBssScanActiveDwell = dot11OBSSScanActiveDwell;	// Unit : TU. 10~1000
	pAd->CommonCfg.Dot11BssWidthTriggerScanInt = dot11BSSWidthTriggerScanInterval;	// Unit : Second	
	pAd->CommonCfg.Dot11OBssScanPassiveTotalPerChannel = dot11OBSSScanPassiveTotalPerChannel;	// Unit : TU. 200~10000
	pAd->CommonCfg.Dot11OBssScanActiveTotalPerChannel = dot11OBSSScanActiveTotalPerChannel;	// Unit : TU. 20~10000
	pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor = dot11BSSWidthChannelTransactionDelayFactor;
	pAd->CommonCfg.Dot11OBssScanActivityThre = dot11BSSScanActivityThreshold;	// Unit : percentage
	pAd->CommonCfg.Dot11BssWidthChanTranDelay = (pAd->CommonCfg.Dot11BssWidthTriggerScanInt * pAd->CommonCfg.Dot11BssWidthChanTranDelayFactor);

	pAd->CommonCfg.bBssCoexEnable = TRUE; // by default, we enable this feature, you can disable it via the profile or ioctl command
#endif  // DOT11N_DRAFT3 //

	NdisZeroMemory(&pAd->CommonCfg.AddHTInfo, sizeof(pAd->CommonCfg.AddHTInfo));
	pAd->CommonCfg.BACapability.field.MMPSmode = MMPS_ENABLE;
	pAd->CommonCfg.BACapability.field.MpduDensity = 0;
	pAd->CommonCfg.BACapability.field.Policy = IMMED_BA;
	pAd->CommonCfg.BACapability.field.RxBAWinLimit = 64; //32;
	pAd->CommonCfg.BACapability.field.TxBAWinLimit = 64; //32;
	DBGPRINT(RT_DEBUG_TRACE, ("--> UserCfgInit. BACapability = 0x%x\n", pAd->CommonCfg.BACapability.word));    

	pAd->CommonCfg.BACapability.field.AutoBA = FALSE;	
	BATableInit(pAd, &pAd->BATable);

	pAd->CommonCfg.bExtChannelSwitchAnnouncement = 1;
	pAd->CommonCfg.bHTProtect = 1;
	pAd->CommonCfg.bMIMOPSEnable = TRUE;
#ifdef GREENAP_SUPPORT
	pAd->ApCfg.bGreenAPEnable=FALSE;
	pAd->ApCfg.bBlockAntDivforGreenAP = FALSE;
	pAd->ApCfg.bGreenAPIsOn= FALSE;
#endif // GREENAP_SUPPORT //
	pAd->CommonCfg.bBADecline = FALSE;
	pAd->CommonCfg.bDisableReordering = FALSE;

	if (pAd->MACVersion == 0x28720200)
	{
		pAd->CommonCfg.TxBASize = 13; //by Jerry recommend
	}else{
		pAd->CommonCfg.TxBASize = 7;
	}

	pAd->CommonCfg.REGBACapability.word = pAd->CommonCfg.BACapability.word;
#endif // DOT11_N_SUPPORT //

	//pAd->CommonCfg.HTPhyMode.field.BW = BW_20;
	//pAd->CommonCfg.HTPhyMode.field.MCS = MCS_AUTO;
	//pAd->CommonCfg.HTPhyMode.field.ShortGI = GI_800;
	//pAd->CommonCfg.HTPhyMode.field.STBC = STBC_NONE;
	pAd->CommonCfg.TxRate = RATE_6;
	
	pAd->CommonCfg.MlmeTransmit.field.MCS = MCS_RATE_6;
	pAd->CommonCfg.MlmeTransmit.field.BW = BW_20;
	pAd->CommonCfg.MlmeTransmit.field.MODE = MODE_OFDM;

	pAd->CommonCfg.BeaconPeriod = 100;     // in mSec


#ifdef MCAST_RATE_SPECIFIC
	pAd->CommonCfg.MCastPhyMode.word
	= pAd->MacTab.Content[MCAST_WCID].HTPhyMode.word;
#endif // MCAST_RATE_SPECIFIC //

	//
	// part II. intialize STA specific configuration
	//
#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		RX_FILTER_SET_FLAG(pAd, fRX_FILTER_ACCEPT_DIRECT);
		RX_FILTER_CLEAR_FLAG(pAd, fRX_FILTER_ACCEPT_MULTICAST);
		RX_FILTER_SET_FLAG(pAd, fRX_FILTER_ACCEPT_BROADCAST);
		RX_FILTER_SET_FLAG(pAd, fRX_FILTER_ACCEPT_ALL_MULTICAST);

		pAd->StaCfg.Psm = PWR_ACTIVE;

		pAd->StaCfg.OrigWepStatus = Ndis802_11EncryptionDisabled;
		pAd->StaCfg.PairCipher = Ndis802_11EncryptionDisabled;
		pAd->StaCfg.GroupCipher = Ndis802_11EncryptionDisabled;
		pAd->StaCfg.bMixCipher = FALSE;	
		pAd->StaCfg.DefaultKeyId = 0;

		// 802.1x port control
		pAd->StaCfg.PrivacyFilter = Ndis802_11PrivFilter8021xWEP;
		pAd->StaCfg.PortSecured = WPA_802_1X_PORT_NOT_SECURED;
		pAd->StaCfg.LastMicErrorTime = 0;
		pAd->StaCfg.MicErrCnt        = 0;
		pAd->StaCfg.bBlockAssoc      = FALSE;
		pAd->StaCfg.WpaState         = SS_NOTUSE;

		pAd->CommonCfg.NdisRadioStateOff = FALSE;		// New to support microsoft disable radio with OID command

		pAd->StaCfg.RssiTrigger = 0;
		NdisZeroMemory(&pAd->StaCfg.RssiSample, sizeof(RSSI_SAMPLE));
		pAd->StaCfg.RssiTriggerMode = RSSI_TRIGGERED_UPON_BELOW_THRESHOLD;
		pAd->StaCfg.AtimWin = 0;
		pAd->StaCfg.DefaultListenCount = 3;//default listen count;
		pAd->StaCfg.BssType = BSS_INFRA;  // BSS_INFRA or BSS_ADHOC or BSS_MONITOR
		pAd->StaCfg.bScanReqIsFromWebUI = FALSE;
		OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_DOZE);
		OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_WAKEUP_NOW);

		pAd->StaCfg.bAutoTxRateSwitch = TRUE;
		pAd->StaCfg.DesiredTransmitSetting.field.MCS = MCS_AUTO;
		pAd->StaCfg.bAutoConnectIfNoSSID = FALSE;
	}

#ifdef EXT_BUILD_CHANNEL_LIST
	pAd->StaCfg.IEEE80211dClientMode = Rt802_11_D_None;
#endif // EXT_BUILD_CHANNEL_LIST //

#endif // CONFIG_STA_SUPPORT //

	// global variables mXXXX used in MAC protocol state machines
	OPSTATUS_SET_FLAG(pAd, fOP_STATUS_RECEIVE_DTIM);
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_ADHOC_ON);
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_INFRA_ON);

	// PHY specification
	pAd->CommonCfg.PhyMode = PHY_11BG_MIXED;		// default PHY mode
	OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_SHORT_PREAMBLE_INUSED);  // CCK use LONG preamble

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	{
		// user desired power mode
		pAd->StaCfg.WindowsPowerMode = Ndis802_11PowerModeCAM;
		pAd->StaCfg.WindowsBatteryPowerMode = Ndis802_11PowerModeCAM;
		pAd->StaCfg.bWindowsACCAMEnable = FALSE;

		RTMPInitTimer(pAd, &pAd->StaCfg.StaQuickResponeForRateUpTimer, GET_TIMER_FUNCTION(StaQuickResponeForRateUpExec), pAd, FALSE);
		pAd->StaCfg.StaQuickResponeForRateUpTimerRunning = FALSE;

		pAd->StaCfg.bHwRadio  = TRUE; // Default Hardware Radio status is On
		pAd->StaCfg.bSwRadio  = TRUE; // Default Software Radio status is On
		pAd->StaCfg.bRadio    = TRUE; // bHwRadio && bSwRadio
		pAd->StaCfg.bHardwareRadio = FALSE;		// Default is OFF
		pAd->StaCfg.bShowHiddenSSID = FALSE;		// Default no show

		// Nitro mode control
		pAd->StaCfg.bAutoReconnect = TRUE;

		// Save the init time as last scan time, the system should do scan after 2 seconds.
		// This patch is for driver wake up from standby mode, system will do scan right away.
		NdisGetSystemUpTime(&pAd->StaCfg.LastScanTime);
		if (pAd->StaCfg.LastScanTime > 10 * OS_HZ)
			pAd->StaCfg.LastScanTime -= (10 * OS_HZ);
		
		NdisZeroMemory(pAd->nickname, IW_ESSID_MAX_SIZE+1);
		RTMPInitTimer(pAd, &pAd->StaCfg.WpaDisassocAndBlockAssocTimer, GET_TIMER_FUNCTION(WpaDisassocApAndBlockAssoc), pAd, FALSE);
#ifdef WPA_SUPPLICANT_SUPPORT
		pAd->StaCfg.IEEE8021X = FALSE;
		pAd->StaCfg.IEEE8021x_required_keys = FALSE;
		pAd->StaCfg.WpaSupplicantUP = WPA_SUPPLICANT_DISABLE;
		pAd->StaCfg.bRSN_IE_FromWpaSupplicant = FALSE;
#ifdef NATIVE_WPA_SUPPLICANT_SUPPORT
		pAd->StaCfg.WpaSupplicantUP = WPA_SUPPLICANT_ENABLE;
#endif // NATIVE_WPA_SUPPLICANT_SUPPORT //
		pAd->StaCfg.bLostAp = FALSE;
		pAd->StaCfg.pWpsProbeReqIe = NULL;
		pAd->StaCfg.WpsProbeReqIeLen = 0;
		pAd->StaCfg.pWpaAssocIe = NULL;
		pAd->StaCfg.WpaAssocIeLen = 0;
#endif // WPA_SUPPLICANT_SUPPORT //

		NdisZeroMemory(pAd->StaCfg.ReplayCounter, 8);


		pAd->StaCfg.bAutoConnectByBssid = FALSE;
		pAd->StaCfg.BeaconLostTime = BEACON_LOST_TIME;
		NdisZeroMemory(pAd->StaCfg.WpaPassPhrase, 64);    
		pAd->StaCfg.WpaPassPhraseLen = 0;
		pAd->StaCfg.bAutoRoaming = FALSE;
		pAd->StaCfg.bForceTxBurst = FALSE;
		pAd->StaCfg.bNotFirstScan = FALSE;
		pAd->StaCfg.bImprovedScan = FALSE;
	}
#endif // CONFIG_STA_SUPPORT //

	// Default for extra information is not valid
	pAd->ExtraInfo = EXTRA_INFO_CLEAR;
	
	// Default Config change flag
	pAd->bConfigChanged = FALSE;

	// 
	// part III. AP configurations
	//


	//
	// part IV. others
	//
	// dynamic BBP R66:sensibity tuning to overcome background noise
	pAd->BbpTuning.bEnable                = TRUE;  
	pAd->BbpTuning.FalseCcaLowerThreshold = 100;
	pAd->BbpTuning.FalseCcaUpperThreshold = 512;
	pAd->BbpTuning.R66Delta               = 4;
	pAd->Mlme.bEnableAutoAntennaCheck = TRUE;
	
	//
	// Also initial R66CurrentValue, RTUSBResumeMsduTransmission might use this value.
	// if not initial this value, the default value will be 0.
	//
	pAd->BbpTuning.R66CurrentValue = 0x38;

	pAd->Bbp94 = BBPR94_DEFAULT;
	pAd->BbpForCCK = FALSE;
	
	// Default is FALSE for test bit 1
	//pAd->bTest1 = FALSE;
	
	// initialize MAC table and allocate spin lock
	NdisZeroMemory(&pAd->MacTab, sizeof(MAC_TABLE));
	InitializeQueueHeader(&pAd->MacTab.McastPsQueue);
	NdisAllocateSpinLock(&pAd->MacTabLock);

	//RTMPInitTimer(pAd, &pAd->RECBATimer, RECBATimerTimeout, pAd, TRUE);
	//RTMPSetTimer(&pAd->RECBATimer, REORDER_EXEC_INTV);

#ifdef RALINK_ATE
	NdisZeroMemory(&pAd->ate, sizeof(ATE_INFO));
	pAd->ate.Mode = ATE_STOP;
	pAd->ate.TxCount = 200;/* to exceed TX_RING_SIZE ... */
	pAd->ate.TxDoneCount = 0;
	pAd->ate.RFFreqOffset = 0;
	pAd->ate.TxLength = 1024;
	pAd->ate.TxWI.ShortGI = 0;// LONG GI : 800 ns
	pAd->ate.TxWI.PHYMODE = MODE_CCK;
	pAd->ate.TxWI.MCS = 3;
	pAd->ate.TxWI.BW = BW_20;
	/* please do not change this default channel value */
	pAd->ate.Channel = 1;
	/* This is supposed just for rt3062 now. */
	pAd->ate.QID = QID_AC_BE;

#if defined (RT2883) || defined (RT3883)
	/* For stream mode in 3T/3R ++ */
	/* use broadcast address as default value */
	pAd->ate.Addr1[0] = 0xFF;
	pAd->ate.Addr1[1] = 0xFF;
	pAd->ate.Addr1[2] = 0xFF;
	pAd->ate.Addr1[3] = 0xFF;
	pAd->ate.Addr1[4] = 0xFF;
	pAd->ate.Addr1[5] = 0xFF;
	/* For stream mode in 3T/3R -- */

	pAd->ate.Addr2[0] = 0x00;
	pAd->ate.Addr2[1] = 0x11;
	pAd->ate.Addr2[2] = 0x22;
	pAd->ate.Addr2[3] = 0xAA;
	pAd->ate.Addr2[4] = 0xBB;
	pAd->ate.Addr2[5] = 0xCC;

	NdisMoveMemory(pAd->ate.Addr3, pAd->ate.Addr2, ETH_LENGTH_OF_ADDRESS);
#else
	pAd->ate.Addr1[0] = 0x00;
	pAd->ate.Addr1[1] = 0x11;
	pAd->ate.Addr1[2] = 0x22;
	pAd->ate.Addr1[3] = 0xAA;
	pAd->ate.Addr1[4] = 0xBB;
	pAd->ate.Addr1[5] = 0xCC;

	NdisMoveMemory(pAd->ate.Addr2, pAd->ate.Addr1, ETH_LENGTH_OF_ADDRESS);
	NdisMoveMemory(pAd->ate.Addr3, pAd->ate.Addr1, ETH_LENGTH_OF_ADDRESS);
#endif // defined (RT2883) || defined (RT3883) //


	pAd->ate.bRxFER = 0;
	pAd->ate.bQATxStart = FALSE;
	pAd->ate.bQARxStart = FALSE;


#ifdef RTMP_MAC_USB
#endif // RTMP_MAC_USB //

#ifdef RALINK_28xx_QA
	pAd->ate.TxStatus = 0;
	pAd->ate.AtePid = THREAD_PID_INIT_VALUE;
#endif // RALINK_28xx_QA //
#endif // RALINK_ATE //


	pAd->CommonCfg.bWiFiTest = FALSE;


#ifdef CONFIG_STA_SUPPORT
#ifdef PCIE_PS_SUPPORT
	RTMP_SET_PSFLAG(pAd, fRTMP_PS_CAN_GO_SLEEP);
#endif // PCIE_PS_SUPPORT //
#endif // CONFIG_STA_SUPPORT //

#ifdef CONFIG_STA_SUPPORT
RTMP_SET_PSFLAG(pAd, fRTMP_PS_CAN_GO_SLEEP);
#endif // CONFIG_STA_SUPPORT //

	for (i = 0; i < MAX_LEN_OF_BSS_TABLE; i++) 
	{
		PBSS_ENTRY	pBssEntry = &pAd->ScanTab.BssEntry[i];
		
		if (pAd->ProbeRespIE[i].pIe)
			pBssEntry->pVarIeFromProbRsp = pAd->ProbeRespIE[i].pIe;
		else
			pBssEntry->pVarIeFromProbRsp = NULL;
	}

#ifdef RT3593
	//
	// NIC uses the maximum Tx capability (3 Tx streams) to transmit any broadcast packets.
	//
	if (IS_RT3593(pAd))
	{
		TX_CHAIN_ADDR0_L_STRUC TxChainAddr0L = { { 0 } };
		TX_CHAIN_ADDR0_H_STRUC TxChainAddr0H = { { 0 } };

		TxChainAddr0L.field.TxChainAddr0L = 0xFFFFFFFF;
		RTMP_IO_WRITE32(pAd, TX_CHAIN_ADDR0_L, TxChainAddr0L.word);
		
		RTMP_IO_READ32(pAd, TX_CHAIN_ADDR0_H, &TxChainAddr0H.word);
		TxChainAddr0H.field.TxChainAddr0H = 0xFFFF;
		TxChainAddr0H.field.TxChainSel0 = 0xF;
		RTMP_IO_WRITE32(pAd, TX_CHAIN_ADDR0_H, TxChainAddr0H.word);
	}
#endif // RT3593 //


	DBGPRINT(RT_DEBUG_TRACE, ("<-- UserCfgInit\n"));
}

// IRQL = PASSIVE_LEVEL
UCHAR BtoH(STRING ch)
{
	if (ch >= '0' && ch <= '9') return (ch - '0');        // Handle numerals
	if (ch >= 'A' && ch <= 'F') return (ch - 'A' + 0xA);  // Handle capitol hex digits
	if (ch >= 'a' && ch <= 'f') return (ch - 'a' + 0xA);  // Handle small hex digits
	return(255);
}

//
//  FUNCTION: AtoH(char *, UCHAR *, int)
//
//  PURPOSE:  Converts ascii string to network order hex
//
//  PARAMETERS:
//    src    - pointer to input ascii string
//    dest   - pointer to output hex
//    destlen - size of dest
//
//  COMMENTS:
//
//    2 ascii bytes make a hex byte so must put 1st ascii byte of pair
//    into upper nibble and 2nd ascii byte of pair into lower nibble.
//
// IRQL = PASSIVE_LEVEL

void AtoH(PSTRING src, PUCHAR dest, int destlen)
{
	PSTRING srcptr;
	PUCHAR destTemp;

	srcptr = src;	
	destTemp = (PUCHAR) dest; 

	while(destlen--)
	{
		*destTemp = BtoH(*srcptr++) << 4;    // Put 1st ascii byte in upper nibble.
		*destTemp += BtoH(*srcptr++);      // Add 2nd ascii byte to above.
		destTemp++;
	}
}


//+++Mark by shiang, not use now, need to remove after confirm
//---Mark by shiang, not use now, need to remove after confirm


/*
	========================================================================
	
	Routine Description:
		Init timer objects

	Arguments:
		pAd			Pointer to our adapter
		pTimer				Timer structure
		pTimerFunc			Function to execute when timer expired
		Repeat				Ture for period timer

	Return Value:
		None

	Note:
		
	========================================================================
*/
VOID	RTMPInitTimer(
	IN	PRTMP_ADAPTER			pAd,
	IN	PRALINK_TIMER_STRUCT	pTimer,
	IN	PVOID					pTimerFunc,
	IN	PVOID					pData,
	IN	BOOLEAN					Repeat)
{
	//
	// Set Valid to TRUE for later used.
	// It will crash if we cancel a timer or set a timer 
	// that we haven't initialize before.
	// 
	pTimer->Valid      = TRUE;
	
	pTimer->PeriodicType = Repeat;
	pTimer->State      = FALSE;
	pTimer->cookie = (ULONG) pData;
	pTimer->pAd = pAd;

	RTMP_OS_Init_Timer(pAd, &pTimer->TimerObj,	pTimerFunc, (PVOID) pTimer);	
}


/*
	========================================================================
	
	Routine Description:
		Init timer objects

	Arguments:
		pTimer				Timer structure
		Value				Timer value in milliseconds

	Return Value:
		None

	Note:
		To use this routine, must call RTMPInitTimer before.
		
	========================================================================
*/
VOID	RTMPSetTimer(
	IN	PRALINK_TIMER_STRUCT	pTimer,
	IN	ULONG					Value)
{
	if (pTimer->Valid)
	{
		RTMP_ADAPTER *pAd;
		
		pAd = (RTMP_ADAPTER *)pTimer->pAd;
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
		{
			DBGPRINT_ERR(("RTMPSetTimer failed, Halt in Progress!\n"));
			return;
		}
		
		pTimer->TimerValue = Value;
		pTimer->State      = FALSE;
		if (pTimer->PeriodicType == TRUE)
		{
			pTimer->Repeat = TRUE;
			RTMP_SetPeriodicTimer(&pTimer->TimerObj, Value);
		}
		else
		{
			pTimer->Repeat = FALSE;
			RTMP_OS_Add_Timer(&pTimer->TimerObj, Value);
		}
	}
	else
	{
		DBGPRINT_ERR(("RTMPSetTimer failed, Timer hasn't been initialize!\n"));
	}
}


/*
	========================================================================
	
	Routine Description:
		Init timer objects

	Arguments:
		pTimer				Timer structure
		Value				Timer value in milliseconds

	Return Value:
		None

	Note:
		To use this routine, must call RTMPInitTimer before.
		
	========================================================================
*/
VOID	RTMPModTimer(
	IN	PRALINK_TIMER_STRUCT	pTimer,
	IN	ULONG					Value)
{
	BOOLEAN	Cancel;

	if (pTimer->Valid)
	{
		pTimer->TimerValue = Value;
		pTimer->State      = FALSE;
		if (pTimer->PeriodicType == TRUE)
		{
			RTMPCancelTimer(pTimer, &Cancel);
			RTMPSetTimer(pTimer, Value);
		}
		else
		{
			RTMP_OS_Mod_Timer(&pTimer->TimerObj, Value);
		}
	}
	else
	{
		DBGPRINT_ERR(("RTMPModTimer failed, Timer hasn't been initialize!\n"));
	}
}


/*
	========================================================================
	
	Routine Description:
		Cancel timer objects

	Arguments:
		Adapter						Pointer to our adapter

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		1.) To use this routine, must call RTMPInitTimer before.
		2.) Reset NIC to initial state AS IS system boot up time.
		
	========================================================================
*/
VOID	RTMPCancelTimer(
	IN	PRALINK_TIMER_STRUCT	pTimer,
	OUT	BOOLEAN					*pCancelled)
{
	if (pTimer->Valid)
	{
		if (pTimer->State == FALSE)
			pTimer->Repeat = FALSE;
		
		RTMP_OS_Del_Timer(&pTimer->TimerObj, pCancelled);
		
		if (*pCancelled == TRUE)
			pTimer->State = TRUE;

#ifdef RTMP_TIMER_TASK_SUPPORT
		// We need to go-through the TimerQ to findout this timer handler and remove it if 
		//		it's still waiting for execution.
		RtmpTimerQRemove(pTimer->pAd, pTimer);
#endif // RTMP_TIMER_TASK_SUPPORT //
	}
	else
	{
		DBGPRINT_ERR(("RTMPCancelTimer failed, Timer hasn't been initialize!\n"));
	}
}

#ifdef LED_CONTROL_SUPPORT
/*
	========================================================================
	
	Routine Description:
		Set LED Status

	Arguments:
		pAd						Pointer to our adapter
		Status					LED Status

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	IRQL = DISPATCH_LEVEL
	
	Note:
		
	========================================================================
*/
VOID RTMPSetLED(
	IN PRTMP_ADAPTER 	pAd, 
	IN UCHAR			Status)
{
	//ULONG			data;
	UCHAR			HighByte = 0;
	UCHAR			LowByte;
	BOOLEAN 		bIgnored = FALSE;
	BOOLEAN			FlgCmdSend = FALSE;

#ifdef RALINK_ATE
	/*
		In ATE mode of RT2860 AP/STA, we have erased 8051 firmware.
		So LED mode is not supported when ATE is running.
	*/
	if (!IS_RT3572(pAd))
	{
		if (ATE_ON(pAd))
			return;
	}
#endif // RALINK_ATE //

	LowByte = pAd->LedCntl.field.LedMode&0x7f;
	switch (Status)
	{
		case LED_LINK_DOWN:
			HighByte = 0x20;
			pAd->LedIndicatorStrength = 0; 
			FlgCmdSend = TRUE;
			break;
		case LED_LINK_UP:
			if (pAd->CommonCfg.Channel > 14)
				HighByte = 0xa0;
			else
				HighByte = 0x60;
			FlgCmdSend = TRUE;
			break;
		case LED_RADIO_ON:
			HighByte = 0x20;
			FlgCmdSend = TRUE;
			break;
		case LED_HALT: 
			LowByte = 0; // Driver sets MAC register and MAC controls LED
		case LED_RADIO_OFF:
			HighByte = 0;
			FlgCmdSend = TRUE;
			break;
		case LED_WPS:
			HighByte = 0x10;
			FlgCmdSend = TRUE;
			break;
		case LED_ON_SITE_SURVEY:
			HighByte = 0x08;
			FlgCmdSend = TRUE;
			break;
		case LED_POWER_UP:
			HighByte = 0x04;
			FlgCmdSend = TRUE;
			break;
#ifdef RALINK_ATE
#endif // RALINK_ATE //
		default:
			DBGPRINT(RT_DEBUG_WARN, ("RTMPSetLED::Unknown Status %d\n", Status));
			break;
	}

	if (FlgCmdSend == TRUE)
	{
		/* command here so we can reduce code size */
		AsicSendCommandToMcu(pAd, 0x50, 0xff, LowByte, HighByte);
	}

    //
	// Keep LED status for LED SiteSurvey mode.
	// After SiteSurvey, we will set the LED mode to previous status.
	//
	if ((Status != LED_ON_SITE_SURVEY) && (Status != LED_POWER_UP) && (bIgnored == FALSE))
		pAd->LedStatus = Status;
    
	DBGPRINT(RT_DEBUG_TRACE, ("RTMPSetLED::Mode=%d,HighByte=0x%02x,LowByte=0x%02x\n", pAd->LedCntl.field.LedMode, HighByte, LowByte));
}

/*
	========================================================================
	
	Routine Description:
		Set LED Signal Stregth 

	Arguments:
		pAd						Pointer to our adapter
		Dbm						Signal Stregth

	Return Value:
		None

	IRQL = PASSIVE_LEVEL
	
	Note:
		Can be run on any IRQL level. 

		According to Microsoft Zero Config Wireless Signal Stregth definition as belows.
		<= -90  No Signal
		<= -81  Very Low
		<= -71  Low
		<= -67  Good
		<= -57  Very Good
		 > -57  Excellent		
	========================================================================
*/
VOID RTMPSetSignalLED(
	IN PRTMP_ADAPTER 	pAd, 
	IN NDIS_802_11_RSSI Dbm)
{
	UCHAR		nLed = 0;

	if (pAd->LedCntl.field.LedMode == LED_MODE_SIGNAL_STREGTH)
	{
	if (Dbm <= -90)
		nLed = 0;
	else if (Dbm <= -81)
		nLed = 1;
	else if (Dbm <= -71)
		nLed = 3;
	else if (Dbm <= -67)
		nLed = 7;
	else if (Dbm <= -57)
		nLed = 15;
	else 
		nLed = 31;

	//
	// Update Signal Stregth to firmware if changed.
	//
	if (pAd->LedIndicatorStrength != nLed)
	{
		AsicSendCommandToMcu(pAd, 0x51, 0xff, nLed, pAd->LedCntl.field.Polarity);
		pAd->LedIndicatorStrength = nLed;
	}
}
}

#endif // LED_CONTROL_SUPPORT //

/*
	========================================================================
	
	Routine Description:
		Enable RX 

	Arguments:
		pAd						Pointer to our adapter

	Return Value:
		None

	IRQL <= DISPATCH_LEVEL
	
	Note:
		Before Enable RX, make sure you have enabled Interrupt.
	========================================================================
*/
VOID RTMPEnableRxTx(
	IN PRTMP_ADAPTER	pAd)
{
//	WPDMA_GLO_CFG_STRUC	GloCfg;
//	ULONG	i = 0;
	UINT32 rx_filter_flag;

	DBGPRINT(RT_DEBUG_TRACE, ("==> RTMPEnableRxTx\n"));

	// Enable Rx DMA.
	RT28XXDMAEnable(pAd);

	// enable RX of MAC block
	if (pAd->OpMode == OPMODE_AP)
	{
		rx_filter_flag = APNORMAL;


		RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, rx_filter_flag);     // enable RX of DMA block
	}
#ifdef CONFIG_STA_SUPPORT
	else
	{
#ifdef XLINK_SUPPORT
		if (pAd->StaCfg.PSPXlink)
			rx_filter_flag = PSPXLINK;
		else
#endif // XLINK_SUPPORT //	
			rx_filter_flag = STANORMAL;     // Staion not drop control frame will fail WiFi Certification.
		RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, rx_filter_flag);
	}
#endif // CONFIG_STA_SUPPORT //
	
	{
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0xc);
	}
	DBGPRINT(RT_DEBUG_TRACE, ("<== RTMPEnableRxTx\n"));	
}


//+++Add by shiang, move from os/linux/rt_main_dev.c
void CfgInitHook(PRTMP_ADAPTER pAd)
{
	//pAd->bBroadComHT = TRUE;
}

//---Add by shiang, move from os/linux/rt_main_dev.c


static INT RtmpChipOpsRegister(
	IN RTMP_ADAPTER *pAd,
	IN INT			infType)
{
	RTMP_CHIP_OP	*pChipOps = &pAd->chipOps;
	int status;
	
	memset(pChipOps, 0, sizeof(RTMP_CHIP_OP));

	/* set eeprom related hook functions */
	status = RtmpChipOpsEepromHook(pAd, infType);

	/* set mcu related hook functions */
	switch(infType)
	{

#ifdef RTMP_USB_SUPPORT
		case RTMP_DEV_INF_USB:
			pChipOps->loadFirmware = RtmpAsicLoadFirmware;
			pChipOps->sendCommandToMcu = RtmpAsicSendCommandToMcu;
			break;
#endif // RTMP_USB_SUPPORT //

		default:
			break;
	}

	return status;
}


INT RtmpRaDevCtrlInit(
	IN RTMP_ADAPTER *pAd, 
	IN RTMP_INF_TYPE infType)
{
	//VOID	*handle;

	// Assign the interface type. We need use it when do register/EEPROM access.
	pAd->infType = infType;

	
#ifdef CONFIG_STA_SUPPORT
	pAd->OpMode = OPMODE_STA;
	DBGPRINT(RT_DEBUG_TRACE, ("STA Driver version-%s\n", STA_DRIVER_VERSION));
#endif // CONFIG_STA_SUPPORT //


#ifdef RTMP_MAC_USB
	RTMP_SEM_EVENT_INIT(&(pAd->UsbVendorReq_semaphore));
	os_alloc_mem(pAd, (PUCHAR *)&pAd->UsbVendorReqBuf, MAX_PARAM_BUFFER_SIZE - 1);
	if (pAd->UsbVendorReqBuf == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("Allocate vendor request temp buffer failed!\n"));
		return FALSE;
	}
#endif // RTMP_MAC_USB //

	RtmpChipOpsRegister(pAd, infType);

#ifdef MULTIPLE_CARD_SUPPORT
{
	extern BOOLEAN RTMP_CardInfoRead(PRTMP_ADAPTER pAd);

	// find its profile path
	pAd->MC_RowID = -1; // use default profile path
	RTMP_CardInfoRead(pAd);

	if (pAd->MC_RowID == -1)
#ifdef CONFIG_STA_SUPPORT
		strcpy(pAd->MC_FileName, STA_PROFILE_PATH);
#endif // CONFIG_STA_SUPPORT //

	DBGPRINT(RT_DEBUG_TRACE, ("MC> ROW = %d, PATH = %s\n", pAd->MC_RowID, pAd->MC_FileName));
}
#endif // MULTIPLE_CARD_SUPPORT //

	return 0;
}


BOOLEAN RtmpRaDevCtrlExit(IN RTMP_ADAPTER *pAd)
{
	INT index;
	
#ifdef MULTIPLE_CARD_SUPPORT
extern UINT8  MC_CardUsed[MAX_NUM_OF_MULTIPLE_CARD];

	if ((pAd->MC_RowID >= 0) && (pAd->MC_RowID <= MAX_NUM_OF_MULTIPLE_CARD))
		MC_CardUsed[pAd->MC_RowID] = 0; // not clear MAC address
#endif // MULTIPLE_CARD_SUPPORT //

#ifdef RTMP_MAC_USB
	if (pAd->UsbVendorReqBuf)
		os_free_mem(pAd, pAd->UsbVendorReqBuf);
#endif // RTMP_MAC_USB //

	/*
		Free ProbeRespIE Table
	*/
	for (index = 0; index < MAX_LEN_OF_BSS_TABLE; index++) 
	{
		if (pAd->ProbeRespIE[index].pIe)
			os_free_mem(pAd, pAd->ProbeRespIE[index].pIe);
	}

	RTMPFreeAdapter(pAd);

	return TRUE;
}






#ifdef VENDOR_FEATURE3_SUPPORT
VOID RTMP_IO_WRITE32(
	PRTMP_ADAPTER pAd,
	UINT32 Offset,
	UINT32 Value)
{
	_RTMP_IO_WRITE32(pAd, Offset, Value);
}

VOID RTMP_BBP_IO_READ8_BY_REG_ID(
	PRTMP_ADAPTER pAd,
	UINT32 Offset,
	UINT8 *pValue)
{
	_RTMP_BBP_IO_READ8_BY_REG_ID(pAd, Offset, pValue);
}

VOID RTMP_BBP_IO_READ8(
	PRTMP_ADAPTER pAd,
	UCHAR Offset,
	UINT8 *pValue,
	BOOLEAN FlgValidMCR)
{
	_RTMP_BBP_IO_READ8(pAd, Offset, pValue, FlgValidMCR);
}

VOID RTMP_BBP_IO_WRITE8_BY_REG_ID(
	PRTMP_ADAPTER pAd,
	UINT32 Offset,
	UINT8 Value)
{
	_RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, Offset, Value);
}

VOID RTMP_BBP_IO_WRITE8(
	PRTMP_ADAPTER pAd,
	UCHAR Offset,
	UINT8 Value,
	BOOLEAN FlgValidMCR)
{
	_RTMP_BBP_IO_WRITE8(pAd, Offset, Value, FlgValidMCR);
}
#endif // VENDOR_FEATURE3_SUPPORT //





/* export wlan function symbol list */
#ifdef OS_ABL_SUPPORT

#ifdef RTMP_MAC_USB
MODULE_LICENSE("GPL");
#endif // RTMP_MAC_USB //

EXPORT_SYMBOL(NICInitializeAdapter);
EXPORT_SYMBOL(NICInitAsicFromEEPROM);
EXPORT_SYMBOL(NICReadEEPROMParameters);
#ifdef RTMP_RF_RW_SUPPORT
EXPORT_SYMBOL(NICInitRFRegisters);
#endif // RTMP_RF_RW_SUPPORT //
EXPORT_SYMBOL(UserCfgInit);
EXPORT_SYMBOL(CfgInitHook);
EXPORT_SYMBOL(RTMPEnableRxTx);
EXPORT_SYMBOL(RTMPSetPhyMode);
EXPORT_SYMBOL(RTMPMaxRssi);
EXPORT_SYMBOL(RTMPAllocAdapterBlock);
EXPORT_SYMBOL(RTMPAllocTxRxRingMemory);
EXPORT_SYMBOL(RTMPFreeTxRxRingMemory);
EXPORT_SYMBOL(NICLoadRateSwitchingParams);
EXPORT_SYMBOL(RtmpRaDevCtrlInit);
EXPORT_SYMBOL(RtmpRaDevCtrlExit);
EXPORT_SYMBOL(NICLoadFirmware);
EXPORT_SYMBOL(RTMPSetProfileParameters);
EXPORT_SYMBOL(RT28XXDMADisable);

#ifdef DOT11_N_SUPPORT
EXPORT_SYMBOL(SetCommonHT);
EXPORT_SYMBOL(N_ChannelCheck);
#endif // DOT11_N_SUPPORT //

EXPORT_SYMBOL(AsicSwitchChannel);
EXPORT_SYMBOL(AsicLockChannel);

EXPORT_SYMBOL(ba_reordering_resource_init);
EXPORT_SYMBOL(ba_reordering_resource_release);

EXPORT_SYMBOL(RTMPDeQueuePacket);

EXPORT_SYMBOL(CMDHandler);
EXPORT_SYMBOL(RTEnqueueInternalCmd);
EXPORT_SYMBOL(RTThreadDequeueCmd);

EXPORT_SYMBOL(TpcReqTabInit);
EXPORT_SYMBOL(TpcReqTabExit);
EXPORT_SYMBOL(MeasureReqTabInit);
EXPORT_SYMBOL(MeasureReqTabExit);

EXPORT_SYMBOL(MlmeInit);
EXPORT_SYMBOL(MlmeHandler);
EXPORT_SYMBOL(MlmeHalt);
EXPORT_SYMBOL(MlmeEnqueue);

EXPORT_SYMBOL(RTMPCancelTimer);
EXPORT_SYMBOL(AsicSendCommandToMcu);
EXPORT_SYMBOL(MacTableLookup);
EXPORT_SYMBOL(RTMPZeroMemory);
EXPORT_SYMBOL(RTMPSoftEncryptWEP);
EXPORT_SYMBOL(getRate);

/* command */
EXPORT_SYMBOL(RTMPIoctlGetSiteSurvey);




#ifdef CONFIG_STA_SUPPORT
EXPORT_SYMBOL(STASendPackets);

EXPORT_SYMBOL(LinkUp);
EXPORT_SYMBOL(LinkDown);
EXPORT_SYMBOL(MlmeRadioOn);
EXPORT_SYMBOL(MlmeRadioOff);

#ifdef DOT11_N_SUPPORT
EXPORT_SYMBOL(BAOriSessionSetUp);
EXPORT_SYMBOL(BAOriSessionTearDown);
EXPORT_SYMBOL(BARecSessionTearDown);
EXPORT_SYMBOL(RTMPSetHT);
#endif // DOT11_N_SUPPORT //
EXPORT_SYMBOL(MlmeDeauthReqAction);
EXPORT_SYMBOL(MlmeDisassocReqAction);

EXPORT_SYMBOL(GetPhyMode);
EXPORT_SYMBOL(ChannelSanity);

EXPORT_SYMBOL(rtstrchr);
EXPORT_SYMBOL(Set_SSID_Proc);
EXPORT_SYMBOL(Show_DescInfo_Proc);
EXPORT_SYMBOL(Show_Adhoc_MacTable_Proc);
EXPORT_SYMBOL(RTMPShowCfgValue);
EXPORT_SYMBOL(MlmeSetTxPreamble);
EXPORT_SYMBOL(RTMPSetDesiredRates);
EXPORT_SYMBOL(Set_PSMode_Proc);
EXPORT_SYMBOL(Set_NetworkType_Proc);
EXPORT_SYMBOL(Set_Channel_Proc);

EXPORT_SYMBOL(AsicAddSharedKeyEntry);
EXPORT_SYMBOL(AsicRemoveSharedKeyEntry);
EXPORT_SYMBOL(RTMPAddKey);
EXPORT_SYMBOL(RTMPSetWcidSecurityInfo);
EXPORT_SYMBOL(AsicRemovePairwiseKeyEntry);
EXPORT_SYMBOL(RTMPWPARemoveKeyProc);

EXPORT_SYMBOL(MlmeUpdateTxRates);
EXPORT_SYMBOL(MlmeSetPsmBit);

EXPORT_SYMBOL(MakeIbssBeacon);
EXPORT_SYMBOL(AsicEnableIbssSync);
EXPORT_SYMBOL(AsicEnableBssSync);
EXPORT_SYMBOL(AsicDisableSync);

EXPORT_SYMBOL(RTMP_MapChannelID2KHZ);
EXPORT_SYMBOL(RTMP_MapKHZ2ChannelID);

EXPORT_SYMBOL(AtoH);
EXPORT_SYMBOL(NetworkTypeInUseSanity);
EXPORT_SYMBOL(NICUpdateRawCounters);
EXPORT_SYMBOL(RTMPSendNullFrame);
EXPORT_SYMBOL(AsicForceWakeup);
EXPORT_SYMBOL(RateIdTo500Kbps);

EXPORT_SYMBOL(MacTableReset);
EXPORT_SYMBOL(RTMPCheckStrPrintAble);
EXPORT_SYMBOL(BssTableDeleteEntry);
EXPORT_SYMBOL(DisassocParmFill);
EXPORT_SYMBOL(RTMPConstructWEPIVHdr);
EXPORT_SYMBOL(RTMPMoveMemory);
EXPORT_SYMBOL(RTMPSTAPrivIoctlSet);
EXPORT_SYMBOL(StaSiteSurvey);

#ifdef RT30xx
EXPORT_SYMBOL(RT30xxReadRFRegister);
EXPORT_SYMBOL(RT30xxWriteRFRegister);
#endif // RT30xx //

#ifdef RTMP_MAC_USB
EXPORT_SYMBOL(RTUSBWriteMACRegister);
EXPORT_SYMBOL(RTUSBBssBeaconInit);
#endif // RTMP_MAC_USB //

#endif // CONFIG_STA_SUPPORT //




#ifdef RTMP_MAC_USB
EXPORT_SYMBOL(RtmpTimerQInit);
EXPORT_SYMBOL(RtmpTimerQExit);

EXPORT_SYMBOL(RTUSBBulkReceive);
EXPORT_SYMBOL(RTUSBWriteBBPRegister);
EXPORT_SYMBOL(RTUSBCancelPendingIRPs);
EXPORT_SYMBOL(RTUSBReadBBPRegister);
EXPORT_SYMBOL(RTUSBCancelPendingBulkInIRP);
EXPORT_SYMBOL(RTUSBReadMACRegister);
EXPORT_SYMBOL(RTUSBKickBulkOut);
EXPORT_SYMBOL(RTUSBCancelPendingBulkOutIRP);

EXPORT_SYMBOL(RtmpTimerQThread);
#endif // RTMP_MAC_USB //




#ifdef APCLI_SUPPORT
EXPORT_SYMBOL(ApCliIfUp);
EXPORT_SYMBOL(RT28xx_ApCli_Remove);
EXPORT_SYMBOL(ApCliIfLookUp);
#endif // APCLI_SUPPORT //

#ifdef WDS_SUPPORT
EXPORT_SYMBOL(WdsDown);
EXPORT_SYMBOL(RT28xx_WDS_Remove);
#endif // WDS_SUPPORT //

EXPORT_SYMBOL(GenerateWpsPinCode);


#ifdef AUTO_CH_SELECT_ENHANCE
EXPORT_SYMBOL(AutoChBssTableDestroy);
EXPORT_SYMBOL(ChannelInfoDestroy);
EXPORT_SYMBOL(AutoChBssTableInit);
EXPORT_SYMBOL(ChannelInfoInit);
EXPORT_SYMBOL(New_APAutoSelectChannel);
#endif // AUTO_CH_SELECT_ENHANCE //

#ifdef DOT11_N_SUPPORT

#ifdef CONFIG_STA_SUPPORT
EXPORT_SYMBOL(N_SetCenCh);
#endif // CONFIG_STA_SUPPORT //
#endif // DOT11_N_SUPPORT //

#ifdef BLOCK_NET_IF
EXPORT_SYMBOL(initblockQueueTab);
#endif // BLOCK_NET_IF //


#ifdef RALINK_ATE
EXPORT_SYMBOL(RtmpDoAte);

#ifdef CONFIG_STA_SUPPORT
#ifdef RTMP_MAC_USB
#endif // RTMP_MAC_USB //
#endif // CONFIG_STA_SUPPORT //
#endif // RALINK_ATE //



#ifdef QOS_DLS_SUPPORT
#ifdef CONFIG_STA_SUPPORT
EXPORT_SYMBOL(Set_DlsEntryInfo_Display_Proc);
EXPORT_SYMBOL(RTMPSendDLSTearDownFrame);
#endif // CONFIG_STA_SUPPORT //
#endif // QOS_DLS_SUPPORT //

#ifdef DFS_SUPPORT
#endif // DFS_SUPPORT //


#endif // OS_ABL_SUPPORT //
