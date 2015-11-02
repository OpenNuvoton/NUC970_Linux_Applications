/*
** $Id: listview.c 808 2010-10-28 00:52:30Z houhuihua $
**
** listview.c: Sample program for MiniGUI Programming Guide
**      Usage of LISTVIEW control.
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define IDC_LISTVIEW    10
#define IDC_CTRL1       20
#define IDC_CTRL2       30


#define SUB_NUM      3


#define COL_NR       TABLESIZE(caption)


#define SCORE_NUM    TABLESIZE(scores)


#ifdef _LANG_ZHCN
#include "listview_res_cn.h"
#elif defined _LANG_ZHTW
#include "listview_res_tw.h"
#else
#include "listview_res_en.h"
#endif


static GHANDLE add_class_item (HWND hlist, PLVITEM lvItem, GHANDLE classent)
{
    LVSUBITEM subdata;
    GHANDLE item = SendMessage (hlist, LVM_ADDITEM, classent, (LPARAM)lvItem);

    subdata.nItem = lvItem->nItem;
    subdata.subItem = 0;
    subdata.pszText = classes[lvItem->nItem];;
    subdata.nTextColor = PIXEL_black;
    subdata.flags = 0;
    subdata.image = 0;
    SendMessage (hlist, LVM_SETSUBITEM, item, (LPARAM) & subdata);

    return item;
}

static GHANDLE add_score_item (HWND hlist, PLVITEM lvItem, GHANDLE classent)
{
    char buff[20];
    LVSUBITEM subdata;
    GHANDLE item = SendMessage (hlist, LVM_ADDITEM, classent, (LPARAM)lvItem);
    int i = lvItem->nItem;
    int j;

    subdata.flags = 0;
    subdata.image = 0;
    subdata.nItem = lvItem->nItem;

    for (j = 0; j < 4; j ++) {

    subdata.subItem = j;
    if (j == 0) {
        subdata.pszText = scores[i].name;
        subdata.nTextColor = PIXEL_black;
    }
    else {
        sprintf (buff, "%d", scores[i].scr[j-1]);
        subdata.pszText = buff;
        if (scores[i].scr[j-1] > 90)
            subdata.nTextColor = PIXEL_red;
        else
            subdata.nTextColor = PIXEL_black;
    }
    SendMessage (hlist, LVM_SETSUBITEM, item, (LPARAM) & subdata);

    }

    return item;
}

//#define CUSTOM_DRAW

#ifdef CUSTOM_DRAW

static void my_draw_hdr_bk (HWND hWnd, HLVHDR hlvhdr, HDC hdc, RECT *rcDraw)
{
    SetBrushColor (hdc, PIXEL_yellow);
    FillBox (hdc, rcDraw->left, rcDraw->top, RECTWP(rcDraw), RECTHP(rcDraw));
}

static void my_draw_hdr_item (HWND hWnd, int idx, HDC hdc, RECT *rcDraw)
{
    Rectangle (hdc, rcDraw->left, rcDraw->top, rcDraw->right, rcDraw->bottom);
    SetBkMode (hdc, BM_TRANSPARENT);
    TextOut (hdc, rcDraw->left+4, rcDraw->top+4, caption[idx]);
}
#endif

static int
ScoreProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    HWND hListView;
    hListView = GetDlgItem (hDlg, IDC_LISTVIEW);

    switch (message)
    {

    case MSG_INITDIALOG:
    {
        int i, j;
        LVITEM item;
        LVCOLUMN lvcol;
        GHANDLE hitem;
#ifdef CUSTOM_DRAW
        LVCUSTOMDRAWFUNCS myfuncs = {my_draw_hdr_bk, my_draw_hdr_item};

        SendMessage (hListView, LVM_SETCUSTOMDRAW, 0, (LPARAM) &myfuncs);
#endif

        for (i = 0; i < COL_NR; i++) {
            lvcol.nCols = i;
            lvcol.pszHeadText = caption[i];
            lvcol.width = 75;
            lvcol.pfnCompare = NULL;
            lvcol.colFlags = 0;
            SendMessage (hListView, LVM_ADDCOLUMN, 0, (LPARAM) &lvcol);
        }

        item.dwFlags &= ~LVIF_FOLD;
        item.nItemHeight = 25;

        SendMessage (hListView, MSG_FREEZECTRL, TRUE, 0);
        hitem = 0;
        for (i = 0; i < 3; i++) {
            item.nItem = i;
            hitem = add_class_item (hListView, &item, 0);

            for (j = 0; j < SCORE_NUM; j++) {
                item.nItem = j;
                add_score_item (hListView, &item, hitem);
            }

        }

        SendMessage (hListView, MSG_FREEZECTRL, FALSE, 0);
        break;
    }

    case MSG_COMMAND:
    {
        int id = LOWORD (wParam);
        int i, j;

        if (id == IDC_CTRL2) {
            float average = 0;
            char buff[20];
            for (i = 0; i < SCORE_NUM; i++) {
                for (j = 0; j < SUB_NUM; j++) {
                    average += scores[i].scr[j];
                }
            }
            average = average / (SCORE_NUM * SUB_NUM);

            sprintf (buff, "%4.1f", average);
            SendDlgItemMessage (hDlg, IDC_CTRL1, MSG_SETTEXT, 0, (LPARAM)buff);
			printf("selected column%d\n", SendDlgItemMessage(hDlg, IDC_LISTVIEW,  LVM_GETSELECTEDCOLUMN, 0,0));
        }
        break;
    }

    case MSG_CLOSE:
    {
        EndDialog (hDlg, 0);
        break;
    }

    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static CTRLDATA CtrlScore[] =
{
    {
        "button",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
         110, 255, 100, 30,
        IDC_CTRL2,
        everage_score,
        0
    },
    {
        "edit",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
         10, 260, 50, 20,
        IDC_CTRL1,
        "",
        0
    },
    {
        "listview",
        /*WS_BORDER |*/ WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | LVS_TREEVIEW,
         10, 10, 320, 220,
        IDC_LISTVIEW,
        "score table",
        0
    },
};

static DLGTEMPLATE DlgScore =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    0, 0, 350, 340,
    getting_the_average_score,
    0, 0,
    0, NULL,
    0
};

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "listview" , 0 , 0);
#endif
    
    DlgScore.controls = CtrlScore;
    DlgScore.controlnr = TABLESIZE(CtrlScore);
    
    DialogBoxIndirectParam (&DlgScore, HWND_DESKTOP, ScoreProc, 0L);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif


