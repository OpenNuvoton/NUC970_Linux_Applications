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
    sta_ioctl.c

    Abstract:
    IOCTL related subroutines

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Rory Chen   01-03-2003    created
	Rory Chen   02-14-2005    modify to support RT61
*/

#include	"rt_config.h"



char * rtstrchr(const char * s, int c)
{
    for(; *s != (char) c; ++s)
        if (*s == '\0')
            return NULL;
    return (char *) s;
}

static struct {
	PSTRING name;
	INT (*set_proc)(PRTMP_ADAPTER pAdapter, PSTRING arg);
} *PRTMP_PRIVATE_SET_PROC, RTMP_PRIVATE_SUPPORT_PROC[] = {
	{"DriverVersion",				Set_DriverVersion_Proc},
	{"CountryRegion",				Set_CountryRegion_Proc},	
	{"CountryRegionABand",			Set_CountryRegionABand_Proc},      
	{"SSID",						Set_SSID_Proc}, 
	{"WirelessMode",				Set_WirelessMode_Proc},       
	{"TxBurst",					Set_TxBurst_Proc},
	{"TxPreamble",				Set_TxPreamble_Proc},
	{"TxPower",					Set_TxPower_Proc},
	{"Channel",					Set_Channel_Proc},            
	{"BGProtection",				Set_BGProtection_Proc},
	{"RTSThreshold",				Set_RTSThreshold_Proc},       
	{"FragThreshold",				Set_FragThreshold_Proc},      
#ifdef DOT11_N_SUPPORT
	{"HtBw",		                Set_HtBw_Proc},
	{"HtMcs",		                Set_HtMcs_Proc},
	{"HtGi",		                Set_HtGi_Proc},
	{"HtOpMode",		            Set_HtOpMode_Proc},
	{"HtExtcha",		            Set_HtExtcha_Proc},
	{"HtMpduDensity",		        Set_HtMpduDensity_Proc},
	{"HtBaWinSize",		        	Set_HtBaWinSize_Proc},
	{"HtRdg",		        		Set_HtRdg_Proc},
	{"HtAmsdu",		        		Set_HtAmsdu_Proc},
	{"HtAutoBa",		        	Set_HtAutoBa_Proc},
	{"HtBaDecline",					Set_BADecline_Proc},
	{"HtProtect",		        	Set_HtProtect_Proc},
	{"HtMimoPs",		        	Set_HtMimoPs_Proc},
	{"HtDisallowTKIP",				Set_HtDisallowTKIP_Proc},
#ifdef DOT11N_DRAFT3
	{"HtBssCoex",				Set_HT_BssCoex_Proc},
#endif // DOT11N_DRAFT3 //
#endif // DOT11_N_SUPPORT //
	
#ifdef AGGREGATION_SUPPORT
	{"PktAggregate",				Set_PktAggregate_Proc},       
#endif // AGGREGATION_SUPPORT //

#ifdef WMM_SUPPORT
	{"WmmCapable",					Set_WmmCapable_Proc},         
#endif         
	{"IEEE80211H",					Set_IEEE80211H_Proc},
    {"NetworkType",                 Set_NetworkType_Proc},        
	{"AuthMode",					Set_AuthMode_Proc},           
	{"EncrypType",					Set_EncrypType_Proc},         
	{"DefaultKeyID",				Set_DefaultKeyID_Proc},       
	{"Key1",						Set_Key1_Proc},               
	{"Key2",						Set_Key2_Proc},               
	{"Key3",						Set_Key3_Proc},               
	{"Key4",						Set_Key4_Proc},               
	{"WPAPSK",						Set_WPAPSK_Proc},
	{"ResetCounter",				Set_ResetStatCounter_Proc},
	{"PSMode",                      Set_PSMode_Proc},
#ifdef DBG
	{"Debug",						Set_Debug_Proc},             
#endif // DBG //

#ifdef RALINK_ATE
	{"ATE",							Set_ATE_Proc},
	{"ATEDA",						Set_ATE_DA_Proc},
	{"ATESA",						Set_ATE_SA_Proc},
	{"ATEBSSID",					Set_ATE_BSSID_Proc},
	{"ATECHANNEL",					Set_ATE_CHANNEL_Proc},
	{"ATETXPOW0",					Set_ATE_TX_POWER0_Proc},
	{"ATETXPOW1",					Set_ATE_TX_POWER1_Proc},
	{"ATETXANT",					Set_ATE_TX_Antenna_Proc},
	{"ATERXANT",					Set_ATE_RX_Antenna_Proc},
	{"ATETXFREQOFFSET",				Set_ATE_TX_FREQOFFSET_Proc},
	{"ATETXBW",						Set_ATE_TX_BW_Proc},
	{"ATETXLEN",					Set_ATE_TX_LENGTH_Proc},
	{"ATETXCNT",					Set_ATE_TX_COUNT_Proc},
	{"ATETXMCS",					Set_ATE_TX_MCS_Proc},
	{"ATETXMODE",					Set_ATE_TX_MODE_Proc},
	{"ATETXGI",						Set_ATE_TX_GI_Proc},
	{"ATERXFER",					Set_ATE_RX_FER_Proc},
	{"ATERRF",						Set_ATE_Read_RF_Proc},
#ifndef RTMP_RF_RW_SUPPORT
	{"ATEWRF1",						Set_ATE_Write_RF1_Proc},
	{"ATEWRF2",						Set_ATE_Write_RF2_Proc},
	{"ATEWRF3",						Set_ATE_Write_RF3_Proc},
	{"ATEWRF4",						Set_ATE_Write_RF4_Proc},
#endif // RTMP_RF_RW_SUPPORT //
	{"ATELDE2P",				    Set_ATE_Load_E2P_Proc},
	{"ATERE2P",						Set_ATE_Read_E2P_Proc},
	{"ATESHOW",						Set_ATE_Show_Proc},
	{"ATEHELP",						Set_ATE_Help_Proc},

#ifdef RALINK_28xx_QA
	{"TxStop",						Set_TxStop_Proc},
	{"RxStop",						Set_RxStop_Proc},	
#endif // RALINK_28xx_QA //
#endif // RALINK_ATE //

#ifdef WPA_SUPPLICANT_SUPPORT
    {"WpaSupport",                  Set_Wpa_Support},
#endif // WPA_SUPPLICANT_SUPPORT //





	{"FixedTxMode",                 Set_FixedTxMode_Proc},
#ifdef CONFIG_APSTA_MIXED_SUPPORT
	{"OpMode",						Set_OpMode_Proc},
#endif // CONFIG_APSTA_MIXED_SUPPORT //
#ifdef DOT11_N_SUPPORT
    {"TGnWifiTest",                 Set_TGnWifiTest_Proc},
#endif // DOT11_N_SUPPORT //
#ifdef QOS_DLS_SUPPORT
	{"DlsAddEntry",					Set_DlsAddEntry_Proc},
	{"DlsTearDownEntry",			Set_DlsTearDownEntry_Proc},
#endif // QOS_DLS_SUPPORT //
	{"LongRetry",	        		Set_LongRetryLimit_Proc},
	{"ShortRetry",	        		Set_ShortRetryLimit_Proc},
	{"AutoFallBack",	        	Set_AutoFallBack_Proc},
#ifdef EXT_BUILD_CHANNEL_LIST
	{"11dClientMode",				Set_Ieee80211dClientMode_Proc},
#endif // EXT_BUILD_CHANNEL_LIST //
#ifdef CARRIER_DETECTION_SUPPORT
	{"CarrierDetect",				Set_CarrierDetect_Proc},
#endif // CARRIER_DETECTION_SUPPORT //


//2008/09/11:KH add to support efuse<--
#ifdef RT30xx
#ifdef RTMP_EFUSE_SUPPORT
	{"efuseFreeNumber",				set_eFuseGetFreeBlockCount_Proc},
	{"efuseDump",					set_eFusedump_Proc},
	{"efuseLoadFromBin",				set_eFuseLoadFromBin_Proc},
#ifdef RALINK_ATE
	{"efuseBufferModeWriteBack",		set_eFuseBufferModeWriteBack_Proc},
#endif // RALINK_ATE //
#endif // RTMP_EFUSE_SUPPORT //
#endif // RT30xx //
//2008/09/11:KH add to support efuse-->
	{"BeaconLostTime",				Set_BeaconLostTime_Proc},
	{"AutoRoaming",					Set_AutoRoaming_Proc},
	{"SiteSurvey",					Set_SiteSurvey_Proc},
	{"ForceTxBurst",				Set_ForceTxBurst_Proc},
#ifdef XLINK_SUPPORT
	{"XlinkMode",					Set_XlinkMode_Proc},
#endif // XLINK_SUPPORT //


	{NULL,}
};


