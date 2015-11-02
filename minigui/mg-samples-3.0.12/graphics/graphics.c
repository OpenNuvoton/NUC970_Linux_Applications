/*
** $Id: graphics.c 732 2009-03-28 07:25:48Z weiym $
**
** The demo for GDI.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
** Copyright (C) 2003 ~ 2007 Feynman Software.
**
** Create date: 2001/10/17
*/

/*
**  This source is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public
**  License as published by the Free Software Foundation; either
**  version 2 of the License, or (at your option) any later version.
**
**  This software is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
**  General Public License for more details.
**
**  You should have received a copy of the GNU General Public
**  License along with this library; if not, write to the Free
**  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
**  MA 02111-1307, USA
*/

/*
** TODO:
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <minigui/fixedmath.h>

#ifdef _LANG_ZHCN
#include "graphics_res_cn.h"
#elif defined _LANG_ZHTW
#include "graphics_res_tw.h"
#else
#include "graphics_res_en.h"
#endif

#define DEFAULT_WIDTH   640
#define DEFAULT_HEIGHT  400
#define DEFAULT_X       320
#define DEFAULT_Y       200

static void 
TellSpeed (HWND hwnd, unsigned int start_tick, unsigned int end_tick, 
        const char* drawing, unsigned int count)
{
        char buff [1024];
        unsigned int mseconds = (end_tick - start_tick) * 10;

        sprintf (buff, GP_ST_TELLSPEED, 
                        count, drawing, mseconds, 
                        (end_tick - start_tick) * 10.0 / count);

        MessageBox (hwnd, buff, drawing, MB_OK | MB_ICONINFORMATION);
}

static void GDIDemo_NormalLines (HWND hwnd, HDC hdc)
{
    int x = DEFAULT_X, y = DEFAULT_Y;
    int tox = DEFAULT_WIDTH, toy = DEFAULT_WIDTH;
    int count;
    unsigned int start_tick, end_tick;
    unsigned int nr_colors = GetGDCapability (hdc, GDCAP_COLORNUM);

    /* Line */
    start_tick = GetTickCount ();
    count = 100000;
    SetPenColor (hdc, PIXEL_yellow);
    while (count--) {
        SetPenColor (hdc, rand() % nr_colors);
        MoveTo (hdc, x, y);
        LineTo (hdc, tox, toy);
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_LINE, 100000);

    /* Circle */
    start_tick = GetTickCount ();
    count = 10000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
        SetPenColor (hdc, rand() % nr_colors);
        Circle (hdc, tox, toy, rand() % DEFAULT_X);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_CIRCLE, 10000);

    /* Ellipse */
    start_tick = GetTickCount ();
    count = 10000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
        SetPenColor (hdc, rand() % nr_colors);
        Ellipse (hdc, tox, toy, rand() % DEFAULT_X, rand() % DEFAULT_Y);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_ELLIPSE, 10000);

    /* CircleArc */
    start_tick = GetTickCount ();
    count = 10000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
        SetPenColor (hdc, rand() % nr_colors);
        CircleArc (hdc, tox, toy, rand() % DEFAULT_X, rand() % 360 * 64, (rand() % 360 - 180) * 64);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_CIRCLEARC, 10000);
}

static void GDIDemo_XoredLines (HWND hwnd, HDC hdc)
{
    int x = DEFAULT_X, y = DEFAULT_Y;
    int tox = DEFAULT_WIDTH, toy = DEFAULT_WIDTH;
    int count;
    unsigned int start_tick, end_tick;
    unsigned int nr_colors = GetGDCapability (hdc, GDCAP_COLORNUM);

    /* XORed Line */
    start_tick = GetTickCount ();
    count = 10000;
//    SetPenColor (hdc, PIXEL_lightwhite);
    SetRasterOperation (hdc, ROP_XOR);
    while (count--) {
        SetPenColor (hdc, rand() % nr_colors);
        MoveTo (hdc, x, y);
        LineTo (hdc, tox, toy);
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_XOR_LINE, 10000);

    /* XORed Circle */
    start_tick = GetTickCount ();
    count = 10000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
        SetPenColor (hdc, rand() % nr_colors);
        Circle (hdc, tox, toy, rand() % DEFAULT_X);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_XOR_CIRCLE, 10000);

    /* XORed Ellipse */
    start_tick = GetTickCount ();
    count = 10000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
        SetPenColor (hdc, rand() % nr_colors);
        Ellipse (hdc, tox, toy, rand() % DEFAULT_X, rand() % DEFAULT_Y);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_XOR_ELLIPSE, 10000);
}

static void GDIDemo_Filling (HWND hwnd, HDC hdc)
{
    int tox = DEFAULT_WIDTH, toy = DEFAULT_WIDTH;
    int count;
    unsigned int start_tick, end_tick;
    unsigned int nr_colors = GetGDCapability (hdc, GDCAP_COLORNUM);

    /* Filled Box */
    start_tick = GetTickCount ();
    count = 1000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
        SetBrushColor (hdc, rand() % nr_colors);
        FillBox (hdc, tox, toy, rand() % DEFAULT_X, rand() % DEFAULT_Y);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_FILLBOX, 1000);

    /* Filled Rect with ROP */
    SetRasterOperation (hdc, ROP_XOR);
    start_tick = GetTickCount ();
    count = 1000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
        SetBrushColor (hdc, rand() % nr_colors);
        FillBox (hdc, tox, toy, rand() % DEFAULT_X, rand() % DEFAULT_Y);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_FILLROPRECT, 1000);

    /* Filled Circle */
    SetRasterOperation (hdc, ROP_SET);
    start_tick = GetTickCount ();
    count = 500;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
        SetBrushColor (hdc, rand() % nr_colors);
        FillCircle (hdc, tox, toy, rand() % DEFAULT_X);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_FILLCIRCLE, 500);

    /* Filled Ellipse */
    SetRasterOperation (hdc, ROP_SET);
    start_tick = GetTickCount ();
    count = 500;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;
        SetBrushColor (hdc, rand() % nr_colors);
        FillEllipse (hdc, tox, toy, rand() % DEFAULT_X, rand() % DEFAULT_Y);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_FILLELLIPSE, 500);
}

