/*
** $Id: Thaifont.c 431 2008-02-03 06:14:35Z wangjian $
**
** The demo of font.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
** Copyright (C) 2003 ~ 2007 Feynman Software.
**
** Create date: 2002/01/17
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
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#ifdef _LANG_ZHCN
#include "thaifont_res_cn.h"
#elif defined _LANG_ZHTW
#include "thaifont_res_tw.h"
#else
#include "thaifont_res_en.h"
#endif

#define ROW                 4 
#define COL                 5
#define WIDTH               120
#define HEIGHT              40

void OnModeThai (HWND hWnd, HDC hdc, int textmodedire, PLOGFONT iso8859_11)
{
    int i;

    int offset_x = 10;
    int offset_y = -30;
    char    buffer[20];
    FILE* thai = fopen("thai", "w+");

    SetBkColor (hdc, COLOR_blue);
    SetBkMode (hdc, BM_TRANSPARENT);

    for(i = 0; i < ROW + 1; i++)
    {
        LineEx(hdc, 0, i*HEIGHT, COL*WIDTH, i*HEIGHT);
    }

    for(i = 0; i < COL + 1; i++)
    {
        LineEx(hdc, i*WIDTH, 0, i*WIDTH, ROW*HEIGHT);
    }

    SetPenColor(hdc, COLOR_red);
    LineEx(hdc, 0, 1*HEIGHT, COL*WIDTH, 1*HEIGHT);
    LineEx(hdc, 0, 2*HEIGHT, COL*WIDTH, 2*HEIGHT);
    LineEx(hdc, 0, 3*HEIGHT, COL*WIDTH, 3*HEIGHT);

    SelectFont (hdc, iso8859_11);

    i = 161;
    while (i <= 251)
    {
        if ((i-161)%25 == 0)
        {
            offset_x = 10;
            offset_y += 40;
        }
        sprintf(buffer, "%c %c %c %c %c", i, i+1, i+2, i+3, i+4);
        fprintf(thai, "%s", buffer); 
        TextOut (hdc, offset_x, offset_y, buffer);
        offset_x += 120;
        i += 5;
    }

    fclose(thai);

    SetPenColor(hdc, COLOR_black);
    TextOut(hdc, 610, 10, "I");
    TextOut(hdc, 610, 50, "II");
    TextOut(hdc, 610, 90, "III");
    TextOut(hdc, 610, 130, "IV");
} 

