/* 
** $Id: dialogbox.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 4.1
**
** dialogbox.c: Sample program for MiniGUI Programming Guide
** 	Usage of DialogBoxIndirectParam
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _LANG_ZHCN
#include "dialogbox_rec_cn.h"
#elif defined _LANG_ZHTW
#include "dialogbox_rec_tw.h"
#else
#include "dialogbox_rec_en.h"
#endif

static DLGTEMPLATE DlgInitProgress =
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    120, 150, 400, 140, 
    the_system_is_initializing,
    0, 0,
    3, NULL,
    0
};

#define IDC_PROMPTINFO	100
#define IDC_PROGRESS  	110

static CTRLDATA CtrlInitProgress [] =
{ 
    {
        "static",
        WS_VISIBLE | SS_SIMPLE,
        10, 10, 380, 16, 
        IDC_PROMPTINFO, 
        Initializing,
        0
    },
    {
        "progressbar",
        WS_VISIBLE,
        10, 40, 380, 20,
        IDC_PROGRESS,
        NULL,
        0
    },
    {
        "button",
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        170, 70, 60, 25,
        IDOK, 
        OK,
        0
    }
};

static int InitDialogBoxProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_INITDIALOG:
        return 1;
        
    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            EndDialog (hDlg, wParam);
            break;
        }
        break;

    case MSG_CLOSE:
        EndDialog (hDlg, IDCANCEL);
        break;
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static void InitDialogBox (HWND hWnd)
{
    DlgInitProgress.controls = CtrlInitProgress;
    
    DialogBoxIndirectParam (&DlgInitProgress, hWnd, InitDialogBoxProc, 0L);
}

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "dialogbox" , 0 , 0);
#endif
    
    InitDialogBox (HWND_DESKTOP);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