static void GDIDemo_NormalBitmaps (HWND hwnd, HDC hdc)
{
    int tox = DEFAULT_WIDTH, toy = DEFAULT_WIDTH;
    int count;
    BITMAP bitmap;
    unsigned int start_tick, end_tick;

    if (LoadBitmap (hdc, &bitmap, "res/sample.bmp"))
        return;
    
    /* normal bitmap */
    start_tick = GetTickCount ();
    count = 2000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;

        FillBoxWithBitmap (hdc, tox, toy, 0, 0, &bitmap);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_NORBMP, 2000);

    /* xored bitmap */
    SetRasterOperation (hdc, ROP_XOR);
    start_tick = GetTickCount ();
    count = 200;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;

        FillBoxWithBitmap (hdc, tox - 100, toy - 100, 0, 0, &bitmap);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_XORBMP, 200);

    /* xored transparent bitmap */
    SetRasterOperation (hdc, ROP_OR);
    bitmap.bmColorKey = GetPixelInBitmap (&bitmap, 0, 0);

    start_tick = GetTickCount ();
    count = 300;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;

        FillBoxWithBitmap (hdc, tox - 100, toy - 100, 0, 0, &bitmap);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_XORTRANSBMP, 300);

    UnloadBitmap (&bitmap);
}

static void GDIDemo_TransparentBitmaps (HWND hwnd, HDC hdc)
{
    int tox = DEFAULT_WIDTH, toy = DEFAULT_WIDTH;
    int count;
    BITMAP bitmap;
    unsigned int start_tick, end_tick;

    /* Transparent bitmap */
    if (LoadBitmap (hdc, &bitmap, "res/sail.bmp"))
        return;
    
    bitmap.bmType = BMP_TYPE_COLORKEY;
    bitmap.bmColorKey = GetPixelInBitmap (&bitmap, 0, 0);

    /* color key */
    start_tick = GetTickCount ();
    count = 1000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;

        FillBoxWithBitmap (hdc, tox, toy, 0, 0, &bitmap);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_TRANSBMP, 1000);
    UnloadBitmap (&bitmap);
}

static void GDIDemo_AlphaBlendedBitmaps (HWND hwnd, HDC hdc)
{
    int tox = DEFAULT_WIDTH, toy = DEFAULT_WIDTH;
    int count;
    BITMAP bitmap;
    unsigned int start_tick, end_tick;

    if (LoadBitmap (hdc, &bitmap, "res/icon.bmp"))
        return;

    bitmap.bmType = BMP_TYPE_ALPHACHANNEL;

    /* alpha blending */
    start_tick = GetTickCount ();
    count = 1000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;

        bitmap.bmAlpha = rand() % 256;
        FillBoxWithBitmap (hdc, tox, toy, 0, 0, &bitmap);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_ALPHABMP, 1000);

    bitmap.bmType = BMP_TYPE_ALPHACHANNEL | BMP_TYPE_COLORKEY;
    bitmap.bmColorKey = GetPixelInBitmap (&bitmap, 0, 0);

    /* alpha blending and transparent */
    start_tick = GetTickCount ();
    count = 1000;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_WIDTH;

        bitmap.bmAlpha = rand() % 256;
        FillBoxWithBitmap (hdc, tox, toy, 0, 0, &bitmap);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_ALPHATRANSBMP, 1000);

    UnloadBitmap (&bitmap);
}

