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
	rtmp_phy.h

	Abstract:
	Ralink Wireless Chip PHY(BBP/RF) related definition & structures

	Revision History:
	Who			When		  What
	--------	----------	  ----------------------------------------------
*/

#ifndef __RTMP_PHY_H__
#define __RTMP_PHY_H__


/*
	RF sections
*/
#define RF_R00			0
#define RF_R01			1
#define RF_R02			2
#define RF_R03			3
#define RF_R04			4
#define RF_R05			5
#define RF_R06			6
#define RF_R07			7
#define RF_R08			8
#define RF_R09			9
#define RF_R10			10
#define RF_R11			11
#define RF_R12			12
#define RF_R13			13
#define RF_R14			14
#define RF_R15			15
#define RF_R16			16
#define RF_R17			17
#define RF_R18			18
#define RF_R19			19
#define RF_R20			20
#define RF_R21			21
#define RF_R22			22
#define RF_R23			23
#define RF_R24			24
#define RF_R25			25
#define RF_R26			26
#define RF_R27			27
#define RF_R28			28
#define RF_R29			29
#define RF_R30			30
#define RF_R31			31
#define	RF_R32					    32
#define	RF_R33					    33
#define	RF_R34					    34
#define	RF_R35					    35
#define	RF_R36					    36
#define	RF_R37					    37
#define	RF_R38					    38
#define	RF_R39					    39
#define	RF_R40					    40
#define	RF_R41					    41
#define	RF_R42					    42
#define	RF_R43					    43
#define	RF_R44					    44
#define	RF_R45					    45
#define	RF_R46					    46
#define	RF_R47					    47
#define	RF_R48					    48
#define	RF_R49					    49
#define	RF_R50					    50
#define	RF_R51					    51
#define	RF_R52					    52
#define	RF_R53					    53
#define	RF_R54					    54
#define	RF_R55					    55
#define	RF_R56					    56
#define	RF_R57					    57
#define	RF_R58					    58
#define	RF_R59					    59
#define	RF_R60					    60
#define	RF_R61					    61
#define	RF_R62					    62
#define	RF_R63					    63


// value domain of pAd->RfIcType
#define RFIC_2820                   1       // 2.4G 2T3R
#define RFIC_2850                   2       // 2.4G/5G 2T3R
#define RFIC_2720                   3       // 2.4G 1T2R
#define RFIC_2750                   4       // 2.4G/5G 1T2R
#define RFIC_3020                   5       // 2.4G 1T1R
#define RFIC_2020                   6       // 2.4G B/G
#define RFIC_3021                   7       // 2.4G 1T2R
#define RFIC_3022                   8       // 2.4G 2T2R
#define RFIC_3052                   9       // 2.4G/5G 2T2R
#define RFIC_3320					11	   // 2.4G 1T1R
#define RFIC_3053					13      // 2.4G/5G 3T3R


/*
	BBP sections
*/
#define BBP_R0			0  // version
#define BBP_R1			1  // TSSI
#define BBP_R2			2  // TX configure
#define BBP_R3			3
#define BBP_R4			4
#define BBP_R5			5
#define BBP_R6			6
#define BBP_R14			14 // RX configure
#define BBP_R16			16
#define BBP_R17			17 // RX sensibility
#define BBP_R18			18
#define BBP_R21			21
#define BBP_R22			22
#define BBP_R24			24
#define BBP_R25			25
#define BBP_R26			26
#define BBP_R27			27
#define BBP_R31			31
#define BBP_R49			49 //TSSI
#define BBP_R50			50
#define BBP_R51			51
#define BBP_R52			52
#define BBP_R55			55
#define BBP_R62			62 // Rx SQ0 Threshold HIGH
#define BBP_R63			63
#define BBP_R64			64
#define BBP_R65			65
#define BBP_R66			66
#define BBP_R67			67
#define BBP_R68			68
#define BBP_R69			69
#define BBP_R70			70 // Rx AGC SQ CCK Xcorr threshold
#define BBP_R73			73
#define BBP_R75			75
#define BBP_R77			77
#define BBP_R78			78
#define BBP_R79			79
#define BBP_R80			80
#define BBP_R81			81
#define BBP_R82			82
#define BBP_R83			83
#define BBP_R84			84
#define BBP_R86			86
#define BBP_R91			91
#define BBP_R92			92
#define BBP_R94			94 // Tx Gain Control
#define BBP_R103		103
#define BBP_R105		105
#define BBP_R106		106
#define BBP_R109		109
#define BBP_R110		110
#define BBP_R113		113
#define BBP_R114		114
#define BBP_R115		115
#define BBP_R116		116
#define BBP_R117		117
#define BBP_R118		118
#define BBP_R119		119
#define BBP_R120		120
#define BBP_R121		121
#define BBP_R122		122
#define BBP_R123		123
#ifdef RT30xx
#define BBP_R138		138 // add by johnli, RF power sequence setup, ADC dynamic on/off control
#endif // RT30xx //
#define BBP_R142		142
#define BBP_R143		143


#define BBPR94_DEFAULT	0x06 // Add 1 value will gain 1db


//
// BBP R49 TSSI (Transmit Signal Strength Indicator)
//
#ifdef RT_BIG_ENDIAN
typedef union _BBP_R49_STRUC {
	struct
	{
		UCHAR	adc5_in_sel:1; // 0: TSSI (from the external components, old version), 1: PSI (internal components, new version - RT3390)
		UCHAR	bypassTSSIAverage:1; // 0: the average TSSI (the average of the 16 samples), 1: the current TSSI
		UCHAR	Reserved:1; // Reserved field
		UCHAR	TSSI:5; // TSSI value
	} field;

	UCHAR		byte;
} BBP_R49_STRUC, *PBBP_R49_STRUC;
#else
typedef union _BBP_R49_STRUC {
	struct
	{
		UCHAR	TSSI:5; // TSSI value
		UCHAR	Reserved:1; // Reserved field
		UCHAR	bypassTSSIAverage:1; // 0: the average TSSI (the average of the 16 samples), 1: the current TSSI
		UCHAR	adc5_in_sel:1; // 0: TSSI (from the external components, old version), 1: PSI (internal components, new version - RT3390)
	} field;
	
	UCHAR		byte;
} BBP_R49_STRUC, *PBBP_R49_STRUC;
#endif

//
// BBP R105 (FEQ control, MLD control and SIG remodulation)
//
#ifdef RT_BIG_ENDIAN
typedef union _BBP_R105_STRUC {
	struct
	{
		UCHAR	Reserve1:4; // Reserved field
		UCHAR	EnableSIGRemodulation:1; // Enable the channel estimation updates based on remodulation of L-SIG and HT-SIG symbols.
		UCHAR	MLDFor2Stream:1; // Apply Maximum Likelihood Detection (MLD) for 2 stream case (reserved field if single RX)
		UCHAR	IndependentFeedForwardCompensation:1; // Apply independent feed-forward compensation for independent stream.
		UCHAR	DetectSIGOnPrimaryChannelOnly:1; // Under 40 MHz band, detect SIG on primary channel only.
	} field;

	UCHAR		byte;
} BBP_R105_STRUC, *PBBP_R105_STRUC;
#else
typedef union _BBP_R105_STRUC {
	struct
	{
		UCHAR	DetectSIGOnPrimaryChannelOnly:1; // Under 40 MHz band, detect SIG on primary channel only.
		UCHAR	IndependentFeedForwardCompensation:1; // Apply independent feed-forward compensation for independent stream.
		UCHAR	MLDFor2Stream:1; // Apply Maximum Likelihood Detection (MLD) for 2 stream case (reserved field if single RX)
		UCHAR	EnableSIGRemodulation:1; // Enable the channel estimation updates based on remodulation of L-SIG and HT-SIG symbols.
		UCHAR	Reserve1:4; // Reserved field
	} field;
	
	UCHAR		byte;
} BBP_R105_STRUC, *PBBP_R105_STRUC;
#endif

