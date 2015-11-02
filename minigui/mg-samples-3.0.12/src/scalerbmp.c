/* 
** $Id: loadbmp.c 474 2008-02-15 01:44:21Z zhounuohua $
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
static BITMAP bmp2;
static HICON icon1;
static BOOL biliner_scaler = TRUE; 

static int LoadBmpWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message) {
        case MSG_CREATE:
            if (LoadBitmap (HDC_SCREEN, &bmp, "res/flower.png"))
                return -1;
            if (LoadBitmap (HDC_SCREEN, &bmp2, "res/music.gif"))
                return -1;
            icon1 = LoadIconFromFile (HDC_SCREEN, "res/audio.ico", 1);
            return 0;

        case MSG_KEYUP:
        {
            int scancode = (int)wParam;
            if (scancode == SCANCODE_F1) 
            {
                if (biliner_scaler )
                    biliner_scaler = FALSE;
                else
                    biliner_scaler = TRUE;
                InvalidateRect(hWnd, NULL, TRUE);
            }
             
            break;
        }
        case MSG_PAINT:
        {
            hdc = BeginPaint (hWnd);
            if (biliner_scaler )
                SetBitmapScalerType(hdc, BITMAP_SCALER_BILINEAR);
            else 
                SetBitmapScalerType(hdc, BITMAP_SCALER_DDA);

            SetBrushColor(hdc, PIXEL_red);
            FillBox(hdc, 0, 0, 640, 480);

            //bmp.bmType |= BMP_TYPE_ALPHA;
            //InitBitmapPixelFormat(hdc, &bmp);
            //bmp.bmColorKey = GetPixelInBitmap(&bmp, 0, 0);
            //printf("type=%x, colorkey=%x\n", bmp.bmType, bmp.bmColorKey);

            Rectangle(hdc, 9, 9, 261, 261);
            FillBoxWithBitmapPart(hdc, 10, 10, 250, 250, 250, 250, &bmp, 0, 0);

            Rectangle(hdc, 269, 9, 332, 45);
            FillBoxWithBitmap (hdc, 270, 10, 54, 34, &bmp);

            Rectangle(hdc, 269, 49, 539, 188);
            FillBoxWithBitmap (hdc, 270, 50, 268, 137, &bmp2);

            Rectangle(hdc, 9, 265, 541, 471);
            FillBoxWithBitmap (hdc, 10, 266, 530, 204, &bmp2);

            DrawIcon(hdc, 270, 200, 15, 15, icon1);
            DrawIcon(hdc, 290, 200, 40, 40, icon1);
            DrawIcon(hdc, 340, 200, 60, 54, icon1);

            EndPaint (hWnd, hdc);
            return 0;
        }
        case MSG_CLOSE:
            UnloadBitmap(&bmp);
            UnloadBitmap(&bmp2);
            DestroyIcon(icon1);
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
    JoinLayer(NAME_DEF_LAYER , "scalerbmp" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "F1 to switch scaler algorithm";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = LoadBmpWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 640;
    CreateInfo.by = 510;
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