static void GDIDemo_MemDC (HWND hwnd, HDC hdc)
{
    HDC mem_dc;
    int tox = DEFAULT_WIDTH, toy = DEFAULT_WIDTH;
    int count;
    unsigned int start_tick, end_tick;
    unsigned int nr_colors = GetGDCapability (hdc, GDCAP_COLORNUM);
    
    /* Compatible DC */
    mem_dc = CreateCompatibleDC (hdc);
    SetBrushColor (mem_dc, 0);
    FillBox (mem_dc, 0, 0, 65535, 65535);
    start_tick = GetTickCount ();
    count = 200;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH; toy = rand() % DEFAULT_WIDTH;
        MoveTo (mem_dc, tox, toy);

        SetPenColor (mem_dc, rand() % nr_colors);
        tox = rand() % DEFAULT_WIDTH; toy = rand() % DEFAULT_WIDTH;
        LineTo (mem_dc, tox, toy);
        Circle (mem_dc, tox, toy, rand() % DEFAULT_X);

        BitBlt (mem_dc, 0, 0, rand () % DEFAULT_WIDTH, rand () % DEFAULT_WIDTH, hdc, 
                        rand () % DEFAULT_WIDTH, rand () % DEFAULT_WIDTH, 0);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_BLIT, 200);

    SetBrushColor (mem_dc, 0);
    FillBox (mem_dc, 0, 0, 65535, 65535);
    SetMemDCColorKey (mem_dc, MEMDC_FLAG_SRCCOLORKEY, 0);
    start_tick = GetTickCount ();
    count = 200;
    while (count--) {
        tox = rand() % DEFAULT_WIDTH; toy = rand() % DEFAULT_WIDTH;
        MoveTo (mem_dc, tox, toy);

        SetPenColor (mem_dc, rand() % nr_colors);
        tox = rand() % DEFAULT_WIDTH; toy = rand() % DEFAULT_WIDTH;
        LineTo (mem_dc, tox, toy);
        Circle (mem_dc, tox, toy, rand() % DEFAULT_X);

        BitBlt (mem_dc, 0, 0, rand () % DEFAULT_WIDTH, rand () % DEFAULT_WIDTH, hdc, 
                        rand () % DEFAULT_WIDTH, rand () % DEFAULT_WIDTH, 0);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_TRANSBLIT, 200);

    DeleteCompatibleDC (mem_dc);

    /* Memory DC with alpha */
    mem_dc = CreateMemDC (400, 100, 18, MEMDC_FLAG_HWSURFACE | MEMDC_FLAG_SRCALPHA,
                    0x0000F000, 0x00000F00, 0x000000F0, 0x0000000F);
    SetBrushColor (mem_dc, RGBA2Pixel (mem_dc, 0xFF, 0xFF, 0x00, 0x80));
    FillBox (mem_dc, 0, 0, 200, 50);
    SetBrushColor (mem_dc, RGBA2Pixel (mem_dc, 0xFF, 0x00, 0xFF, 0x80));
    FillBox (mem_dc, 200, 0, 200, 50);
    SetBrushColor (mem_dc, RGBA2Pixel (mem_dc, 0x00, 0xFF, 0xFF, 0x80));
    FillBox (mem_dc, 0, 50, 200, 50);
    SetBrushColor (mem_dc, RGBA2Pixel (mem_dc, 0xFF, 0xFF, 0xFF, 0x80));
    FillBox (mem_dc, 200, 50, 200, 50);

    SetTextColor (mem_dc, RGBA2Pixel (mem_dc, 0x00, 0x00, 0x00, 0x80));
    SetBkColor (mem_dc, RGBA2Pixel (mem_dc, 0xFF, 0xFF, 0xFF, 0x80));
    SetBkMode (mem_dc, BM_TRANSPARENT);
    TabbedTextOut (mem_dc, 0, 0, GP_ST_PIXELALPHADC);

    start_tick = GetTickCount ();
    count = 200;
    while (count--) {
        BitBlt (mem_dc, 0, 0, 400, 100, hdc, rand () % DEFAULT_WIDTH, rand () % DEFAULT_WIDTH, 0);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_PERPIXELALPHA, 200);
    DeleteMemDC (mem_dc);

    /* Memory DC with alpha*/
    mem_dc = CreateMemDC (400, 100, 32, MEMDC_FLAG_HWSURFACE | MEMDC_FLAG_SRCALPHA | MEMDC_FLAG_SRCCOLORKEY,
                    0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000);
    SetBrushColor (mem_dc, RGB2Pixel (mem_dc, 0xFF, 0xFF, 0x00));
    FillBox (mem_dc, 0, 0, 400, 100);
    SetTextColor (mem_dc, RGB2Pixel (mem_dc, 0x00, 0x00, 0xFF));
    SetBkColor (mem_dc, RGB2Pixel (mem_dc, 0x00, 0x00, 0x00));
    SetBkMode (mem_dc, BM_TRANSPARENT);
    TabbedTextOut (mem_dc, 0, 0, GP_ST_SURFALPHADC);

    start_tick = GetTickCount ();
    count = 200;
    while (count--) {
        SetMemDCAlpha (mem_dc, MEMDC_FLAG_SRCALPHA | MEMDC_FLAG_RLEACCEL, rand () % 256);
        BitBlt (mem_dc, 0, 0, 400, 100, hdc, rand () % DEFAULT_WIDTH, rand () % DEFAULT_WIDTH, 0);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_PERSURFALPHA, 200);

    FillBox (mem_dc, 0, 0, 400, 100);
    SetBrushColor (mem_dc, RGB2Pixel (mem_dc, 0xFF, 0x00, 0xFF));
    FillBox (mem_dc, 200, 0, 400, 100);
    TabbedTextOut (mem_dc, 0, 0, GP_ST_COLKEY);
    SetMemDCColorKey (mem_dc, MEMDC_FLAG_SRCCOLORKEY | MEMDC_FLAG_RLEACCEL, 
                    RGB2Pixel (mem_dc, 0xFF, 0xFF, 0x00));
    start_tick = GetTickCount ();
    count = 200;
    while (count--) {
        SetMemDCAlpha (mem_dc, MEMDC_FLAG_SRCALPHA | MEMDC_FLAG_RLEACCEL, rand () % 256);
        BitBlt (mem_dc, 0, 0, 400, 100, hdc, rand () % DEFAULT_WIDTH, rand () % DEFAULT_WIDTH, 0);
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GP_ST_ALPHACOLKEYB, 200);

    DeleteMemDC (mem_dc);
}

static BLOCKHEAP my_cliprc_heap;
static BOOL ch_inited = FALSE;