//
// BBP R106 (GI remover)
//
#ifdef RT_BIG_ENDIAN
typedef union _BBP_R106_STRUC {
	struct
	{
		UCHAR	EnableLowPowerFSD:1; // enable/disable the low power FSD
		UCHAR	ShortGI_Offset40:4; // Delay GI remover when the short GI is detected in 40MHz band (40M sampling rate)
		UCHAR	ShortGI_Offset20:3; // Delay GI remover when the short GI is detected in 20MHz band (20M sampling rate)
	} field;

	UCHAR		byte;
} BBP_R106_STRUC, *PBBP_R106_STRUC;
#else
typedef union _BBP_R106_STRUC {
	struct
	{
		UCHAR	ShortGI_Offset20:3; // Delay GI remover when the short GI is detected in 20MHz band (20M sampling rate)
		UCHAR	ShortGI_Offset40:4; // Delay GI remover when the short GI is detected in 40MHz band (40M sampling rate)
		UCHAR	EnableLowPowerFSD:1; // enable/disable the low power FSD
	} field;
	
	UCHAR		byte;
} BBP_R106_STRUC, *PBBP_R106_STRUC;
#endif

//
// BBP R109 (Tx power control in 0.1dB step)
//
#ifdef RT_BIG_ENDIAN
typedef union _BBP_R109_STRUC {
	struct
	{
		UCHAR	Tx1PowerCtrl:4; // Tx1 power control in 0.1dB step (valid: 0~10)
		UCHAR	Tx0PowerCtrl:4; // Tx0 power control in 0.1dB step (valid: 0~10)
	} field;

	UCHAR		byte;
} BBP_R109_STRUC, *PBBP_R109_STRUC;
#else
typedef union _BBP_R109_STRUC {
	struct
	{
		UCHAR	Tx0PowerCtrl:4; // Tx0 power control in 0.1dB step (valid: 0~10)
		UCHAR	Tx1PowerCtrl:4; // Tx0 power control in 0.1dB step (valid: 0~10)
	} field;
	
	UCHAR		byte;
} BBP_R109_STRUC, *PBBP_R109_STRUC;
#endif

//
// BBP R110 (Tx power control in 0.1dB step)
//
#ifdef RT_BIG_ENDIAN
typedef union _BBP_R110_STRUC {
	struct
	{
		UCHAR	Tx2PowerCtrl:4; // Tx2 power control in 0.1dB step (valid: 0~10)
		UCHAR	AllTxPowerCtrl:4; // All transmitters' fine power control in 0.1dB (valid: 0~10)
	} field;

	UCHAR		byte;
} BBP_R110_STRUC, *PBBP_R110_STRUC;
#else
typedef union _BBP_R110_STRUC {
	struct
	{
		UCHAR	AllTxPowerCtrl:4; // All transmitters' fine power control in 0.1dB (valid: 0~10)
		UCHAR	Tx2PowerCtrl:4; // Tx2 power control in 0.1dB step (valid: 0~10)
	} field;
	
	UCHAR		byte;
} BBP_R110_STRUC, *PBBP_R110_STRUC;
#endif


#ifdef MERGE_ARCH_TEAM
	#define MAX_BBP_ID	200
	#define MAX_BBP_MSG_SIZE	4096
#else
#ifdef RT30xx
	// edit by johnli, RF power sequence setup, add BBP R138 for ADC dynamic on/off control
	#define MAX_BBP_ID	138
#elif defined(RT2883)
	#define MAX_BBP_ID	180
#else
	#define MAX_BBP_ID	136
#endif // RT30xx //
	#define MAX_BBP_MSG_SIZE	2048
#endif // MERGE_ARCH_TEAM //


//
// BBP & RF are using indirect access. Before write any value into it.
// We have to make sure there is no outstanding command pending via checking busy bit.
//
#define MAX_BUSY_COUNT  100         // Number of retry before failing access BBP & RF indirect register

