/*
** $Id$
**
** bmpf-test.c: Bitmap Font unit test.
**
** Copyright (C) 2003 ~ 2008 Feynman Software
**
** All right reserved by Feynman Software.
**
** Current maintainer: MaLin.
**
** Create date: 2008-2-25
*/

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

void OnModeBmpfont(HWND hWnd, HDC hdc, PLOGFONT bmplogfont)
{
    LOGFONT *old;
    TextOut (hdc, 0, 0, "The result of bmp-led-rrncnn-10-15-ISO8859-1:");
    old = SelectFont (hdc, bmplogfont);
    SetBkColor (hdc, PIXEL_blue);
    TextOut (hdc, 10, 20, "013456");
    TextOut (hdc, 10, 40, "ABCD");
    TextOut (hdc, 10, 60, "AC");
    TextOut (hdc, 10, 80, "3.5+A:B-");            

    SelectFont (hdc, old);

}

