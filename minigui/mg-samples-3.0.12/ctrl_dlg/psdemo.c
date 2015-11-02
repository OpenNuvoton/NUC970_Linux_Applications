/*
** $Id: psdemo.c 554 2008-03-03 09:09:48Z malin $
**
** psdemo.c: The PropertySheet demo program.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
** Copyright (C) 2003 ~ 2007 Feynman Software.
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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "dlgdemo.h"
#include "resouce.h"

static int 
PageProc1 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{

    switch (message) {
    case MSG_INITPAGE:
        break;
    case MSG_SHOWPAGE:
        return 1;

    case MSG_SHEETCMD:
        if (wParam == IDOK) {
            char buffer [20];
            GetDlgItemText (hDlg, IDC_EDIT1, buffer, 18);
            buffer [18] = '\0';

            if (buffer [0] == '\0') {
                MessageBox (hDlg, 
                            "Please input something in the first edit box.", 
                            "Warning!", 
                            MB_OK | MB_ICONEXCLAMATION | MB_BASEDONPARENT);
                return -1;
            }
        }
        return 0;

    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            break;
        }
        break;
    }
    
    return DefaultPageProc (hDlg, message, wParam, lParam);
}

static int 
PageProc2 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_INITPAGE:
        break;

    case MSG_SHOWPAGE:
        return 1;
        
    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            break;
        }
        break;
        
    }
    
    return DefaultPageProc (hDlg, message, wParam, lParam);
}

static int 
PageProc3 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    int i;
    char temp [20];

    switch (message) {
    case MSG_INITPAGE:
        for (i = 0; i < 20; i++) { 
            sprintf (temp, "%d-Welcome", i);
            SendDlgItemMessage (hDlg, IDC_BOX1, CB_ADDSTRING, 0, (LPARAM)temp) ;
            SendDlgItemMessage (hDlg, IDC_BOX2, CB_ADDSTRING, 0, (LPARAM)temp) ;
            SendDlgItemMessage (hDlg, IDC_BOX4, CB_ADDSTRING, 0, (LPARAM)temp) ;
        } 
        break;

    case MSG_SHOWPAGE:
        return 1;

    case MSG_GETDLGCODE:
        return DLGC_WANTARROWS;

    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            break;
        }
        break;
        
    }
    
    return DefaultPageProc (hDlg, message, wParam, lParam);
}

static int 
PageProc4 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_INITPAGE:
        break;
    case MSG_SHOWPAGE:
        return 1;

    case MSG_COMMAND:
        switch (wParam) {
        case IDOK:
        case IDCANCEL:
            break;
        }
    }
    
    return DefaultPageProc (hDlg, message, wParam, lParam);
}

static int PropSheetProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_INITDIALOG:
        {
            HWND pshwnd = GetDlgItem (hDlg, IDC_PROPSHEET);

            DlgStructParams.controls = CtrlStructParams;
            SendMessage (pshwnd, PSM_ADDPAGE,
                            (WPARAM)&DlgStructParams, (LPARAM)PageProc1);
            
            DlgPassword.controls = CtrlPassword;
            SendMessage ( pshwnd, PSM_ADDPAGE, 
                            (WPARAM)&DlgPassword,(LPARAM) PageProc2);
            
            DlgStartupMode.controls = CtrlStartupMode;
            SendMessage ( pshwnd, PSM_ADDPAGE, 
                            (WPARAM)&DlgStartupMode,(LPARAM)PageProc3);

            DlgInitProgress.controls = CtrlInitProgress;
            SendMessage ( pshwnd, PSM_ADDPAGE, 
                            (WPARAM)&DlgInitProgress, (LPARAM) PageProc4);

            break;
        }

        case MSG_COMMAND:
        switch (wParam) 
        {
            case IDC_APPLY:
            break;

            case IDOK:
            {
                int index = SendDlgItemMessage (hDlg, IDC_PROPSHEET, 
                                PSM_SHEETCMD, IDOK, 0);
                if (index) {
                    SendDlgItemMessage (hDlg, IDC_PROPSHEET, 
                                    PSM_SETACTIVEINDEX, index - 1, 0);
                }
                else 
                    EndDialog (hDlg, wParam);

                break;
            }
            case IDCANCEL:
                EndDialog (hDlg, wParam);
            break;
        }
        break;
    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

void testPropertySheet (HWND hWnd)
{
    DlgPropertySheet.controls = CtrlPropertySheet;
    
    DialogBoxIndirectParam (&DlgPropertySheet, hWnd, PropSheetProc, 0L);
}