INT RTMPSTAPrivIoctlSet(
	IN RTMP_ADAPTER *pAd, 
	IN PSTRING SetProcName,
	IN PSTRING ProcArg)
{
	int ret = 0;
	
	for (PRTMP_PRIVATE_SET_PROC = RTMP_PRIVATE_SUPPORT_PROC; PRTMP_PRIVATE_SET_PROC->name; PRTMP_PRIVATE_SET_PROC++)            
	{                                                                                                                           
	    if (strcmp(SetProcName, PRTMP_PRIVATE_SET_PROC->name) == 0)                                                               
	    {						                                                                                                
	        if(!PRTMP_PRIVATE_SET_PROC->set_proc(pAd, ProcArg))                                                              
	        {	//FALSE:Set private failed then return Invalid argument                                                         
			    return NDIS_STATUS_FAILURE;                                                                                               
	        }                                                                                                                   
		    break;	//Exit for loop.                                                                                      
	    }
	}
	
	if(PRTMP_PRIVATE_SET_PROC->name == NULL)                                                                                    
	{  //Not found argument     
	 	DBGPRINT(RT_DEBUG_TRACE, ("===>rt_ioctl_setparam:: (iwpriv) Not Support Set Command [%s=%s]\n", SetProcName, ProcArg));
	    return -EINVAL;                                                                                                       
	   
	}         

	return ret;
}


/* 
    ==========================================================================
    Description:
        Set SSID
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_SSID_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING          arg)
{
    NDIS_802_11_SSID                    Ssid, *pSsid=NULL;
    BOOLEAN                             StateMachineTouched = FALSE;
    int                                 success = TRUE;

    if( strlen(arg) <= MAX_LEN_OF_SSID)
    {
        NdisZeroMemory(&Ssid, sizeof(NDIS_802_11_SSID));
        if (strlen(arg) != 0)
        {
            NdisMoveMemory(Ssid.Ssid, arg, strlen(arg));
            Ssid.SsidLength = strlen(arg);
        }
        else   //ANY ssid
        {    
            Ssid.SsidLength = 0; 
	    memcpy(Ssid.Ssid, "", 0);
			pAd->StaCfg.BssType = BSS_INFRA;	
			pAd->StaCfg.AuthMode = Ndis802_11AuthModeOpen;
	        pAd->StaCfg.WepStatus  = Ndis802_11EncryptionDisabled;		    
	}	 
        pSsid = &Ssid;

        if (pAd->Mlme.CntlMachine.CurrState != CNTL_IDLE)
        {
            RTMP_MLME_RESET_STATE_MACHINE(pAd);
            DBGPRINT(RT_DEBUG_TRACE, ("!!! MLME busy, reset MLME state machine !!!\n"));
        }

		if ((pAd->StaCfg.WpaPassPhraseLen >= 8) &&
			(pAd->StaCfg.WpaPassPhraseLen <= 64))
		{
			UCHAR keyMaterial[40];
			
			RTMPZeroMemory(pAd->StaCfg.PMK, 32);
			if (pAd->StaCfg.WpaPassPhraseLen == 64)
			{
			    AtoH((PSTRING) pAd->StaCfg.WpaPassPhrase, pAd->StaCfg.PMK, 32);
			}
			else
			{
			    RtmpPasswordHash((PSTRING) pAd->StaCfg.WpaPassPhrase, Ssid.Ssid, Ssid.SsidLength, keyMaterial);
			    NdisMoveMemory(pAd->StaCfg.PMK, keyMaterial, 32);		
			}
		}

		// Record the desired user settings to MlmeAux
		NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
		NdisMoveMemory(pAd->MlmeAux.Ssid, Ssid.Ssid, Ssid.SsidLength);
		pAd->MlmeAux.SsidLen = (UCHAR)Ssid.SsidLength;

        pAd->MlmeAux.CurrReqIsFromNdis = TRUE;
        pAd->StaCfg.bScanReqIsFromWebUI = FALSE;
		pAd->bConfigChanged = TRUE;

        if (pAd->StaCfg.BssType == BSS_ADHOC)
            pAd->StaCfg.bNotFirstScan = FALSE;            
        

        MlmeEnqueue(pAd, 
                    MLME_CNTL_STATE_MACHINE, 
                    OID_802_11_SSID,
                    sizeof(NDIS_802_11_SSID),
                    (VOID *)pSsid, 0);

        StateMachineTouched = TRUE;
        DBGPRINT(RT_DEBUG_TRACE, ("Set_SSID_Proc::(Len=%d,Ssid=%s)\n", Ssid.SsidLength, Ssid.Ssid));
    }
    else
        success = FALSE;

    if (StateMachineTouched) // Upper layer sent a MLME-related operations
    	RTMP_MLME_HANDLER(pAd);

    return success;
}

#ifdef WMM_SUPPORT
/* 
    ==========================================================================
    Description:
        Set WmmCapable Enable or Disable
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT	Set_WmmCapable_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	BOOLEAN	bWmmCapable;

	bWmmCapable = simple_strtol(arg, 0, 10);

	if ((bWmmCapable == 1)
#ifdef RTMP_MAC_USB
		&& (pAd->NumberOfPipes >= 5)
#endif // RTMP_MAC_USB //		
		)
		pAd->CommonCfg.bWmmCapable = TRUE;
	else if (bWmmCapable == 0)
		pAd->CommonCfg.bWmmCapable = FALSE;
	else
		return FALSE;  //Invalid argument 
	
	DBGPRINT(RT_DEBUG_TRACE, ("Set_WmmCapable_Proc::(bWmmCapable=%d)\n", 
		pAd->CommonCfg.bWmmCapable));

	return TRUE;
}
#endif // WMM_SUPPORT //

/* 
    ==========================================================================
    Description:
        Set Network Type(Infrastructure/Adhoc mode)
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_NetworkType_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING          arg)
{
    UINT32	Value = 0;

    if (strcmp(arg, "Adhoc") == 0)
	{
		if (pAd->StaCfg.BssType != BSS_ADHOC)
		{				    
			// Config has changed
			pAd->bConfigChanged = TRUE;
            if (MONITOR_ON(pAd))
            {
                RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, STANORMAL);
                RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);
				Value &= (~0x80);
				RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Value);
                OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);
                pAd->StaCfg.bAutoReconnect = TRUE;
                LinkDown(pAd, FALSE);
            }
			if (INFRA_ON(pAd))
			{
				//BOOLEAN Cancelled;
				// Set the AutoReconnectSsid to prevent it reconnect to old SSID
				// Since calling this indicate user don't want to connect to that SSID anymore.
				pAd->MlmeAux.AutoReconnectSsidLen= 32;
				NdisZeroMemory(pAd->MlmeAux.AutoReconnectSsid, pAd->MlmeAux.AutoReconnectSsidLen);		
				
				LinkDown(pAd, FALSE);

				DBGPRINT(RT_DEBUG_TRACE, ("NDIS_STATUS_MEDIA_DISCONNECT Event BB!\n"));
			}
		}			
		pAd->StaCfg.BssType = BSS_ADHOC;
		RTMP_OS_NETDEV_SET_TYPE(pAd->net_dev, pAd->StaCfg.OriDevType);
		
		DBGPRINT(RT_DEBUG_TRACE, ("===>Set_NetworkType_Proc::(AD-HOC)\n"));
	}
    else if (strcmp(arg, "Infra") == 0)
	{
		if (pAd->StaCfg.BssType != BSS_INFRA)
		{			    
			// Config has changed
			pAd->bConfigChanged = TRUE;
            if (MONITOR_ON(pAd))
            {
                RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, STANORMAL);
                RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);
				Value &= (~0x80);
				RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Value);
                OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);
                pAd->StaCfg.bAutoReconnect = TRUE;
                LinkDown(pAd, FALSE);
            }
			if (ADHOC_ON(pAd))
			{
				// Set the AutoReconnectSsid to prevent it reconnect to old SSID
				// Since calling this indicate user don't want to connect to that SSID anymore.
				pAd->MlmeAux.AutoReconnectSsidLen= 32;
				NdisZeroMemory(pAd->MlmeAux.AutoReconnectSsid, pAd->MlmeAux.AutoReconnectSsidLen);			
			
				LinkDown(pAd, FALSE);
			}
		}			
		pAd->StaCfg.BssType = BSS_INFRA;
		RTMP_OS_NETDEV_SET_TYPE(pAd->net_dev, pAd->StaCfg.OriDevType);
		DBGPRINT(RT_DEBUG_TRACE, ("===>Set_NetworkType_Proc::(INFRA)\n"));            
	}
#ifdef MONITOR_FLAG_11N_SNIFFER_SUPPORT
	/*
		Monitor2 is for 3593 11n wireshark sniffer tool.
		The name, Monitor2, follows the command format in RT2883.
	*/
    else if ((strcmp(arg, "Monitor") == 0) || (strcmp(arg, "Monitor2") == 0))