static void GDIDemo_Region (HWND hWnd, HDC hdc)
{
    CLIPRGN my_cliprgn1;
    CLIPRGN my_cliprgn2;
    CLIPRGN my_cliprgn3;

    if (!ch_inited) {
        InitFreeClipRectList (&my_cliprc_heap, 100);
        ch_inited = TRUE;
    }

    InitClipRgn (&my_cliprgn1, &my_cliprc_heap);
    InitClipRgn (&my_cliprgn2, &my_cliprc_heap);
    InitClipRgn (&my_cliprgn3, &my_cliprc_heap);

    InitCircleRegion (&my_cliprgn1, 100, 100, 60);
    InitEllipseRegion (&my_cliprgn2, 100, 100, 50, 70);
    SetBrushColor (hdc, PIXEL_blue);
    FillBox (hdc, 0, 0, DEFAULT_WIDTH, 200);

    SubtractRegion (&my_cliprgn1, &my_cliprgn1, &my_cliprgn2);
    SelectClipRegion (hdc, &my_cliprgn1);

    SetBrushColor (hdc, PIXEL_red);
    FillBox (hdc, 0, 0, 180, 200);

    InitCircleRegion (&my_cliprgn1, 300, 100, 60);
    OffsetRegion (&my_cliprgn2, 200, 0);

    XorRegion (&my_cliprgn1, &my_cliprgn1, &my_cliprgn2);
    SelectClipRegion (hdc, &my_cliprgn1);

    FillBox (hdc, 200, 0, 180, 200);

    InitCircleRegion (&my_cliprgn1, 500, 100, 60);
    OffsetRegion (&my_cliprgn2, 200, 0);

    IntersectRegion (&my_cliprgn1, &my_cliprgn1, &my_cliprgn2);
    SelectClipRegion (hdc, &my_cliprgn1);

    FillBox (hdc, 400, 0, 180, 200);

#if 0
    {
    static POINT pts [] = {
        {400, 500},
        {200, 200},
        {500, 100},
        {100, 50},
    };

    InitPolygonRegion (&my_cliprgn3, pts, 4);
    SelectClipRegion (hdc, &my_cliprgn3);
    SetBrushColor (hdc, PIXEL_green);
    FillBox (hdc, 0, 0, DEFAULT_WIDTH, 200);
    
    EmptyClipRgn (&my_cliprgn1);
    EmptyClipRgn (&my_cliprgn2);
    EmptyClipRgn (&my_cliprgn3);

    }
#endif

    /* You should not forget to destroy the FreeClipRectList in your applications */
}

#ifdef _MGHAVE_ADV_2DAPI
const unsigned char stipple_bits [] = "\x81\x42\x24\x18\x18\x24\x42\x81";

static STIPPLE my_stipple =
{
    8, 8, 1, 8,
    stipple_bits
};

#if 1


