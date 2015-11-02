/* 
** $Id: animation.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 36.1
**
** animation.c: Sample program for MiniGUI Programming Guide
**      Usage of ANIMATION control and GIF89a loader.
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
#include "animation_res_cn.h"
#elif defined _LANG_ZHTW
#include "animation_res_tw.h"
#else
#include "animation_res_en.h"
#endif

static const char* filename = "res/9.gif";
int ret;

static ANIMATION* anim = NULL;

static WINDOW_ELEMENT_ATTR we_attrs[] = 
{
    {WE_BGC_WINDOW, 0xFFFF0000},
    {-1, 0}
};

static int AnimationWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
    {
        anim = CreateAnimationFromGIF89aFile (HDC_SCREEN, filename);
        if (anim == NULL)
            return 1;

        SetWindowAdditionalData (hWnd, (DWORD) anim);
        CreateWindowEx (CTRL_ANIMATION, 
                          "", 
                          WS_VISIBLE | ANS_AUTOLOOP | ANS_SCALED, 
                          WS_EX_NONE,
                          100, 
                          50, 10, 120, 60, hWnd, (DWORD)anim);
        SendMessage (GetDlgItem (hWnd, 100), ANM_STARTPLAY, 0, 0);

        CreateWindowEx2 (CTRL_ANIMATION, 
                          "", 
                          WS_VISIBLE | ANS_AUTOLOOP | ANS_FITTOANI,
                          WS_EX_TRANSPARENT,
                          200, 
                          50, 160, 120, 60, hWnd, NULL, we_attrs, (DWORD)anim);
        break;
    }

    case MSG_LBUTTONDOWN:
        SendMessage (GetDlgItem (hWnd, 200), ANM_SETANIMATION, 0, (LPARAM)anim);
        SendMessage (GetDlgItem (hWnd, 200), ANM_STARTPLAY, 0, 0);
	break;

    case MSG_DESTROY:
        DestroyAnimation ((ANIMATION*)GetWindowAdditionalData (hWnd), TRUE);
        DestroyAllControls (hWnd);
        return 0;

    case MSG_CLOSE:
        ret = PostQuitMessage (hWnd);
        DestroyMainWindow (hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    if (argc >= 2)
        filename = argv[1];

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "animation" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_CAPTION | WS_BORDER | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = AN_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (IDC_ARROW);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = AnimationWinProc;
    CreateInfo.lx = 0; 
    CreateInfo.ty = 0;
    CreateInfo.rx = 240;//GetGDCapability(HDC_SCREEN, GDCAP_HPIXEL);
    CreateInfo.by = 300;//GetGDCapability(HDC_SCREEN, GDCAP_VPIXEL);
    CreateInfo.iBkColor = PIXEL_lightgray;
    CreateInfo.dwAddData = 0;
    CreateInfo.dwReserved = 0;
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

