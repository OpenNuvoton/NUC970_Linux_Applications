/* 
** $Id: newtoolbar.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 27.1
**
** newtooltar.c: Sample program for MiniGUI Programming Guide
**      Usage of NEWTOOLBAR control.
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
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

#ifdef _LANG_ZHCN
#include "newtoolbar_res_cn.h"
#elif defined _LANG_ZHTW
#include "newtoolbar_res_tw.h"
#else
#include "newtoolbar_res_en.h"
#endif

#define IDC_NTB_LEFT    100
#define IDC_NTB_RIGHT   110
#define IDC_NTB_UP      120
#define IDC_NTB_SETBMP  130

static int offset = 0;
static RECT rcCircle = {0, 40, 300, 300};

static BITMAP ntb_bmp;
static BITMAP ntb_newbmp;

static void my_notif_proc (HWND hwnd, int id, int nc, DWORD add_data)
{
    NTBINFO ntb_newinfo;
    static int nFlag=0;
    gal_pixel pixel;

    if (nc == IDC_NTB_LEFT) {
        offset -= 10;
        InvalidateRect (GetParent (hwnd), &rcCircle, TRUE);
    }
    else if (nc == IDC_NTB_RIGHT) {
        offset += 10;
        InvalidateRect (GetParent (hwnd), &rcCircle, TRUE);
    }
    else if (nc == IDC_NTB_SETBMP) {
        ntb_newinfo.nr_cells = 4;
        ntb_newinfo.w_cell  = 0;
        ntb_newinfo.h_cell  = 0;
        ntb_newinfo.nr_cols = 0;
        if (nFlag ==0){
            ntb_newinfo.image = &ntb_newbmp;
            nFlag =1;
            pixel = GetPixelInBitmap (&ntb_newbmp, 0, 0);
            SetWindowBkColor (hwnd, pixel);
            InvalidateRect (hwnd, NULL, TRUE);

        }
        else {
            ntb_newinfo.image = &ntb_bmp;
            nFlag =0;
            pixel = GetPixelInBitmap (&ntb_bmp, 0, 0);
            SetWindowBkColor (hwnd, pixel);
            InvalidateRect (hwnd, NULL, TRUE);

        }
        SendMessage (hwnd, NTBM_SETBITMAP, 0, (LPARAM)&ntb_newinfo);
    }
}

static void create_new_toolbar (HWND hWnd)
{
    HWND ntb;
    NTBINFO ntb_info;
    NTBITEMINFO ntbii;
    gal_pixel pixel;

    ntb_info.nr_cells = 4;
    ntb_info.w_cell  = 0;
    ntb_info.h_cell  = 0;
    ntb_info.nr_cols = 0;
    ntb_info.image = &ntb_bmp;

    ntb = CreateWindow (CTRL_NEWTOOLBAR,
                    "",
                    WS_CHILD | WS_VISIBLE, 
                    100,
                    0, 0, 1024, 0,
                    hWnd,
                    (DWORD) &ntb_info);

    SetNotificationCallback (ntb, my_notif_proc);

    pixel = GetPixelInBitmap (&ntb_bmp, 0, 0);
    SetWindowBkColor (ntb, pixel);
    InvalidateRect (ntb, NULL, TRUE);

    memset (&ntbii, 0, sizeof (ntbii));
    ntbii.flags = NTBIF_PUSHBUTTON;
    ntbii.id = IDC_NTB_LEFT;
    ntbii.bmp_cell = 1;
    SendMessage(ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);

    ntbii.flags = NTBIF_PUSHBUTTON;
    ntbii.id = IDC_NTB_RIGHT;
    ntbii.bmp_cell = 2;
    SendMessage (ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);

    ntbii.flags = NTBIF_PUSHBUTTON;
    ntbii.id = IDC_NTB_SETBMP;
    ntbii.bmp_cell = 3;
    SendMessage (ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);
    
    ntbii.flags = NTBIF_SEPARATOR;
    ntbii.id = 0;
    ntbii.bmp_cell = 0;
    ntbii.text = NULL;
    SendMessage (ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);

    ntbii.flags = NTBIF_PUSHBUTTON | NTBIF_DISABLED;
    ntbii.id = IDC_NTB_UP;
    ntbii.bmp_cell = 0;
    SendMessage (ntb, NTBM_ADDITEM, 0, (LPARAM)&ntbii);
}

static int ToolBarWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        if (LoadBitmap (HDC_SCREEN, &ntb_bmp, "res/new2.jpg"))
            return -1;

        if (LoadBitmap (HDC_SCREEN, &ntb_newbmp, "res/new3.jpg"))
            return -1;

        create_new_toolbar (hWnd);
        break;

    case MSG_PAINT:
    {
        HDC hdc = BeginPaint (hWnd);

        ClipRectIntersect (hdc, &rcCircle);

        SetBrushColor (hdc, PIXEL_blue);
        FillCircle (hdc, 140 + offset, 120, 50);
        EndPaint (hWnd, hdc);
        return 0;
    }

    case MSG_DESTROY:
        UnloadBitmap (&ntb_bmp);
        UnloadBitmap (&ntb_newbmp);
        DestroyAllControls (hWnd);
        return 0;

    case MSG_CLOSE:
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "newtoolbar" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = CAPTION;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(IDC_ARROW);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = ToolBarWinProc;
    CreateInfo.lx = 0; 
    CreateInfo.ty = 0;
    CreateInfo.rx = 300;
    CreateInfo.by = 220;
    CreateInfo.iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    CreateInfo.dwAddData = 0;
    CreateInfo.dwReserved = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