void GetPentagram (POINT * pts, int cx, int cy, int r)
{
    /** size of pts must have more than 11 */

    const double PI = 3.1514926;

    /** r0: radius of inscribed circle*/
    /** r : radius of circumcircle */

    int r0 = (int) (r * cos (72 * PI / 180));
    int i;
    pts [0].x = 0;
    pts [1].x = (int)(r0*cos (54*PI / 180));
    pts [2].x = (int)(r*sin (72*PI/180));
    pts [3].x = (int)(r0*cos(18*PI/180));
    pts [4].x = (int)(r*cos(54*PI/180));
    pts [5].x = (int)0;
    pts [6].x = (int)((-r)*cos(54*PI/180));
    pts [7].x = (int)((-r0)*cos(18*PI/180));
    pts [8].x = (int)((-r)*sin(72*PI/180));
    pts [9].x = (int)((-r0)*cos(54*PI/180));
    pts [10].x = pts [0].x;

    pts [0].y = r;
    pts [1].y = (int)(r0*sin(54*PI/180));
    pts [2].y = (int)(r*cos(72*PI/180));
    pts [3].y = (int)((-r0)*sin(18*PI/180));
    pts [4].y = (int)((-r)*(sin(54*PI/180)));
    pts [5].y = (int)(-r0);
    pts [6].y = (int)((-r)*(sin(54*PI/180)));
    pts [7].y = (int)((-r0)*sin(18*PI/180));
    pts [8].y = (int)(r*cos(72*PI/180));
    pts [9].y = (int)(r0*sin(54*PI/180));
    pts [10].y = pts [0].y;

    /** parallel move */
    for (i = 0; i < 11; ++i)
    {
        pts [i].x = pts [i].x + r + cx;
        pts [i].y = pts [i].y + r + cy;
    }
}
void GDIDemo_Adv2DAPI (HWND hWnd, HDC hdc)
{
    POINT pt [11];
    BITMAP bitmap;
    int scr_center_x = g_rcScr.right / 2;
    int scr_center_y = g_rcScr.bottom / 2;

    int head_width = 160;
    int head_height = 260;
    int head_center_x = scr_center_x - head_width / 2;
    int head_center_y = scr_center_y - head_height / 2;
    int i = 0;
    ARC arc_hat[1];
    ARC arc_mouth [2];
    int eye_width = 16;

    if (LoadBitmap (hdc, &bitmap, "res/icon.bmp"))
        return;

    /** set pen and brush style */
    SetPenType (hdc, PT_SOLID);
    SetPenDashes (hdc, 0, (const unsigned char*)"\1\1", 2);
    SetPenJoinStyle (hdc, PT_JOIN_BEVEL);
    SetPenCapStyle (hdc, PT_CAP_ROUND);
    SetBrushType (hdc, BT_SOLID);

    /** head */
    SetPenWidth (hdc, 3);
    SetPenColor (hdc, PIXEL_darkgray);
    ArcEx (hdc, head_center_x, head_center_y, 
        head_width, head_height, 130 * 64, 280 * 64);
    SetBrushColor (hdc, PIXEL_lightgray);
    FillArcEx (hdc, scr_center_x - head_width / 2, 
            scr_center_y - head_height / 2, 
            head_width, head_height, 0, 360 * 64);

    /** hat shape*/
    arc_hat [0].width = 240;
    arc_hat [0].height = 240;
    arc_hat [0].x = scr_center_x - arc_hat [0].width / 2;
    arc_hat [0].y = scr_center_y - 180 - arc_hat [0].height / 2;
    arc_hat [0].angle1 = 236 * 64;
    arc_hat [0].angle2 = 68 * 64;
    SetBrushType (hdc, BT_TILED);
    SetBrushInfo (hdc, &bitmap, &my_stipple);
    SetBrushColor (hdc, PIXEL_darkred);
    SetBrushOrigin (hdc, arc_hat [0].x, arc_hat [0].y);
    PolyFillArcEx (hdc, arc_hat, 1);
     
    SetBrushType (hdc, BT_SOLID);

    SetPenWidth (hdc, 1);
    SetPenJoinStyle (hdc, PT_JOIN_ROUND);
    SetPenColor (hdc, PIXEL_darkred);
    LineEx (hdc, scr_center_x, scr_center_y - 178, scr_center_x - 65, scr_center_y - 80);
    LineEx (hdc, scr_center_x, scr_center_y - 178, scr_center_x + 65, scr_center_y - 80);
    LineEx (hdc, scr_center_x, scr_center_y - 178, scr_center_x, scr_center_y - 60);
    LineEx (hdc, scr_center_x, scr_center_y - 178, scr_center_x - 30, scr_center_y - 65);
    LineEx (hdc, scr_center_x, scr_center_y - 178, scr_center_x + 30, scr_center_y - 65);
    LineEx (hdc, scr_center_x, scr_center_y - 178, scr_center_x - 50, scr_center_y - 72);
    LineEx (hdc, scr_center_x, scr_center_y - 178, scr_center_x + 50, scr_center_y - 72);

    /** eye */
    SetPenWidth (hdc, 2);
    SetPenColor (hdc, PIXEL_black);
    SetBrushColor (hdc, PIXEL_black);
    FillArcEx (hdc, scr_center_x - 30 - 20, scr_center_y - 60, 40, 60, 0, 360 * 64);
    FillArcEx (hdc, scr_center_x + 30 - 20, scr_center_y - 60, 40, 60, 0, 360 * 64);

    /** eye ball */
    SetPenColor (hdc, PIXEL_lightwhite);
    SetBrushColor (hdc, PIXEL_lightwhite);
    FillArcEx (hdc, scr_center_x - 38 - eye_width / 2, scr_center_y - 20, eye_width, eye_width, 0, 360 * 64 );
    FillArcEx (hdc, scr_center_x + 38 - eye_width / 2, scr_center_y - 20, eye_width, eye_width, 0, 360 * 64 );

    /** draw quadrangle at face */
    pt [0].x = scr_center_x + 60;
    pt [0].y = scr_center_y + 60;
    pt [1].x = pt [0].x;
    pt [1].y = pt [0].y + 10;
    pt [2].x = pt [0].x - 10;
    pt [2].y = pt [0].y + 5;
    pt [3].x = pt [0].x - 10;
    pt [3].y = pt [0].y - 10;
    pt [4].x = pt [0].x;
    pt [4].y = pt [0].y;

    SetBrushColor (hdc, PIXEL_darkgreen);
    SetPenWidth (hdc, 1);
    SetPenColor (hdc, PIXEL_black);

    FillPolygon (hdc, pt, 5);
    PolyLineEx (hdc, pt, 5);

    for (i= 0; i < 5; ++i)
    {
        pt [i].x -= 30;
        pt [i].y -= 30;
    }
    FillPolygon (hdc, pt, 5);
    PolyLineEx (hdc, pt, 5);

    for (i= 0; i < 5; ++i)
    {
        pt [i].x -= 90;
        pt [i].y -= 80;
    }
    FillPolygon (hdc, pt, 5);
    PolyLineEx (hdc, pt, 5);

    /** draw GetPentagram at face*/

    SetBrushColor (hdc, PIXEL_blue);
    SetPenColor (hdc, PIXEL_darkblue);
    GetPentagram (pt, scr_center_x - 60 - 10, scr_center_y - 40 , 10);
    FillPolygon (hdc, pt, 11);
    PolyLineEx (hdc, pt, 11);
    GetPentagram (pt, scr_center_x + 60 - 10, scr_center_y - 40 , 10);
    FillPolygon (hdc, pt, 11);
    PolyLineEx (hdc, pt, 11);

    SetBrushColor (hdc, PIXEL_red);
    SetPenColor (hdc, PIXEL_darkred);
    GetPentagram (pt, scr_center_x - 10, scr_center_y + 110, 10);
    FillPolygon (hdc, pt, 11);
    PolyLineEx (hdc, pt, 11);

    /** line to fix nose */
    SetPenWidth (hdc, 1);
    SetPenColor (hdc, PIXEL_black);
    ArcEx (hdc, scr_center_x - 100, scr_center_y - 125 , 200, 180, 216 * 64, 108 * 64);

    /** nose */
    SetBrushColor (hdc, PIXEL_red); 
    FillArcEx (hdc, scr_center_x - 20, scr_center_y + 20, 40, 40, 0, 360 * 64);

    SetPenWidth (hdc, 2);
    SetPenColor (hdc, PIXEL_darkred);
    ArcEx (hdc, scr_center_x - 20, scr_center_y + 20, 40, 40, 0, 360 * 64);


    /** mouth */
    arc_mouth [0].width = 160;
    arc_mouth [0].height = 140;
    arc_mouth [0].x = scr_center_x - arc_mouth [0].width / 2;
    arc_mouth [0].y = scr_center_y + 30 - arc_mouth [0].height / 2;
    arc_mouth [0].angle1 = 220 * 64;
    arc_mouth [0].angle2 = 100 * 64;

    arc_mouth [1].width = 160;
    arc_mouth [1].height = 170;
    arc_mouth [1].x = scr_center_x - arc_mouth [1].width / 2;
    arc_mouth [1].y = scr_center_y + 25 - arc_mouth [1].height / 2;
    arc_mouth [1].angle1 = 220 * 64;
    arc_mouth [1].angle2 = 100 * 64;
    
    SetPenWidth (hdc, 6);
    SetPenColor (hdc, PIXEL_darkred);
    SetPenJoinStyle (hdc, PT_JOIN_BEVEL);
    SetPenCapStyle (hdc, PT_CAP_ROUND);
    PolyArcEx (hdc, arc_mouth, 2);

    /** ear */
    SetBrushColor (hdc, PIXEL_darkred);
    FillArcEx (hdc, scr_center_x - 90 - 20, scr_center_y + 10 - 20, 
            40, 40, 60 * 64, 240 * 64);
    FillArcEx (hdc, scr_center_x + 90 - 20, scr_center_y + 10 - 20, 
            40, 40, -120 * 64, 240 * 64);
    
    UnloadBitmap (&bitmap);
}