#else
    else if (strcmp(arg, "Monitor") == 0)
#endif // MONITOR_FLAG_11N_SNIFFER_SUPPORT //
    {
			UCHAR	bbpValue = 0;
			BCN_TIME_CFG_STRUC csr;

#ifdef MONITOR_FLAG_11N_SNIFFER_SUPPORT
			if (strcmp(arg, "Monitor2") == 0)
				pAd->StaCfg.BssMonitorFlag |= MONITOR_FLAG_11N_SNIFFER;
			/* End of if */
#endif // MONITOR_FLAG_11N_SNIFFER_SUPPORT //

			OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_INFRA_ON);
            OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_ADHOC_ON);
			OPSTATUS_SET_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);
			// disable all periodic state machine
			pAd->StaCfg.bAutoReconnect = FALSE;
			// reset all mlme state machine
			RTMP_MLME_RESET_STATE_MACHINE(pAd);
			DBGPRINT(RT_DEBUG_TRACE, ("fOP_STATUS_MEDIA_STATE_CONNECTED \n"));
            if (pAd->CommonCfg.CentralChannel == 0)
            {
#ifdef DOT11_N_SUPPORT
                if (pAd->CommonCfg.PhyMode == PHY_11AN_MIXED)
                    pAd->CommonCfg.CentralChannel = 36;
                else
#endif // DOT11_N_SUPPORT //
                    pAd->CommonCfg.CentralChannel = 6;
            }
#ifdef DOT11_N_SUPPORT
            else
                N_ChannelCheck(pAd);
#endif // DOT11_N_SUPPORT //


			/* same procedure with window driver */
#ifdef DOT11_N_SUPPORT
			if (pAd->CommonCfg.PhyMode >= PHY_11ABGN_MIXED &&
                pAd->CommonCfg.RegTransmitSetting.field.BW == BW_40 &&
                pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE)
			{
				// 40MHz ,control channel at lower
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &bbpValue);
				bbpValue &= (~0x18);
				bbpValue |= 0x10;
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, bbpValue);
				pAd->CommonCfg.BBPCurrentBW = BW_40;
				//  RX : control channel at lower 
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &bbpValue);
				bbpValue &= (~0x20);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, bbpValue);

				RTMP_IO_READ32(pAd, TX_BAND_CFG, &Value);
				Value &= 0xfffffffe;
				RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);
				pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel + 2;
                AsicSwitchChannel(pAd, pAd->CommonCfg.CentralChannel, FALSE);
			    AsicLockChannel(pAd, pAd->CommonCfg.CentralChannel);
                DBGPRINT(RT_DEBUG_TRACE, ("BW_40 ,control_channel(%d), CentralChannel(%d) \n", 
                                           pAd->CommonCfg.Channel,
                                           pAd->CommonCfg.CentralChannel));
			}
			else if (pAd->CommonCfg.PhyMode >= PHY_11ABGN_MIXED &&
                     pAd->CommonCfg.RegTransmitSetting.field.BW == BW_40 &&
                     pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_BELOW)
			{
				// 40MHz ,control channel at upper
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &bbpValue);
				bbpValue &= (~0x18);
				bbpValue |= 0x10;
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, bbpValue);
				pAd->CommonCfg.BBPCurrentBW = BW_40;
				RTMP_IO_READ32(pAd, TX_BAND_CFG, &Value);
				Value |= 0x1;
				RTMP_IO_WRITE32(pAd, TX_BAND_CFG, Value);
				
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R3, &bbpValue);
				bbpValue |= (0x20);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3, bbpValue);
				pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel - 2;
                AsicSwitchChannel(pAd, pAd->CommonCfg.CentralChannel, FALSE);
			    AsicLockChannel(pAd, pAd->CommonCfg.CentralChannel);
                DBGPRINT(RT_DEBUG_TRACE, ("BW_40 ,control_channel(%d), CentralChannel(%d) \n", 
                                           pAd->CommonCfg.Channel,
                                           pAd->CommonCfg.CentralChannel));
			}
			else
#endif // DOT11_N_SUPPORT //
			{
				// 20MHz
				RTMP_BBP_IO_READ8_BY_REG_ID(pAd, BBP_R4, &bbpValue);
				bbpValue &= (~0x18);
				RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R4, bbpValue);
				pAd->CommonCfg.BBPCurrentBW = BW_20;
                AsicSwitchChannel(pAd, pAd->CommonCfg.Channel, FALSE);
			    AsicLockChannel(pAd, pAd->CommonCfg.Channel);
				DBGPRINT(RT_DEBUG_TRACE, ("BW_20, Channel(%d)\n", pAd->CommonCfg.Channel));
			}

#ifdef RT3593
			if (IS_RT3593(pAd))
				RTMPSendNullFrame(pAd, pAd->CommonCfg.TxRate, TRUE);
