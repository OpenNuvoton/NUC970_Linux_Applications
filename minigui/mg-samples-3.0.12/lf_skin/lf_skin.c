/*
** $Id$
**
** skindemo.c: Sample program for MiniGUI Programming Guide
**      Usage of SKIN.
**
** Copyright (C) 2004 ~ 2008 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _MGLF_RDR_SKIN

#include "lf_skin.h"

#ifdef _LANG_ZHCN
#include "lf_skin_res_cn.h"
#elif defined _LANG_ZHTW
#include "lf_skin_res_tw.h"
#else
#include "lf_skin_res_en.h"
#endif

static HWND g_hHelpWin;
static HWND g_hMainWin;
static HWND g_hTreeView;
static HWND g_hSpinBox;
static HWND g_hTooltip;

static int g_nSkinType;

static WNDPROC old_btn_proc;
static WNDPROC old_trackbar_proc;

static CTRLDATA CtrlHelpInfo [] =
{
    {
        CTRL_STATIC,
        WS_VISIBLE | SS_LEFT, 
        20, 10, 240, 100,
        IDC_INFO,
        "",
        0,
        WS_EX_NONE
    },
    {
        CTRL_SPINBOX,             
        SPS_AUTOSCROLL | WS_CHILD | WS_VISIBLE,
        250, 130, 20, 20,
        IDC_SPINBOX,
        0L, 
        WS_EX_NONE
    }
};

static CTRLDATA CtrlHelpList [] =
{ 
    {
        "treeview", 
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
        10, 10, 240, 110,
        IDC_TREEVIEW,
        "TreeView",
        (DWORD)&ListViewInfo,
        WS_EX_NONE
    },
    {
        "button", 
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        100, 125, 60, 23,
        IDC_SKIN_BUTTON,
        "",
        0L,
        WS_EX_NONE
    }
};

static DLGTEMPLATE HelpInfo =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_TRANSPARENT,
    0, 0, 280, 180,
    "",
    0, 0,
    2, NULL,
    0
};

static int HelpListPageProc (HWND hWnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
    switch (nMessage) 
    {
        case MSG_INITPAGE:
            {
                int i;
                int item;
                HWND hButtonWnd;
                TVITEMINFO tvItemInfo;

                hButtonWnd = GetDlgItem (hWnd, IDC_SKIN_BUTTON);
                g_hTreeView = GetDlgItem (hWnd, IDC_TREEVIEW);

                SetWindowCaption (hButtonWnd, g_pTitleButton);
                for (i = 0; i < CHAPTER_NUM; i++) {
                    tvItemInfo.text = (char*)chapter[i];
                    item = SendMessage (g_hTreeView, TVM_ADDITEM, 0, (LPARAM)&tvItemInfo);
                    tvItemInfo.text = (char*)section[i];
                    SendMessage (g_hTreeView, TVM_ADDITEM, item, (LPARAM)&tvItemInfo);
                }

                break;
            }
        case MSG_SHOWPAGE:
            return 1;
        case MSG_COMMAND:
            {
                if (wParam == IDC_SKIN_BUTTON)
                {
                    if (g_nSkinType ==  SKINS_BLUE)
                        g_nSkinType = SKINS_BLACK;
                    else 
                        g_nSkinType = SKINS_BLUE;
                    
                    SendMessage (g_hHelpWin, MSG_CHANGE_SKINS, 0, 0); 
                    SendMessage (g_hMainWin, MSG_CHANGE_SKINS, 0, 0); 
                }
                break;
            }
    }

    return DefaultPageProc (hWnd, nMessage, wParam, lParam);
}

static int HelpInfoPageProc (HWND hWnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
    switch (nMessage) 
    {
        case MSG_INITPAGE:
            {
                SPININFO stSpin;
                
                HWND hStatic = GetDlgItem (hWnd, IDC_INFO);
                g_hSpinBox = GetDlgItem (hWnd, IDC_SPINBOX);
                
                SetWindowCaption (hStatic , help_info_text);
                stSpin.min = 0;
                stSpin.max = 3;
                stSpin.cur = 0;
                SendMessage (g_hSpinBox, SPM_SETTARGET, 0, (LPARAM)hStatic);
                SendMessage (g_hSpinBox, SPM_SETINFO, 0, (LPARAM)&stSpin);
                
                break;
            }
        case MSG_SHOWPAGE:
            return 1;

        case MSG_KEYDOWN:
        {
            int cur;
            
            if (wParam == SCANCODE_CURSORBLOCKUP || 
                        wParam == SCANCODE_CURSORBLOCKDOWN) {
                if (!(lParam & KS_SPINPOST)) {
                    cur = SendMessage (g_hSpinBox, SPM_GETCUR, 0, 0);
                    if (wParam == SCANCODE_CURSORBLOCKUP)
                        cur --;
                    else
                        cur ++;
                    SendMessage (g_hSpinBox, SPM_SETCUR, cur, 0);
                }
            } else if (wParam == SCANCODE_PAGEDOWN || 
                    wParam == SCANCODE_PAGEUP) {
                if (!(lParam & KS_SPINPOST)) {
                    cur = SendMessage (g_hSpinBox, SPM_GETCUR, 0, 0);
                    if (wParam == SCANCODE_PAGEUP)
                        cur -= 4;
                    else
                        cur += 4;
                    if (cur < 0) cur = 0;
                    else if (cur > 10) cur = 10;
                    SendMessage (g_hSpinBox, SPM_SETCUR, cur, 0);
                }
            }
            
            break;
        }
    }

    return DefaultPageProc (hWnd, nMessage, wParam, lParam);
}

static int HelpProc (HWND hWnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
    static HWND s_hPropSheet;

    switch (nMessage)
    {
        case MSG_CREATE:
            {
                g_hHelpWin = hWnd;

                s_hPropSheet = CreateWindowEx (CTRL_PROPSHEET, NULL,
                        WS_VISIBLE | PSS_COMPACTTAB, WS_EX_NONE,
                        IDC_PROPSHEET,
                        10, 10, 280, 180,
                        hWnd, 0);

                HelpInfo.controls   = CtrlHelpList;
                HelpInfo.caption    = g_pHelpList;
                HelpInfo.dwAddData  = PAGE_HELPLIST;
                SendMessage (s_hPropSheet, PSM_ADDPAGE, 
                        (WPARAM)&HelpInfo, (LPARAM)HelpListPageProc);

                HelpInfo.controls   = CtrlHelpInfo;
                HelpInfo.caption    = g_pHelpInfo;
                HelpInfo.dwAddData  = PAGE_INFO;
                SendMessage (s_hPropSheet, PSM_ADDPAGE, 
                        (WPARAM)&HelpInfo, (LPARAM)HelpInfoPageProc);

                SendMessage (hWnd, MSG_CHANGE_SKINS, 0, 0); 
                break;
            }
        case MSG_CLOSE:
            {
                g_hHelpWin = HWND_NULL;
                DestroyMainWindow(hWnd);
                break;
            }
        case MSG_CHANGE_SKINS:
            { 
                if (g_nSkinType ==  SKINS_BLACK)
                {
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_WND_BKGND, (DWORD)LFSKIN_OTHER_HELPBACK);
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_CAPTION, (DWORD)LFSKIN_OTHER_CAPTION);
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_CAPTION_BTN, (DWORD)LFSKIN_OTHER_CAPTIONBTN);
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_BORDER_RIGHT, (DWORD)LFSKIN_OTHER_RIGHTBORDER);
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_BORDER_LEFT, (DWORD)LFSKIN_OTHER_LEFTBORDER);
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_BORDER_BOTTOM, (DWORD)LFSKIN_OTHER_BOTTOMBORDER);

                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_WND_BKGND, (DWORD)LFSKIN_OTHER_HELPTREEBACK);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_ARROWS, (DWORD)LFSKIN_OTHER_SB_ARROWS);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_HTHUMB, (DWORD)LFSKIN_OTHER_SB_HTHUMB);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_VTHUMB, (DWORD)LFSKIN_OTHER_SB_VTHUMB);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_VSHAFT, (DWORD)LFSKIN_OTHER_SB_VSHAFT);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_HSHAFT, (DWORD)LFSKIN_OTHER_SB_HSHAFT);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_TREE, (DWORD)LFSKIN_OTHER_TREE);

                    SetWindowElementAttr (s_hPropSheet, 
                            WE_LFSKIN_TAB, (DWORD)LFSKIN_OTHER_TAB);
                    SetWindowElementAttr (s_hPropSheet, 
                            WE_LFSKIN_WND_BKGND,(DWORD)LFSKIN_OTHER_HELPBACK);

                    SetWindowElementAttr (g_hSpinBox, 
                            WE_LFSKIN_ARROWS_SHELL, (DWORD)LFSKIN_OTHER_ARROW_SHAFT);
                    SetWindowElementAttr (g_hSpinBox, 
                            WE_LFSKIN_ARROWS, (DWORD)LFSKIN_OTHER_ARROWS);
                }
                else
                {
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_WND_BKGND, (DWORD)LFSKIN_HELPBACK);
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_CAPTION, (DWORD)LFSKIN_CAPTION);
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_CAPTION_BTN, (DWORD)LFSKIN_CAPTIONBTN);

                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_BORDER_RIGHT, (DWORD)LFSKIN_RIGHTBORDER);
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_BORDER_LEFT, (DWORD)LFSKIN_LEFTBORDER);
                    SetWindowElementAttr (g_hHelpWin, 
                            WE_LFSKIN_BORDER_BOTTOM, (DWORD)LFSKIN_BOTTOMBORDER);

                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_WND_BKGND, (DWORD)LFSKIN_HELPBACK);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_ARROWS, (DWORD)LFSKIN_SB_ARROWS);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_HTHUMB, (DWORD)LFSKIN_SB_HTHUMB);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_VTHUMB, (DWORD)LFSKIN_SB_VTHUMB);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_VSHAFT, (DWORD)LFSKIN_SB_VSHAFT);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_SCROLLBAR_HSHAFT, (DWORD)LFSKIN_SB_HSHAFT);
                    SetWindowElementAttr (g_hTreeView, 
                            WE_LFSKIN_TREE, (DWORD)LFSKIN_TREE);

                    SetWindowElementAttr (s_hPropSheet, 
                            WE_LFSKIN_TAB, (DWORD)LFSKIN_TAB);
                    SetWindowElementAttr (s_hPropSheet, 
                            WE_LFSKIN_WND_BKGND, (DWORD)LFSKIN_HELPBACK);

                    SetWindowElementAttr (g_hSpinBox, 
                            WE_LFSKIN_ARROWS_SHELL, (DWORD)LFSKIN_ARROW_SHAFT);
                    SetWindowElementAttr (g_hSpinBox, 
                            WE_LFSKIN_ARROWS, (DWORD)LFSKIN_ARROWS);
                }

                InvalidateRect (g_hTreeView, NULL, FALSE);
                InvalidateRect (s_hPropSheet, NULL, FALSE);
                UpdateWindow (hWnd, TRUE);
                return 0;
            }
    }
    return DefaultMainWinProc(hWnd, nMessage, wParam, lParam);
}

static int CreateHelpWin (HWND hWnd)
{
   // MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle      = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle    = WS_EX_TROUNDCNS;
    CreateInfo.spCaption    = g_pHelpTitle;
    CreateInfo.hMenu        = 0;
    CreateInfo.hCursor      = GetSystemCursor(0);
    CreateInfo.hIcon        = 0;
    CreateInfo.MainWindowProc = HelpProc;
    CreateInfo.lx           = 100;
    CreateInfo.ty           = 100;
    CreateInfo.rx           = 400 + 6;
    CreateInfo.by           = 300 + 25; 
    CreateInfo.iBkColor     = COLOR_lightgray;
    CreateInfo.dwAddData    = 0;
    CreateInfo.hHosting     = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

#if 0
    ShowWindow(hMainWnd, SW_SHOWNORMAL);
    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
#endif
    return 1;
}

static int DefButtonProc (HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    if (message == MSG_ERASEBKGND) {
        return 0;
    }
    else if (message == MSG_MOUSEMOVEIN) {
        RECT rc;
        BOOL in_out = (BOOL)wParam;
        char *tip = (char *)GetWindowAdditionalData (hwnd);
       
        if (NULL != tip && in_out) {
            GetClientRect (hwnd, &rc); 
            ClientToScreen (hwnd, &rc.right, &rc.bottom);
            ResetToolTipWin (g_hTooltip, rc.right, rc.bottom, tip);
        }
    }

    return (*old_btn_proc) (hwnd, message, wParam, lParam);
}

static int DefTrackBarProc (HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    if (message == MSG_ERASEBKGND) {
        return 0;
    }

    return (*old_trackbar_proc) (hwnd, message, wParam, lParam);
}

#if 0
static int OpenDlgProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    if (message == MSG_NCCREATE)
    {
        SetWindowElementAttr (hDlg, WE_LFSKIN_WND_BKGND, (DWORD)LFSKIN_HELPBACK);
        SetWindowElementAttr (hDlg, WE_LFSKIN_CAPTION, (DWORD)LFSKIN_CAPTION);
        SetWindowElementAttr (hDlg, WE_LFSKIN_CAPTION_BTN, (DWORD)LFSKIN_CAPTIONBTN);
        SetWindowElementAttr (hDlg, WE_LFSKIN_BORDER_RIGHT, (DWORD)LFSKIN_RIGHTBORDER);
        SetWindowElementAttr (hDlg, WE_LFSKIN_BORDER_LEFT, (DWORD)LFSKIN_LEFTBORDER);
        SetWindowElementAttr (hDlg, WE_LFSKIN_BORDER_BOTTOM, (DWORD)LFSKIN_BOTTOMBORDER);
    }
    return DefFileDialogProc(hDlg, message, wParam, lParam);
}
#endif

static int SkinDemoProc (HWND hWnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
    static BOOL s_bPlay;
    static BOOL s_bVolume;
    
    static int s_nTMin;
    static int s_nTSec;
    static int s_nProgress;
    
    static HWND s_hStart;
    static HWND s_hStop;
    static HWND s_hVolume;
    static HWND s_hTrackBar;
    static HWND s_hProgress;
    static HWND s_hConfig;
    static HWND s_hHelp;
    
    static BITMAP s_stShowHead;
    static BITMAP s_stBMPNumber;
    static BITMAP s_stBMPInsert;
    
    static DEVFONT  *s_bmpDevFont; 
    static LOGFONT  *s_bmpLogFont;

    switch (nMessage)
    {
        case MSG_CREATE:
            {
                g_hMainWin = hWnd;
                g_hHelpWin = HWND_NULL;
                
                s_bPlay     = TRUE;
                s_bVolume   = TRUE;
                s_nTMin     = 0;
                s_nTSec     = 0;
                s_nProgress = 0;
                g_nSkinType = SKINS_BLUE;
                
                LoadBitmap (HDC_SCREEN, &s_stShowHead, "res/lfskin_ShowHead.gif");
                LoadBitmap (HDC_SCREEN, &s_stBMPNumber, "res/lfskin_Number.bmp"); 
                LoadBitmap (HDC_SCREEN, &s_stBMPInsert, "res/lfskin_Dot.bmp");
                
                s_bmpDevFont = CreateBMPDevFont ("bmp-led-rrncnn-10-17-ISO8859-1", 
                                &s_stBMPNumber, "0", 10, 13);

                AddGlyphsToBMPFont (s_bmpDevFont, &s_stBMPInsert, ":", 1, 10);

                g_hTooltip = CreateToolTipWin (hWnd, 0, 0, 1000, "");
                SetWindowElementAttr (g_hTooltip, WE_LFSKIN_WND_BKGND, 0L);
                SetWindowBkColor (g_hTooltip, PIXEL_cyan);
                ShowWindow (g_hTooltip, SW_HIDE);
                
                s_bmpLogFont = CreateLogFont (FONT_TYPE_NAME_BITMAP_BMP, "led",
                        "ISO8859-1",
                        FONT_WEIGHT_BOLD, FONT_SLANT_ITALIC,
                        FONT_SETWIDTH_NORMAL, FONT_SPACING_CHARCELL,
                        FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
                        17, 0);
                
                s_hStart = CreateWindowEx (CTRL_BUTTON, "",
                        WS_VISIBLE | BS_PUSHBUTTON, WS_EX_NONE, 
                        IDC_START,
                        20, 96, 24, 24, 
                        hWnd, (DWORD)"Start/Pause");
                old_btn_proc = SetWindowCallbackProc (s_hStart, DefButtonProc);

                s_hStop = CreateWindowEx (CTRL_BUTTON, "",
                        WS_VISIBLE | BS_PUSHBUTTON, WS_EX_NONE, 
                        IDC_STOP,
                        70, 96, 24, 24, 
                        hWnd, (DWORD)"Stop");
                SetWindowCallbackProc (s_hStop, DefButtonProc);
                
                s_hHelp = CreateWindowEx (CTRL_BUTTON, "",
                        WS_VISIBLE | BS_PUSHBUTTON, WS_EX_NONE, 
                        IDC_SUBHELP,
                        320, 7, 21, 15, 
                        hWnd, (DWORD)"Help");
                SetWindowCallbackProc (s_hHelp, DefButtonProc);

                s_hVolume = CreateWindowEx (CTRL_BUTTON, "",
                        WS_VISIBLE | BS_PUSHBUTTON, WS_EX_NONE, 
                        IDC_VOLUME,
                        170, 97, 21, 22, 
                        hWnd, (DWORD)"Muting");
                SetWindowCallbackProc (s_hVolume, DefButtonProc);

                s_hConfig = CreateWindowEx (CTRL_BUTTON, NULL,
                        WS_VISIBLE | BS_PUSHBUTTON, WS_EX_NONE, 
                        IDC_CONFIG,
                        285, 102, 41, 15, 
                        hWnd, (DWORD)"Configure");
                SetWindowCallbackProc (s_hConfig, DefButtonProc);

                s_hTrackBar = CreateWindowEx (CTRL_TRACKBAR, NULL, 
                        WS_VISIBLE, WS_EX_NONE, 
                        IDC_TRACKBAR, 
                        200, 105, 60, 7,
                        hWnd, 0L);
                old_trackbar_proc = SetWindowCallbackProc (s_hTrackBar, DefTrackBarProc);
                SendMessage (s_hTrackBar, TBM_SETRANGE, 0, 60);
                SendMessage (s_hTrackBar, TBM_SETLINESIZE, 1, 0);
                SendMessage (s_hTrackBar, TBM_SETPAGESIZE, 10, 0);
                SendMessage (s_hTrackBar, TBM_SETTICKFREQ, 10, 0);
                SendMessage (s_hTrackBar, TBM_SETPOS, 1, 0);

                s_hProgress = CreateWindowEx ("progressbar", NULL, 
                        WS_VISIBLE, WS_EX_NONE,
                        IDC_PROGRESS,
                        10, 75, 300, 8,
                        hWnd, 0L);
                SendMessage (s_hProgress, PBM_SETRANGE, 0, 200);

                SendMessage (hWnd, MSG_CHANGE_SKINS, 0, 0); 

                SetTimer (hWnd, TIMER_SEC, 100);
                
                break;
            }
        case MSG_TIMER:
            {
                RECT text_rc = {TIME_POS_X, TIME_POS_Y, 
                    TIME_POS_X + 80, TIME_POS_Y + 40};
                
                if (s_bPlay && wParam == TIMER_SEC) {
                    s_nTSec++;
                    if (s_nTSec == 60) {
                        s_nTSec = 0;
                        s_nTMin ++;
                    }
                    if (s_nTMin == 60) {
                        s_nTMin = 0;
                        s_nTSec = 0;
                    }
                    s_nProgress ++;
                    SendMessage (s_hProgress, PBM_SETPOS, s_nProgress, 0L);

                    InvalidateRect (hWnd, &text_rc, FALSE);
                }
                break;
            }
        case MSG_PAINT:
            {
                HDC hdc;
                char cBuff[6];

                hdc = BeginPaint (hWnd);

                FillBoxWithBitmap (hdc, 11, 1, 300, 70, &s_stShowHead); 
                sprintf (cBuff, "%.2d:%.2d", s_nTMin, s_nTSec);
                cBuff [5] = '\0';
                SelectFont (hdc, s_bmpLogFont);
                TextOut (hdc, TIME_POS_X, TIME_POS_Y, cBuff);

                EndPaint (hWnd, hdc);
                return 0;
            }
        case MSG_CHANGE_SKINS:
            {
                if (g_nSkinType == SKINS_BLACK)
                {
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_BORDER_RIGHT, (DWORD)LFSKIN_OTHER_RIGHTBORDER);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_BORDER_LEFT, (DWORD)LFSKIN_OTHER_LEFTBORDER);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_BORDER_BOTTOM, (DWORD)LFSKIN_OTHER_BOTTOMBORDER);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_WND_BKGND, (DWORD)LFSKIN_OTHER_MAIN);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_CAPTION, (DWORD)LFSKIN_OTHER_CAPTION);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_CAPTION_BTN, (DWORD)LFSKIN_OTHER_CAPTIONBTN);

                    SetWindowElementAttr (s_hStop, 
                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_STOP);
                    SetWindowElementAttr (s_hConfig, 
                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_CONFIG);
                    SetWindowElementAttr (s_hHelp, 
                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_HELP);

                    SetWindowElementAttr (s_hTrackBar, 
                            WE_LFSKIN_TBSLIDER_H, (DWORD)LFSKIN_OTHER_TRACKBAR);
                    SetWindowElementAttr (s_hTrackBar, 
                            WE_LFSKIN_TRACKBAR_HORZ, (DWORD)LFSKIN_OTHER_HTRACKBAR);

                    SetWindowElementAttr (s_hProgress, 
                            WE_LFSKIN_PROGRESS_HCHUNK, (DWORD)LFSKIN_OTHER_PROTRACK);
                    SetWindowElementAttr (s_hProgress, 
                            WE_LFSKIN_PROGRESS_HTRACKBAR, (DWORD)LFSKIN_OTHER_HPROGRESSBAR);

                    if (s_bPlay)
                        SetWindowElementAttr (s_hStart, 
                                WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_PAUSE);
                    else
                        SetWindowElementAttr (s_hStart, 
                                WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_START);

                    if (s_bVolume) 
                        SetWindowElementAttr (s_hVolume, 
                                WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_VOLUME);
                    else
                        SetWindowElementAttr (s_hVolume, 
                                WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_DISVOLUME);
                }
                else
                {
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_BORDER_RIGHT, (DWORD)LFSKIN_RIGHTBORDER);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_BORDER_LEFT, (DWORD)LFSKIN_LEFTBORDER);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_BORDER_BOTTOM, (DWORD)LFSKIN_BOTTOMBORDER);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_WND_BKGND, (DWORD)LFSKIN_MAIN);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_CAPTION, (DWORD)LFSKIN_CAPTION);
                    SetWindowElementAttr (hWnd, 
                            WE_LFSKIN_CAPTION_BTN, (DWORD)LFSKIN_CAPTIONBTN);

                    SetWindowElementAttr (s_hStop, 
                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_STOP);
                    SetWindowElementAttr (s_hConfig, 
                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_CONFIG);
                    SetWindowElementAttr (s_hHelp, 
                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_HELP);

                    SetWindowElementAttr (s_hTrackBar, 
                            WE_LFSKIN_TBSLIDER_H, (DWORD)LFSKIN_TRACKBAR);
                    SetWindowElementAttr (s_hTrackBar, 
                            WE_LFSKIN_TRACKBAR_HORZ, (DWORD)LFSKIN_HTRACKBAR);

                    SetWindowElementAttr (s_hProgress, 
                            WE_LFSKIN_PROGRESS_HCHUNK, (DWORD)LFSKIN_PROTRACK);
                    SetWindowElementAttr (s_hProgress, 
                            WE_LFSKIN_PROGRESS_HTRACKBAR, (DWORD)LFSKIN_HPROGRESSBAR);

                    if (s_bPlay)
                        SetWindowElementAttr (s_hStart, 
                                WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_PAUSE);
                    else
                        SetWindowElementAttr (s_hStart, 
                                WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_START);

                    if (s_bVolume) 
                        SetWindowElementAttr (s_hVolume, 
                                WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_VOLUME);
                    else
                        SetWindowElementAttr (s_hVolume, 
                                WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_DISVOLUME);
                }

                InvalidateRect (s_hStop, NULL, FALSE);
                InvalidateRect (s_hConfig, NULL, FALSE);
                InvalidateRect (s_hHelp, NULL, FALSE);
                InvalidateRect (s_hTrackBar, NULL, FALSE);
                InvalidateRect (s_hProgress, NULL, FALSE);
                InvalidateRect (s_hStart, NULL, FALSE);
                InvalidateRect (s_hVolume, NULL, FALSE);
                UpdateWindow (hWnd, TRUE);

                return 0;
            }
        case MSG_COMMAND:
            {
                int id = LOWORD(wParam);
                switch (id) 
                {
                    case IDC_START:
                        {
                            if (!s_bPlay) {
                                s_bPlay = TRUE;
                                if (g_nSkinType == SKINS_BLUE)
                                    SetWindowElementAttr (s_hStart, 
                                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_PAUSE);
                                else
                                    SetWindowElementAttr (s_hStart, 
                                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_PAUSE);
                            }
                            else {
                                s_bPlay = FALSE;
                                if (g_nSkinType == SKINS_BLUE )
                                    SetWindowElementAttr (s_hStart, 
                                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_START);
                                else
                                    SetWindowElementAttr (s_hStart, 
                                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_START);
                            }
                            InvalidateRect (s_hStart, NULL, FALSE);
                            break;
                        }
                    case IDC_STOP:
                        {
                            s_bPlay = FALSE;
                            if (g_nSkinType == SKINS_BLUE )
                                SetWindowElementAttr (s_hStart, 
                                        WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_START);
                            else
                                SetWindowElementAttr (s_hStart, 
                                        WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_START);
                            InvalidateRect (s_hStart, NULL, FALSE);

                            s_nTMin = 0;
                            s_nTSec = 0;
                            s_nProgress = 0;
                            SendMessage (s_hProgress, PBM_SETPOS, s_nProgress, 0L);
                            break;
                        }
                    case IDC_VOLUME:
                        {
                            if (!s_bVolume) {
                                s_bVolume = TRUE;
                                if (g_nSkinType == SKINS_BLUE )
                                    SetWindowElementAttr (s_hVolume, 
                                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_VOLUME);
                                else
                                    SetWindowElementAttr (s_hVolume, 
                                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_VOLUME);
                                InvalidateRect (s_hVolume, NULL, FALSE);
                            }
                            else {
                                s_bVolume = FALSE;
                                if (g_nSkinType == SKINS_BLUE )
                                    SetWindowElementAttr (s_hVolume, 
                                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_DISVOLUME);
                                else
                                    SetWindowElementAttr (s_hVolume, 
                                            WE_LFSKIN_PUSHBUTTON, (DWORD)LFSKIN_OTHER_DISVOLUME);
                                InvalidateRect (s_hVolume, NULL, FALSE);
                            }
                            break;
                        }
                    case IDC_CONFIG:
                        {
#if 0
                            FILEDLGDATA pfdd = {0};

                            pfdd.filterindex = 1;
                            strcpy (pfdd.filepath, "./");
                            strcpy (pfdd.filter,
                                    "All file(*.*)|Text file(*.txt)|Bitmap file(*.bmp)");
                            pfdd.hook = NULL;

                            FileOpenSaveDialog  (&DefFileDlg, hWnd, OpenDlgProc, &pfdd);
#endif
                            break; 
                        }
                    case IDC_SUBHELP:
                        {
                            if (HWND_NULL == g_hHelpWin) {
                                CreateHelpWin (hWnd);
                            }
                            break; 
                        }
                }
            }
            break;
        case MSG_CLOSE:
            {
                KillTimer (hWnd, TIMER_SEC);
                DestroyBMPFont (s_bmpDevFont);
                UnloadBitmap (&s_stShowHead);
                UnloadBitmap (&s_stBMPNumber); 
                UnloadBitmap (&s_stBMPInsert);
                DestroyToolTipWin (g_hTooltip);
                DestroyAllControls (hWnd);
                DestroyMainWindow (g_hHelpWin);
                DestroyMainWindow (hWnd);
                break;
            }
    }
    return DefaultMainWinProc(hWnd, nMessage, wParam, lParam);
}

int MiniGUIMain (int argc, const char *argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
    
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "player" , 0 , 0);
#endif

    register_all_pic ();

    SetDefaultWindowElementRenderer ("skin");
    
    CreateInfo.dwStyle      = WS_VISIBLE | WS_CAPTION;
    CreateInfo.dwExStyle    = WS_EX_TROUNDCNS;
    CreateInfo.spCaption    = g_pMainTitle;
    CreateInfo.hMenu        = 0;
    CreateInfo.hCursor      = GetSystemCursor(0);
    CreateInfo.hIcon        = 0;
    CreateInfo.MainWindowProc = SkinDemoProc;
    CreateInfo.lx           = 0;
    CreateInfo.ty           = 0;
    CreateInfo.rx           = 354;
    CreateInfo.by           = 126 + 25; 
    CreateInfo.iBkColor     = COLOR_lightgray;
    CreateInfo.dwAddData    = 0;
    CreateInfo.hHosting     = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow (hMainWnd, SW_SHOWNORMAL);

    while (GetMessage (&Msg, hMainWnd)) {
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
   
    unregister_all_pic ();
    
    return 0;
}
#else
int MiniGUIMain (int argc, const char *argv[])
{
    fprintf (stderr, "Please open the support for skin LF in MiniGUI. \n");
    return 0;
}
#endif

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
