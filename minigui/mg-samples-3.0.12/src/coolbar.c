/* 
** $Id: coolbar.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 35.1
**
** coolbar.c: Sample program for MiniGUI Programming Guide
**      Usage of COOLBAR control.
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
#include "coolbar_res_cn.h"
#elif defined _LANG_ZHTW
#include "coolbar_res_tw.h"
#else
#include "coolbar_res_en.h"
#endif

#define ITEM_NUM	10

static const char* caption[] =
{
   "0", "1", "2", "3", "4", "5","6", "7", "8", "9"
};
static void create_coolbar (HWND hWnd)
{
    HWND cb;
    COOLBARITEMINFO item;
    int i;

    cb = CreateWindow (CTRL_COOLBAR,
                    "",
                    WS_CHILD | WS_VISIBLE | WS_BORDER, 
                    100,
                    10, 100, 100, 20,
                    hWnd,
                    0);

    item.ItemType = TYPE_TEXTITEM;
    item.Bmp = NULL;
    item.dwAddData = 0;
    for (i = 0; i < ITEM_NUM; i++) {
        item.insPos = i;
        item.id = i;
        item.Caption = caption[i];
        item.ItemHint = hint[i];
        SendMessage (cb, CBM_ADDITEM, 0, (LPARAM)&item);
        if (i == 3)
            SendMessage (cb, CBM_ENABLE, i, FALSE);
    }
}

static int CoolbarWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static HWND ed;

    switch (message) {
    case MSG_CREATE:
    ed = CreateWindow (CTRL_SLEDIT,
                    "",
                    WS_CHILD | WS_VISIBLE | WS_BORDER, 
                    200,
                    10, 10, 100, 20,
                    hWnd,
                    0);

	create_coolbar (hWnd);
        break;

    case MSG_COMMAND:
	{
	    int id = LOWORD (wParam);
	    int code = HIWORD (wParam);

	    if (id == 100) {
                PostMessage (ed, MSG_CHAR, '0' + code, 0);
	    }
	}
	break;

    case MSG_DESTROY:
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
    JoinLayer(NAME_DEF_LAYER , "coolbar" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = CLB_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(IDC_ARROW);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = CoolbarWinProc;
    CreateInfo.lx = 0; 
    CreateInfo.ty = 0;
    CreateInfo.rx = 300;
    CreateInfo.by = 220;
    CreateInfo.iBkColor = GetWindowElementPixel (HWND_DESKTOP, WE_MAINC_THREED_BODY);
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
