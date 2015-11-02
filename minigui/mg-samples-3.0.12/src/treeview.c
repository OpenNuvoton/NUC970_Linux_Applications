/*
** $Id: treeview.c 747 2009-04-13 03:57:38Z houhuihua $
**
** treeview.c: Sample program for MiniGUI Programming Guide
**      Usage of TREEVIEW control.
**
** Copyright (C) 2004 ~ 2007 Fynman Software.
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

#define IDC_TREEVIEW  100

#define CHAPTER_NUM	5

#ifdef _LANG_ZHCN
#include "treeview_res_cn.h"
#elif defined _LANG_ZHTW
#include "treeview_res_tw.h"
#else
#include "treeview_res_en.h"
#endif

static int BookProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{

    switch (message) {
        case MSG_INITDIALOG:
            {
                TVITEMINFO tvItemInfo = {0};
                int item;
                int i, j;

                for (i = 0; i < CHAPTER_NUM; i++) {
                    tvItemInfo.text = (char*)chapter[i];
                    item = SendMessage (GetDlgItem(hDlg, IDC_TREEVIEW), TVM_ADDITEM, 
                            0, (LPARAM)&tvItemInfo);
                    for (j = 0; j < 3; j++) {
                        tvItemInfo.text = (char*)section[j];
                        SendMessage (GetDlgItem(hDlg, IDC_TREEVIEW), TVM_ADDITEM, 
                                item, (LPARAM)&tvItemInfo);
                    }
                }
            }
            break;

        case MSG_COMMAND:
        	switch (LOWORD(wParam)) {
        	case IDOK:
            	EndDialog (hDlg, wParam);
			}	
			break;
		
        case MSG_CLOSE:
	    EndDialog (hDlg, 0);
            return 0;
    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static TVITEMINFO bookInfo =
{
   contents 
};

static DLGTEMPLATE DlgBook =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    0, 0, 300, 260,
    book_contents,
    0, 0,
    2, NULL,
    0
};

static CTRLDATA CtrlBook[] =
{
    {
        "treeview",
        WS_BORDER | WS_CHILD | WS_VISIBLE | 
		WS_VSCROLL | WS_HSCROLL,
         10, 10, 280, 180,
        IDC_TREEVIEW,
        "treeview control",
        (LPARAM)&bookInfo
    },
    {
        "button",
        WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP,
        110, 195, 70, 25,
        IDOK, 
        OK,
        0
    },
};

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "treeview" , 0 , 0);
#endif

    DlgBook.controls = CtrlBook;
    
    DialogBoxIndirectParam (&DlgBook, HWND_DESKTOP, BookProc, 0L);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