#else

void GDIDemo_Adv2DAPI (HWND hWnd, HDC hdc)
{
    POINT pt [10];
    BITMAP bitmap;

    pt[0].x = 80;
    pt[0].y = 80;
    pt[1].x = 70;
    pt[1].y = 90;
    pt[2].x = 90;
    pt[2].y = 90;
    pt[3].x = 80;
    pt[3].y = 80;

    SetPenType (hdc, PT_SOLID);
    PolyLineEx (hdc, pt, 4);
    return;

    if (LoadBitmap (hdc, &bitmap, "res/sample.bmp"))
        return;

    SetPenType (hdc, PT_SOLID);
    SetPenWidth (hdc, 5);

    LineEx (hdc, 10, 10, 210, 10);

    SetPenDashes (hdc, 0, "\5\5", 2);
    SetPenType (hdc, PT_ON_OFF_DASH);
    LineEx (hdc, 10, 20, 210, 20);

    SetPenDashes (hdc, 0, "\x8\x8", 2);
    SetPenType (hdc, PT_DOUBLE_DASH);

    SetBrushType (hdc, BT_SOLID);
    SetBrushColor (hdc, PIXEL_green);
    SetBkColor (hdc, PIXEL_lightwhite);
    LineEx (hdc, 10, 30, 210, 30);

    SetPenDashes (hdc, 0, "\x10\x10", 2);
    SetBrushType (hdc, BT_STIPPLED);
    SetBrushInfo (hdc, &bitmap, &my_stipple);
    SetPenWidth (hdc, 10);
    LineEx (hdc, 10, 50, 210, 50);

    MoveTo (hdc, 10, 60);
    LineTo (hdc, 10, 120);
    MoveTo (hdc, 210, 60);
    LineTo (hdc, 210, 120);

    SetPenType (hdc, PT_SOLID);
    SetPenWidth (hdc, 10);

    SetPenCapStyle (hdc, PT_CAP_BUTT);
    LineEx (hdc, 10, 70, 210, 70);

    SetPenCapStyle (hdc, PT_CAP_ROUND);
    LineEx (hdc, 10, 90, 210, 90);

    SetPenCapStyle (hdc, PT_CAP_PROJECTING);
    LineEx (hdc, 10, 110, 210, 110);

    SetPenJoinStyle (hdc, PT_JOIN_MITER);
    pt [0].x = 10; pt [0].y = 150;
    pt [1].x = 100; pt [1].y = 150;
    pt [2].x = 10; pt [2].y = 190;
    PolyLineEx (hdc, pt, 3);

    SetPenJoinStyle (hdc, PT_JOIN_ROUND);
    pt [0].x = 210; pt [0].y = 150;
    pt [1].x = 300; pt [1].y = 150;
    pt [2].x = 210; pt [2].y = 190;
    PolyLineEx (hdc, pt, 3);

    SetPenJoinStyle (hdc, PT_JOIN_BEVEL);
    pt [0].x = 410; pt [0].y = 150;
    pt [1].x = 500; pt [1].y = 150;
    pt [2].x = 410; pt [2].y = 190;
    PolyLineEx (hdc, pt, 3);

    SetPenWidth (hdc, 1);
    SetPenType (hdc, PT_ON_OFF_DASH);
    SetPenCapStyle (hdc, PT_CAP_BUTT);

    SetPenDashes (hdc, 0, "\x1\x1", 2);
    LineEx (hdc, 300, 20, 510, 20);

    SetPenDashes (hdc, 0, "\x4\x1", 2);
    LineEx (hdc, 300, 30, 510, 30);

    SetPenDashes (hdc, 0, "\x4\x1\x1\x1", 4);
    LineEx (hdc, 300, 40, 510, 40);

    SetPenDashes (hdc, 0, "\x4\x1\x1\x1\x1\x1", 6);
    LineEx (hdc, 300, 50, 510, 50);

    SetPenCapStyle (hdc, PT_CAP_BUTT);
    SetPenWidth (hdc, 20);

    SetPenCapStyle (hdc, PT_CAP_ROUND);
    LineEx (hdc, 10, 10, 50, 50);

    SetPenJoinStyle (hdc, PT_JOIN_BEVEL);

    pt [0].x = 20;
    pt [0].y = 20;
    pt [1].x = 80;
    pt [1].y = 20;
    pt [2].x = 80;
    pt [2].y = 80;
    pt [3].x = 20;
    pt [3].y = 80;
    pt [4].x = 20;
    pt [4].y = 20;
    PolyLineEx (hdc, pt, 5);

    SetPenWidth (hdc, 20);
    SetPenColor (hdc, PIXEL_red);
    SetPenCapStyle (hdc, PT_CAP_ROUND);

    LineEx (hdc, 80, 80, 400, 300);

    SetPenColor (hdc, PIXEL_blue);

    ArcEx (hdc, 100, 100, 200, 300, 180*64, 180*64);

    SetBrushType (hdc, BT_SOLID);
    SetBrushColor (hdc, PIXEL_green);

    FillArcEx (hdc, 100, 0, 200, 100, 0, 120*64);

    SetBrushType (hdc, BT_TILED);
    SetBrushInfo (hdc, &bitmap, &my_stipple);

    SetBrushOrigin (hdc, 100, 100);
    FillArcEx (hdc, 100, 100, 200, 100, 0, 270*64);

    SetBrushType (hdc, BT_STIPPLED);
    FillArcEx (hdc, 100, 300, 200, 100, 0, 360*64);

    SetPenType (hdc, PT_DOUBLE_DASH);
    SetPenDashes (hdc, 0, "\20\40", 2);
    SetPenCapStyle (hdc, PT_CAP_BUTT);
    SetPenWidth (hdc, 20);

    LineEx (hdc, 500, 0, 20, 100);

    SetBrushType (hdc, BT_OPAQUE_STIPPLED);
    ArcEx (hdc, 400, 100, 200, 300, 180*64, 180*64);

    UnloadBitmap (&bitmap);
}
#endif