#endif // RT3593 //

			// Enable Rx with promiscuous reception
			RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, 0x3);
			// ASIC supporsts sniffer function with replacing RSSI with timestamp.
			//RTMP_IO_READ32(pAdapter, MAC_SYS_CTRL, &Value);
			//Value |= (0x80);
			//RTMP_IO_WRITE32(pAdapter, MAC_SYS_CTRL, Value);
			// disable sync
			RTMP_IO_READ32(pAd, BCN_TIME_CFG, &csr.word);
			csr.field.bBeaconGen = 0;
			csr.field.bTBTTEnable = 0;
			csr.field.TsfSyncMode = 0;
			RTMP_IO_WRITE32(pAd, BCN_TIME_CFG, csr.word);
            
			pAd->StaCfg.BssType = BSS_MONITOR;
			RTMP_OS_NETDEV_SET_TYPE(pAd->net_dev, ARPHRD_IEEE80211_PRISM);  //ARPHRD_IEEE80211; // IEEE80211
			DBGPRINT(RT_DEBUG_TRACE, ("===>Set_NetworkType_Proc::(MONITOR)\n"));
    }

    // Reset Ralink supplicant to not use, it will be set to start when UI set PMK key
    pAd->StaCfg.WpaState = SS_NOTUSE;

    DBGPRINT(RT_DEBUG_TRACE, ("Set_NetworkType_Proc::(NetworkType=%d)\n", pAd->StaCfg.BssType));

    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Authentication mode
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_AuthMode_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING          arg)
{
    if ((strcmp(arg, "WEPAUTO") == 0) || (strcmp(arg, "wepauto") == 0))
        pAd->StaCfg.AuthMode = Ndis802_11AuthModeAutoSwitch;
    else if ((strcmp(arg, "OPEN") == 0) || (strcmp(arg, "open") == 0))
        pAd->StaCfg.AuthMode = Ndis802_11AuthModeOpen;
    else if ((strcmp(arg, "SHARED") == 0) || (strcmp(arg, "shared") == 0))
        pAd->StaCfg.AuthMode = Ndis802_11AuthModeShared;
    else if ((strcmp(arg, "WPAPSK") == 0) || (strcmp(arg, "wpapsk") == 0))
        pAd->StaCfg.AuthMode = Ndis802_11AuthModeWPAPSK;
    else if ((strcmp(arg, "WPANONE") == 0) || (strcmp(arg, "wpanone") == 0))
        pAd->StaCfg.AuthMode = Ndis802_11AuthModeWPANone;
    else if ((strcmp(arg, "WPA2PSK") == 0) || (strcmp(arg, "wpa2psk") == 0))
        pAd->StaCfg.AuthMode = Ndis802_11AuthModeWPA2PSK;    
#ifdef WPA_SUPPLICANT_SUPPORT    
    else if ((strcmp(arg, "WPA") == 0) || (strcmp(arg, "wpa") == 0))
        pAd->StaCfg.AuthMode = Ndis802_11AuthModeWPA;    
    else if ((strcmp(arg, "WPA2") == 0) || (strcmp(arg, "wpa2") == 0))
        pAd->StaCfg.AuthMode = Ndis802_11AuthModeWPA2;
#endif // WPA_SUPPLICANT_SUPPORT //
    else
        return FALSE;  

    pAd->StaCfg.PortSecured = WPA_802_1X_PORT_NOT_SECURED;

    DBGPRINT(RT_DEBUG_TRACE, ("Set_AuthMode_Proc::(AuthMode=%d)\n", pAd->StaCfg.AuthMode));

    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Encryption Type
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_EncrypType_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING          arg)
{
    if ((strcmp(arg, "NONE") == 0) || (strcmp(arg, "none") == 0))
    {
        if (pAd->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
            return TRUE;    // do nothing
            
        pAd->StaCfg.WepStatus     = Ndis802_11WEPDisabled;
        pAd->StaCfg.PairCipher    = Ndis802_11WEPDisabled;
	    pAd->StaCfg.GroupCipher   = Ndis802_11WEPDisabled;
    }
    else if ((strcmp(arg, "WEP") == 0) || (strcmp(arg, "wep") == 0))
    {
        if (pAd->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
            return TRUE;    // do nothing
            
        pAd->StaCfg.WepStatus     = Ndis802_11WEPEnabled;
        pAd->StaCfg.PairCipher    = Ndis802_11WEPEnabled;
	    pAd->StaCfg.GroupCipher   = Ndis802_11WEPEnabled;		
    }
    else if ((strcmp(arg, "TKIP") == 0) || (strcmp(arg, "tkip") == 0))
    {
        if (pAd->StaCfg.AuthMode < Ndis802_11AuthModeWPA)
            return TRUE;    // do nothing
            
        pAd->StaCfg.WepStatus     = Ndis802_11Encryption2Enabled;
        pAd->StaCfg.PairCipher    = Ndis802_11Encryption2Enabled;
	    pAd->StaCfg.GroupCipher   = Ndis802_11Encryption2Enabled;
    }
    else if ((strcmp(arg, "AES") == 0) || (strcmp(arg, "aes") == 0))
    {
        if (pAd->StaCfg.AuthMode < Ndis802_11AuthModeWPA)
            return TRUE;    // do nothing
            
        pAd->StaCfg.WepStatus     = Ndis802_11Encryption3Enabled;
        pAd->StaCfg.PairCipher    = Ndis802_11Encryption3Enabled;
	    pAd->StaCfg.GroupCipher   = Ndis802_11Encryption3Enabled;
    }
    else
        return FALSE;

    pAd->StaCfg.OrigWepStatus = pAd->StaCfg.WepStatus;

	if (pAd->StaCfg.BssType == BSS_ADHOC)
	{
		// Build all corresponding channel information
		RTMPSetPhyMode(pAd, pAd->CommonCfg.DesiredPhyMode);
#ifdef DOT11_N_SUPPORT
		SetCommonHT(pAd);
#endif // DOT11_N_SUPPORT //
	}

    DBGPRINT(RT_DEBUG_TRACE, ("Set_EncrypType_Proc::(EncrypType=%d)\n", pAd->StaCfg.WepStatus));

    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Default Key ID
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_DefaultKeyID_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PSTRING          arg)
{
    ULONG                               KeyIdx;

    KeyIdx = simple_strtol(arg, 0, 10);
    if((KeyIdx >= 1 ) && (KeyIdx <= 4))
        pAdapter->StaCfg.DefaultKeyId = (UCHAR) (KeyIdx - 1 );
    else
        return FALSE;  //Invalid argument 

    DBGPRINT(RT_DEBUG_TRACE, ("Set_DefaultKeyID_Proc::(DefaultKeyID=%d)\n", pAdapter->StaCfg.DefaultKeyId));

    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WEP KEY1
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_Key1_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PSTRING          arg)
{
    int                                 KeyLen;
    int                                 i;
    UCHAR                               CipherAlg=CIPHER_WEP64;

    if (pAdapter->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
        return TRUE;    // do nothing
    
    KeyLen = strlen(arg);

    switch (KeyLen)
    {
        case 5: //wep 40 Ascii type
            pAdapter->SharedKey[BSS0][0].KeyLen = KeyLen;
            memcpy(pAdapter->SharedKey[BSS0][0].Key, arg, KeyLen);
            CipherAlg = CIPHER_WEP64;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key1_Proc::(Key1=%s and type=%s)\n", arg, "Ascii"));       
            break;
        case 10: //wep 40 Hex type
            for(i=0; i < KeyLen; i++)
            {
                if( !isxdigit(*(arg+i)) )
                    return FALSE;  //Not Hex value;
            }
            pAdapter->SharedKey[BSS0][0].KeyLen = KeyLen / 2 ;
            AtoH(arg, pAdapter->SharedKey[BSS0][0].Key, KeyLen / 2);
            CipherAlg = CIPHER_WEP64;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key1_Proc::(Key1=%s and type=%s)\n", arg, "Hex"));     
            break;
        case 13: //wep 104 Ascii type
            pAdapter->SharedKey[BSS0][0].KeyLen = KeyLen;
            memcpy(pAdapter->SharedKey[BSS0][0].Key, arg, KeyLen);
            CipherAlg = CIPHER_WEP128;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key1_Proc::(Key1=%s and type=%s)\n", arg, "Ascii"));       
            break;
        case 26: //wep 104 Hex type
            for(i=0; i < KeyLen; i++)
            {
                if( !isxdigit(*(arg+i)) )
                    return FALSE;  //Not Hex value;
            }
            pAdapter->SharedKey[BSS0][0].KeyLen = KeyLen / 2 ;
            AtoH(arg, pAdapter->SharedKey[BSS0][0].Key, KeyLen / 2);
            CipherAlg = CIPHER_WEP128;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key1_Proc::(Key1=%s and type=%s)\n", arg, "Hex"));     
            break;
        default: //Invalid argument 
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key1_Proc::Invalid argument (=%s)\n", arg));       
            return FALSE;
    }
    
    pAdapter->SharedKey[BSS0][0].CipherAlg = CipherAlg;

    // Set keys (into ASIC)
    if (pAdapter->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
        ;   // not support
    else    // Old WEP stuff
    {
        AsicAddSharedKeyEntry(pAdapter, 
                              0, 
                              0, 
                              &pAdapter->SharedKey[BSS0][0]);
    }
    
    return TRUE;
}
/* 
    ==========================================================================

    Description:
        Set WEP KEY2
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_Key2_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PSTRING          arg)
{
    int                                 KeyLen;
    int                                 i;
    UCHAR                               CipherAlg=CIPHER_WEP64;

    if (pAdapter->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
        return TRUE;    // do nothing
    
    KeyLen = strlen(arg);

    switch (KeyLen)
    {
        case 5: //wep 40 Ascii type
            pAdapter->SharedKey[BSS0][1].KeyLen = KeyLen;
            memcpy(pAdapter->SharedKey[BSS0][1].Key, arg, KeyLen);
            CipherAlg = CIPHER_WEP64;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key2_Proc::(Key2=%s and type=%s)\n", arg, "Ascii"));
            break;
        case 10: //wep 40 Hex type
            for(i=0; i < KeyLen; i++)
            {
                if( !isxdigit(*(arg+i)) )
                    return FALSE;  //Not Hex value;
            }
            pAdapter->SharedKey[BSS0][1].KeyLen = KeyLen / 2 ;
            AtoH(arg, pAdapter->SharedKey[BSS0][1].Key, KeyLen / 2);
            CipherAlg = CIPHER_WEP64;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key2_Proc::(Key2=%s and type=%s)\n", arg, "Hex"));
            break;
        case 13: //wep 104 Ascii type
            pAdapter->SharedKey[BSS0][1].KeyLen = KeyLen;
            memcpy(pAdapter->SharedKey[BSS0][1].Key, arg, KeyLen);
            CipherAlg = CIPHER_WEP128;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key2_Proc::(Key2=%s and type=%s)\n", arg, "Ascii"));
            break;
        case 26: //wep 104 Hex type
            for(i=0; i < KeyLen; i++)
            {
                if( !isxdigit(*(arg+i)) )
                    return FALSE;  //Not Hex value;
            }
            pAdapter->SharedKey[BSS0][1].KeyLen = KeyLen / 2 ;
            AtoH(arg, pAdapter->SharedKey[BSS0][1].Key, KeyLen / 2);
            CipherAlg = CIPHER_WEP128;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key2_Proc::(Key2=%s and type=%s)\n", arg, "Hex"));
            break;
        default: //Invalid argument 
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key2_Proc::Invalid argument (=%s)\n", arg));
            return FALSE;
    }
    pAdapter->SharedKey[BSS0][1].CipherAlg = CipherAlg;

    // Set keys (into ASIC)
    if (pAdapter->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
        ;   // not support
    else    // Old WEP stuff
    {
        AsicAddSharedKeyEntry(pAdapter, 
                              0, 
                              1, 
                              &pAdapter->SharedKey[BSS0][1]);
    }        
    
    return TRUE;
}
/* 
    ==========================================================================
    Description:
        Set WEP KEY3
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_Key3_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PSTRING          arg)
{
    int                                 KeyLen;
    int                                 i;
    UCHAR                               CipherAlg=CIPHER_WEP64;

    if (pAdapter->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
        return TRUE;    // do nothing
    
    KeyLen = strlen(arg);

    switch (KeyLen)
    {
        case 5: //wep 40 Ascii type
            pAdapter->SharedKey[BSS0][2].KeyLen = KeyLen;
            memcpy(pAdapter->SharedKey[BSS0][2].Key, arg, KeyLen);
            CipherAlg = CIPHER_WEP64;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key3_Proc::(Key3=%s and type=Ascii)\n", arg));
            break;
        case 10: //wep 40 Hex type
            for(i=0; i < KeyLen; i++)
            {
                if( !isxdigit(*(arg+i)) )
                    return FALSE;  //Not Hex value;
            }
            pAdapter->SharedKey[BSS0][2].KeyLen = KeyLen / 2 ;
            AtoH(arg, pAdapter->SharedKey[BSS0][2].Key, KeyLen / 2);
            CipherAlg = CIPHER_WEP64;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key3_Proc::(Key3=%s and type=Hex)\n", arg));
            break;
        case 13: //wep 104 Ascii type
            pAdapter->SharedKey[BSS0][2].KeyLen = KeyLen;
            memcpy(pAdapter->SharedKey[BSS0][2].Key, arg, KeyLen);
            CipherAlg = CIPHER_WEP128;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key3_Proc::(Key3=%s and type=Ascii)\n", arg));
            break;
        case 26: //wep 104 Hex type
            for(i=0; i < KeyLen; i++)
            {
                if( !isxdigit(*(arg+i)) )
                    return FALSE;  //Not Hex value;
            }
            pAdapter->SharedKey[BSS0][2].KeyLen = KeyLen / 2 ;
            AtoH(arg, pAdapter->SharedKey[BSS0][2].Key, KeyLen / 2);
            CipherAlg = CIPHER_WEP128;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key3_Proc::(Key3=%s and type=Hex)\n", arg));
            break;
        default: //Invalid argument 
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key3_Proc::Invalid argument (=%s)\n", arg));
            return FALSE;
    }
    pAdapter->SharedKey[BSS0][2].CipherAlg = CipherAlg;
    
    // Set keys (into ASIC)
    if (pAdapter->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
        ;   // not support
    else    // Old WEP stuff
    {
        AsicAddSharedKeyEntry(pAdapter, 
                              0, 
                              2, 
                              &pAdapter->SharedKey[BSS0][2]);
    }
    
    return TRUE;
}
/* 
    ==========================================================================
    Description:
        Set WEP KEY4
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_Key4_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PSTRING          arg)
{
    int                                 KeyLen;
    int                                 i;
    UCHAR                               CipherAlg=CIPHER_WEP64;

    if (pAdapter->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
        return TRUE;    // do nothing
    
    KeyLen = strlen(arg);

    switch (KeyLen)
    {
        case 5: //wep 40 Ascii type
            pAdapter->SharedKey[BSS0][3].KeyLen = KeyLen;
            memcpy(pAdapter->SharedKey[BSS0][3].Key, arg, KeyLen);
            CipherAlg = CIPHER_WEP64;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key4_Proc::(Key4=%s and type=%s)\n", arg, "Ascii"));
            break;
        case 10: //wep 40 Hex type
            for(i=0; i < KeyLen; i++)
            {
                if( !isxdigit(*(arg+i)) )
                    return FALSE;  //Not Hex value;
            }
            pAdapter->SharedKey[BSS0][3].KeyLen = KeyLen / 2 ;
            AtoH(arg, pAdapter->SharedKey[BSS0][3].Key, KeyLen / 2);
            CipherAlg = CIPHER_WEP64;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key4_Proc::(Key4=%s and type=%s)\n", arg, "Hex"));
            break;
        case 13: //wep 104 Ascii type
            pAdapter->SharedKey[BSS0][3].KeyLen = KeyLen;
            memcpy(pAdapter->SharedKey[BSS0][3].Key, arg, KeyLen);
            CipherAlg = CIPHER_WEP128;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key4_Proc::(Key4=%s and type=%s)\n", arg, "Ascii"));
            break;
        case 26: //wep 104 Hex type
            for(i=0; i < KeyLen; i++)
            {
                if( !isxdigit(*(arg+i)) )
                    return FALSE;  //Not Hex value;
            }
            pAdapter->SharedKey[BSS0][3].KeyLen = KeyLen / 2 ;
            AtoH(arg, pAdapter->SharedKey[BSS0][3].Key, KeyLen / 2);
            CipherAlg = CIPHER_WEP128;
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key4_Proc::(Key4=%s and type=%s)\n", arg, "Hex"));
            break;
        default: //Invalid argument 
            DBGPRINT(RT_DEBUG_TRACE, ("Set_Key4_Proc::Invalid argument (=%s)\n", arg));
            return FALSE;
    } 
    pAdapter->SharedKey[BSS0][3].CipherAlg = CipherAlg;
    
    // Set keys (into ASIC)
    if (pAdapter->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
        ;   // not support
    else    // Old WEP stuff
    {
        AsicAddSharedKeyEntry(pAdapter, 
                              0, 
                              3, 
                              &pAdapter->SharedKey[BSS0][3]);
    }
    
    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set WPA PSK key
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_WPAPSK_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING          arg)
{
    int status;
	
    if ((pAd->StaCfg.AuthMode != Ndis802_11AuthModeWPAPSK) && 
        (pAd->StaCfg.AuthMode != Ndis802_11AuthModeWPA2PSK) &&
	    (pAd->StaCfg.AuthMode != Ndis802_11AuthModeWPANone)
		)
        return TRUE;    // do nothing
        
    DBGPRINT(RT_DEBUG_TRACE, ("Set_WPAPSK_Proc::(WPAPSK=%s)\n", arg));

	status = RT_CfgSetWPAPSKKey(pAd, arg, pAd->MlmeAux.Ssid, pAd->MlmeAux.SsidLen, pAd->StaCfg.PMK);
	if (status == FALSE)
	{
		DBGPRINT(RT_DEBUG_TRACE, ("Set_WPAPSK_Proc(): Set key failed!\n"));
		return FALSE;
	}
	NdisZeroMemory(pAd->StaCfg.WpaPassPhrase, 64);
    NdisMoveMemory(pAd->StaCfg.WpaPassPhrase, arg, strlen(arg));
    pAd->StaCfg.WpaPassPhraseLen = (UINT)strlen(arg);



    if(pAd->StaCfg.BssType == BSS_ADHOC &&
       pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPANone)
    {
        pAd->StaCfg.WpaState = SS_NOTUSE;     
    }
    else
    {
        // Start STA supplicant state machine
        pAd->StaCfg.WpaState = SS_START;
    }    

    return TRUE;
}

/* 
    ==========================================================================
    Description:
        Set Power Saving mode
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_PSMode_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PSTRING          arg)
{
    if (pAdapter->StaCfg.BssType == BSS_INFRA)
    {
        if ((strcmp(arg, "Max_PSP") == 0) || 
			(strcmp(arg, "max_psp") == 0) ||
			(strcmp(arg, "MAX_PSP") == 0))
        {
            // do NOT turn on PSM bit here, wait until MlmeCheckPsmChange()
            // to exclude certain situations.
            if (pAdapter->StaCfg.bWindowsACCAMEnable == FALSE)
                pAdapter->StaCfg.WindowsPowerMode = Ndis802_11PowerModeMAX_PSP;
            pAdapter->StaCfg.WindowsBatteryPowerMode = Ndis802_11PowerModeMAX_PSP;
            OPSTATUS_SET_FLAG(pAdapter, fOP_STATUS_RECEIVE_DTIM);
            pAdapter->StaCfg.DefaultListenCount = 5;
        
        }
        else if ((strcmp(arg, "Fast_PSP") == 0) || 
				 (strcmp(arg, "fast_psp") == 0) ||
                 (strcmp(arg, "FAST_PSP") == 0))
        {
            // do NOT turn on PSM bit here, wait until MlmeCheckPsmChange()
            // to exclude certain situations.
            OPSTATUS_SET_FLAG(pAdapter, fOP_STATUS_RECEIVE_DTIM);
            if (pAdapter->StaCfg.bWindowsACCAMEnable == FALSE)
                pAdapter->StaCfg.WindowsPowerMode = Ndis802_11PowerModeFast_PSP;
            pAdapter->StaCfg.WindowsBatteryPowerMode = Ndis802_11PowerModeFast_PSP;
            pAdapter->StaCfg.DefaultListenCount = 3;
        }
        else if ((strcmp(arg, "Legacy_PSP") == 0) || 
                 (strcmp(arg, "legacy_psp") == 0) || 
                 (strcmp(arg, "LEGACY_PSP") == 0))
        {
            // do NOT turn on PSM bit here, wait until MlmeCheckPsmChange()
            // to exclude certain situations.
            OPSTATUS_SET_FLAG(pAdapter, fOP_STATUS_RECEIVE_DTIM);
            if (pAdapter->StaCfg.bWindowsACCAMEnable == FALSE)
                pAdapter->StaCfg.WindowsPowerMode = Ndis802_11PowerModeLegacy_PSP;
            pAdapter->StaCfg.WindowsBatteryPowerMode = Ndis802_11PowerModeLegacy_PSP;
            pAdapter->StaCfg.DefaultListenCount = 3;
        }
        else
        {
            //Default Ndis802_11PowerModeCAM
            // clear PSM bit immediately
            RTMP_SET_PSM_BIT(pAdapter, PWR_ACTIVE);
            OPSTATUS_SET_FLAG(pAdapter, fOP_STATUS_RECEIVE_DTIM);
            if (pAdapter->StaCfg.bWindowsACCAMEnable == FALSE)
                pAdapter->StaCfg.WindowsPowerMode = Ndis802_11PowerModeCAM;
            pAdapter->StaCfg.WindowsBatteryPowerMode = Ndis802_11PowerModeCAM;
        }

        DBGPRINT(RT_DEBUG_TRACE, ("Set_PSMode_Proc::(PSMode=%ld)\n", pAdapter->StaCfg.WindowsPowerMode));
    }
    else
        return FALSE;

        
    return TRUE;
}

#ifdef WPA_SUPPLICANT_SUPPORT
/* 
    ==========================================================================
    Description:
        Set WpaSupport flag.
    Value:
        0: Driver ignore wpa_supplicant.
        1: wpa_supplicant initiates scanning and AP selection.
        2: driver takes care of scanning, AP selection, and IEEE 802.11 association parameters.
    Return:
        TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT Set_Wpa_Support(
    IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{

    if ( simple_strtol(arg, 0, 10) == 0)
        pAd->StaCfg.WpaSupplicantUP = WPA_SUPPLICANT_DISABLE;
    else if ( simple_strtol(arg, 0, 10) == 1)
        pAd->StaCfg.WpaSupplicantUP = WPA_SUPPLICANT_ENABLE;
    else if ( simple_strtol(arg, 0, 10) == 2)
        pAd->StaCfg.WpaSupplicantUP = WPA_SUPPLICANT_ENABLE_WITH_WEB_UI;
    else
        pAd->StaCfg.WpaSupplicantUP = WPA_SUPPLICANT_DISABLE;

    DBGPRINT(RT_DEBUG_TRACE, ("Set_Wpa_Support::(WpaSupplicantUP=%d)\n", pAd->StaCfg.WpaSupplicantUP));
    
    return TRUE;    
}
#endif // WPA_SUPPLICANT_SUPPORT //




INT Set_TGnWifiTest_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING          arg)
{
    if (simple_strtol(arg, 0, 10) == 0)
        pAd->StaCfg.bTGnWifiTest = FALSE;
    else
        pAd->StaCfg.bTGnWifiTest = TRUE;

    DBGPRINT(RT_DEBUG_TRACE, ("IF Set_TGnWifiTest_Proc::(bTGnWifiTest=%d)\n", pAd->StaCfg.bTGnWifiTest));
	return TRUE;
}

#ifdef EXT_BUILD_CHANNEL_LIST
INT Set_Ieee80211dClientMode_Proc(
    IN  PRTMP_ADAPTER   pAdapter, 
    IN  PSTRING          arg)
{
    if (simple_strtol(arg, 0, 10) == 0)
        pAdapter->StaCfg.IEEE80211dClientMode = Rt802_11_D_None;
    else if (simple_strtol(arg, 0, 10) == 1)
        pAdapter->StaCfg.IEEE80211dClientMode = Rt802_11_D_Flexible;
    else if (simple_strtol(arg, 0, 10) == 2)
        pAdapter->StaCfg.IEEE80211dClientMode = Rt802_11_D_Strict;
    else
        return FALSE;  

    DBGPRINT(RT_DEBUG_TRACE, ("Set_Ieee802dMode_Proc::(IEEEE0211dMode=%d)\n", pAdapter->StaCfg.IEEE80211dClientMode));
    return TRUE;
}
#endif // EXT_BUILD_CHANNEL_LIST //

#ifdef CARRIER_DETECTION_SUPPORT
INT Set_CarrierDetect_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
    if (simple_strtol(arg, 0, 10) == 0)
        pAd->CommonCfg.CarrierDetect.Enable = FALSE;
    else
        pAd->CommonCfg.CarrierDetect.Enable = TRUE;

    DBGPRINT(RT_DEBUG_TRACE, ("IF Set_CarrierDetect_Proc::(CarrierDetect.Enable=%d)\n", pAd->CommonCfg.CarrierDetect.Enable));
	return TRUE;
}
#endif // CARRIER_DETECTION_SUPPORT //


INT	Show_Adhoc_MacTable_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			extra)
{
	INT i;
	
	sprintf(extra, "\n");

#ifdef DOT11_N_SUPPORT
	sprintf(extra, "%sHT Operating Mode : %d\n", extra, pAd->CommonCfg.AddHTInfo.AddHtInfo2.OperaionMode);
#endif // DOT11_N_SUPPORT //

	sprintf(extra, "%s\n%-19s%-4s%-4s%-7s%-7s%-7s%-10s%-6s%-6s%-6s%-6s\n", extra, 
			"MAC", "AID", "BSS", "RSSI0", "RSSI1", "RSSI2", "PhMd", "BW", "MCS", "SGI", "STBC");
	
	for (i=1; i<MAX_LEN_OF_MAC_TABLE; i++)
	{
		PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[i];
		
		if (strlen(extra) > (IW_PRIV_SIZE_MASK - 30))
		    break;
		if ((IS_ENTRY_CLIENT(pEntry) || IS_ENTRY_APCLI(pEntry)) && (pEntry->Sst == SST_ASSOC))
		{
			sprintf(extra, "%s%02X:%02X:%02X:%02X:%02X:%02X  ", extra,
				pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
				pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]);
			sprintf(extra, "%s%-4d", extra, (int)pEntry->Aid);
			sprintf(extra, "%s%-4d", extra, (int)pEntry->apidx);
			sprintf(extra, "%s%-7d", extra, pEntry->RssiSample.AvgRssi0);
			sprintf(extra, "%s%-7d", extra, pEntry->RssiSample.AvgRssi1);
			sprintf(extra, "%s%-7d", extra, pEntry->RssiSample.AvgRssi2);
			sprintf(extra, "%s%-10s", extra, GetPhyMode(pEntry->HTPhyMode.field.MODE));
			sprintf(extra, "%s%-6s", extra, GetBW(pEntry->HTPhyMode.field.BW));
			sprintf(extra, "%s%-6d", extra, pEntry->HTPhyMode.field.MCS);
			sprintf(extra, "%s%-6d", extra, pEntry->HTPhyMode.field.ShortGI);
			sprintf(extra, "%s%-6d", extra, pEntry->HTPhyMode.field.STBC);
			sprintf(extra, "%s%-10d, %d, %d%%\n", extra, pEntry->DebugFIFOCount, pEntry->DebugTxCount, 
						(pEntry->DebugTxCount) ? ((pEntry->DebugTxCount-pEntry->DebugFIFOCount)*100/pEntry->DebugTxCount) : 0);
			sprintf(extra, "%s\n", extra);
		}
	} 

	return TRUE;
}


INT Set_BeaconLostTime_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	ULONG ltmp = (ULONG)simple_strtol(arg, 0, 10);

	if ((ltmp != 0) && (ltmp <= 60))
		pAd->StaCfg.BeaconLostTime = (ltmp * OS_HZ);

    DBGPRINT(RT_DEBUG_TRACE, ("IF Set_BeaconLostTime_Proc::(BeaconLostTime=%ld)\n", pAd->StaCfg.BeaconLostTime));
	return TRUE;
}

INT Set_AutoRoaming_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
    if (simple_strtol(arg, 0, 10) == 0)
        pAd->StaCfg.bAutoRoaming = FALSE;
    else
        pAd->StaCfg.bAutoRoaming = TRUE;

    DBGPRINT(RT_DEBUG_TRACE, ("IF Set_AutoRoaming_Proc::(bAutoRoaming=%d)\n", pAd->StaCfg.bAutoRoaming));
	return TRUE;
}


/* 
    ==========================================================================
    Description:
        Issue a site survey command to driver
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
        None

    Note:
        Usage: 
               1.) iwpriv ra0 set site_survey
    ==========================================================================
*/
INT Set_SiteSurvey_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg)
{
	NDIS_802_11_SSID Ssid;

	//check if the interface is down
	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
	{
		DBGPRINT(RT_DEBUG_TRACE, ("INFO::Network is down!\n"));
		return -ENETDOWN;   
	}

	if (MONITOR_ON(pAd))
    {
        DBGPRINT(RT_DEBUG_TRACE, ("!!! Driver is in Monitor Mode now !!!\n"));
        return -EINVAL;
    }

	RTMPZeroMemory(&Ssid, sizeof(NDIS_802_11_SSID));	
	Ssid.SsidLength = 0; 
	if ((arg != NULL) &&
		(strlen(arg) <= MAX_LEN_OF_SSID))
    {
        RTMPMoveMemory(Ssid.Ssid, arg, strlen(arg));
        Ssid.SsidLength = strlen(arg);
	}

	pAd->StaCfg.bScanReqIsFromWebUI = TRUE;

	StaSiteSurvey(pAd, &Ssid, SCAN_ACTIVE);
    DBGPRINT(RT_DEBUG_TRACE, ("Set_SiteSurvey_Proc\n"));

    return TRUE;
}

INT Set_ForceTxBurst_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
    if (simple_strtol(arg, 0, 10) == 0)
        pAd->StaCfg.bForceTxBurst = FALSE;
    else
        pAd->StaCfg.bForceTxBurst = TRUE;

    DBGPRINT(RT_DEBUG_TRACE, ("IF Set_ForceTxBurst_Proc::(bForceTxBurst=%d)\n", pAd->StaCfg.bForceTxBurst));
	return TRUE;
}


#ifdef XLINK_SUPPORT
INT Set_XlinkMode_Proc(
    IN  PRTMP_ADAPTER   pAd, 
    IN  PSTRING         arg)
{
	UINT32 Value = 0;

    if (simple_strtol(arg, 0, 10) == 0)
        pAd->StaCfg.PSPXlink = 0;
    else
        pAd->StaCfg.PSPXlink = 1;

	if (pAd->StaCfg.PSPXlink)
		Value = PSPXLINK;
	else
		Value = STANORMAL;
	RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, Value);
	Value = 0;
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);
	Value &= (~0x80);
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Value);

    DBGPRINT(RT_DEBUG_TRACE, ("IF Set_XlinkMode_Proc::(PSPXlink=%d)\n", pAd->StaCfg.PSPXlink));
	return TRUE;
}
#endif // XLINK_SUPPORT //


