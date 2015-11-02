/* 
 ** $Id: tiny_listbox1.c 406 2008-02-01 03:12:21Z zhounuohua $
 **
 ** Listing 20.1
 **
 ** button.c: Sample program for MiniGUI Programming Guide
 **     Usage of BUTTON control.
 **
 ** Copyright (C) 2004 ~ 2007 Feynman Software.
 **
 ** License: GPL
 */

#include <stdio.h>
#include <stdlib.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

extern int tiny_listbox2 (HWND hwnd);
extern HWND hwnd_my;

#define MY_IDC_LISTBOX1             300

static HICON icon1, icon2, icon3, icon4, icon5, icon6, icon7, icon8, icon9, icon10, icon11;

static WNDPROC old_callback;
static int notif_proc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_COMMAND:
            switch (wParam) {
                case IDOK:
                    SendMessage (GetParent(hwnd), MSG_CLOSE, 0, 0);
                    tiny_listbox2 (hwnd_my);
                    return 0;
                case IDCANCEL:
                    SendMessage (GetParent(hwnd), MSG_CLOSE, 0, 0);
                    ShowWindow (hwnd_my, SW_SHOWNORMAL);
                    return 0;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return old_callback(hwnd, message, wParam, lParam);
}

static CTRLDATA CtrlYourTaste[] =
{ 
    {
        CTRL_LISTBOX,
        WS_VISIBLE | WS_CHILD | LBS_USEICON | WS_VSCROLL, 
        0, 0, 240, 320,
        MY_IDC_LISTBOX1,
        "Mutimedia",
        0,
        WS_EX_TRANSPARENT |
        WS_EX_LFRDR_CUSTOM1 | WS_EX_LFRDR_CUSTOM4 
            | WS_EX_NOCLOSEBOX
            | WS_EX_LFRDR_CUSTOM2
            | WS_EX_LFRDR_CUSTOM3
            ,
        "tiny",
        0
    }
};

static DLGTEMPLATE DlgYourTaste =
{
    WS_VISIBLE,
    WS_EX_NONE,
    0, 0, 240, 320,
    "Tiny test",
    0, 0,
    TABLESIZE(CtrlYourTaste), CtrlYourTaste,
    0
};

static int DialogBoxProc2 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_INITDIALOG:
            {
                LISTBOXITEMINFO lbii;

                lbii.hIcon = icon1;
                lbii.string = "Read Card";
            
                lbii.cmFlag = LBS_USEICON;
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_SETITEMHEIGHT, 0, 32);

                lbii.hIcon = icon2;
                lbii.string = "Clock";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon3;
                lbii.string = "Digital Cenima";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                lbii.hIcon = icon4;
                lbii.string = "Dictionary";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon5;
                lbii.string = "Receive Email";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon6;
                lbii.string = "Image View";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon7;
                lbii.string = "Recorde";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);

                lbii.hIcon = icon8;
                lbii.string = "Month Calendar";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                lbii.hIcon = icon9;
                lbii.string = "Music";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                lbii.hIcon = icon10;
                lbii.string = "Note Book";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                lbii.hIcon = icon11;
                lbii.string = "Mobile Set";
                SendMessage (GetDlgItem (hDlg, MY_IDC_LISTBOX1), LB_ADDSTRING, 0, (LPARAM)&lbii);
                
                old_callback = SetWindowCallbackProc (GetDlgItem (hDlg, MY_IDC_LISTBOX1), notif_proc);
                break;
            }
        case MSG_CLOSE:
            EndDialog (hDlg, 0);
            break;
        default:
            break;
    }
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

int tiny_listbox1 (HWND hwnd)
{
    icon1 = LoadIconFromFile (HDC_SCREEN, "res/card.ico", 1);
    icon2 = LoadIconFromFile (HDC_SCREEN, "res/clock.ico", 1);
    icon3 = LoadIconFromFile (HDC_SCREEN, "res/dc.ico", 1);
    icon4 = LoadIconFromFile (HDC_SCREEN, "res/dict.ico", 1);
    icon5 = LoadIconFromFile (HDC_SCREEN, "res/email.ico", 1);
    icon6 = LoadIconFromFile (HDC_SCREEN, "res/ime_view.ico", 1);
    icon7 = LoadIconFromFile (HDC_SCREEN, "res/mac_phone.ico", 1);
    icon8 = LoadIconFromFile (HDC_SCREEN, "res/month_cal.ico", 1);
    icon9 = LoadIconFromFile (HDC_SCREEN, "res/music.ico", 1);
    icon10 = LoadIconFromFile (HDC_SCREEN, "res/notebook.ico", 1);
    icon11 = LoadIconFromFile (HDC_SCREEN, "res/set.ico", 1);

    DialogBoxIndirectParamEx (&DlgYourTaste, hwnd, DialogBoxProc2, 0L,
           "tiny", 0, 0, 0 );

    return 0;
}
