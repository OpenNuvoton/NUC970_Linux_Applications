/*    
** flame: This program demonstrates how to write directly to video memory.
**   It implements a simple fire effect, first by calling getpixel() and
**   putpixel(), then by accessing video memory directly a byte at a
**   time, and finally using block memory copy operations.

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
#include <math.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define FIRE_HOTSPOTS  60 
#define SCREEN_W 320 
#define SCREEN_H 200
#define IDT_FLAME 100
#define IDT_FLY   200
#define SCRLEFT 	0
#define SCRTOP 		0
#define SCRRIGHT 	1024-SCREEN_W
#define SCRBOTTOM 	768-SCREEN_H

/* The fire is formed from several 'hotspots' which are moved randomly
 * across the bottom of the screen.
 */

int hotspot[FIRE_HOTSPOTS]; 
static HDC draw_layer;
unsigned char* temp;
GAL_Color palette[256];
BOOL KEYDOWN=0;

/* This function updates the bottom line of the screen with a pattern
 * of varying intensities which are then moved upwards and faded out
 * by the code in main().
 */
void draw_bottom_line_of_fire(HDC hdc)
{
	int c, c2;

	/* zero the buffer */
	for (c=0; c<SCREEN_W; c++)
    	temp[c] = 0;

   	for (c=0; c<FIRE_HOTSPOTS; c++) { 
      	/* hotspots */
      	for (c2=hotspot[c]-20; c2<hotspot[c]+20; c2++)
			if ((c2 >= 0) && (c2 < SCREEN_W))
	    		temp[c2] = MIN(temp[c2] + 20-ABS(hotspot[c]-c2), 192);
		/* move the hotspots */
      	hotspot[c] += (rand() & 7) - 3; 
      	if (hotspot[c] < 0)
	 		hotspot[c] += SCREEN_W;
      	else
	 		if (hotspot[c] >= SCREEN_W)
	    		hotspot[c] -= SCREEN_W;
   		}

   	/* display the buffer */
   	for (c=0; c<SCREEN_W; c++) {
      	SetPixel(hdc, c, SCREEN_H-1, temp[c]);
    }
}


static int DrawFlameWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int c;
	int i;
	static int x= 0;
	static int y= 0;
	
	switch ( message ) {
        case MSG_SETCURSOR:
            SetCursorEx(0, FALSE);
            return 0;

        case MSG_CREATE:
            temp = (unsigned char*)malloc(sizeof(unsigned char) * SCREEN_W);
            draw_layer = CreateMemDC (SCREEN_W, SCREEN_H+30, 8, 
                            MEMDC_FLAG_SWSURFACE, 0x00, 0x00, 0x00, 0x00);
			/* fill our palette with a gradually altering sequence of colors */
           	for (c=0; c<64; c++) {
      	        palette[c].r = c*4;
      		    palette[c].g = 0;
          		palette[c].b = 0;
           	}
           	for (c=64; c<128; c++) {
                palette[c].r = 255;
                palette[c].g = (c-64)*4;
                palette[c].b = 0;
           	}
           	for (c=128; c<192; c++) {
                palette[c].r = 255;
                palette[c].g = 255;
                palette[c].b = (c-192)*4;
           	}
           	for (c=192; c<256; c++) {
            	palette[c].r = 255;
            	palette[c].g = 255;
            	palette[c].b = 255;
           	}
            SetPalette (draw_layer, 0, 255, palette);

            if (!temp)
 		return -1;
            SetTimer(hWnd,IDT_FLAME,2);
            for (i=0; i<FIRE_HOTSPOTS; i++)
      			hotspot[i] = rand() % SCREEN_W;

		SetTimer(hWnd, IDT_FLY, 10);
		break;
	case MSG_TIMER:
        switch (wParam){
            case IDT_FLAME:
                draw_bottom_line_of_fire(draw_layer);
                for ( y = 64; y<SCREEN_H-1; y++){
                    for (x =0; x<SCREEN_W; x++){
                        c = GetPixel(draw_layer,x, y+1);
                        
                        if (c>0)
                            c--;
                        SetPixel(draw_layer, x, y,c);
                    }
                }
                                
				hdc=GetClientDC(hWnd);
                BitBlt(draw_layer, 0, 20, SCREEN_W, SCREEN_H, 
                        hdc, 0, 0, 0);
				ReleaseDC(hdc);
               break;
        }
        break;
	case MSG_CLOSE:
		KillTimer(hWnd, IDT_FLY);
		free (temp);
		DeleteMemDC (draw_layer);
		KillTimer(hWnd,IDT_FLAME);
		KillTimer(hWnd,IDT_FLY);
		DestroyMainWindow (hWnd);
		PostQuitMessage (hWnd);
	break;
	}
		

	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* arg[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#if defined(_LITE_VERSION) && !(_STAND_ALONE)


    if (JoinLayer (arg[1], arg[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif

    
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "Flame";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = DrawFlameWinProc;
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