VOID RTMPAddKey(
	IN	PRTMP_ADAPTER	    pAd, 
	IN	PNDIS_802_11_KEY    pKey)
{
	ULONG				KeyIdx;
	MAC_TABLE_ENTRY  	*pEntry;
		
    DBGPRINT(RT_DEBUG_TRACE, ("RTMPAddKey ------>\n"));

	if (pAd->StaCfg.AuthMode >= Ndis802_11AuthModeWPA)
	{
		if (pKey->KeyIndex & 0x80000000)
		{
		    if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPANone)
            {
                NdisZeroMemory(pAd->StaCfg.PMK, 32);
                NdisMoveMemory(pAd->StaCfg.PMK, pKey->KeyMaterial, pKey->KeyLength);
                goto end;
            }
		    // Update PTK
		    NdisZeroMemory(&pAd->SharedKey[BSS0][0], sizeof(CIPHER_KEY));  
            pAd->SharedKey[BSS0][0].KeyLen = LEN_TK;
            NdisMoveMemory(pAd->SharedKey[BSS0][0].Key, pKey->KeyMaterial, LEN_TK);
#ifdef WPA_SUPPLICANT_SUPPORT            
            if (pAd->StaCfg.PairCipher == Ndis802_11Encryption2Enabled)
            {
                NdisMoveMemory(pAd->SharedKey[BSS0][0].RxMic, pKey->KeyMaterial + LEN_TK, LEN_TKIP_MIC);            
                NdisMoveMemory(pAd->SharedKey[BSS0][0].TxMic, pKey->KeyMaterial + LEN_TK + LEN_TKIP_MIC, LEN_TKIP_MIC);
            }
            else
#endif // WPA_SUPPLICANT_SUPPORT //
            {
            	NdisMoveMemory(pAd->SharedKey[BSS0][0].TxMic, pKey->KeyMaterial + LEN_TK, LEN_TKIP_MIC);            
                NdisMoveMemory(pAd->SharedKey[BSS0][0].RxMic, pKey->KeyMaterial + LEN_TK + LEN_TKIP_MIC, LEN_TKIP_MIC);
            }

            // Decide its ChiperAlg
        	if (pAd->StaCfg.PairCipher == Ndis802_11Encryption2Enabled)
        		pAd->SharedKey[BSS0][0].CipherAlg = CIPHER_TKIP;
        	else if (pAd->StaCfg.PairCipher == Ndis802_11Encryption3Enabled)
        		pAd->SharedKey[BSS0][0].CipherAlg = CIPHER_AES;
        	else
        		pAd->SharedKey[BSS0][0].CipherAlg = CIPHER_NONE; 

            // Update these related information to MAC_TABLE_ENTRY
        	pEntry = &pAd->MacTab.Content[BSSID_WCID];
            NdisMoveMemory(pEntry->PairwiseKey.Key, pAd->SharedKey[BSS0][0].Key, LEN_TK);            
        	NdisMoveMemory(pEntry->PairwiseKey.RxMic, pAd->SharedKey[BSS0][0].RxMic, LEN_TKIP_MIC);
        	NdisMoveMemory(pEntry->PairwiseKey.TxMic, pAd->SharedKey[BSS0][0].TxMic, LEN_TKIP_MIC);
        	pEntry->PairwiseKey.CipherAlg = pAd->SharedKey[BSS0][0].CipherAlg;

        	// Update pairwise key information to ASIC Shared Key Table	   
        	AsicAddSharedKeyEntry(pAd, 
        						  BSS0, 
        						  0, 
        						  &pAd->SharedKey[BSS0][0]);

        	// Update ASIC WCID attribute table and IVEIV table
        	RTMPSetWcidSecurityInfo(pAd, 
        							  BSS0, 
        							  0, 
        							  pAd->SharedKey[BSS0][0].CipherAlg, 
        							  BSSID_WCID,
        							  SHAREDKEYTABLE);

            if (pAd->StaCfg.AuthMode >= Ndis802_11AuthModeWPA2)
            {
                // set 802.1x port control
	            //pAd->StaCfg.PortSecured = WPA_802_1X_PORT_SECURED;
				STA_PORT_SECURED(pAd);
                
                // Indicate Connected for GUI
                pAd->IndicateMediaState = NdisMediaStateConnected;
            }
		}
        else
        {
            // Update GTK            
            pAd->StaCfg.DefaultKeyId = (pKey->KeyIndex & 0xFF);
            NdisZeroMemory(&pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId], sizeof(CIPHER_KEY));  
            pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId].KeyLen = LEN_TK;
            NdisMoveMemory(pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId].Key, pKey->KeyMaterial, LEN_TK);
