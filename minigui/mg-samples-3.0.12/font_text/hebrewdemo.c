/*
** $Id: Arabic.c  2008-02-21 MaLin $
**
** The demo of font.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
** Copyright (C) 2003 ~ 2008 Feynman Software.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

char linebuf[4096];
static FILE *fp = NULL;

RECT heberw_rc = {14, 0, 300, 400};
static int reset_fp_pos (void)
{
    heberw_rc.left = 0;
    heberw_rc.top = 0;
    heberw_rc.right = 800 -14;
    heberw_rc.bottom = 400;

    if (!fp){
        fp = fopen("hebrewtext.txt", "rb");
        if (!fp) {
            fprintf (stderr, "cannot open %s \n", "hebrewtext.txt");
        }
    }
    if (fp){
        fseek(fp, 0, SEEK_SET);
    }
    return 1;
}

static char* read_one_line(BOOL* read_over)
{
    int i = 0;
    char c = 0;
    if (fp == NULL){
        *read_over = TRUE;
        return NULL;
    }
    memset (linebuf, 0, 4096);
    while(!(*read_over = feof(fp))) {
        c = fgetc(fp);
        if (c == 0xa || c == 0xd) {
            break;
        }
        linebuf[i] = c;
        i++;
    }
    if (i > 0)
        return linebuf;
    else
        return NULL;
}

static void OnModeTextOut (HDC hdc, BOOL mode, BOOL direction)
{
    char* pline = NULL;
    int i = 0;
    int line_height = GetFontHeight (hdc);
    BOOL read_over = FALSE;
    while(1){
        pline = read_one_line(&read_over);
        if (read_over) break;
        if (!pline) continue;

        if (mode == TRUE){ //TextOut
            if (GetTextAlign (hdc) == TA_RIGHT)
                TextOut (hdc, heberw_rc.right, 10+i*25, pline);
            else
                TextOut (hdc, heberw_rc.left, 10+i*line_height, pline);
        }
        else {
            if (direction == TRUE)
                DrawText (hdc, pline, -1, &heberw_rc, DT_RIGHT);
            else
                DrawText (hdc, pline, -1, &heberw_rc, DT_LEFT);
            heberw_rc.top += line_height;
        }
        i++;
    }
} 

void OnModeHebrew (HWND hWnd, HDC hdc, int modedire, PLOGFONT iso8859_8)
{
    SelectFont (hdc, iso8859_8);
    SetTextColor (hdc, COLOR_darkcyan);
    reset_fp_pos();

    switch (modedire) {
        case 1:
            SetTextAlign (hdc, TA_RIGHT);
            OnModeTextOut (hdc, TRUE, 0);
            break;
        case 2:
            SetTextAlign (hdc, TA_LEFT);
            OnModeTextOut (hdc, TRUE, 0);
            break;
        case 3:
            OnModeTextOut (hdc, FALSE, TRUE); //right
            break;
        case 4:
            OnModeTextOut (hdc, FALSE, FALSE);//left
            break;
    }
}


