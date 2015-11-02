/*
** $Id: dlgdemo.c 786 2010-05-20 08:16:59Z cxzhang $
**
** dlgdemo.c: The DialogBox demo program.
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
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
//#include <mgplus/mgplus.h>

#include "dlgdemo.h"
#include "resouce.h"
#include "ctrls.h"

#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif

static HMENU createpmenuabout (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)About;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_ABOUT;
    mii.typedata    = (DWORD)About_MiniGUI;
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    return hmnu;
}

static HMENU createpmenufile (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)File;
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_EXIT;
    mii.typedata    = (DWORD)Exit;
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    return hmnu;
}

static HMENU createpmenudialogs (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)Dialogs;
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_DIALOG1;
    mii.typedata    = (DWORD)Dialog1;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
     
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_DIALOG2;
    mii.typedata    = (DWORD)Dialog2;
    InsertMenuItem(hmnu, 1, TRUE, &mii);  
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_DIALOG3;
    mii.typedata    = (DWORD)Dialog3;
    InsertMenuItem(hmnu, 2, TRUE, &mii);  

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_DIALOG4;
    mii.typedata    = (DWORD)Dialog4;
    InsertMenuItem(hmnu, 3, TRUE, &mii);  

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_DIALOG5;
    mii.typedata    = (DWORD)Property_Sheet;
    InsertMenuItem(hmnu, 4, TRUE, &mii);  

    return hmnu;
}

static HMENU createpmenuobject (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)Controls;
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_STATIC;
    mii.typedata    = (DWORD)Static_control;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_BUTTON;
    mii.typedata    = (DWORD)Button_control;
    InsertMenuItem(hmnu, 1, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_MENUBUTTON;
    mii.typedata    = (DWORD)MenuButton_control;
    InsertMenuItem(hmnu, 2, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_EDIT;
    mii.typedata    = (DWORD)Edit_control;
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_LISTBOX;
    mii.typedata    = (DWORD)ListBox_control;
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_PROGRESSBAR;
    mii.typedata    = (DWORD)ProgressBar_control;
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_COMBOBOX;
    mii.typedata    = (DWORD)ComboBox_control;
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_TOOLBAR;
    mii.typedata    = (DWORD)NewToolBar_control;
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_TRACKBAR;
    mii.typedata    = (DWORD)TrackBar_control;
    InsertMenuItem(hmnu, 8, TRUE, &mii);

    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 9, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_LISTVIEW;
    mii.typedata    = (DWORD)ListView_control;
    InsertMenuItem(hmnu, 10, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_TREEVIEW;
    mii.typedata    = (DWORD)TreeView_control;
    InsertMenuItem(hmnu, 12, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_MONTHCALENDAR;
    mii.typedata    = (DWORD)MonthCalendar_control;
    InsertMenuItem(hmnu, 13, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_COOLBAR;
    mii.typedata    = (DWORD)CoolBar_control;
    InsertMenuItem(hmnu, 14, TRUE, &mii);

    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 15, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_SUBCLASS;
    mii.typedata    = (DWORD)Control_Subclass;
    InsertMenuItem(hmnu, 16, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_TIMEEDIT;
    mii.typedata    = (DWORD)Time_Editor;
    InsertMenuItem(hmnu, 17, TRUE, &mii);

    return hmnu;
}

static HMENU createmenu (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)File;
    mii.hsubmenu    = createpmenufile ();

    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)Dialogs;
    mii.hsubmenu    = createpmenudialogs ();
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)Controls;
    mii.hsubmenu    = createpmenuobject();
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 120;
    mii.typedata    = (DWORD)About;
    mii.hsubmenu    = createpmenuabout ();
    InsertMenuItem(hmnu, 3, TRUE, &mii);
                   
    return hmnu;
}

static int 
DialogBoxProc1 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
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
        return 0;
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static int 
DialogBoxProc2 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    int i;
    char temp [20];

    switch (message) {
    case MSG_INITDIALOG:
        for (i = 0; i < 20; i++) { 
            sprintf (temp, "%d-Welcome", i);
            SendDlgItemMessage (hDlg, IDC_BOX1, CB_ADDSTRING, 0, (LPARAM)temp) ;
            SendDlgItemMessage (hDlg, IDC_BOX2, CB_ADDSTRING, 0, (LPARAM)temp) ;
            SendDlgItemMessage (hDlg, IDC_BOX4, CB_ADDSTRING, 0, (LPARAM)temp) ;
        } 
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

static int 
DialogBoxProc3 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
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
        
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static int 
DialogBoxProc4 (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
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
        
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static void testDialogBox1 (HWND hWnd)
{
    DlgInitProgress.controls = CtrlInitProgress;
    
    DialogBoxIndirectParam (&DlgInitProgress, hWnd, DialogBoxProc1, 0L);
}

static void testDialogBox2 (HWND hWnd)
{
    DlgStartupMode.controls = CtrlStartupMode;
    
    DialogBoxIndirectParam (&DlgStartupMode, hWnd, DialogBoxProc2, 0L);
}

static void testDialogBox3 (HWND hWnd)
{
    DlgPassword.controls = CtrlPassword;
    
    DialogBoxIndirectParam (&DlgPassword, hWnd, DialogBoxProc3, 0L);
}

static void testDialogBox4 (HWND hWnd)
{
    DlgStructParams.controls = CtrlStructParams;
    
    DialogBoxIndirectParam (&DlgStructParams, hWnd, DialogBoxProc4, 0L);
}

int DialogTestMainWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case MSG_CREATE:
        break;
        
        case MSG_COMMAND:
        switch (wParam) 
        {
            case IDM_NEW:
            case IDM_OPEN:
            case IDM_SAVE:
            case IDM_SAVEAS:
            case IDM_CLOSE:
            break;
            
            case IDM_EXIT:
                SendMessage (hWnd, MSG_CLOSE, 0, 0L);
            break;

            case IDM_DIALOG1:
                testDialogBox1 (hWnd);
            break;
            
            case IDM_DIALOG2:
                testDialogBox2 (hWnd);
            break;
            
            case IDM_DIALOG3:
                testDialogBox3 (hWnd);
            break;
            
            case IDM_DIALOG4:
                testDialogBox4 (hWnd);
            break;

            case IDM_DIALOG5:
                testPropertySheet (hWnd);
            break;

            case IDM_STATIC:
                static_demo (hWnd);
                break;
            case IDM_BUTTON:
                button_demo (hWnd);
                break;
            case IDM_MENUBUTTON:
                menubutton_demo (hWnd);
                break;
            case IDM_EDIT:
                edit_demo (hWnd);
                break;
            case IDM_LISTBOX:
                listbox_demo (hWnd);
                break;
            case IDM_PROGRESSBAR:
                progressbar_demo (hWnd);
                break;
            case IDM_COMBOBOX:
                combobox_demo (hWnd);
                break;
            case IDM_TOOLBAR:
                newtoolbar_demo(hWnd);
                break;
            case IDM_TRACKBAR:
                trackbar_demo (hWnd);
                break;

            case IDM_LISTVIEW:
                listview_demo (hWnd);
                break;

            case IDM_TREEVIEW:
                treeview_demo (hWnd);
                break;
            case IDM_MONTHCALENDAR:
                monthcalendar_demo (hWnd);
                break;
            case IDM_COOLBAR:
                coolbar_demo (hWnd);
                break;

            case IDM_SUBCLASS:
                subclass_demo (hWnd);
                break;

            case IDM_TIMEEDIT:
                timeeditor (hWnd);
                break;

            case IDM_ABOUT:
#ifdef _MGMISC_ABOUTDLG
#ifdef _MGRM_THREADS
                OpenAboutDialog ();
#else
                OpenAboutDialog (hWnd);
#endif
#endif
            break;
        }
        break;

        case MSG_CLOSE:
            if (MessageBox (hWnd, Are_you_sure_to_quit, DlgTest, 
                            MB_YESNOCANCEL | MB_ICONQUESTION |
                            MB_BASEDONPARENT) != IDYES)
                return 0;

            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    pCreateInfo->dwExStyle = WS_EX_NONE | WS_EX_USEPRIVATECDC;
    pCreateInfo->spCaption = The_dialog_and_control_demo;
    pCreateInfo->hMenu = createmenu ();
    pCreateInfo->hCursor = GetSystemCursor (IDC_ARROW);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = DialogTestMainWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 500;
    pCreateInfo->by = 400;
    pCreateInfo->iBkColor = COLOR_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

BITMAP bmp_bkgnd;
int MiniGUIMain (int args, const char* arg[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

#ifdef _MGRM_PROCESSES
    int i;
    const char* layer = NULL;

    for (i = 1; i < args; i++) {
        if (strcmp (arg[i], "-layer") == 0) {
            layer = arg[i + 1];
            break;
        }
    }

    GetLayerInfo (layer, NULL, NULL, NULL);

    if (JoinLayer (layer, arg[0], 0, 0 ) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif
    if (LoadBitmap (HDC_SCREEN, &bmp_bkgnd, "./bkgnd.jpg"))
        return 1;

	//MGPlusRegisterFashionLFRDR();
	if(args > 1)
		SetDefaultWindowElementRenderer(arg[1]);

    InitCreateInfo (&CreateInfo);

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow (hMainWnd, SW_SHOWNORMAL);

    while( GetMessage (&Msg, hMainWnd) ) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    UnloadBitmap (&bmp_bkgnd);


    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