#ifdef WPA_SUPPLICANT_SUPPORT            
            if (pAd->StaCfg.GroupCipher == Ndis802_11Encryption2Enabled)
            {
                NdisMoveMemory(pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId].RxMic, pKey->KeyMaterial + LEN_TK, LEN_TKIP_MIC);            
                NdisMoveMemory(pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId].TxMic, pKey->KeyMaterial + LEN_TK + LEN_TKIP_MIC, LEN_TKIP_MIC);        	
            }
            else
#endif // WPA_SUPPLICANT_SUPPORT //                
            {
            	NdisMoveMemory(pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId].TxMic, pKey->KeyMaterial + LEN_TK, LEN_TKIP_MIC);            
                NdisMoveMemory(pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId].RxMic, pKey->KeyMaterial + LEN_TK + LEN_TKIP_MIC, LEN_TKIP_MIC);        	
            }

            // Update Shared Key CipherAlg
    		pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId].CipherAlg = CIPHER_NONE;
    		if (pAd->StaCfg.GroupCipher == Ndis802_11Encryption2Enabled)
    			pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId].CipherAlg = CIPHER_TKIP;
    		else if (pAd->StaCfg.GroupCipher == Ndis802_11Encryption3Enabled)
    			pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId].CipherAlg = CIPHER_AES;

            // Update group key information to ASIC Shared Key Table	   
        	AsicAddSharedKeyEntry(pAd, 
        						  BSS0, 
        						  pAd->StaCfg.DefaultKeyId, 
        						  &pAd->SharedKey[BSS0][pAd->StaCfg.DefaultKeyId]);

			/* STA doesn't need to set WCID attribute for group key */

            // set 802.1x port control
	        //pAd->StaCfg.PortSecured = WPA_802_1X_PORT_SECURED;
			STA_PORT_SECURED(pAd);

            // Indicate Connected for GUI
            pAd->IndicateMediaState = NdisMediaStateConnected;
        }
	}
	else	// dynamic WEP from wpa_supplicant
	{
		UCHAR	CipherAlg;
    	PUCHAR	Key;

		if(pKey->KeyLength == 32)
			goto end;
		
		KeyIdx = pKey->KeyIndex & 0x0fffffff;

		if (KeyIdx < 4)
		{
			// it is a default shared key, for Pairwise key setting
			if (pKey->KeyIndex & 0x80000000)
			{
				pEntry = MacTableLookup(pAd, pKey->BSSID);

				if (pEntry)
				{
					DBGPRINT(RT_DEBUG_TRACE, ("RTMPAddKey: Set Pair-wise Key\n"));
		
					// set key material and key length
 					pEntry->PairwiseKey.KeyLen = (UCHAR)pKey->KeyLength;
					NdisMoveMemory(pEntry->PairwiseKey.Key, &pKey->KeyMaterial, pKey->KeyLength);
					
					// set Cipher type
					if (pKey->KeyLength == 5)
						pEntry->PairwiseKey.CipherAlg = CIPHER_WEP64;
					else
						pEntry->PairwiseKey.CipherAlg = CIPHER_WEP128;
						
					// Add Pair-wise key to Asic
					AsicAddPairwiseKeyEntry(
						pAd, 
						(UCHAR)pEntry->Aid,
                		&pEntry->PairwiseKey);

					// update WCID attribute table and IVEIV table for this entry
					RTMPSetWcidSecurityInfo(pAd, 
											BSS0, 
											KeyIdx, 
											pEntry->PairwiseKey.CipherAlg, 
											pEntry->Aid, 
											PAIRWISEKEYTABLE);
				}	
			}
			else	
            {
				// Default key for tx (shared key)
				pAd->StaCfg.DefaultKeyId = (UCHAR) KeyIdx;
                     
				// set key material and key length
				pAd->SharedKey[BSS0][KeyIdx].KeyLen = (UCHAR) pKey->KeyLength;
				NdisMoveMemory(pAd->SharedKey[BSS0][KeyIdx].Key, &pKey->KeyMaterial, pKey->KeyLength);
				
				// Set Ciper type
				if (pKey->KeyLength == 5)
					pAd->SharedKey[BSS0][KeyIdx].CipherAlg = CIPHER_WEP64;
				else
					pAd->SharedKey[BSS0][KeyIdx].CipherAlg = CIPHER_WEP128;
			
    			CipherAlg = pAd->SharedKey[BSS0][KeyIdx].CipherAlg;
    			Key = pAd->SharedKey[BSS0][KeyIdx].Key;

				// Set Group key material to Asic
    			AsicAddSharedKeyEntry(pAd, BSS0, KeyIdx, &pAd->SharedKey[BSS0][KeyIdx]);
		
				/* STA doesn't need to set WCID attribute for group key */
			}
		}
	}