//#define PHY_TR_SWITCH_TIME          5  // usec

//#define BBP_R17_LOW_SENSIBILITY     0x50
//#define BBP_R17_MID_SENSIBILITY     0x41
//#define BBP_R17_DYNAMIC_UP_BOUND    0x40

#define RSSI_FOR_VERY_LOW_SENSIBILITY   -35
#define RSSI_FOR_LOW_SENSIBILITY		-58
#define RSSI_FOR_MID_LOW_SENSIBILITY	-80
#define RSSI_FOR_MID_SENSIBILITY		-90

/*****************************************************************************
	RF register Read/Write marco definition
 *****************************************************************************/

#ifdef RTMP_MAC_USB
#define RTMP_RF_IO_WRITE32(_A, _V)                 RTUSBWriteRFRegister(_A, _V)
#endif // RTMP_MAC_USB //


#ifdef RT30xx
#define RTMP_RF_IO_READ8_BY_REG_ID(_A, _I, _pV)    RT30xxReadRFRegister(_A, _I, _pV)
#define RTMP_RF_IO_WRITE8_BY_REG_ID(_A, _I, _V)    RT30xxWriteRFRegister(_A, _I, _V)
#endif // RT30xx //


/*****************************************************************************
	BBP register Read/Write marco definitions.
	we read/write the bbp value by register's ID. 
	Generate PER to test BA
 *****************************************************************************/

#ifdef RTMP_MAC_USB
#define RTMP_BBP_IO_READ8_BY_REG_ID(_A, _I, _pV)   RTUSBReadBBPRegister(_A, _I, _pV)
#define RTMP_BBP_IO_WRITE8_BY_REG_ID(_A, _I, _V)   RTUSBWriteBBPRegister(_A, _I, _V)

#define BBP_IO_WRITE8_BY_REG_ID(_A, _I, _V)			RTUSBWriteBBPRegister(_A, _I, _V)
#define BBP_IO_READ8_BY_REG_ID(_A, _I, _pV)   		RTUSBReadBBPRegister(_A, _I, _pV)
#endif // RTMP_MAC_USB //


#ifdef RT30xx

#define RTMP_ASIC_MMPS_DISABLE(_pAd)							\
	do{															\
		UCHAR _bbpData = 0;											\
		UINT32 _macData;											\
		/* disable MMPS BBP control register */						\
		RTMP_BBP_IO_READ8_BY_REG_ID(_pAd, BBP_R3, &_bbpData);	\
		_bbpData &= ~(0x04);	/*bit 2*/								\
		RTMP_BBP_IO_WRITE8_BY_REG_ID(_pAd, BBP_R3, _bbpData);	\
																\
		/* disable MMPS MAC control register */						\
		RTMP_IO_READ32(_pAd, 0x1210, &_macData);				\
		_macData &= ~(0x09);	/*bit 0, 3*/							\
		RTMP_IO_WRITE32(_pAd, 0x1210, _macData);				\
	}while(0)


#define RTMP_ASIC_MMPS_ENABLE(_pAd)							\
	do{															\
		UCHAR _bbpData = 0;											\
		UINT32 _macData;											\
		/* enable MMPS BBP control register */						\
		RTMP_BBP_IO_READ8_BY_REG_ID(_pAd, BBP_R3, &_bbpData);	\
		_bbpData |= (0x04);	/*bit 2*/								\
		RTMP_BBP_IO_WRITE8_BY_REG_ID(_pAd, BBP_R3, _bbpData);	\
																\
		/* enable MMPS MAC control register */						\
		RTMP_IO_READ32(_pAd, 0x1210, &_macData);				\
		_macData |= (0x09);	/*bit 0, 3*/							\
		RTMP_IO_WRITE32(_pAd, 0x1210, _macData);				\
	}while(0)
				
#endif // RT30xx //

#endif // __RTMP_PHY_H__ //

