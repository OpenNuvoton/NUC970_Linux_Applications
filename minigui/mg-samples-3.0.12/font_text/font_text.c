/*
 ** $Id: font_text.c 782 2010-01-31 08:19:48Z wanzheng $
 **
 ** The demo of font.
 **
 ** Copyright (C) 2001 ~ 2002 Wei Yongming.
 ** Copyright (C) 2003 ~ 2007 Feynman Software.
 **
 ** Create date: 2002/01/17
 */

/*
 **  This source is free software; you can redistribute it and/or
 **  modify it under the terms of the GNU General Public
 **  License as published by the Free Software Foundation; either
 **  version 2 of the License, or (at your option) any later version.
 **
 **  This software is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 **  General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public
 **  License along with this library; if not, write to the Free
 **  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 **  MA 02111-1307, USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#ifdef _LANG_ZHCN
#include "font_text_res_cn.h"
#elif defined _LANG_ZHTW
#include "font_text_res_tw.h"
#else
#include "font_text_res_en.h"
#endif

#define IDM_EXIT            160

#define IDM_TEXTOUT        190
#define IDM_TEXTOUTRBF        200
#define IDM_TEXTOUTVBF        210
#define IDM_TEXTOUTUPF        215
#define IDM_TEXTOUTQPF        216
#define IDM_TEXTOUTTTF        217
#define IDM_BMPFONT           218

#define IDM_TABBEDTEXTOUT   220
#define IDM_TABBEDTEXTOUTEX 230
#define IDM_DRAWTEXT        240
#define IDM_ROTATION        250
#define IDM_ARABIC          260
#define IDM_HEBREW          270
#define IDM_JAPAN           280
#define IDM_HANGUL          290
#define IDM_THAI            300

#define IDM_ABOUT           310
#define IDM_TEXT_R2L        320
#define IDM_TEXT_L2R        321
#define IDM_DRAW_R2L        322
#define IDM_DRAW_L2R        323

#define BMP_FILE_DIGIT    "digits1.png"
#define BMP_FILE_LETTER   "digits2.png"
#define BMP_FILE_SYMBOL   "digits3.png"
#define BMP_FILE_DDOT     "digits4.png"


#define MODE_TEXTOUTRBF           11
#define MODE_TEXTOUTVBF           12
#define MODE_TEXTOUTUPF           13
#define MODE_TEXTOUTQPF           14
#define MODE_TEXTOUTTTF           25

#define MODE_TABBEDTEXTOUT      15
#define MODE_TABBEDTEXTOUTEX    16
#define MODE_DRAWTEXT           17
#define MODE_ROTATION           18

#define MODE_ARABIC             19
#define MODE_HEBREW             20
#define MODE_JAPAN              21
#define MODE_HANGUL             22
#define MODE_THAI               23
#define MODE_BMPFONT            24

#define TEXTOUT_R2L             1
#define TEXTOUT_L2R             2
#define DRAWTEXT_R2L            3
#define DRAWTEXT_L2R            4

static LOGFONT *logfont_bmp; 
static DEVFONT *dev_font;

static BITMAP digit_bmp;
static BITMAP letter_bmp;
static BITMAP symbol_bmp;
static BITMAP ddot_bmp;

static PLOGFONT logfont_iso8859_6, logfont_iso8859_8, logfont_jisx0208, logfont_ksc5601_0,
                logfont_iso8859_11; 

//logfont for rbf
static PLOGFONT logfont_rbf_iso12, logfont_rbf_iso16, logfont_rbf_iso16_bold,logfont_rbf_iso16_book, 
                logfont_rbf_iso16_light, logfont_rbf_iso16_flip1, logfont_rbf_iso16_flip2,
                logfont_rbf_gb12_song, logfont_rbf_gb16_song, logfont_rbf_gb24_song,
                logfont_rbf_gb12_fmfsong, logfont_rbf_gb16_fmfsong, logfont_rbf_gb24_fmfsong,
                logfont_rbf_gb16_fmhei, logfont_rbf_gb20_fmhei, logfont_rbf_gb24_fmhei,
                logfont_rbf_big5_12, logfont_rbf_big5_16, logfont_rbf_big5_24;
//logfont for vbf
static PLOGFONT logfont_vbf_Courier1, logfont_vbf_Courier2, logfont_vbf_Hel1, logfont_vbf_Hel2,
                logfont_vbf_Times1, logfont_vbf_Times2, 
                logfont_vbf_Hel_bold, logfont_vbf_Hel_book,
                logfont_vbf_Hel_light, logfont_vbf_Hel_flip1, logfont_vbf_Hel_flip2;
//logfont for upf
static PLOGFONT logfont_upf_Courier12, logfont_upf_Courier18, logfont_upf_Courier24, 
                logfont_upf_helvetical12, logfont_upf_helvetical18, logfont_upf_helvetical24, 
                logfont_upf_Times12, logfont_upf_Times18, logfont_upf_Times24, 
                logfont_upf_fmsong12, logfont_upf_fmsong16, logfont_upf_fmfsong12, logfont_upf_fmfsong16,
                logfont_upf_fmhei16, logfont_upf_fmhei20, logfont_upf_fmkai12, logfont_upf_fmkai16,
                logfont_upf_big5_12, logfont_upf_big5_16, logfont_upf_utf8_12, logfont_upf_utf8_16;

#ifdef CONFIG_HAS_QPF
//logfont for qpf
static PLOGFONT  logfont_qpf_Courier14, logfont_qpf_Courier18, logfont_qpf_Courier24, 
                 logfont_qpf_Lucida14, logfont_qpf_Lucida18, logfont_qpf_Lucida19, 
		 logfont_qpf_Lucida24, logfont_qpf_Times14, logfont_qpf_Times18, 
		 logfont_qpf_Times24, logfont_qpf_helvetica14, logfont_qpf_helvetica18,
		 logfont_qpf_helvetica24, logfont_qpf_fmkai16,logfont_qpf_fmkai18, 
		 logfont_qpf_fmkai20, logfont_qpf_fmfsong16, logfont_qpf_fmfsong18, 
		 logfont_qpf_fmfsong20, logfont_qpf_fmsong15, logfont_qpf_fmsong16, 
		 logfont_qpf_fmsong18; 
#endif

//unicode -> GB2312
static UChar32 src2[] = {
    0x6B22, 0x8FCE, 0x6D4B, 0x8BD5, 0x4F7F, 0x7528, 0x004D, 0x0069, 0x006E, 0x0069, 0x0047, 0x0055, 0x0049
};

//unicode -> BIG5
static UChar32 src3[] = {
    0x6B61, 0x8FCE, 0x6E2C, 0x8A66, 0x4F7F, 0x7528, 0x004D, 0x0069, 0x006E, 0x0069, 0x0047, 0x0055, 0x0049
};
//unicode -> UTF-8
static UChar32 src4[] = {
    0x6B22, 0x8FCE, 0x6D4B, 0x8BD5, 0x4F7F, 0x7528, 0x6B61, 0x8FCE, 0x6E2C, 0x8A66, 0x4F7F, 0x7528
};

extern void OnModeArabic (HWND hWnd, HDC hdc, int textmodedire, PLOGFONT logfont_iso8859_6);
extern void OnModeHebrew (HWND hWnd, HDC hdc, int modedire, PLOGFONT iso8859_8);
extern void OnModeJapan (HWND hWnd, HDC hdc, int textmodedire, PLOGFONT jisx0201);
extern void OnModeHangul (HWND hWnd, HDC hdc, int textmodedire, PLOGFONT ksc5601_0);
extern void OnModeThai (HWND hWnd, HDC hdc, int textmodedire, PLOGFONT iso8859_11);
extern void OnModeBmpfont(HWND hWnd, HDC hdc, PLOGFONT bmplogfont);

#define  FONTFILE_PATH   "/usr/local/share/minigui/res/"

typedef struct tag_LOGFONTINFO{
    char*  filename;
    char*  devfontname;
}DEVFONTINFO;

static const DEVFONTINFO  FontName[]={
    //0-12
    {FONTFILE_PATH"font/6x12-iso8859-1.bin","rbf-fixed-rrncnn-6-12-ISO8859-1"},
    {FONTFILE_PATH"font/8x16-iso8859-1.bin","rbf-fixed-rrncnn-8-16-ISO8859-1"},
    {FONTFILE_PATH"font/song-12-gb2312.bin","rbf-song-rrncnn-12-12-GB2312-0"},
    {FONTFILE_PATH"font/song-16-gb2312.bin","rbf-song-rrncnn-16-16-GB2312-0"},
    {FONTFILE_PATH"font/song-24-gb2312.bin","rbf-song-rrncnn-24-24-GB2312-0"},
    {FONTFILE_PATH"font/fmfsong-12-gb2312.bin","rbf-fmfsong-rrncnn-12-12-GB2312-0"},
    {FONTFILE_PATH"font/fmfsong-16-gb2312.bin","rbf-fmfsong-rrncnn-16-16-GB2312-0"},
    {FONTFILE_PATH"font/fmfsong-24-gb2312.bin","rbf-fmfsong-rrncnn-24-24-GB2312-0"},
    {FONTFILE_PATH"font/fmhei-16-gb2312.bin","rbf-fmhei-rrncnn-16-16-GB2312-0"},
    {FONTFILE_PATH"font/fmhei-20-gb2312.bin","rbf-fmhei-rrncnn-20-20-GB2312-0"},
    {FONTFILE_PATH"font/fmhei-24-gb2312.bin","rbf-fmhei-rrncnn-24-24-GB2312-0"},
    {FONTFILE_PATH"font/ming-12-big5.bin","rbf-ming-rrncnn-12-12-BIG5"},
    {FONTFILE_PATH"font/ming-16-big5.bin","rbf-ming-rrncnn-16-16-BIG5"},
    {FONTFILE_PATH"font/ming-24-big5.bin","rbf-ming-rrncnn-24-24-BIG5"},
    //13-18
    {FONTFILE_PATH"font/Courier-rr-8-13.vbf","vbf-Courier-rrncnn-8-13-ISO8859-1"},
    {FONTFILE_PATH"font/Courier-rr-10-15.vbf","vbf-Courier-rrncnn-10-15-ISO8859-1"},
    {FONTFILE_PATH"font/Helvetica-rr-11-12.vbf","vbf-Helvetica-rrncnn-11-12-ISO8859-1"},
    {FONTFILE_PATH"font/Helvetica-rr-15-16.vbf","vbf-Helvetica-rrncnn-15-16-ISO8859-1"},
    {FONTFILE_PATH"font/Times-rr-10-12.vbf", "vbf-Times-rrncnn-10-12-ISO8859-1"},
    {FONTFILE_PATH"font/Times-rr-13-15.vbf", "vbf-Times-rrncnn-13-15-ISO8859-1"},
    //19 - 35
    {FONTFILE_PATH"font/courier-mr-12.upf","upf-Courier-rrncnn-12-12-ISO8859-1"},
    {FONTFILE_PATH"font/courier-mr-18.upf","upf-Courier-rrncnn-18-18-ISO8859-1"},
    {FONTFILE_PATH"font/courier-mr-24.upf","upf-Courier-rrncnn-24-24-ISO8859-1"},
    {FONTFILE_PATH"font/helvetica-mr-12.upf","upf-Helvetica-rrncnn-12-12-ISO8859-1"},
    {FONTFILE_PATH"font/helvetica-mr-18.upf","upf-Helvetica-rrncnn-18-18-ISO8859-1"},
    {FONTFILE_PATH"font/helvetica-mr-24.upf","upf-Helvetica-rrncnn-24-24-ISO8859-1"},
    {FONTFILE_PATH"font/times-mr-12.upf", "upf-Times-rrncnn-12-12-ISO8859-1"},
    {FONTFILE_PATH"font/times-mr-18.upf", "upf-Times-rrncnn-18-18-ISO8859-1"},
    {FONTFILE_PATH"font/times-mr-24.upf", "upf-Times-rrncnn-24-24-ISO8859-1"},
    {FONTFILE_PATH"font/fmsong-latin-gbk-12.upf", "upf-fmsong-rrncnn-12-12-GB2312-0,BIG5,UTF-8"},
    {FONTFILE_PATH"font/fmsong-latin-gbk-16.upf", "upf-fmsong-rrncnn-16-16-GB2312-0,BIG5,UTF-8"},
    {FONTFILE_PATH"font/fmfsong-latin-gb2312-12.upf", "upf-fmfsong-rrncnn-12-12-GB2312-0,UTF-8"},
    {FONTFILE_PATH"font/fmfsong-latin-gb2312-16.upf", "upf-fmfsong-rrncnn-16-16-GB2312-0,UTF-8"},
    {FONTFILE_PATH"font/fmhei-latin-gbk-16.upf", "upf-fmhei-rrncnn-16-16-GB2312-0,UTF-8"},
    {FONTFILE_PATH"font/fmhei-latin-gbk-20.upf", "upf-fmhei-rrncnn-20-20-GB2312-0,UTF-8"},
    {FONTFILE_PATH"font/fmkai-latin-gb2312-12.upf", "upf-fmkai-rrncnn-12-12-GB2312-0,UTF-8"},
    {FONTFILE_PATH"font/fmkai-latin-gb2312-16.upf", "upf-fmkai-rrncnn-16-16-GB2312-0,UTF-8"},

    //iso8859-6,iso8859-8, JISX0201-1, EUC-KR 36 - 44
    {FONTFILE_PATH"font/naskhi-18-21-iso8859-6.vbf", "vbf-naskhi-rrncnn-18-21-ISO8859-6"},
    {FONTFILE_PATH"font/fixed-10-20-iso8859-8.vbf", "vbf-fixed-mrncnn-10-20-ISO8859-8"},
    {FONTFILE_PATH"font/gothic-6x12-jisx0201.bin", "rbf-gothic-rrncnn-12-12-JISX0201-1"},
    {FONTFILE_PATH"font/batang-12-ksc5601.bin", "rbf-batang-rrncnn-12-12-EUC-KR"},
    {FONTFILE_PATH"font/gothic-12-jiskan.bin","rbf-gothic-rrncnn-12-12-JISX0208-1"},
    {FONTFILE_PATH"font/fixed-16-jiskan.bin","rbf-fixed-rrncnn-16-16-JISX0208-1"},
    {FONTFILE_PATH"font/gothic-12-jiskan.bin","rbf-gothic-rrncnn-12-12-JISX0208-1"},
    {FONTFILE_PATH"font/mincho-12-jiskan.bin","rbf-mincho-rrncnn-12-12-JISX0208-1"},
    {FONTFILE_PATH"font/gothic-6x12rk.bin","rbf-gothic-rrncnn-12-12-JISX0201-1"},
#ifdef CONFIG_HAS_QPF
    //45 -66
    {FONTFILE_PATH"font/courier_140_50.qpf","qpf-courier-rrncnn-14-14-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/courier_180_50.qpf","qpf-courier-rrncnn-18-18-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/courier_240_50.qpf","qpf-courier-rrncnn-24-24-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/lucida_140_50.qpf","qpf-lucida-rrncnn-14-14-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/lucida_180_50.qpf","qpf-lucida-rrncnn-18-18-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/lucida_190_50.qpf","qpf-lucida-rrncnn-19-19-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/lucida_240_50.qpf","qpf-lucida-rrncnn-24-24-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/times_140_50.qpf","qpf-times-rrncnn-14-14-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/times_180_50.qpf","qpf-times-rrncnn-18-18-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/times_240_50.qpf","qpf-times-rrncnn-24-24-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/helvetica_140_50.qpf","qpf-helvetica-rrncnn-14-14-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/helvetica_180_50.qpf","qpf-helvetica-rrncnn-18-18-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/helvetica_240_50.qpf","qpf-helvetica-rrncnn-24-24-ISO8859-1,ISO8859-15"},
    {FONTFILE_PATH"font/fmfsong-16.qpf","qpf-fmfsong-rrncnn-16-16-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE"},
    {FONTFILE_PATH"font/fmfsong-18.qpf","qpf-fmfsong-rrncnn-18-18-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE"},
    {FONTFILE_PATH"font/fmfsong-20.qpf","qpf-fmfsong-rrncnn-20-20-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE"},
    {FONTFILE_PATH"font/fmsong-15.qpf","qpf-fmsong-rrncnn-15-15-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE"},
    {FONTFILE_PATH"font/fmsong-16.qpf","qpf-fmsong-rrncnn-16-16-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE"},
    {FONTFILE_PATH"font/fmsong-18.qpf","qpf-fmsong-rrncnn-18-18-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE"},
    {FONTFILE_PATH"font/fmkai-16.qpf","qpf-fmkai-rrncnn-16-16-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE"},
    {FONTFILE_PATH"font/fmkai-18.qpf","qpf-fmkai-rrncnn-18-18-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE"},
    {FONTFILE_PATH"font/fmkai-20.qpf","qpf-fmkai-rrncnn-20-20-ISO8859-1,ISO8859-15,GB2312,UTF-8,UTF-16LE,UTF-16BE"},
#endif
};

static HMENU CreatePmenuShowMode (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"ShowMode";
    hmnu = CreatePopupMenu (&mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUT;
    mii.typedata    = (DWORD)"TextOut";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TABBEDTEXTOUT;
    mii.typedata    = (DWORD)"TabbedTextOut";
    InsertMenuItem(hmnu, 1, TRUE, &mii);  

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TABBEDTEXTOUTEX;
    mii.typedata    = (DWORD)"TabbedTextOutEx";
    InsertMenuItem(hmnu, 2, TRUE, &mii);  

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_DRAWTEXT;
    mii.typedata    = (DWORD)"DrawText";
    InsertMenuItem(hmnu, 3, TRUE, &mii);  

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_ROTATION;
    mii.typedata    = (DWORD)"Rotate";
    InsertMenuItem(hmnu, 4, TRUE, &mii);  

    return hmnu;
}

static HMENU CreatePmenuAbout (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)FT_ST_ABOUT;
    hmnu = CreatePopupMenu (&mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_ABOUT;
    mii.typedata    = (DWORD)FT_ST_ABOUTM;
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    return hmnu;
}

static HMENU CreatePmenuType (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"FontType";
    hmnu = CreatePopupMenu (&mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUTRBF;
    mii.typedata    = (DWORD)"RBF";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUTVBF;
    mii.typedata    = (DWORD)"VPF";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUTUPF;
    mii.typedata    = (DWORD)"UPF";
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUTQPF;
    mii.typedata    = (DWORD)"QPF";
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXTOUTTTF;
    mii.typedata    = (DWORD)"TTF";
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_BMPFONT;
    mii.typedata    = (DWORD)"BmpFont";
    InsertMenuItem(hmnu, 5, TRUE, &mii);
    return hmnu;
}

static HMENU CreatePmenuLanguage (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)"Language";
    hmnu = CreatePopupMenu (&mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_ARABIC;
    mii.typedata    = (DWORD)"Arabic";
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_HEBREW;
    mii.typedata    = (DWORD)"Hebrew";
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_JAPAN;
    mii.typedata    = (DWORD)"Japanese";
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_HANGUL;
    mii.typedata    = (DWORD)"Korean";
    InsertMenuItem(hmnu, 3, TRUE, &mii);
/*
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_THAI;
    mii.typedata    = (DWORD)"Thai";
    InsertMenuItem(hmnu, 4, TRUE, &mii);
    */


    return hmnu;
}