end:
	return;
}



/*
    ==========================================================================
    Description:
        Site survey entry point

    NOTE:
    ==========================================================================
*/
VOID StaSiteSurvey(
	IN	PRTMP_ADAPTER  		pAd,
	IN	PNDIS_802_11_SSID	pSsid,
	IN	UCHAR				ScanType)
{
	if (INFRA_ON(pAd))
	{
		pAd->StaCfg.bImprovedScan = TRUE;
		pAd->StaCfg.ScanChannelCnt = 0;	// reset channel counter to 0
	}

			
	if (pAd->Mlme.CntlMachine.CurrState != CNTL_IDLE)
	{
		RTMP_MLME_RESET_STATE_MACHINE(pAd);
		DBGPRINT(RT_DEBUG_TRACE, ("!!! MLME busy, reset MLME state machine !!!\n"));
	}

	NdisGetSystemUpTime(&pAd->StaCfg.LastScanTime);
	if (pSsid)
		MlmeEnqueue(pAd, 
			MLME_CNTL_STATE_MACHINE, 
			OID_802_11_BSSID_LIST_SCAN, 
			pSsid->SsidLength,
			pSsid->Ssid, 
			0);
	else
		MlmeEnqueue(pAd, 
			MLME_CNTL_STATE_MACHINE, 
			OID_802_11_BSSID_LIST_SCAN, 
			0,
			"",
			0);
	
	RTMP_MLME_HANDLER(pAd);
}

/* End of sta_cfg.c */


