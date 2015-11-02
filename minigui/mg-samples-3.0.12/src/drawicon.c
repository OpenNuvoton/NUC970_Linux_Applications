/* 
** $Id: drawicon.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 10.1
**
** drawicon.c: Sample program for MiniGUI Programming Guide
**      Demo of loading and drawing icon
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _LANG_ZHCN
#include "drawicon_res_cn.h"
#elif defined _LANG_ZHTW
#include "drawicon_res_tw.h"
#else
#include "drawicon_res_en.h"
#endif

static int DrawiconWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static HICON myicon_small, myicon_large;
    HDC hdc;

    switch (message) {
    case MSG_CREATE:
        myicon_small = LoadIconFromFile(HDC_SCREEN, "myicon.ico", 0);
        if (myicon_small == 0)
            fprintf (stderr, "Load icon file failure!");
        myicon_large = LoadIconFromFile(HDC_SCREEN, "myicon.ico", 1);
        if (myicon_large == 0)
            fprintf (stderr, "Load icon file failure!");
        break;

    case MSG_PAINT:
        hdc = BeginPaint(hWnd);
        if (myicon_small != 0)
            DrawIcon(hdc, 10, 10, 0, 0, myicon_small);
        if (myicon_large != 0)
            DrawIcon(hdc, 60, 60, 0, 0, myicon_large);
        EndPaint(hWnd, hdc);
        return 0;

    case MSG_CLOSE:
        DestroyIcon(myicon_small);
        DestroyIcon(myicon_large);
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int args, const char* arg[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "drawicon" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = DI_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = DrawiconWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 320;
    CreateInfo.by = 240;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
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

