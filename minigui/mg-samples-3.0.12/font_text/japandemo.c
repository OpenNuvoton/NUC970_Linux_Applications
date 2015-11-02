/*
** $Id: Arabic.c  2008-02-20 MaLin $
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

static RECT drawtext_rc = {10, 10, 600, 400};

static int reset_fp_pos (void)
{
    drawtext_rc.left = 10;
    drawtext_rc.top = 10;
    drawtext_rc.right = 600;
    drawtext_rc.bottom = 480;

    if (!fp){
        fp = fopen("japantext.txt", "rb");
        if (!fp) {
            fprintf (stderr, "cannot open %s \n", "japantext.txt");
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
        if (c == 0xa) {
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
    int i = 0, linenum = 0;
    BOOL read_over = FALSE;
    while(1){
        pline = read_one_line(&read_over);
        if (read_over) break;
        if (!pline) continue;

        if (mode == TRUE){
            if (GetTextAlign (hdc) == TA_RIGHT)
                TextOut (hdc, drawtext_rc.right, 10+i*25, pline);
            else
                TextOut (hdc, drawtext_rc.left, 10+i*25, pline);
        }
        else{
            if (direction == TRUE)
                linenum = DrawText (hdc, pline, -1, &drawtext_rc, DT_RIGHT);
            else
                linenum = DrawText (hdc, pline, -1, &drawtext_rc, DT_LEFT);
            drawtext_rc.top += linenum;
        }
        i++;
    }
}

void OnModeJapan (HWND hWnd, HDC hdc, int textmodedire, PLOGFONT jisx0201)
{
    SelectFont (hdc, jisx0201);
    //SetBkColor (hdc, COLOR_green);
    reset_fp_pos();

    switch (textmodedire) {
        case 1:
            SetTextAlign (hdc, TA_RIGHT);
            OnModeTextOut (hdc, TRUE, 0);
            break;
        case 2:
            SetTextAlign (hdc, TA_LEFT);
            OnModeTextOut (hdc, TRUE, 0); //TextOut
            break;
        case 3:
            SetTextAlign (hdc, TA_RIGHT);
            OnModeTextOut (hdc, FALSE, TRUE);
            break;
        case 4:
            SetTextAlign (hdc, TA_LEFT);
            OnModeTextOut (hdc, FALSE, FALSE); //DrawText
            break;
    }
}