#endif

#define TEXT_WIDTH      630
#define TEXT_HEIGHT     120
#define LOGO_WIDTH      220
#define LOGO_HEIGHT     68
#define IDT_ANIMATION   100
#define FRAME_SPEED     10      /* 10 frames per second */

static BOOL anim_inited = FALSE;
static HDC shadow;
static HDC logo_layer;
static HDC text_layer;
static int text_line;
static const char* text[] = 
{
    GP_ST_TEXT1,
    GP_ST_TEXT2,
    GP_ST_TEXT3,
    GP_ST_TEXT4,
    GP_ST_TEXT5,
};

static PLOGFONT logfonttimes;
static RECT text_rc;
static int x, y = 20, oldx = -100, oldy, xdir = 1, ydir = 1, count;
static int xspeed = 4, yspeed = 4;

int GDIDemoWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    unsigned int start_tick, end_tick;
    HDC hdc;

    switch (message) {
        case MSG_TIMER:
            if (!anim_inited || wParam != IDT_ANIMATION)
                return 0;

            start_tick = GetTickCount ();
            /* prepaer the text layer */
            if (count == 0) {
                SetBrushColor (text_layer, 
                                RGB2Pixel (text_layer, 0x00, 0x00, 0x00));
                FillBox (text_layer, 0, 0, TEXT_WIDTH, TEXT_HEIGHT);
                SetBkColor (text_layer, 
                                RGB2Pixel (text_layer, 0x00, 0x00, 0xFF));
                SetTextColor (text_layer, 
                                RGB2Pixel (text_layer, 0xFF, 0xFF, 0xFF));
                SetBkMode (text_layer, BM_OPAQUE);
                DrawText (text_layer, text [text_line], -1, &text_rc, 
                                DT_CENTER | DT_WORDBREAK);
                text_line ++;
                if (text_line == 5) text_line = 0;
            }

            SetBrushColor (shadow, PIXEL_lightwhite);
            FillBox (shadow, 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

            x += xspeed * xdir;
            y += yspeed * ydir;
            if (x < 0) {
                xdir = 1;
                xspeed = rand() % 4 + 2;
            }
            if (x >= DEFAULT_WIDTH) {
                xdir = -1;
                xspeed = rand() % 4 + 2;
            }
            if (y < 0) {
                ydir = 1;
                yspeed = rand() % 4 + 2;
            }
            if (y >= DEFAULT_HEIGHT) {
                ydir = -1;
                yspeed = rand() % 4 + 2;
            }

            BitBlt (logo_layer, 0, 0, 0, 0, shadow, x, y, 0);

            SetMemDCAlpha (text_layer, MEMDC_FLAG_SRCALPHA, 255 - count * 4);
            BitBlt (text_layer, 0, 0, 0, 0, shadow, 
                            0, (DEFAULT_HEIGHT - TEXT_HEIGHT)/2, 0);

            hdc = GetClientDC (hWnd);
#if 1
            if (oldx > -100) {
                BitBlt (shadow, oldx, oldy, 
                                LOGO_WIDTH, LOGO_HEIGHT, hdc, oldx, oldy, 0);
            }
            oldx = x;
            oldy = y;
            BitBlt (shadow, x, y, LOGO_WIDTH, LOGO_HEIGHT, hdc, x, y, 0);
            BitBlt (shadow, 0, (DEFAULT_HEIGHT - TEXT_HEIGHT)/2, 
                            DEFAULT_WIDTH, TEXT_HEIGHT, 
                            hdc, 
                            0, (DEFAULT_HEIGHT - TEXT_HEIGHT)/2, 0);
            SetTextColor (shadow, 
                            RGB2Pixel (hdc, (int)(255 * sin(count * M_PI / 63)),
                                       0x00, 0x00));
            SetBkColor (shadow, PIXEL_lightwhite);
            SetBkMode (shadow, BM_TRANSPARENT);
            TextOut (shadow, 0, 0, GP_ST_STOP);
            BitBlt (shadow, 0, 0, 220, 20, hdc, 0, 0, 0);
#else
            TextOut (shadow, 0, 0, GP_ST_STOP);
            BitBlt (shadow, 0, 0, 0, 0, hdc, 0, 0, 0);
#endif
            ReleaseDC (hdc);
            end_tick = GetTickCount ();

            count++;
            if (count == 64)
                count = 0;
            break;

        case MSG_CHAR:
            if (anim_inited && wParam != 's' && wParam != 'S')
                break;

            srand (time (NULL));
            hdc = GetClientDC (hWnd);
            switch (wParam) {
            case 'a':
            case 'A':
                GDIDemo_NormalLines (hWnd, hdc);
                break;
            case 'b':
            case 'B':
                GDIDemo_XoredLines (hWnd, hdc);
                break;
            case 'c':
            case 'C':
                GDIDemo_Filling (hWnd, hdc);
                break;
            case 'd':
            case 'D':
                GDIDemo_NormalBitmaps (hWnd, hdc);
                break;
            case 'e':
            case 'E':
                GDIDemo_TransparentBitmaps (hWnd, hdc);
                break;
            case 'f':
            case 'F':
                GDIDemo_AlphaBlendedBitmaps (hWnd, hdc);
                break;
            case 'g':
            case 'G':
                GDIDemo_MemDC (hWnd, hdc);
                break;
            case 'h':
            case 'H':
                if (!anim_inited) {
                    RECT rc;
                    BITMAP logo;

                    GetClientRect (hWnd, &rc);
                    FillBox (hdc, 0, 0, RECTW(rc), RECTH(rc));

                    if (LoadBitmap (hdc, &logo, "res/logo.bmp")) {
                        break;
                    }
                    logo_layer = CreateCompatibleDCEx (hdc, 
                                    LOGO_WIDTH, LOGO_HEIGHT);
                    FillBoxWithBitmap (logo_layer, 0, 0, 0, 0, &logo);
                    UnloadBitmap (&logo);

                    shadow = CreateCompatibleDC (hdc);
                    text_layer = CreateMemDC (TEXT_WIDTH, TEXT_HEIGHT, 24, 
                                    MEMDC_FLAG_HWSURFACE | 
                                    MEMDC_FLAG_SRCALPHA | 
                                    MEMDC_FLAG_SRCCOLORKEY,
                                    0x00FF0000, 0x0000FF00, 
                                    0x000000FF, 0x0000000);
                    SetMemDCColorKey (text_layer, MEMDC_FLAG_SRCCOLORKEY, 0);
                    logfonttimes = CreateLogFont (NULL, "Times", "GB2312", 
                        FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, 
                        FONT_SETWIDTH_NORMAL, FONT_SPACING_CHARCELL, 
                        FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                        10, 0);
                    SelectFont (text_layer, logfonttimes);

                    SetBrushColor (hdc, PIXEL_lightwhite);
                    FillBox (hdc, 0, 0, RECTW(rc), RECTH(rc));
                    text_rc.left = 0; text_rc.top = 0;
                    text_rc.right = TEXT_WIDTH; text_rc.bottom = TEXT_HEIGHT;
                    anim_inited = TRUE;
                }
                text_line = 0;
                SetTimer (hWnd, IDT_ANIMATION, FRAME_SPEED);
                ReleaseDC (hdc);
                return 0;

            case 's':
            case 'S':
                if (anim_inited) {
                    DeleteMemDC (shadow);
                    DeleteMemDC (logo_layer);
                    DeleteMemDC (text_layer);
                    if (logfonttimes)
                        DestroyLogFont (logfonttimes);
                    anim_inited = FALSE;
                    KillTimer (hWnd, IDT_ANIMATION);
                    break;
                }
                break;

            case 'i':
            case 'I':
                GDIDemo_Region (hWnd, hdc);
                ReleaseDC (hdc);
                return 0;

#ifdef _MGHAVE_ADV_2DAPI
            case 'j':
            case 'J':
                GDIDemo_Adv2DAPI (hWnd, hdc);
                ReleaseDC (hdc);
                return 0;
#endif

            case 'q':
            case 'Q':
                PostMessage (hWnd, MSG_CLOSE, 0, 0);
                break;
            }
            ReleaseDC (hdc);
            InvalidateRect (hWnd, NULL, TRUE);
        break;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
            SetTextColor (hdc, PIXEL_black);
            SetBkColor (hdc, PIXEL_lightwhite);
            SetBkMode (hdc, BM_TRANSPARENT);
            TabbedTextOut (hdc, 100, 100, GP_ST_PROMPT);
            EndPaint (hWnd, hdc);
            return 0;

        case MSG_CLOSE:
            if (anim_inited) {
                DeleteMemDC (shadow);
                DeleteMemDC (logo_layer);
                DeleteMemDC (text_layer);
                DestroyLogFont (logfonttimes);
                anim_inited = FALSE;
                KillTimer (hWnd, IDT_ANIMATION);
            }
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_VISIBLE | WS_BORDER;
    pCreateInfo->dwExStyle = 0;
    pCreateInfo->spCaption = GP_ST_CAP;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor (0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = GDIDemoWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0; 
    pCreateInfo->rx = pCreateInfo->lx + DEFAULT_WIDTH;
    pCreateInfo->by = pCreateInfo->ty + DEFAULT_HEIGHT;
    pCreateInfo->iBkColor = PIXEL_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain (int args, const char* arg[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

#ifdef _MGRM_PROCESSES
    int i;
    const char* layer = NULL;

    for (i = 1; i < args; i++) {
        if (strcmp (arg[i], "-layer") == 0) {
            layer = arg[i + 1];
            break;
        }
    }

    GetLayerInfo (layer, NULL, NULL, NULL);

    if (JoinLayer (layer, arg[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif

    InitCreateInfo (&CreateInfo);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
