/* 
** $Id: loadbmp.c 741 2009-03-31 07:16:18Z weiym $
**
** loadbmp.c: Sample program for MiniGUI Programming Guide
**         Load and display a bitmap.
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <stdlib.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _LANG_ZHCN
#include "loadbmp_res_cn.h"
#elif defined _LANG_ZHTW
#include "loadbmp_res_tw.h"
#else
#include "loadbmp_res_en.h"
#endif

static BITMAP bmp;

static int LoadBmpWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message) {
        case MSG_CREATE:
            if (LoadBitmap (HDC_SCREEN, &bmp, "res/new2.jpg"))
                return -1;
            return 0;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);

            FillBoxWithBitmap (hdc, 0, 0, 100, 100, &bmp);
            Rectangle (hdc, 0, 0, 100, 100);

            FillBoxWithBitmap (hdc, 100, 0, 200, 200, &bmp);
            Rectangle (hdc, 100, 0, 300, 200);

            FillBoxWithBitmapPart (hdc, 0, 200, 150, 150, 200, 200, &bmp, 10, 10);
            Rectangle (hdc, 0, 200, 400, 400);

            EndPaint (hWnd, hdc);
            return 0;

        case MSG_CLOSE:
            UnloadBitmap (&bmp);
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
    JoinLayer(NAME_DEF_LAYER , "loadbmp" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = CAPTION;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = LoadBmpWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 430;
    CreateInfo.by = 420;
    CreateInfo.iBkColor = PIXEL_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow (hMainWnd, SW_SHOWNORMAL);

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

