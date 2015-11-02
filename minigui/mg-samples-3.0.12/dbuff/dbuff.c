/* 
** $Id$
**
** button.c: Sample program for MiniGUI Programming Guide
**     Usage of Double Buffering.
**
** Copyright (C) 2009 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define IDC_WATCH0              100
#define IDC_WATCH1              101
#define IDC_WATCH2              102
#define IDC_WATCH3              103

static DLGTEMPLATE DlgSelectApp =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_AUTOSECONDARYDC,
    0, 0, 240, 320,
    "Please select a UE type",
    0, 0,
    7, NULL,
    0
};

static CTRLDATA CtrlSelectApp[] =
{ 
    {
        "static",
        WS_VISIBLE | SS_GROUPBOX, 
        15, 10, 210, 100,
        IDC_STATIC,
        "User Experience",
        0,
        WS_EX_TRANSPARENT
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CHECKED | WS_TABSTOP | WS_GROUP, 
        20, 28, 200, 20, 
        IDC_WATCH0, 
        "Watch (No special UE)",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        20, 48, 200, 20,
        IDC_WATCH1,
        "Watch (UE type 1)",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON, 
        20, 68, 200, 20, 
        IDC_WATCH2, 
        "Watch (UE type 2)",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON, 
        20, 88, 200, 20, 
        IDC_WATCH3, 
        "Watch (UE type 3)",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP,
        20, 160, 200, 30,
        IDOK, 
        "Start Application",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        120, 230, 100, 30,
        IDCANCEL,
        "Exit",
        0
    },
};

extern int app_watch (HWND hosting, int ue_type);

static void my_notif_proc (HWND hwnd, int id, int nc, DWORD add_data)
{
    HWND parent = GetParent (hwnd);

    if (nc == BN_CLICKED) {
        if (IsDlgButtonChecked (parent, IDC_WATCH0))
            app_watch (parent, 0);
        else if (IsDlgButtonChecked (parent, IDC_WATCH1))
            app_watch (parent, 1);
        else if (IsDlgButtonChecked (parent, IDC_WATCH2))
            app_watch (parent, 2);
        else
            app_watch (parent, 3);
    }
}

static void disable_show_window (HWND hwnd)
{
    HDC sec_dc;

    sec_dc = GetSecondaryDC (hwnd);
    SetSecondaryDC (hwnd, sec_dc, ON_UPDSECDC_DONOTHING);
}

static void show_window_specially (HWND hwnd)
{
    HDC sec_dc;

    sec_dc = GetSecondaryDC (hwnd);

    StretchBlt (sec_dc, 0, 0, 240, 320, 
        HDC_SCREEN, 0, 0, 240/4, 320/4, 0);
    usleep (100000);

    StretchBlt (sec_dc, 0, 0, 240, 320, 
        HDC_SCREEN, 0, 0, 240/2, 320/2, 0);
    usleep (100000);

    /* restore to default behavior */
    SetMemDCAlpha (sec_dc, 0, 0);
    BitBlt (sec_dc, 0, 0, 240, 320, 
        HDC_SCREEN, 0, 0, 0);

    SetSecondaryDC (hwnd, sec_dc, ON_UPDSECDC_DEFAULT);
}

static int first_idle;
static int DialogBoxProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_INITDIALOG:
        SetNotificationCallback (GetDlgItem (hDlg, IDOK), my_notif_proc);
        disable_show_window (hDlg);
        first_idle = 1;
        return 1;

    case MSG_IDLE:
        if (first_idle) {
            show_window_specially (hDlg);
            first_idle = 0;
        }
        break;

    case MSG_COMMAND:
        switch (wParam) {
        case IDCANCEL:
            EndDialog (hDlg, wParam);
            break;
        }
        break;
    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "dbuff" , 0 , 0);
#endif

    SetDefaultWindowElementRenderer ("skin");

    DlgSelectApp.controls = CtrlSelectApp;
    DialogBoxIndirectParam (&DlgSelectApp, HWND_DESKTOP, DialogBoxProc, 0L);

    return 0;
}