static HMENU CreatePmenuTextMode (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)FT_ST_TEXTMODE;
    hmnu = CreatePopupMenu (&mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXT_R2L;
    mii.typedata    = (DWORD)FT_ST_TEXT_R2L;
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_TEXT_L2R;
    mii.typedata    = (DWORD)FT_ST_TEXT_L2R;
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_DRAW_R2L;
    mii.typedata    = (DWORD)FT_ST_DRAW_R2L;
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_DRAW_L2R;
    mii.typedata    = (DWORD)FT_ST_DRAW_L2R;
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    return hmnu;
}

static HMENU createmenu (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    hmnu = CreateMenu();


    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)"FontType";
    mii.hsubmenu    = CreatePmenuType ();
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)"DisplayMode";
    mii.hsubmenu    = CreatePmenuShowMode();
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 120;
    mii.typedata    = (DWORD)"Language";
    mii.hsubmenu    = CreatePmenuLanguage ();
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 130;
    mii.typedata    = (DWORD)"AlignMode";
    mii.hsubmenu    = CreatePmenuTextMode ();
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 140;
    mii.typedata    = (DWORD)"About";
    mii.hsubmenu    = CreatePmenuAbout ();
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    return hmnu;
}


static void CreateFont (void)
{
    logfont_rbf_iso12 = CreateLogFont ("rbf", "fixed", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_rbf_iso16 = CreateLogFont ("rbf", "fixed", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_rbf_iso16_bold = CreateLogFont ("rbf", "fixed", "ISO8859-1",
            FONT_WEIGHT_BOLD, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_LINE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_rbf_iso16_book = CreateLogFont ("rbf", "fixed", "ISO8859-1",
            FONT_WEIGHT_BOOK, FONT_SLANT_ITALIC, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_LINE, 
            16, 0);
    logfont_rbf_iso16_light = CreateLogFont ("rbf", "fixed", "ISO8859-1",
            FONT_WEIGHT_LIGHT, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_rbf_iso16_flip1 = CreateLogFont ("rbf", "fixed", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_HORZ, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_rbf_iso16_flip2 = CreateLogFont ("rbf", "fixed", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_VERT, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_rbf_gb12_song = CreateLogFont ("rbf", "song", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_rbf_gb16_song = CreateLogFont ("rbf", "song", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_rbf_gb24_song = CreateLogFont ("rbf", "song", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);
    logfont_rbf_gb12_fmfsong = CreateLogFont ("rbf", "fmfsong", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_rbf_gb16_fmfsong = CreateLogFont ("rbf", "fmfsong", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_rbf_gb24_fmfsong = CreateLogFont ("rbf", "fmfsong", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);
    logfont_rbf_gb16_fmhei = CreateLogFont ("rbf", "fmhei", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_rbf_gb20_fmhei = CreateLogFont ("rbf", "fmhei", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            20, 0);
    logfont_rbf_gb24_fmhei = CreateLogFont ("rbf", "fmhei", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);

    logfont_rbf_big5_12 = CreateLogFont ("rbf", "ming", "BIG5",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_rbf_big5_16 = CreateLogFont ("rbf", "ming", "BIG5",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_rbf_big5_24 = CreateLogFont ("rbf", "ming", "BIG5",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);

    logfont_vbf_Courier1 = CreateLogFont ("vbf", "Courier", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            13, 0);
    logfont_vbf_Courier2 = CreateLogFont ("vbf", "Courier", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            15, 0);
    logfont_vbf_Times1 = CreateLogFont ("vbf", "Times", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_vbf_Times2 = CreateLogFont ("vbf", "Times", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            15, 0);
    logfont_vbf_Hel1 = CreateLogFont ("vbf", "Helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_vbf_Hel2 = CreateLogFont ("vbf", "Helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_vbf_Hel_bold = CreateLogFont ("vbf", "Helvetica", "ISO8859-1",
            FONT_WEIGHT_BOLD, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_LINE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_vbf_Hel_book = CreateLogFont ("vbf", "Helvetica", "ISO8859-1",
            FONT_WEIGHT_BOOK, FONT_SLANT_ITALIC, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_LINE, 
            16, 0);
    logfont_vbf_Hel_light = CreateLogFont ("vbf", "Helvetica", "ISO8859-1",
            FONT_WEIGHT_LIGHT, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_vbf_Hel_flip1 = CreateLogFont ("vbf", "Helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_HORZ, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_vbf_Hel_flip2 = CreateLogFont ("vbf", "Helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_VERT, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);

    logfont_upf_Courier12 = CreateLogFont ("upf", "Courier", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            10, 0);
    logfont_upf_Courier18 = CreateLogFont ("upf", "Courier", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);
    logfont_upf_Courier24 = CreateLogFont ("upf", "Courier", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);
    logfont_upf_helvetical12 = CreateLogFont ("upf", "helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);


    logfont_upf_helvetical18 = CreateLogFont ("upf", "helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);
    logfont_upf_helvetical24 = CreateLogFont ("upf", "helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);
    logfont_upf_Times12 = CreateLogFont ("upf", "Times", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_upf_Times18 = CreateLogFont ("upf", "Times", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);
    logfont_upf_Times24 = CreateLogFont ("upf", "Times", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);
    logfont_upf_fmsong12 = CreateLogFont ("upf", "song", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_upf_fmsong16 = CreateLogFont ("upf", "song", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_upf_fmfsong12 = CreateLogFont ("upf", "fmfsong", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_upf_fmfsong16 = CreateLogFont ("upf", "fmfsong", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);

    logfont_upf_fmhei16 = CreateLogFont ("upf", "fmhei", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);

    logfont_upf_fmhei20 = CreateLogFont ("upf", "fmhei", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            20, 0);

    logfont_upf_fmkai12 = CreateLogFont ("upf", "fmkai", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);
    logfont_upf_fmkai16 = CreateLogFont ("upf", "fmkai", "GB2312-0",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
    logfont_upf_big5_12 = CreateLogFont ("upf", "fmsong", "BIG5",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);

    logfont_upf_big5_16 = CreateLogFont ("upf", "fmsong", "BIG5",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);
   
    logfont_upf_utf8_12 = CreateLogFont ("upf", "fmhei", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            15, 0);
        
    logfont_upf_utf8_16 = CreateLogFont ("upf", "fmsong", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);

#ifdef CONFIG_HAS_QPF
//qpf logfont 
    logfont_qpf_Courier14 = CreateLogFont ("qpf", "courier", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            14, 0);

    logfont_qpf_Courier18 = CreateLogFont ("qpf", "courier", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);

    logfont_qpf_Courier24 = CreateLogFont ("qpf", "courier", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);

    logfont_qpf_Lucida14 = CreateLogFont ("qpf", "lucida", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            14, 0);

    logfont_qpf_Lucida18 = CreateLogFont ("qpf", "lucida", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);

    logfont_qpf_Lucida19 = CreateLogFont ("qpf", "lucida", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            19, 0);

    logfont_qpf_Lucida24 = CreateLogFont ("qpf", "lucida", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);

    logfont_qpf_Times14 = CreateLogFont ("qpf", "times", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            14, 0);

    logfont_qpf_Times18 = CreateLogFont ("qpf", "times", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);

    logfont_qpf_Times24 = CreateLogFont ("qpf", "times", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);

    logfont_qpf_helvetica14 = CreateLogFont ("qpf", "helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            14, 0);

    logfont_qpf_helvetica18 = CreateLogFont ("qpf", "helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);

    logfont_qpf_helvetica24 = CreateLogFont ("qpf", "helvetica", "ISO8859-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            24, 0);

    logfont_qpf_fmkai16 = CreateLogFont ("qpf", "fmkai", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);

    logfont_qpf_fmkai18 = CreateLogFont ("qpf", "fmkai", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);

    logfont_qpf_fmkai20 = CreateLogFont ("qpf", "fmkai", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            20, 0);

    logfont_qpf_fmfsong16 = CreateLogFont ("qpf", "fmfsong", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);

    logfont_qpf_fmfsong18 = CreateLogFont ("qpf", "fmfsong", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);

    logfont_qpf_fmfsong20 = CreateLogFont ("qpf", "fmfsong", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            20, 0);

    logfont_qpf_fmsong15 = CreateLogFont ("qpf", "fmsong", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            15, 0);

    logfont_qpf_fmsong16 = CreateLogFont ("qpf", "fmsong", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            16, 0);

    logfont_qpf_fmsong18 = CreateLogFont ("qpf", "fmsong", "UTF-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);
#endif

//vbf logfont
    logfont_iso8859_6 = CreateLogFont ("vbf", "naskhi", "ISO8859-6",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            18, 0);
    logfont_iso8859_8 = CreateLogFont ("vbf", "fixed", "ISO8859-8",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            10, 0);
    logfont_jisx0208 = CreateLogFont ("rbf", "mincho", "JISX0208-1",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);

    logfont_ksc5601_0 = CreateLogFont ("rbf", "batang", "EUC-KR",
            FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
            FONT_FLIP_NIL, FONT_OTHER_NIL,
            FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
            12, 0);


}

static void DestroyFont (void)
{
    DestroyLogFont (logfont_rbf_iso12);
    DestroyLogFont (logfont_rbf_iso16);
    DestroyLogFont (logfont_rbf_iso16_bold);
    DestroyLogFont (logfont_rbf_iso16_book);
    DestroyLogFont (logfont_rbf_iso16_light);
    DestroyLogFont (logfont_rbf_iso16_flip1);
    DestroyLogFont (logfont_rbf_iso16_flip2);
    DestroyLogFont (logfont_rbf_gb12_song);
    DestroyLogFont (logfont_rbf_gb16_song);
    DestroyLogFont (logfont_rbf_gb24_song);
    DestroyLogFont (logfont_rbf_gb12_fmfsong);
    DestroyLogFont (logfont_rbf_gb16_fmfsong);
    DestroyLogFont (logfont_rbf_gb24_fmfsong);
    DestroyLogFont (logfont_rbf_gb16_fmhei);
    DestroyLogFont (logfont_rbf_gb20_fmhei);
    DestroyLogFont (logfont_rbf_gb24_fmhei);
    DestroyLogFont (logfont_rbf_big5_12);
    DestroyLogFont (logfont_rbf_big5_16);


    DestroyLogFont (logfont_vbf_Courier1);
    DestroyLogFont (logfont_vbf_Courier2);
    DestroyLogFont (logfont_vbf_Times1);
    DestroyLogFont (logfont_vbf_Times2);
    DestroyLogFont (logfont_vbf_Hel1);
    DestroyLogFont (logfont_vbf_Hel2);
    DestroyLogFont (logfont_vbf_Hel_bold);
    DestroyLogFont (logfont_vbf_Hel_book);
    DestroyLogFont (logfont_vbf_Hel_light);
    DestroyLogFont (logfont_vbf_Hel_flip1);
    DestroyLogFont (logfont_vbf_Hel_flip2);

    DestroyLogFont (logfont_upf_Courier12);
    DestroyLogFont (logfont_upf_Courier18);
    DestroyLogFont (logfont_upf_Courier24);
    DestroyLogFont (logfont_upf_helvetical12);
    DestroyLogFont (logfont_upf_helvetical18);
    DestroyLogFont (logfont_upf_helvetical24);
    DestroyLogFont (logfont_upf_Times12);
    DestroyLogFont (logfont_upf_Times18);
    DestroyLogFont (logfont_upf_Times24);
    DestroyLogFont (logfont_upf_fmsong12);
    DestroyLogFont (logfont_upf_fmsong16);
    DestroyLogFont (logfont_upf_fmfsong12);
    DestroyLogFont (logfont_upf_fmfsong16);
    DestroyLogFont (logfont_upf_fmhei16);
    DestroyLogFont (logfont_upf_fmhei20);
    DestroyLogFont (logfont_upf_fmkai12);
    DestroyLogFont (logfont_upf_fmkai16);
    DestroyLogFont (logfont_upf_big5_12);
    DestroyLogFont (logfont_upf_big5_16);

#ifdef CONFIG_HAS_QPF
    DestroyLogFont (logfont_qpf_Courier14);
    DestroyLogFont (logfont_qpf_Courier18);
    DestroyLogFont (logfont_qpf_Courier24); 
    DestroyLogFont (logfont_qpf_Lucida14);
    DestroyLogFont (logfont_qpf_Lucida18);
    DestroyLogFont (logfont_qpf_Lucida19); 
    DestroyLogFont (logfont_qpf_Lucida24); 
    DestroyLogFont (logfont_qpf_Times14);
    DestroyLogFont (logfont_qpf_Times18); 
    DestroyLogFont (logfont_qpf_Times24);
    DestroyLogFont (logfont_qpf_helvetica14); 
    DestroyLogFont (logfont_qpf_helvetica18);
    DestroyLogFont (logfont_qpf_helvetica24);
    DestroyLogFont (logfont_qpf_fmkai16);
    DestroyLogFont (logfont_qpf_fmkai18); 
    DestroyLogFont (logfont_qpf_fmkai20);
    DestroyLogFont (logfont_qpf_fmfsong16);
    DestroyLogFont (logfont_qpf_fmfsong18); 
    DestroyLogFont (logfont_qpf_fmfsong20);
    DestroyLogFont (logfont_qpf_fmsong15);
    DestroyLogFont (logfont_qpf_fmsong16); 
    DestroyLogFont (logfont_qpf_fmsong18);
#endif

    DestroyLogFont (logfont_iso8859_6);
    DestroyLogFont (logfont_iso8859_8);
    DestroyLogFont (logfont_jisx0208);
    DestroyLogFont (logfont_ksc5601_0);

}

static void OnModeTextOutRBF (HDC hdc)
{
    char buf1[128];
    char buf2[64];

    SetBkColor (hdc, COLOR_blue);
    SetBkMode (hdc, BM_TRANSPARENT);

    memset (buf1, 0, sizeof(buf1));
    memset (buf2, 0, sizeof(buf2));
    strcpy (buf2, " The quick brown fox jumps over the lazy dog.");


    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_rbf_iso12->type, logfont_rbf_iso12->family, 
            logfont_rbf_iso12->charset, logfont_rbf_iso12->size);
    TextOut (hdc, 0, 0, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_rbf_iso16->type, logfont_rbf_iso16->family, 
            logfont_rbf_iso16->charset, logfont_rbf_iso16->size);
    TextOut (hdc, 0, 20, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-bold-roman-underline: ", 
            logfont_rbf_iso16_bold->type, logfont_rbf_iso16_bold->family, 
            logfont_rbf_iso16_bold->charset, logfont_rbf_iso16_bold->size);
    TextOut (hdc, 0, 40, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-book-italic-struckout: ", 
            logfont_rbf_iso16_book->type, logfont_rbf_iso16_book->family, 
            logfont_rbf_iso16_book->charset, logfont_rbf_iso16_book->size);
    TextOut (hdc, 0, 60,  buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-light-roman: ", 
            logfont_rbf_iso16_light->type, logfont_rbf_iso16_light->family, 
            logfont_rbf_iso16_light->charset, logfont_rbf_iso16_light->size);
    TextOut (hdc, 0, 80,  buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-flip1(horz): ",
            logfont_rbf_iso16_flip1->type, logfont_rbf_iso16_flip1->family, 
            logfont_rbf_iso16_flip1->charset, logfont_rbf_iso16_flip1->size);
    TextOut (hdc, 0, 100,  buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-flip(vert): ", 
            logfont_rbf_iso16_flip2->type, logfont_rbf_iso16_flip2->family, 
            logfont_rbf_iso16_flip2->charset, logfont_rbf_iso16_flip2->size);
    TextOut (hdc, 0, 120,  buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_gb12_song->type, logfont_rbf_gb12_song->family, 
            logfont_rbf_gb12_song->charset, logfont_rbf_gb12_song->size);
    TextOut (hdc, 0, 160, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_gb16_song->type, logfont_rbf_gb16_song->family, 
            logfont_rbf_gb16_song->charset, logfont_rbf_gb16_song->size);
    TextOut (hdc, 0, 180, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_gb24_song->type, logfont_rbf_gb24_song->family, 
            logfont_rbf_gb24_song->charset, logfont_rbf_gb24_song->size);
    TextOut (hdc, 0, 200, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_gb12_fmfsong->type, logfont_rbf_gb12_fmfsong->family, 
            logfont_rbf_gb12_fmfsong->charset, logfont_rbf_gb12_fmfsong->size);
    TextOut (hdc, 0, 240, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_rbf_gb16_fmfsong->type, logfont_rbf_gb16_fmfsong->family,
            logfont_rbf_gb16_fmfsong->charset, logfont_rbf_gb16_fmfsong->size);
    TextOut (hdc, 0, 260, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_rbf_gb24_fmfsong->type, logfont_rbf_gb24_fmfsong->family,
            logfont_rbf_gb24_fmfsong->charset, logfont_rbf_gb24_fmfsong->size);
    TextOut (hdc, 0, 280, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_gb16_fmhei->type, logfont_rbf_gb16_fmhei->family, 
            logfont_rbf_gb16_fmhei->charset, logfont_rbf_gb16_fmhei->size);
    TextOut (hdc, 0, 320, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_gb20_fmhei->type, logfont_rbf_gb20_fmhei->family, 
            logfont_rbf_gb20_fmhei->charset, logfont_rbf_gb20_fmhei->size);
    TextOut (hdc, 0, 340, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_gb24_fmhei->type, logfont_rbf_gb24_fmhei->family, 
            logfont_rbf_gb24_fmhei->charset, logfont_rbf_gb24_fmhei->size);
    TextOut (hdc, 0, 360, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_big5_12->type, logfont_rbf_big5_12->family, 
            logfont_rbf_big5_12->charset, logfont_rbf_big5_12->size);
    TextOut (hdc, 0, 400, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_big5_16->type, logfont_rbf_big5_16->family, 
            logfont_rbf_big5_16->charset, logfont_rbf_big5_16->size);
    TextOut (hdc, 0, 420, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_rbf_big5_24->type, logfont_rbf_big5_24->family, 
            logfont_rbf_big5_24->charset, logfont_rbf_big5_24->size);
    TextOut (hdc, 0, 440, buf1);


    SelectFont (hdc, logfont_rbf_iso12);
    TextOut (hdc, 330, 0, buf2);

    SelectFont (hdc, logfont_rbf_iso16);
    TextOut (hdc, 330, 20, buf2);

    SelectFont (hdc, logfont_rbf_iso16_bold);
    TextOut (hdc, 330, 40,  buf2);


    SelectFont (hdc, logfont_rbf_iso16_book);
    TextOut (hdc, 330, 60,  buf2);

    SelectFont (hdc, logfont_rbf_iso16_light);
    TextOut (hdc, 330, 80, buf2);

    SelectFont (hdc, logfont_rbf_iso16_flip1);
    TextOut (hdc, 330, 100,  buf2);

    SelectFont (hdc, logfont_rbf_iso16_flip2);
    TextOut (hdc, 330, 120,  buf2);

    memset (buf2, 0, sizeof(buf2));
    SelectFont (hdc, logfont_rbf_gb12_song);
    WCS2MBS (logfont_rbf_gb12_song, (unsigned char*)buf2, src2, TABLESIZE(src2), TABLESIZE(buf2));
    TextOut (hdc, 330, 160, buf2);

    SelectFont (hdc, logfont_rbf_gb16_song);
    TextOut (hdc, 330, 180, buf2);

    SelectFont (hdc, logfont_rbf_gb24_song);
    TextOut (hdc, 330, 200, buf2);

    SelectFont (hdc, logfont_rbf_gb12_fmfsong);
    TextOut (hdc, 330, 240, buf2);

    SelectFont (hdc, logfont_rbf_gb16_fmfsong);
    TextOut (hdc, 330, 260, buf2);

    SelectFont (hdc, logfont_rbf_gb24_fmfsong);
    TextOut (hdc, 330, 280, buf2);

    SelectFont (hdc, logfont_rbf_gb16_fmhei);
    TextOut (hdc, 330, 320, buf2);

    SelectFont (hdc, logfont_rbf_gb20_fmhei);
    TextOut (hdc, 330, 340, buf2);

    SelectFont (hdc, logfont_rbf_gb24_fmhei);
    TextOut (hdc, 330, 360, buf2);

    memset (buf2, 0, sizeof(buf2));
    SelectFont (hdc, logfont_rbf_big5_12);
    WCS2MBS (logfont_rbf_big5_12, (unsigned char*)buf2, src3, TABLESIZE(src3), TABLESIZE(buf2));
    TextOut (hdc, 330, 400, buf2);

    SelectFont (hdc, logfont_rbf_big5_16);
    TextOut (hdc, 330, 420, buf2);

    SelectFont (hdc, logfont_rbf_big5_16);
    TextOut (hdc, 330, 440, buf2);


}

void OnModeTextOutVBF (HDC hdc)
{
    char buf1[128];
    char buf2[64];


    SetBkColor (hdc, COLOR_blue);
    SetBkMode (hdc, BM_TRANSPARENT);

    memset (buf1, 0, sizeof(buf1));
    memset (buf2, 0, sizeof(buf2));
    strcpy (buf2, " The quick brown fox jumps over the lazy dog.");


    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_vbf_Courier1->type, logfont_vbf_Courier1->family, 
            logfont_vbf_Courier1->charset, logfont_vbf_Courier1->size);
    TextOut (hdc, 0, 0, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_vbf_Courier2->type, logfont_vbf_Courier2->family, 
            logfont_vbf_Courier2->charset, logfont_vbf_Courier2->size);
    TextOut (hdc, 0, 20, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_vbf_Times1->type, logfont_vbf_Times1->family, 
            logfont_vbf_Times1->charset, logfont_vbf_Times1->size);
    TextOut (hdc, 0, 50, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_vbf_Times2->type, logfont_vbf_Times2->family, 
            logfont_vbf_Times2->charset, logfont_vbf_Times2->size);
    TextOut (hdc, 0, 70, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_vbf_Hel1->type, logfont_vbf_Hel1->family, 
            logfont_vbf_Hel1->charset, logfont_vbf_Hel1->size);
    TextOut (hdc, 0, 100, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_vbf_Hel2->type, logfont_vbf_Hel2->family, 
            logfont_vbf_Hel2->charset, logfont_vbf_Hel2->size);
    TextOut (hdc, 0, 120, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-bold-roman-underline: ", 
            logfont_vbf_Hel_bold->type, logfont_vbf_Hel_bold->family, 
            logfont_vbf_Hel_bold->charset, logfont_vbf_Hel_bold->size);
    TextOut (hdc, 0, 150, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-book-italic-struckout: ", 
            logfont_vbf_Hel_book->type, logfont_vbf_Hel_book->family, 
            logfont_vbf_Hel_book->charset, logfont_vbf_Hel_book->size);
    TextOut (hdc, 0, 170,  buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-light-roman: ", 
            logfont_vbf_Hel_light->type, logfont_vbf_Hel_light->family, 
            logfont_vbf_Hel_light->charset, logfont_vbf_Hel_light->size);
    TextOut (hdc, 0, 190,  buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-flip1(horz): ",
            logfont_vbf_Hel_flip1->type, logfont_vbf_Hel_flip1->family, 
            logfont_vbf_Hel_flip1->charset, logfont_vbf_Hel_flip1->size);
    TextOut (hdc, 0, 210,  buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-flip(vert): ", 
            logfont_vbf_Hel_flip2->type, logfont_vbf_Hel_flip2->family, 
            logfont_vbf_Hel_flip2->charset, logfont_vbf_Hel_flip2->size);
    TextOut (hdc, 0, 230,  buf1);



    SelectFont (hdc, logfont_vbf_Courier1);
    TextOut (hdc, 330, 0, buf2);

    SelectFont (hdc, logfont_vbf_Courier2);
    TextOut (hdc, 330, 20, buf2);

    SelectFont (hdc, logfont_vbf_Times1);
    TextOut (hdc, 330, 50, buf2);

    SelectFont (hdc, logfont_vbf_Times2);
    TextOut (hdc, 330, 70, buf2);

    SelectFont (hdc, logfont_vbf_Hel1);
    TextOut (hdc, 330, 100, buf2);

    SelectFont (hdc, logfont_vbf_Hel2);
    TextOut (hdc, 330, 120, buf2);

    SelectFont (hdc, logfont_vbf_Hel_bold);
    TextOut (hdc, 350, 150,  buf2);

    SelectFont (hdc, logfont_vbf_Hel_book);
    TextOut (hdc, 350, 170,  buf2);

    SelectFont (hdc, logfont_vbf_Hel_light);
    TextOut (hdc, 330, 190, buf2);

    SelectFont (hdc, logfont_vbf_Hel_flip1);
    TextOut (hdc, 330, 210,  buf2);

    SelectFont (hdc, logfont_vbf_Hel_flip2);
    TextOut (hdc, 330, 230,  buf2);
}

void OnModeTextOutUPF (HDC hdc)
{
    char buf1[128];
    char buf2[64];

    SetBkColor (hdc, COLOR_blue);
    SetBkMode (hdc, BM_TRANSPARENT);

    memset (buf1, 0, sizeof(buf1));
    memset (buf2, 0, sizeof(buf2));
    strcpy (buf2, " The quick brown fox jumps over the lazy dog.");

    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_Courier12->type, logfont_upf_Courier12->family, 
            logfont_upf_Courier12->charset, logfont_upf_Courier12->size);
    TextOut (hdc, 0, 0, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_Courier18->type, logfont_upf_Courier18->family, 
            logfont_upf_Courier18->charset, logfont_upf_Courier18->size);
    TextOut (hdc, 0, 20, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_Courier24->type, logfont_upf_Courier24->family, 
            logfont_upf_Courier24->charset, logfont_upf_Courier24->size);
    TextOut (hdc, 0, 40, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_helvetical12->type, logfont_upf_helvetical12->family, 
            logfont_upf_helvetical12->charset, logfont_upf_helvetical12->size);
    TextOut (hdc, 0, 70, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_helvetical18->type, logfont_upf_helvetical18->family, 
            logfont_upf_helvetical18->charset, logfont_upf_helvetical18->size);
    TextOut (hdc, 0, 90, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_helvetical24->type, logfont_upf_helvetical24->family, 
            logfont_upf_helvetical24->charset, logfont_upf_helvetical24->size);
    TextOut (hdc, 0, 110, buf1);


    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_Times12->type, logfont_upf_Times12->family, 
            logfont_upf_Times12->charset, logfont_upf_Times12->size);
    TextOut (hdc, 0, 140, buf1);

    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_Times18->type, logfont_upf_Times18->family, 
            logfont_upf_Times18->charset, logfont_upf_Times18->size);
    TextOut (hdc, 0, 160, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_Times24->type, logfont_upf_Times24->family, 
            logfont_upf_Times24->charset, logfont_upf_Times24->size);
    TextOut (hdc, 0, 180, buf1);



    memset (buf1, 0, sizeof(buf1));
    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_fmsong12->type, logfont_upf_fmsong12->family, 
            logfont_upf_fmsong12->charset, logfont_upf_fmsong12->size);
    TextOut (hdc, 0, 210, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_fmsong16->type, logfont_upf_fmsong16->family, 
            logfont_upf_fmsong16->charset, logfont_upf_fmsong16->size);
    TextOut (hdc, 0, 230, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_fmfsong12->type, logfont_upf_fmfsong12->family, 
            logfont_upf_fmfsong12->charset, logfont_upf_fmfsong12->size);
    TextOut (hdc, 0, 260, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
            logfont_upf_fmfsong16->type, logfont_upf_fmfsong16->family,
            logfont_upf_fmfsong16->charset, logfont_upf_fmfsong16->size);
    TextOut (hdc, 0, 280, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_fmhei16->type, logfont_upf_fmhei16->family, 
            logfont_upf_fmhei16->charset, logfont_upf_fmhei16->size);
    TextOut (hdc, 0, 310, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_fmhei20->type, logfont_upf_fmhei20->family, 
            logfont_upf_fmhei20->charset, logfont_upf_fmhei20->size);
    TextOut (hdc, 0, 330, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_fmkai12->type, logfont_upf_fmkai12->family, 
            logfont_upf_fmkai12->charset, logfont_upf_fmkai12->size);
    TextOut (hdc, 0, 360, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_fmkai16->type, logfont_upf_fmkai16->family, 
            logfont_upf_fmkai16->charset, logfont_upf_fmkai16->size);
    TextOut (hdc, 0, 380, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_big5_12->type, logfont_upf_big5_12->family, 
            logfont_upf_big5_12->charset, logfont_upf_big5_12->size);
    TextOut (hdc, 0, 410, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_big5_16->type, logfont_upf_big5_16->family, 
            logfont_upf_big5_16->charset, logfont_upf_big5_16->size);
    TextOut (hdc, 0, 430, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_utf8_12->type, logfont_upf_utf8_12->family, 
            logfont_upf_utf8_12->charset, logfont_upf_utf8_12->size);
    TextOut (hdc, 0, 460, buf1);

    sprintf (buf1, "%s-%s-%s-size%d-regular: ",
            logfont_upf_utf8_16->type, logfont_upf_utf8_16->family, 
            logfont_upf_utf8_16->charset, logfont_upf_utf8_16->size);
    TextOut (hdc, 0, 480, buf1);



    SelectFont (hdc, logfont_upf_Courier12);
    TextOut (hdc, 330, 0, buf2);

    SelectFont (hdc, logfont_upf_Courier18);
    TextOut (hdc, 330, 20, buf2);

    SelectFont (hdc, logfont_upf_Courier24);
    TextOut (hdc, 330, 40, buf2);

    SelectFont (hdc, logfont_upf_helvetical12);
    TextOut (hdc, 330, 70, buf2);

    SelectFont (hdc, logfont_upf_helvetical18);
    TextOut (hdc, 330, 90, buf2);

    SelectFont (hdc, logfont_upf_helvetical24);
    TextOut (hdc, 330, 110, buf2);

    SelectFont (hdc, logfont_upf_Times12);
    TextOut (hdc, 330, 140, buf2);

    SelectFont (hdc, logfont_upf_Times18);
    TextOut (hdc, 330, 160, buf2);

    SelectFont (hdc, logfont_upf_Times24);
    TextOut (hdc, 330, 180, buf2);


    memset (buf2, 0, sizeof(buf2));
    SelectFont (hdc, logfont_upf_fmsong12);
    WCS2MBS (logfont_upf_fmsong12, (unsigned char*)buf2, src2, TABLESIZE(src2), TABLESIZE(buf2));
    TextOut (hdc, 330, 210, buf2);

    SelectFont (hdc, logfont_upf_fmsong16);
    TextOut (hdc, 330, 230, buf2);

    SelectFont (hdc, logfont_upf_fmfsong12);
    TextOut (hdc, 330, 260, buf2);

    SelectFont (hdc, logfont_upf_fmfsong16);
    TextOut (hdc, 330, 280, buf2);

    SelectFont (hdc, logfont_upf_fmhei16);
    TextOut (hdc, 330, 310, buf2);

    SelectFont (hdc, logfont_upf_fmhei20);
    TextOut (hdc, 330, 330, buf2);

    SelectFont (hdc, logfont_upf_fmkai12);
    TextOut (hdc, 330, 360, buf2);

    SelectFont (hdc, logfont_upf_fmkai16);
    TextOut (hdc, 330, 380, buf2);

    memset (buf2, 0, sizeof(buf2));
    SelectFont (hdc, logfont_upf_big5_12);
    WCS2MBS (logfont_upf_big5_12, (unsigned char*)buf2, src3, TABLESIZE(src3), TABLESIZE(buf2));
    TextOut (hdc, 330, 410, buf2);

    SelectFont (hdc, logfont_upf_big5_16);
    TextOut (hdc, 330, 430, buf2);
    
    memset (buf2, 0, sizeof(buf2));
    SelectFont (hdc, logfont_upf_utf8_12);
    WCS2MBS (logfont_upf_utf8_12, (unsigned char*)buf2, src4, TABLESIZE(src4), TABLESIZE(buf2));
    TextOut (hdc, 330, 460, buf2);

    SelectFont (hdc, logfont_upf_utf8_16);
    TextOut (hdc, 330, 480, buf2);
    
}

void OnModeTextOutQPF (HDC hdc)
{
#ifndef CONFIG_HAS_QPF
    TextOut (hdc, 0, 0, "no display because of no qpf font in MiniGUI-res");
#else
	char buf1[128];
	char buf2[64];

	SetBkColor (hdc, COLOR_blue);
	SetBkMode (hdc, BM_TRANSPARENT);

	memset (buf1, 0, sizeof(buf1));
	memset (buf2, 0, sizeof(buf2));
	strcpy (buf2, " The quick brown fox jumps over the lazy dog.");

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Courier14->type, logfont_qpf_Courier14->family, 
			logfont_qpf_Courier14->charset, logfont_qpf_Courier14->size);
	TextOut (hdc, 0, 0, buf1);

	memset (buf1, 0, sizeof(buf1));
	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Courier18->type, logfont_qpf_Courier18->family, 
			logfont_qpf_Courier18->charset, logfont_qpf_Courier18->size);
	TextOut (hdc, 0, 20, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Courier24->type, logfont_qpf_Courier24->family, 
			logfont_qpf_Courier24->charset, logfont_qpf_Courier24->size);
	TextOut (hdc, 0, 40, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Lucida14->type, logfont_qpf_Lucida14->family, 
			logfont_qpf_Lucida14->charset, logfont_qpf_Lucida14->size);
	TextOut (hdc, 0, 70, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Lucida18->type, logfont_qpf_Lucida18->family, 
			logfont_qpf_Lucida18->charset, logfont_qpf_Lucida18->size);
	TextOut (hdc, 0, 90, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Lucida19->type, logfont_qpf_Lucida19->family, 
			logfont_qpf_Lucida19->charset, logfont_qpf_Lucida19->size);
	TextOut (hdc, 0, 110, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Lucida24->type, logfont_qpf_Lucida24->family, 
			logfont_qpf_Lucida24->charset, logfont_qpf_Lucida24->size);
	TextOut (hdc, 0, 130, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Times14->type, logfont_qpf_Times14->family, 
			logfont_qpf_Times14->charset, logfont_qpf_Times14->size);
	TextOut (hdc, 0, 160, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Times18->type, logfont_qpf_Times18->family, 
			logfont_qpf_Times18->charset, logfont_qpf_Times18->size);
	TextOut (hdc, 0, 180, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_Times24->type, logfont_qpf_Times24->family, 
			logfont_qpf_Times24->charset, logfont_qpf_Times24->size);
	TextOut (hdc, 0, 200, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_helvetica14->type, logfont_qpf_helvetica14->family, 
			logfont_qpf_helvetica14->charset, logfont_qpf_helvetica14->size);
	TextOut (hdc, 0, 230, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_helvetica18->type, logfont_qpf_helvetica18->family, 
			logfont_qpf_helvetica18->charset, logfont_qpf_helvetica18->size);
	TextOut (hdc, 0, 250, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_helvetica24->type, logfont_qpf_helvetica24->family, 
			logfont_qpf_helvetica24->charset, logfont_qpf_helvetica24->size);
	TextOut (hdc, 0, 270, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_fmkai16->type, logfont_qpf_fmkai16->family, 
			logfont_qpf_fmkai16->charset, logfont_qpf_fmkai16->size);
	TextOut (hdc, 0, 300, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_fmkai18->type, logfont_qpf_fmkai18->family, 
			logfont_qpf_fmkai18->charset, logfont_qpf_fmkai18->size);
	TextOut (hdc, 0, 320, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_fmkai20->type, logfont_qpf_fmkai20->family, 
			logfont_qpf_fmkai20->charset, logfont_qpf_fmkai20->size);
	TextOut (hdc, 0, 340, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_fmfsong16->type, logfont_qpf_fmfsong16->family, 
			logfont_qpf_fmfsong16->charset, logfont_qpf_fmfsong16->size);
	TextOut (hdc, 0, 370, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_fmfsong18->type, logfont_qpf_fmfsong18->family, 
			logfont_qpf_fmfsong18->charset, logfont_qpf_fmfsong18->size);
	TextOut (hdc, 0, 390, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_fmfsong20->type, logfont_qpf_fmfsong20->family, 
			logfont_qpf_fmfsong20->charset, logfont_qpf_fmfsong20->size);
	TextOut (hdc, 0, 410, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_fmsong15->type, logfont_qpf_fmsong15->family, 
			logfont_qpf_fmsong15->charset, logfont_qpf_fmsong15->size);
	TextOut (hdc, 0, 440, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_fmsong16->type, logfont_qpf_fmsong16->family, 
			logfont_qpf_fmsong16->charset, logfont_qpf_fmsong16->size);
	TextOut (hdc, 0, 460, buf1);

	sprintf (buf1, "%s-%s-%s-size%d-regular: ", 
			logfont_qpf_fmsong18->type, logfont_qpf_fmsong18->family, 
			logfont_qpf_fmsong18->charset, logfont_qpf_fmsong18->size);
	TextOut (hdc, 0, 480, buf1);


	SelectFont (hdc, logfont_qpf_Courier14);
	TextOut (hdc, 330, 0, buf2);

	SelectFont (hdc, logfont_qpf_Courier18);
	TextOut (hdc, 330, 20, buf2);

	SelectFont (hdc, logfont_qpf_Courier24);
	TextOut (hdc, 330, 40, buf2);

	SelectFont (hdc, logfont_qpf_Lucida14);
	TextOut (hdc, 330, 70, buf2);

	SelectFont (hdc, logfont_qpf_Lucida18);
	TextOut (hdc, 330, 90, buf2);

	SelectFont (hdc, logfont_qpf_Lucida19);
	TextOut (hdc, 330, 110, buf2);

	SelectFont (hdc, logfont_qpf_Lucida24);
	TextOut (hdc, 330, 130, buf2);

	SelectFont (hdc, logfont_qpf_Times14);
	TextOut (hdc, 330, 160, buf2);

	SelectFont (hdc, logfont_qpf_Times18);
	TextOut (hdc, 330, 180, buf2);

	SelectFont (hdc, logfont_qpf_Times24);
	TextOut (hdc, 330, 200, buf2);

	SelectFont (hdc, logfont_qpf_helvetica14);
	TextOut (hdc, 330, 230, buf2);

	SelectFont (hdc, logfont_qpf_helvetica18);
	TextOut (hdc, 330, 250, buf2);

	SelectFont (hdc, logfont_qpf_helvetica24);
	TextOut (hdc, 330, 270, buf2);

	memset (buf2, 0, sizeof(buf2));
	SelectFont (hdc, logfont_qpf_fmkai16);
	WCS2MBS (logfont_qpf_fmkai16, (unsigned char*)buf2, src4, TABLESIZE(src4), TABLESIZE(buf2));
	TextOut (hdc, 330, 300, buf2);

	SelectFont (hdc, logfont_qpf_fmkai18);
	TextOut (hdc, 330, 320, buf2);

	SelectFont (hdc, logfont_qpf_fmkai20);
	TextOut (hdc, 330, 340, buf2);

	SelectFont (hdc, logfont_qpf_fmfsong16);
	TextOut (hdc, 330, 370, buf2);

	SelectFont (hdc, logfont_qpf_fmfsong18);
	TextOut (hdc, 330, 390, buf2);

	SelectFont (hdc, logfont_qpf_fmfsong20);
	TextOut (hdc, 330, 410, buf2);

	SelectFont (hdc, logfont_qpf_fmsong15);
	TextOut (hdc, 330, 440, buf2);

	SelectFont (hdc, logfont_qpf_fmsong16);
	TextOut (hdc, 330, 460, buf2);

	SelectFont (hdc, logfont_qpf_fmsong18);
	TextOut (hdc, 330, 480, buf2);
#endif
}

void OnModeTextOutTTF (HWND hWnd, HDC hdc)
{
    TextOut (hdc, 0, 0, "no display because of no ttf font in MiniGUI-res ...");
}

void OnModeTabbedTextOut (HDC hdc)
{
    char buf[196];
    char buf2[64];
    SetBkColor (hdc, COLOR_lightwhite);


    memset (buf, 0, sizeof(buf));
    memset (buf2, 0, sizeof(buf2));
    sprintf (buf, "ABCD1234\tEFGHIJK\t\tLMNOPQRST\tUVWXYZ\t0123456789\n"
            "The quick fox jumps over the lazy dog.\n%s-%s-%s-size%d: ",
            logfont_rbf_gb16_song->type, logfont_rbf_gb16_song->family, logfont_rbf_gb16_song->charset,
            logfont_rbf_gb16_song->size);

    UChar32 src_gb[] = {
        0x8FD9, 0x662F, 0x5229, 0x7528, 
        0x0054, 0x0065, 0x0078, 0x0074, 0x004F, 0x0075, 0x0074,
        0x7ED8, 0x5236, 0x7684, 0x6587, 0x672C
    };
    WCS2MBS (logfont_rbf_gb16_song, (unsigned char*)buf2, src_gb, TABLESIZE(src_gb), TABLESIZE(buf));
    strcat (buf, buf2);
    SelectFont (hdc, logfont_rbf_gb16_song);
    TabbedTextOut (hdc, 0, 0, buf);


    memset (buf, 0, sizeof(buf));
    memset (buf2, 0, sizeof(buf2));
    sprintf (buf, "ABCD1234\tEFGHIJK\t\tLMNOPQRST\tUVWXYZ\t0123456789\n"
            "The quick fox jumps over the lazy dog.\n%s-%s-%s-size%d: ",
            logfont_rbf_big5_16->type, logfont_rbf_big5_16->family, logfont_rbf_big5_16->charset,
            logfont_rbf_big5_16->size);
    UChar32 src_big5[] = {
        0x9019, 0x662F, 0x5229, 0x7528, 
        0x0054, 0x0065, 0x0078, 0x0074, 0x004F, 0x0075, 0x0074,
        0x7E6A, 0x5236, 0x7684, 0x6587, 0x672C
    };
    WCS2MBS (logfont_rbf_big5_16, (unsigned char*)buf2, src_big5, TABLESIZE(src_big5), TABLESIZE(buf));
    strcat (buf, buf2);
    SelectFont (hdc, logfont_rbf_big5_16);
    TabbedTextOut (hdc, 0, 100, buf);
}

void OnModeTabbedTextOutEx (HDC hdc)
{
    int i;
    int pos = 0;
    int tab_stops [] = {40, 40, 70, 40, 70, 90};

    SetBkColor (hdc, COLOR_lightwhite);

    MoveTo (hdc, 0, 5);
    LineTo (hdc, 600, 5);

    for (i = 0; i < 6; i++) {
        pos += tab_stops [i];
        MoveTo (hdc, pos, 2);
        LineTo (hdc, pos, 8);
    }

    TabbedTextOutEx (hdc, 0, 10, "ABCD\tEFGHIJK\t\tLMNOPQRST\tUVWXYZ\t\t0123456789\t", -1,
            6, tab_stops, 0);
    TabbedTextOutEx (hdc, 0, 50, "ABCD\tEFGHIJK\t\tLMNOPQRST\t\tUVWXYZ\t\t0123456789", -1,
            6, tab_stops, 10);
    TabbedTextOutEx (hdc, 0, 90, "ABCD\tEFGHIJK\t\tLMNOPQRST\n\t\tUVWXY\t\t0123456789", -1,
            6, tab_stops, 0);
    TabbedTextOutEx (hdc, 0, 150, "The\tquick\tfox\tjumps\tover\tthe\tlazy\tdog.", -1,
            6, tab_stops, 0);

    tab_stops [0] = 200;
    TabbedTextOutEx (hdc, 0, 200, "TabbedTextOutEx:\tChange the first tabstop from 40 to 200\n"
            "\tThe quick fox jumps over the lazy dog.\n"
            "\t~!@#$%^&*()_+|{}:", -1, 6, tab_stops, 0);

    SetTextCharacterExtra (hdc, 3);
    TabbedTextOutEx (hdc, 0, 270, "TabbedTextOutEx:\tSetTextCharacterExtra = 3\n"
            "\tThe quick fox jumps over the lazy dog.\n"
            "\t~!@#$%^&*()_+|{}:", -1, 6, tab_stops, 0);

    SetTextCharacterExtra (hdc, 0);
    SetTextAboveLineExtra (hdc, 10);
    TabbedTextOutEx (hdc, 0, 330, "TabbedTextOutEx:\tSetTextAboveLineExtra = 30\n"
            "\tThe quick fox jumps over the lazy dog.\n"
            "\t~!@#$%^&*()_+|{}:", -1, 6, tab_stops, 0);

    SetTextAboveLineExtra (hdc, 0);
    SetTextBellowLineExtra (hdc, 20);
    TabbedTextOutEx (hdc, 0, 430, "TabbedTextOutEx:\tSetTextBellowLineExtra = 30\n"
            "\tThe quick fox jumps over the lazy dog.\n"
            "\t~!@#$%^&*()_+|{}:", -1, 6, tab_stops, 0);
    SetTextCharacterExtra (hdc, 0);
    SetTextAboveLineExtra (hdc, 0);
    SetTextBellowLineExtra (hdc, 0);
}

void OnModeDrawText (HDC hdc)
{
    RECT rc1, rc2, rc3, rc4;
    const char* szBuff1 = "This is a good day. \n"
        "这是利用 DrawText 绘制的　文本, 使用字体 GB2312 Song 12. "
        "文本垂直靠上, 水平居中";
    const char* szBuff2 = "This is a good day. \n"
        "这是利用 DrawText 绘制的　文本, 使用字体 GB2312 Song 16. \n"
        "文本垂直靠上, 水平靠右";
    const char* szBuff3 = "单行文本垂直居中, 水平居中";
    const char* szBuff4 = 
        "这是利用 DrawTextEx 绘制的文本, 使用字体 GB2312 Song 16. "
        "首行缩进值为 32. 文本垂直靠上, 水平靠左";

    rc1.left = 1; rc1.top  = 1; rc1.right = 401; rc1.bottom = 101;
    rc2.left = 0; rc2.top  = 110; rc2.right = 401; rc2.bottom = 251;
    rc3.left = 0; rc3.top  = 261; rc3.right = 401; rc3.bottom = 351;
    rc4.left = 0; rc4.top  = 361; rc4.right = 401; rc4.bottom = 451;

    SetBkColor (hdc, COLOR_lightwhite);

    Rectangle (hdc, rc1.left, rc1.top, rc1.right, rc1.bottom);
    Rectangle (hdc, rc2.left, rc2.top, rc2.right, rc2.bottom);
    Rectangle (hdc, rc3.left, rc3.top, rc3.right, rc3.bottom);
    Rectangle (hdc, rc4.left, rc4.top, rc4.right, rc4.bottom);

    InflateRect (&rc1, -1, -1);
    InflateRect (&rc2, -1, -1);
    InflateRect (&rc3, -1, -1);
    InflateRect (&rc4, -1, -1);

    SelectFont (hdc, logfont_rbf_gb12_song);
    DrawText (hdc, szBuff1, -1, &rc1, DT_NOCLIP | DT_CENTER | DT_WORDBREAK);

    SelectFont (hdc, logfont_rbf_gb16_song);
    DrawText (hdc, szBuff2, -1, &rc2, DT_NOCLIP | DT_RIGHT | DT_WORDBREAK);

    DrawText (hdc, szBuff3, -1, &rc3, DT_NOCLIP | DT_SINGLELINE | DT_CENTER | DT_VCENTER);

    DrawTextEx (hdc, szBuff4, -1, &rc4, 32, DT_LEFT | DT_WORDBREAK);

}

static void OnModeRotation (HWND hWnd, HDC hdc)
{
    int degrees, i, x, y;
    PLOGFONT logfont;
    RECT client;

    GetClientRect (hWnd, &client);

    SetBkMode (hdc, BM_TRANSPARENT);

    srandom (time(NULL));
    for (i = 0; i < 500; i++) {
        x = random() % (RECTW (client));
        y = random() % (RECTH (client));
        SetTextColor(hdc, RGB2Pixel (hdc, random() % 0x100, 
                    random() % 0x100,
                    random() % 0x100));
        TextOut (hdc, x, y, "MiniGUI");
    }

    SetPenColor(hdc, COLOR_black);
    for (i = 0; i < 400; i+=1) {
        Rectangle (hdc, 400 - i, 340 - i, 400 + i, 340 + i);
    }

    SetBkColor(hdc, COLOR_black);
    for (degrees = 0; degrees < 360; degrees += 10) {
        logfont = CreateLogFont ("ttf", "times", "ISO8859-1", 
                FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, FONT_SETWIDTH_NORMAL,
                FONT_SPACING_CHARCELL, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                36, degrees * 10);
        SelectFont (hdc, logfont);
        i = degrees / 10;
        SetTextColor (hdc, SysPixelIndex [i % 15 + 1]);
        TextOut (hdc, 400, 320, "MiniGUI IS GREAT!");
        DestroyLogFont (logfont);
    }

}



int FontTestMainWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    DEVFONT* devfont[68];
    static int mode = MODE_TEXTOUTRBF;
    static int textmodedire = TEXTOUT_L2R; 
    switch (message) {
        case MSG_CREATE:

            for (i = 0; i < TABLESIZE(FontName); i++)
            {
                devfont[i] = LoadDevFontFromFile (FontName[i].devfontname, FontName[i].filename);
            }
            CreateFont ();

            if (LoadBitmap (HDC_SCREEN, &digit_bmp, BMP_FILE_DIGIT)) {
                fprintf (stderr, "Fail to load bitmap. \n");
                return 1;
            }

            if (LoadBitmap (HDC_SCREEN, &letter_bmp, BMP_FILE_LETTER)) {
                fprintf (stderr, "Fail to load bitmap. \n");
                return 1;
            }

            if (LoadBitmap (HDC_SCREEN, &symbol_bmp, BMP_FILE_SYMBOL)) {
                fprintf (stderr, "Fail to load bitmap. \n");
                return 1;
            }

            if (LoadBitmap (HDC_SCREEN, &ddot_bmp, BMP_FILE_DDOT)) {
                fprintf (stderr, "Fail to load bitmap. \n");
                return 1;
            }

            dev_font = CreateBMPDevFont ("bmp-led-rrncnn-10-15-ISO8859-1",
                    &letter_bmp, "A", 6, 10);
            AddGlyphsToBMPFont (dev_font, &digit_bmp, "0", 10, 10);
            AddGlyphsToBMPFont (dev_font, &symbol_bmp, "+", 4, 10);
            AddGlyphsToBMPFont (dev_font, &ddot_bmp, ":", 1, 10);

            logfont_bmp = CreateLogFont (FONT_TYPE_NAME_BITMAP_BMP, "led",
                    "ISO8859-1",
                    FONT_WEIGHT_BOLD, FONT_SLANT_ITALIC,
                    FONT_SETWIDTH_NORMAL, FONT_SPACING_CHARCELL,
                    FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
                    10, 0);
            break;

        case MSG_PAINT:
            {
                HDC hdc;

                hdc = BeginPaint (hWnd);

                switch (mode) {
                    case MODE_TEXTOUTRBF:
                        OnModeTextOutRBF (hdc);
                        break;
                    case MODE_TEXTOUTVBF:
                        OnModeTextOutVBF (hdc);
                        break;
                    case MODE_TEXTOUTUPF:
                        OnModeTextOutUPF (hdc);
                        break;
                    case MODE_TEXTOUTQPF:
                        OnModeTextOutQPF (hdc);
                        break;
                    case MODE_TEXTOUTTTF:
                        OnModeTextOutTTF(hWnd,hdc);
                        break;
                    case MODE_TABBEDTEXTOUT:
                        OnModeTabbedTextOut (hdc);
                        break;
                    case MODE_TABBEDTEXTOUTEX:
                        OnModeTabbedTextOutEx (hdc);
                        break;
                    case MODE_DRAWTEXT:
                        OnModeDrawText (hdc);
                        break;
                    case MODE_ROTATION:
                        OnModeRotation (hWnd, hdc);
                        break;
                    case MODE_ARABIC:
                        OnModeArabic (hWnd, hdc, textmodedire, logfont_iso8859_6);
                        break;
                    case MODE_HEBREW:
                        OnModeHebrew (hWnd, hdc, textmodedire, logfont_iso8859_8);
                        break;
                    case MODE_JAPAN:
                        OnModeJapan (hWnd, hdc, textmodedire, logfont_jisx0208);
                        break;
                    case MODE_HANGUL:
                        OnModeHangul (hWnd, hdc, textmodedire, logfont_ksc5601_0);
                        break;
                    case MODE_THAI:
                        OnModeThai (hWnd, hdc, textmodedire, logfont_iso8859_11);
                        break;
                    case MODE_BMPFONT:
                        OnModeBmpfont (hWnd, hdc, logfont_bmp);
                        break;
                }

                EndPaint (hWnd, hdc);
                return 0;
            }

        case MSG_COMMAND:
            switch (wParam) 
            {
                case IDM_EXIT:
                    SendMessage (hWnd, MSG_CLOSE, 0, 0L);
                    break;

                case IDM_TEXTOUTRBF:
                    mode = MODE_TEXTOUTRBF;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;

                case IDM_TEXTOUTVBF:
                    mode = MODE_TEXTOUTVBF;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;

                case IDM_TEXTOUTUPF:
                    mode = MODE_TEXTOUTUPF;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;

                case IDM_TEXTOUTQPF:
                    mode = MODE_TEXTOUTQPF;
                    InvalidateRect (hWnd,NULL,TRUE);
                    break;

                case IDM_TEXTOUTTTF:
                    mode =MODE_TEXTOUTTTF;
                    InvalidateRect(hWnd,NULL,TRUE);
                    break;

                case IDM_TABBEDTEXTOUT:
                    mode = MODE_TABBEDTEXTOUT;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;

                case IDM_TABBEDTEXTOUTEX:
                    mode = MODE_TABBEDTEXTOUTEX;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;

                case IDM_DRAWTEXT:
                    mode = MODE_DRAWTEXT;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;

                case IDM_ROTATION:
                    mode = MODE_ROTATION;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;

                case IDM_ARABIC:
                    mode = MODE_ARABIC;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;
                case IDM_HEBREW:
                    mode = MODE_HEBREW;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;
                case IDM_JAPAN:
                    mode = MODE_JAPAN;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;
                case IDM_HANGUL:
                    mode = MODE_HANGUL;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;
                case IDM_THAI:
                    mode = MODE_THAI;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break; 
                case IDM_BMPFONT:
                    mode = MODE_BMPFONT;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;
                case IDM_TEXT_R2L:
                    textmodedire = TEXTOUT_R2L;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;
                case IDM_TEXT_L2R:
                    textmodedire = TEXTOUT_L2R;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;
                case IDM_DRAW_R2L:
                    textmodedire = DRAWTEXT_R2L;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;
                case IDM_DRAW_L2R:
                    textmodedire = DRAWTEXT_L2R;
                    InvalidateRect (hWnd, NULL, TRUE);
                    break;
                case IDM_ABOUT:
                    break;
            }
            break;

        case MSG_CLOSE:
            
            if (MessageBox (hWnd, 
                        FT_ST_QUIT, 
                        FT_ST_CAP, 
                        MB_YESNOCANCEL | MB_ICONQUESTION) != IDYES)
                return 0;

            UnloadBitmap (&digit_bmp);
            UnloadBitmap (&letter_bmp);
            UnloadBitmap (&symbol_bmp);
            UnloadBitmap (&ddot_bmp);
            if (dev_font != NULL)
                DestroyBMPFont (dev_font);

            DestroyFont();
            for (i = 0; i < TABLESIZE(FontName); i++)
                DestroyDynamicDevFont (&devfont[i]);

            DestroyLogFont (logfont_bmp);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            
            return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_SYSMENU;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = FT_ST_CAP;
    pCreateInfo->hMenu = createmenu ();
    pCreateInfo->hCursor = GetSystemCursor (IDC_ARROW);
    pCreateInfo->hIcon = GetSmallSystemIcon (IDI_APPLICATION);
    pCreateInfo->MainWindowProc = FontTestMainWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 800;
    pCreateInfo->by = 600;
    pCreateInfo->iBkColor = COLOR_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain (int args, const char* arg[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

#ifdef _MGRM_PROCESSES
    int i;
    const char* layer = NULL;

    for (i = 1; i < args; i++) {
        if (strcmp (arg[i], "-layer") == 0) {
            layer = arg[i + 1];
            break;
        }
    }

    GetLayerInfo (layer, NULL, NULL, NULL);

    if (JoinLayer (layer, arg[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }

    if (!InitVectorialFonts ()) {
        printf ("InitVectorialFonts: error.\n");
        exit (2);
    }

#endif

    InitCreateInfo (&CreateInfo);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        exit (3);

    ShowWindow (hMainWnd, SW_SHOWNORMAL);
    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

#ifndef _MGRM_THREADS
    TermVectorialFonts ();
#endif
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
