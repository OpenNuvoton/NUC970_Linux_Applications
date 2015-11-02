//
// NoteBook, the M$ Windows like notepad Editor on MiniGUI.
// Copyright (c) 2000, Feng Da ke (minx@thtfchain.com).
//
// Some idea and source come from VConGUI (Virture Console GUI) 

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

// Create date: 2000.07.05

#ifndef	NOTEBOOK_H
#define	NOTEBOOK_H

//#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#define IDM_NEW         301
#define IDM_OPEN        120
#define IDM_SAVE        130
#define IDM_SAVEAS      140
#define IDM_PRINT       150
#define IDM_EXIT        160

#define IDM_COPY	    200
#define IDM_PASTE	    210
#define IDM_CUT		    220
#define IDM_DEL		    230
#define IDM_UNDO	    240
#define IDM_SELALL	    250
#define IDM_INSDATE	    260
#define IDM_AUTOWRAP	270
#define IDM_SEARCH	    280
#define IDM_SECHNEXT	290

#define IDM_40X15       310
#define IDM_80X24       320
#define IDM_80X25       330
#define IDM_80X40       340
#define IDM_80X52       350
#define IDM_96X25       360
#define IDM_96X40       370
#define IDM_96X52       380
#define IDM_CUSTOMIZE   390

#define IDM_DEFAULT     391
#define IDM_ANSI        392
#define IDM_GB2312      393
#define IDM_THAI        394
#define IDM_BIG5        395

#define IDM_ABOUT       410
#define IDM_ABOUT_THIS  411

#define IDC_MLEDIT      104

#define MAX_NUMBER_ROWS     52
#define MAX_NUMBER_COLS     96

#define VGASTD_NUMBER_ROWS  25
#define VGASTD_NUMBER_COLS  80

#define VGAEXT_NUMBER_ROWS  52
#define VGAEXT_NUMBER_COLS  80

#define WIDTH_TABSTOP       8

#define MIN_COLS    10
#define MAX_COLS    100
#define MIN_ROWS    10
#define MAX_ROWS    60

typedef struct _NoteInfo
{
    BOOL isChanged;
    char fileName[NAME_MAX+1];
    char filePath[PATH_MAX+1];
    char fileFullName[NAME_MAX+PATH_MAX+1];
    long fileSize;
    char* Buffer;
    HWND hMLEditWnd;

    int lx, ty;         // left x, and top y.
    int rows, cols;     // row and column of window.
    int winType;        // window size.
    int editCharset;    // charset of medit control.
    PLOGFONT log_font;  // logical font used by medit control.
}NOTEINFO;
typedef NOTEINFO* PNOTEINFO;

void* NoteBook (void* data);
void* NewNoteBook (PNOTEINFO pNoteInfo);
//static void AboutLaodan();
//static void AboutNotebook();

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif

