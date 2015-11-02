/*
** $Id: spinbox.c 741 2009-03-31 07:16:18Z weiym $
**
** spinbox.c: Sample program for MiniGUI Programming Guide
**      Usage of SPINBOX control.
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

#ifdef _LANG_ZHCN
#include "spinbox_res_cn.h"
#elif defined _LANG_ZHTW
#include "spinbox_res_tw.h"
#else
#include "spinbox_res_en.h"
#endif

#define IDC_SPIN        10
#define IDC_CTRL1       20
#define IDC_CTRL2       30
#define IDC_CTRL3       40
#define IDC_CTRL4       50


static int
SpinProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    HWND hSpin;
    SPININFO spinfo;

    hSpin = GetDlgItem (hDlg, IDC_SPIN);

    switch (message)
    {

    case MSG_INITDIALOG:
    {
        spinfo.min = 0;
        spinfo.max = 10;
        spinfo.cur = 0;
        SendMessage (hSpin, SPM_SETTARGET, 0, (LPARAM)hDlg);
        SendMessage (hSpin, SPM_SETINFO, 0, (LPARAM)&spinfo);
    }
    break;

    case MSG_KEYDOWN:
    {
        if (wParam == SCANCODE_CURSORBLOCKUP || 
                        wParam == SCANCODE_CURSORBLOCKDOWN) {
            if (!(lParam & KS_SPINPOST)) {
                int cur;
                cur = SendMessage (hSpin, SPM_GETCUR, 0, 0);
                printf ("cur pos: %d\n", cur);
                if (wParam == SCANCODE_CURSORBLOCKUP)
                    cur --;
                else
                    cur ++;
                SendMessage (hSpin, SPM_SETCUR, cur, 0);
                printf ("set pos: %d\n", cur);
            }
            InvalidateRect (hDlg, NULL, TRUE);
        }
        else if (wParam == SCANCODE_PAGEDOWN || 
                        wParam == SCANCODE_PAGEUP) {
            if (!(lParam & KS_SPINPOST)) {
                int cur;
                cur = SendMessage (hSpin, SPM_GETCUR, 0, 0);
                if (wParam == SCANCODE_PAGEUP)
                    cur -= 4;
                else
                    cur += 4;
                if (cur < 0) cur = 0;
                else if (cur > 10) cur = 10;
                SendMessage (hSpin, SPM_SETCUR, cur, 0);
            }
            InvalidateRect (hDlg, NULL, TRUE);
        }
    }
    break;

    case MSG_PAINT:
    {
        HDC hdc;
        int x, y, w, h;
        int cur;

        cur = SendMessage (hSpin, SPM_GETCUR, 0, (LPARAM)&spinfo);
        x = 10;
        y = cur*10 + 10;
        w = 60;
        h = 10;

        if (y < 10)
            y = 10;
        else if (y > 110)
            y = 110;

        hdc = BeginPaint (hDlg);
        MoveTo (hdc, 2, 10);
        LineTo (hdc, 100, 10);
        Rectangle (hdc, x, y, x+w, y+h);
        SetBrushColor (hdc, PIXEL_black);
        FillBox (hdc, x, y, w, h);
        MoveTo (hdc, 2, 120);
        LineTo (hdc, 100, 120);
        EndPaint (hDlg, hdc);
    }
    return 0;

    case MSG_CLOSE:
    {
        EndDialog (hDlg, 0);
    }
    break;

    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static DLGTEMPLATE DlgSpin =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    100, 100, 320, 240,
    CAPTION,
    0, 0,
    1, NULL,
    0
};

static CTRLDATA CtrlSpin[] =
{
    {
        CTRL_SPINBOX,
        SPS_AUTOSCROLL | WS_BORDER | WS_CHILD | WS_VISIBLE,
         230, 50, 20, 20,
        IDC_SPIN,
        "",
        0
    }
};

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "spinbox" , 0 , 0);
#endif

    DlgSpin.controls = CtrlSpin;
    
    DialogBoxIndirectParam (&DlgSpin, HWND_DESKTOP, SpinProc, 0L);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

