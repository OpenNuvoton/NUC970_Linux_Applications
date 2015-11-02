/* 
** $Id: capture.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 9.2
**
** capture.c: Sample program for MiniGUI Programming Guide
**      Demo of using mouse capture
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
#include "capture_res_cn.h"
#elif defined _LANG_ZHTW
#include "capture_res_tw.h"
#else
#include "capture_res_en.h"
#endif

#define IDC_MYBUTTON    100

/* a simple button control */
static int MybuttonWindowProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static int status = 0;

    switch (message) {
    case MSG_LBUTTONDOWN:
        status = 1;
        SetCapture (hWnd);
        InvalidateRect (hWnd, NULL, TRUE);
    break;

    case MSG_LBUTTONUP:
        if (GetCapture() != hWnd)
            break;
        status = 0;
        ReleaseCapture ();
        InvalidateRect (hWnd, NULL, TRUE);
    break;

    case MSG_PAINT:
        hdc = BeginPaint (hWnd);
        if (status) {
            SetBkMode(hdc, BM_TRANSPARENT);
            TextOut(hdc,10, 0, CP_ST_PRESSED);
        }
        EndPaint(hWnd, hdc);
        return 0;

    case MSG_DESTROY:
        return 0;
    }

    return DefaultControlProc (hWnd, message, wParam, lParam);
}

BOOL RegisterMybutton (void)
{
    WNDCLASS WndClass;

    WndClass.spClassName = "mybutton";
    WndClass.dwStyle     = 0;
    WndClass.dwExStyle   = 0;
    WndClass.hCursor     = GetSystemCursor(0);
    WndClass.iBkColor    = PIXEL_lightgray;
    WndClass.WinProc     = MybuttonWindowProc;

    return RegisterWindowClass (&WndClass);
}

/* main windoww proc */
static int CaptureWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
        RegisterMybutton();
        CreateWindow ("mybutton", "", WS_VISIBLE | WS_CHILD, IDC_MYBUTTON, 
                110, 80, 70, 20, hWnd, 0);
        break;

    case MSG_CLOSE:
        DestroyAllControls (hWnd);
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
    JoinLayer(NAME_DEF_LAYER , "capture" , 0 , 0);
#endif
    
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = CP_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = CaptureWinProc;
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

