/*    
** palette: palette program.
** Copyright (C) 2002~2007  Feynman Software

** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.

** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.

** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define SCREEN_W    320
#define SCREEN_H    200
#define IDT_PAL     100
#define IDT_FLY     200

#define SCRLEFT 	0
#define SCRTOP 		0
#define SCRRIGHT 	1024-SCREEN_W
#define SCRBOTTOM 	768-SCREEN_H

GAL_Color palette[256];
GAL_Color temp;
int c;
HDC hdc;
HDC mem_dc;
gal_pixel pix;

static int PaletteWndProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	
	switch ( message ) {
	case MSG_SETCURSOR:
        SetCursorEx(0, FALSE);
        return 0;
 case MSG_CREATE:
        /* fill our palette with a gradually altering sequence of colors */
        for (c=0; c<64; c++) {
            palette[c].r = c*4;
            palette[c].g = 0;
            palette[c].b = 0;
        }
        for (c=64; c<128; c++) {
            palette[c].r = (127-c)*4;
            palette[c].g = (c-64)*4;
            palette[c].b = 0;
        }
        for (c=128; c<192; c++) {
            palette[c].r = 0;
            palette[c].g = (191-c)*4;
            palette[c].b = (c-128)*4;
        }
        for (c=192; c<256; c++) {
            palette[c].r = 0;
            palette[c].g = 0;
            palette[c].b = (255-c)*4;
        }
        SetTimer(hWnd, IDT_PAL, 1); 
        mem_dc = CreateCompatibleDC (hdc);
	break;
	case MSG_TIMER:
		switch(wParam){
            case IDT_PAL:
		temp = palette[255];
		for (c =255; c>0; c--)
		palette[c] = palette [c-1];
		palette[0] = temp;
		SetPalette(mem_dc, 0, 255, palette);

                for (c=255; c>0; c--){
                    pix = RGB2Pixel (mem_dc, palette[c].r, palette[c].g, palette[c].b);
                    SetBrushColor (mem_dc, pix);
                    FillCircle (mem_dc, SCREEN_W/2, SCREEN_H/2, c);
                }
		InvalidateRect (hWnd , NULL , FALSE);
                break;
        }
        break;
	case MSG_PAINT:
		hdc = BeginPaint(hWnd);	
		BitBlt (mem_dc, 0, 0, SCREEN_W, SCREEN_H, hdc, 0, 0, 0);
                
		EndPaint(hWnd, hdc);
	break;
	case MSG_CLOSE:
		KillTimer(hWnd , IDT_PAL);
		DeleteCompatibleDC(mem_dc);
		DestroyMainWindow (hWnd);
		PostQuitMessage (hWnd);
		break;
	}
		

	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#if defined(_LITE_VERSION) && !(_STAND_ALONE)

    int i;
    const char* layer =NULL;

    for (i = 1; i < argc; i++) {
        if (strcmp (argv[i], "-layer") == 0) {
            layer = argv[i + 1];
            break;
        }
    }

    //GetLayerInfo (layer, NULL, NULL, NULL);

    if (JoinLayer (layer, argv[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif


    
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Palette";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = PaletteWndProc;
    CreateInfo.lx = 512-SCREEN_W/2;
    CreateInfo.ty = 384-SCREEN_H/2;
    CreateInfo.rx = 512+SCREEN_W/2;
    CreateInfo.by = 384+SCREEN_H/2;
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

#ifndef _LITE_VERSION
#include <minigui/dti.c>
#endif

