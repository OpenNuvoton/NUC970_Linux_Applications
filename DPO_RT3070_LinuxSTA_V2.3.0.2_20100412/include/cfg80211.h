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
 *************************************************************************/

/****************************************************************************

	Abstract:

	All MAC80211/CFG80211 Related Structure & Definition.

***************************************************************************/

#ifdef RT_CFG80211_SUPPORT

#include <linux/ieee80211.h>
#define RT_CFG80211_CRDA_REG_RULE_APPLY(__pAd)							\
		if (__pAd->pCfg80211_CB != NULL)								\
			CFG80211_RegRuleApply(__pAd, NULL, __pAd->Cfg80211_Alpha2);

#define RT_CFG80211_REINIT(__pAd)										\
		if (__pAd->pCfg80211_CB != NULL)								\
			CFG80211_SupBandReInit(__pAd);		

#define RT_CFG80211_SCANNING_INFORM					CFG80211_Scaning


/*
========================================================================
Routine Description:
	Register MAC80211 Module.

Arguments:
	pAd				- WLAN control block pointer
	pDev			- Generic device interface
	pNetDev			- Network device

Return Value:
	NONE

Note:
	pDev != pNetDev
	#define SET_NETDEV_DEV(net, pdev)	((net)->dev.parent = (pdev))

	Can not use pNetDev to replace pDev; Or kernel panic.
========================================================================
*/
VOID CFG80211_Register(
	IN PRTMP_ADAPTER 		pAd,
	IN struct device		*pDev,
	IN struct net_device	*pNetDev);

/*
========================================================================
Routine Description:
	UnRegister MAC80211 Module.

Arguments:
	pAd				- WLAN control block pointer
	pNetDev			- Network device

Return Value:
	NONE

Note:
========================================================================
*/
VOID CFG80211_UnRegister(
	IN PRTMP_ADAPTER 		pAd,
	IN struct net_device	*pNetDev);

/*
========================================================================
Routine Description:
	Parse and handle country region in beacon from associated AP.

Arguments:
	pAd				- WLAN control block pointer
	pVIE			- Beacon elements
	LenVIE			- Total length of Beacon elements

Return Value:
	NONE

Note:
========================================================================
*/
VOID CFG80211_BeaconCountryRegionParse(
	IN PRTMP_ADAPTER			pAd,
	IN NDIS_802_11_VARIABLE_IEs	*pVIE,
	IN UINT16					LenVIE);

/*
========================================================================
Routine Description:
	Hint to the wireless core a regulatory domain from driver.

Arguments:
	pAd				- WLAN control block pointer
	pCountryIe		- pointer to the country IE
	CountryIeLen	- length of the country IE

Return Value:
	NONE

Note:
	Must call the function in kernel thread.
========================================================================
*/
VOID CFG80211_RegHint(
	IN PRTMP_ADAPTER	pAd,
	IN UCHAR			*pCountryIe,
	IN ULONG			CountryIeLen);

/*
========================================================================
Routine Description:
	Hint to the wireless core a regulatory domain from country element.

Arguments:
	pAd				- WLAN control block pointer
	pCountryIe		- pointer to the country IE
	CountryIeLen	- length of the country IE

Return Value:
	NONE

Note:
========================================================================
*/
VOID CFG80211_RegHint11D(
	IN PRTMP_ADAPTER	pAd,
	IN UCHAR			*pCountryIe,
	IN ULONG			CountryIeLen);

/*
========================================================================
Routine Description:
	Apply new regulatory rule.

Arguments:
	pAd				- WLAN control block pointer
	pWiphy			- Wireless hardware description
	pAlpha2			- Regulation domain (2B)

Return Value:
	NONE

Note:
	Can only be called when interface is up.
========================================================================
*/
VOID CFG80211_RegRuleApply(
	IN PRTMP_ADAPTER				pAd,
	IN struct wiphy					*pWiphy,
	IN UCHAR						*pAlpha2);

#if defined(AP_SCAN_SUPPORT) || defined(CONFIG_STA_SUPPORT)
/*
========================================================================
Routine Description:
	Inform us that a scan is got.

Arguments:
	pAd				- WLAN control block pointer

Return Value:
	NONE

Note:
	Call RT_CFG80211_SCANNING_INFORM, not CFG80211_Scaning
========================================================================
*/
VOID CFG80211_Scaning(
	IN PRTMP_ADAPTER				pAd,
	IN UINT32						BssIdx,
	IN UINT32						ChanId,
	IN UCHAR						*pFrame,
	IN UINT32						FrameLen,
	IN INT32						RSSI,
	IN INT32						MemFlag);

/*
========================================================================
Routine Description:
	Inform us that scan ends.

Arguments:
	pAd				- WLAN control block pointer
	FlgIsAborted	- 1: scan is aborted

Return Value:
	NONE

Note:
========================================================================
*/
VOID CFG80211_ScanEnd(
	IN PRTMP_ADAPTER				pAd,
	IN BOOLEAN						FlgIsAborted);
#endif // AP_SCAN_SUPPORT || CONFIG_STA_SUPPORT //

/*
========================================================================
Routine Description:
	Re-Initialize wireless channel/PHY in 2.4GHZ and 5GHZ.

Arguments:
	pAd				- WLAN control block pointer

Return Value:
	TRUE			- re-init successfully
	FALSE			- re-init fail

Note:
	CFG80211_SupBandInit() is called in xx_probe().
	But we do not have complete chip information in xx_probe() so we
	need to re-init bands in xx_open().
========================================================================
*/
BOOLEAN CFG80211_SupBandReInit(
	IN PRTMP_ADAPTER 				pAd);

/*
========================================================================
Routine Description:
	Inform CFG80211 about association status.

Arguments:
	pAd				- WLAN control block pointer
	pBSSID			- the BSSID of the AP
	pReqIe			- the element list in the association request frame
	ReqIeLen		- the request element length
	pRspIe			- the element list in the association response frame
	RspIeLen		- the response element length
	FlgIsSuccess	- 1: success; otherwise: fail

Return Value:
	None

Note:
========================================================================
*/
void CFG80211_ConnectResultInform(
	IN PRTMP_ADAPTER 				pAd,
	IN UCHAR						*pBSSID,
	IN UCHAR						*pReqIe,
	IN UINT32						ReqIeLen,
	IN UCHAR						*pRspIe,
	IN UINT32						RspIeLen,
	IN UCHAR						FlgIsSuccess);

#endif // RT_CFG80211_SUPPORT //

/* End of cfg80211.h */
