/*
** $Id: main.c,v 1.26 2006/06/08 09:50:37 xwyan Exp $
**
** NoteBook, the M$ Windows like notepad Editor on MiniGUI.
** Copyright (c) 2000, Feng Da ke (minx@thtfchain.com).
**
** Some idea and source come from VConGUI (Virture Console GUI) 
**
** Create date: 2000/07/05
** BTW: today, my sweetheart lovly MinMin comes back from millitery training.
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

/*
** Modify records:
**
**  Who             When        Where       For What                Status
**-----------------------------------------------------------------------------
** FrankXM         2000.07.05   CIMS104     Create                  processing
** leon            2000.11.07   BJ.blpt     Fix bugs                Finished
**
** TODO:
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#if defined(MINIGUI_MAJOR_VERSION)&&(MINIGUI_MAJOR_VERSION <= 2)
#include <minigui/mgext.h>
#include <minigui/mywindows.h>
#include <minigui/filedlg.h>
#include <minigui/newfiledlg.h>
#else
//#include <mgutils/mgutils.h>
#endif

#include "notebook.h"

#ifdef _LANG_ZHCN
#include "notebook_res_cn.h"
#elif defined _LANG_ZHTW
#include "notebook_res_tw.h"
#else
#include "notebook_res_en.h"
#endif

#define dbg() printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__)

static void AboutNotebook (HWND hParent)
{
    MessageBox (hParent, 
        NB_STR_ABOUT_INFO,
        NB_STR_CAP,
        MB_OK | MB_ICONEXCLAMATION);
};


static BOOL MergeFileFullName(PNOTEINFO pNoteInfo)
{
    if ((strlen(pNoteInfo->filePath) < 1) || (strlen(pNoteInfo->fileName) < 1))
        return FALSE;
    if (pNoteInfo->filePath[strlen(pNoteInfo->filePath)-1] != '/')
        strcat (pNoteInfo->filePath,"/");
    strcpy (pNoteInfo->fileFullName,pNoteInfo->filePath);
    strcat (pNoteInfo->fileFullName,pNoteInfo->fileName);
    return TRUE;
}
BOOL DivideFileFullName(PNOTEINFO pNoteInfo)
{
       char currentpath [PATH_MAX + 1];
    char *str;
    int temp ,i,j;
    temp = strlen(pNoteInfo->fileFullName);
        getcwd(currentpath,PATH_MAX);
    if (temp <1) return FALSE;
    if (pNoteInfo->fileFullName[temp-1]=='/') return FALSE;
    if (strchr(pNoteInfo->fileFullName,'/')==NULL)
    {
        strcpy(pNoteInfo->filePath,currentpath);
        strcpy(pNoteInfo->fileName,pNoteInfo->fileFullName);
        MergeFileFullName(pNoteInfo);
        return TRUE;
    }
    for (i= temp-1;i>=0;i--)
    {
        if (pNoteInfo->fileFullName[i]=='/')
            break;
    }
    for (j=0;j<=i;j++)
    {
        pNoteInfo->filePath[j]=pNoteInfo->fileFullName[j];
    }
    pNoteInfo->filePath[i+1]=0;
    str = (char*)(pNoteInfo->fileFullName + i + 1);
    strcpy (pNoteInfo->fileName,str);
    return TRUE;
}
static HMENU createpmenuabout (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)NB_STR_HELP;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_ABOUT_THIS;
    mii.typedata    = (DWORD)NB_STR_ABOUT;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
    
    return hmnu;
}
static HMENU createpmenusearch (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)NB_STR_SEARCH;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_SEARCH;
    mii.typedata    = (DWORD)NB_STR_FIND;
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_SECHNEXT;
    mii.typedata    = (DWORD)NB_STR_FIND_NEXT;
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    return hmnu;
}

static HMENU createpmenufile (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)NB_STR_FILE;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_NEW;
    mii.typedata    = (DWORD)NB_STR_NEW;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_OPEN;
    mii.typedata    = (DWORD)NB_STR_OPEN;
    InsertMenuItem(hmnu, 1, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_SAVE;
    mii.typedata    = (DWORD)NB_STR_SAVE;
    InsertMenuItem(hmnu, 2, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_SAVEAS;
    mii.typedata    = (DWORD)NB_STR_SAVE_AS;
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_PRINT;
    mii.typedata    = (DWORD)NB_STR_PRINT;
    InsertMenuItem(hmnu, 5, TRUE, &mii);
    
    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_EXIT;
    mii.typedata    = (DWORD)NB_STR_EXIT;
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    return hmnu;
}

static HMENU createpmenuedit (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)NB_STR_EDIT;
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_UNDO;
    mii.typedata    = (DWORD)NB_STR_UNDO;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
     
    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_CUT;
    mii.typedata    = (DWORD)NB_STR_CUT;
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_COPY;
    mii.typedata    = (DWORD)NB_STR_COPY;
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_PASTE;
    mii.typedata    = (DWORD)NB_STR_PASTE;
    InsertMenuItem(hmnu, 4, TRUE, &mii);  
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_DEL;
    mii.typedata    = (DWORD)NB_STR_DELETE;
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_SELALL;
    mii.typedata    = (DWORD)NB_STR_SELECT_ALL;
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_INSDATE;
    mii.typedata    = (DWORD)NB_STR_INSERT_DATE;
    InsertMenuItem(hmnu, 8, TRUE, &mii);

    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 9, TRUE, &mii);

    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_AUTOWRAP;
    mii.typedata    = (DWORD)NB_STR_AUTO_WRAP;
    InsertMenuItem(hmnu, 10, TRUE, &mii);

    return hmnu;
}


static HMENU createpmenuview (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)NB_STR_VIEW;
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_40X15;
    mii.typedata    = (DWORD)"40x15 (small)";
    InsertMenuItem(hmnu, 0, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_80X24;
    mii.typedata    = (DWORD)"80x24 (vt100)";
    InsertMenuItem(hmnu, 1, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = MF_CHECKED;
    mii.id          = IDM_80X25;
    mii.typedata    = (DWORD)"80x25 (ibmpc)";
    InsertMenuItem(hmnu, 2, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_80X40;
    mii.typedata    = (DWORD)"80x40 (xterm)";
    InsertMenuItem(hmnu, 3, TRUE, &mii);
     
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_80X52;
    mii.typedata    = (DWORD)"80x52 (ibmvga)";
    InsertMenuItem(hmnu, 4, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_96X25;
    mii.typedata    = (DWORD)"96x25 (wide)";
    InsertMenuItem(hmnu, 5, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_96X40;
    mii.typedata    = (DWORD)"96x40 (My favorite)";
    InsertMenuItem(hmnu, 6, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_96X52;
    mii.typedata    = (DWORD)"96x52 (large)";
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_CUSTOMIZE;
    mii.typedata    = (DWORD)NB_STR_MODIFY_CFG;
    InsertMenuItem(hmnu, 8, TRUE, &mii);
    
    mii.type        = MFT_SEPARATOR;
    mii.state       = 0;
    mii.id          = 0;
    mii.typedata    = 0;
    InsertMenuItem(hmnu, 9, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_DEFAULT;
    mii.typedata    = (DWORD)NB_STR_DFL_CHAR_SET;
    InsertMenuItem(hmnu, 10, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_ANSI;
    mii.typedata    = (DWORD)"ANSI";
    InsertMenuItem(hmnu, 11, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_GB2312;
    mii.typedata    = (DWORD)"GB2312";
    InsertMenuItem(hmnu, 12, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_THAI;
    mii.typedata    = (DWORD)"THAI";
    InsertMenuItem(hmnu, 13, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_BIG5;
    mii.typedata    = (DWORD)"BIG5";
    InsertMenuItem(hmnu, 14, TRUE, &mii);

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
    mii.typedata    = (DWORD)NB_STR_FILE;
    mii.hsubmenu    = createpmenufile ();

    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)NB_STR_EDIT;
    mii.hsubmenu    = createpmenuedit ();
    InsertMenuItem(hmnu, 1, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.id          = 120;
    mii.typedata    = (DWORD)NB_STR_VIEW;
    mii.hsubmenu    = createpmenuview ();
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 130;
    mii.typedata    = (DWORD)NB_STR_SEARCH;
    mii.hsubmenu    = createpmenusearch ();
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 140;
    mii.typedata    = (DWORD)NB_STR_HELP;
    mii.hsubmenu    = createpmenuabout ();
    InsertMenuItem(hmnu, 4, TRUE, &mii);
                       
    return hmnu;
}


static void SetWindowSize (HWND hWnd, PNOTEINFO pNoteInfo, WPARAM cmd_id)
{
    int col, row;
    
    switch (cmd_id) {
    case IDM_40X15:
        col = 40;
        row = 15;
        break;
    case IDM_80X24:
        col = 80;
        row = 24;
        break;
    case IDM_80X25:
        col = 80;
        row = 25;
        break;
    case IDM_80X40:
        col = 80;
        row = 40;
        break;
    case IDM_80X52:
        col = 80;
        row = 52;
        break;
    case IDM_96X25:
        col = 96;
        row = 25;
        break;
    case IDM_96X40:
        col = 96;
        row = 40;
        break;
    case IDM_96X52:
        col = 96;
        row = 52;
        break;
    case IDM_CUSTOMIZE:
    {
        /*char cols [10];
        char rows [10];
        char* newcols = cols;
        char* newrows = rows;
        myWINENTRY entries [] = {
            { NB_STR_COL_NUM, &newcols, 0, 0 },
            { NB_STR_ROW_NUM, &newrows, 0, 0 },
            { NULL, NULL, 0, 0 }
        };
        myWINBUTTON buttons[] = {
            { NB_STR_OK, IDOK, BS_DEFPUSHBUTTON },
            { NB_STR_CANCEL, IDCANCEL, 0 },
            { NULL, 0, 0}
        };
        int result;

        sprintf (cols, "%d", pNoteInfo->cols);
        sprintf (rows, "%d", pNoteInfo->rows);
        result = myWinEntries (hWnd, 
		NB_STR_NEW_NOTEBOOK_SIZE, 
		NB_STR_SPECIFY_NEW_WIN_SIZE,
		240, 150, FALSE, entries, buttons);
        if (newcols) {
            col = atoi (newcols);
            free (newcols);
        }
        else
            return;

        if (newrows) {
            row = atoi (newrows);
            free (newrows);
        }
        else
            return;

        if (result != IDOK)
            return;

        if (col < MIN_COLS || col > MAX_COLS 
                || row < MIN_ROWS || row > MAX_ROWS) {
            MessageBox (hWnd, 
			NB_STR_SPECIFY_VALID_SIZE, 
			NB_STR_CAP, 
			MB_OK | MB_ICONINFORMATION);
            return;
        }
        break;*/
    }
    default:
        return;
    }

    pNoteInfo->winType = cmd_id;
}

static BOOL SetCharset (HWND hWnd, PNOTEINFO pNoteInfo, WPARAM cmd_id)
{
    LOGFONT log_font;
    PLOGFONT sys_font;
    char* charset;

    if (pNoteInfo->editCharset == cmd_id)
        return FALSE;

    switch (cmd_id) {
    case IDM_ANSI:
        charset = "ISO8859-1";
        break;

    case IDM_GB2312:
        charset = "GB2312-80";
        break;

    case IDM_THAI:
        charset = "ISO8859-11";
        break;

    case IDM_BIG5:
        charset = "BIG5";
        break;

    case IDM_DEFAULT:
        charset = NULL;
        break;

    default:
        return FALSE;
    }

    if (cmd_id == IDM_DEFAULT)
        pNoteInfo->log_font = NULL;
    else {
        sys_font = GetSystemFont (SYSLOGFONT_WCHAR_DEF);
        memcpy (&log_font, sys_font, sizeof (LOGFONT));

        if (pNoteInfo->log_font)
            DestroyLogFont (pNoteInfo->log_font);

        strcpy (log_font.charset, charset);
        pNoteInfo->log_font = CreateLogFontIndirect (&log_font);
    }

    pNoteInfo->editCharset = cmd_id;

    SetWindowFont (pNoteInfo->hMLEditWnd, pNoteInfo->log_font);
    return TRUE;
}

//modified by leon to fix the saveas bug
BOOL NBSaveAs (PNOTEINFO pNoteInfo, HWND hParent, HWND hMLEditWnd)
{
    //NEWFILEDLGDATA myWinFileData;
    char filepath[256];
    memset(filepath, 0, 256);
    FILE * file;
    //int choise=0;
    int reallength=0;
    char buffer[102400];

#if 0
    int cwd_len=0;
    //char* ppath = getcwd(filepath, cwd_len); 
    strcpy (myWinFileData.filepath, pNoteInfo->filePath);
    strcpy (myWinFileData.filter, "All file (*.*) |Text file (*.txt)");
    myWinFileData.IsSave = TRUE;
    
    choise = ShowOpenDialog (hParent, 0, 0, 320, 240, &myWinFileData);
    if (choise == IDOK)
    {
        strcpy(pNoteInfo->fileFullName,myWinFileData.filefullname);
        DivideFileFullName(pNoteInfo);
        umask (S_IXGRP | S_IWOTH);
        if ((file = fopen (pNoteInfo->fileFullName, "w+")) == NULL)
        {
             MessageBox (hParent, NB_STR_FILE_OPEN_FAILED, NB_STR_CAP, MB_OK | MB_ICONSTOP);
             return FALSE;
        }
        reallength = GetWindowTextLength(hMLEditWnd);
        GetWindowText(hMLEditWnd,buffer,102400);
        //fprintf(stderr,"\n in saveas:\nreallength:%d\ntext:\n%s",reallength,buffer);
        if (fwrite(buffer, 1, reallength, file) < 0) 
             MessageBox (hParent, NB_STR_FILE_SAVE_FAILED, NB_STR_CAP, MB_OK | MB_ICONEXCLAMATION);
        pNoteInfo->isChanged = FALSE;
        fclose (file);
        return TRUE;
    }
#endif

    strcpy(pNoteInfo->fileFullName, "./notebook1.txt");
    DivideFileFullName(pNoteInfo);
    umask (S_IXGRP | S_IWOTH);
    if ((file = fopen (pNoteInfo->fileFullName, "w+")) == NULL)
    {
        MessageBox (hParent, NB_STR_FILE_OPEN_FAILED, NB_STR_CAP, MB_OK | MB_ICONSTOP);
        return FALSE;
    }
    reallength = GetWindowTextLength(hMLEditWnd);
    GetWindowText(hMLEditWnd,buffer,102400);
    //fprintf(stderr,"\n in saveas:\nreallength:%d\ntext:\n%s",reallength,buffer);
    if (fwrite(buffer, 1, reallength, file) < 0) 
        MessageBox (hParent, NB_STR_FILE_SAVE_FAILED, NB_STR_CAP, MB_OK | MB_ICONEXCLAMATION);
    pNoteInfo->isChanged = FALSE;
    fclose (file);
    return TRUE;
}

BOOL NBSave (PNOTEINFO pNoteInfo, HWND hParent, HWND hMLEditWnd)
{
    FILE *file;
    char buffer[102400];
    long reallength=0;
    if (strcmp (pNoteInfo->fileName,"tmp.txt")==0)
    {
        return NBSaveAs (pNoteInfo, hParent, hMLEditWnd);
    }
    strcpy(pNoteInfo->fileFullName, "./notebook1.txt");
    file = fopen(pNoteInfo->fileFullName, "wb+");
    if (file == NULL)
    {
         MessageBox (hParent, NB_STR_FILE_OPEN_FAILED, NB_STR_CAP, MB_OK | MB_ICONSTOP);
         return FALSE;
    }
    reallength = GetWindowTextLength(hMLEditWnd);
    GetWindowText(hMLEditWnd,buffer,102399);
    if (fwrite(buffer, 1, reallength, file) < 0) 
         MessageBox (hParent, NB_STR_FILE_SAVE_FAILED, NB_STR_CAP, MB_OK | MB_ICONEXCLAMATION);
    fclose (file);
    pNoteInfo->isChanged = FALSE;
    return TRUE;
}

BOOL NBOpen(PNOTEINFO pNoteInfo, HWND hParent, HWND hMLEditWnd)
{
    //FILEDLGDATA myWinFileData;
    char filepath[256];
    memset(filepath, 0, 256);

    char* ppath = getcwd (filepath, 255); 
    int choise=0,fd;
    long reallength=0;
    char buffer[102400];

    //strcpy (myWinFileData.filepath, ".");
    strcpy (filepath, ".");
    //myWinFileData.is_save = FALSE;
    if (pNoteInfo->isChanged) {
         choise = MessageBox (hParent,NB_STR_MODIFIED_IS_SAVE, NB_STR_CAP, MB_YESNOCANCEL | MB_ICONQUESTION);
        if (choise == IDYES)
            NBSave(pNoteInfo, hParent, hMLEditWnd);
        else if (choise == IDCANCEL)
            return FALSE;
        pNoteInfo->isChanged = FALSE;
    }
//    else 
//        fprintf(stderr,"unchanged");
#if 0
#if defined(MINIGUI_MAJOR_VERSION)&&(MINIGUI_MAJOR_VERSION <= 2)
    choise = OpenFileDialog (hParent, FALSE, &myWinFileData);
#else
    choise = FileOpenSaveDialog(NULL,hParent,FALSE,&myWinFileData);
#endif
    if(choise == IDOK)
    {
        HCURSOR old_cursor;

//        fprintf(stderr,"Open File: %s \n",myWinFileData.filefullname);
        if ( access (myWinFileData.filefullname, F_OK) < 0)
            MessageBox (hParent, NB_STR_FILE_NOT_EXIST, NB_STR_CAP, MB_OK | MB_ICONSTOP);
        else if ( access (myWinFileData.filefullname, R_OK) < 0)
            MessageBox (hParent, NB_STR_FILE_NOT_READABLE, NB_STR_CAP, MB_OK | MB_ICONSTOP);
        else 
        {
            if ( access (myWinFileData.filefullname, W_OK) < 0)
                MessageBox (hParent, NB_STR_FILE_NOT_WRITABLE, NB_STR_CAP, MB_OK | MB_ICONEXCLAMATION);
            fd = open(myWinFileData.filefullname,O_RDONLY);
            if (fd <= 0)
            {
                 MessageBox (hParent, NB_STR_FILE_OPEN_FAILED, NB_STR_CAP, MB_OK | MB_ICONSTOP);
                 return FALSE;
            }

            old_cursor = SetDefaultCursor (GetSystemCursor (IDC_WAIT));
            if ((reallength=read(fd,buffer,102399)) >= 102399) 
                 MessageBox (hParent, NB_STR_FILE_TRUNCTED, NB_STR_CAP, MB_OK | MB_ICONEXCLAMATION);
            close (fd);
            buffer[reallength]=0;
            SetWindowText (hMLEditWnd, buffer);
            SetDefaultCursor (old_cursor);
            strcpy(pNoteInfo->fileFullName,myWinFileData.filefullname);
            DivideFileFullName(pNoteInfo);
            return TRUE;
        }
    }    
#endif
    fd = open("./notebook1.txt", O_CREAT | O_RDWR, 0644);
    if (fd <= 0)
    {
        MessageBox (hParent, NB_STR_FILE_OPEN_FAILED, NB_STR_CAP, MB_OK | MB_ICONSTOP);
        return FALSE;
    }
    
    HCURSOR old_cursor;
    old_cursor = SetDefaultCursor (GetSystemCursor (IDC_WAIT));
    if ((reallength=read(fd,buffer,102399)) >= 102399) 
        MessageBox (hParent, NB_STR_FILE_TRUNCTED, NB_STR_CAP, MB_OK | MB_ICONEXCLAMATION);
    close (fd);
    buffer[reallength]=0;
    SetWindowText (hMLEditWnd, buffer);
    SetDefaultCursor (old_cursor);
    char buf[256];
    memset(buf, 0, 256);
    strcpy(buf, ppath);
    strcpy(buf+strlen(ppath)-1, "/notebook1.txt");
    strcpy(pNoteInfo->fileFullName,buf);
    DivideFileFullName(pNoteInfo);
    return TRUE;
    //return FALSE;
}

BOOL NBNew(PNOTEINFO pNoteInfo, HWND hParent, HWND hMLEditWnd)
{
    if (pNoteInfo->isChanged)
        NBSave(pNoteInfo, hParent, hMLEditWnd);
    SetWindowText(hMLEditWnd,"");
    strcpy(pNoteInfo->fileFullName,"tmp.txt");
    return DivideFileFullName(pNoteInfo);
}

BOOL NBPrint(HWND hMLEditWnd)
{
    char temp [255];
    GetWindowTextLength (hMLEditWnd); 
    GetWindowText (hMLEditWnd, temp, 254);
    return TRUE;
}

int NoteBookWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HWND hMLEditWnd;
    RECT client;
    char title[NAME_MAX + 10];
    PNOTEINFO pNoteInfo;
    pNoteInfo = (PNOTEINFO) GetWindowAdditionalData(hWnd);
    hMLEditWnd = pNoteInfo->hMLEditWnd;
    GetClientRect(hWnd,&client);
    switch (message) {
    case MSG_CREATE:
        pNoteInfo->hMLEditWnd = CreateWindow ("medit", 
                 "",  WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL,
                IDC_MLEDIT, 0, 0, client.right,client.bottom , hWnd, 0);
        strcpy(title,pNoteInfo->fileName);
        strcat(title," - notebook");
        SetWindowText(hWnd,title);
        break;

    case MSG_SHOWWINDOW:
        if (hMLEditWnd != HWND_INVALID)
            SetFocus (hMLEditWnd);
        return 0;

    case MSG_ACTIVEMENU:
        if (wParam == 2) {
                CheckMenuRadioItem ((HMENU)lParam,
                    IDM_40X15, IDM_96X52,
                    pNoteInfo->winType, MF_BYCOMMAND);
                CheckMenuRadioItem ((HMENU)lParam,
                    IDM_DEFAULT, IDM_BIG5,
                    pNoteInfo->editCharset, MF_BYCOMMAND);
        }
        break;

    case MSG_COMMAND:
        switch (wParam) {
        case IDM_NEW:
            if(NBNew(pNoteInfo, hWnd, hMLEditWnd)) {
                strcpy(title,pNoteInfo->fileName);
                strcat(title," - notebook");
                SetWindowText(hWnd,title);
            }
            break;

        case IDM_OPEN:
            if (NBOpen(pNoteInfo, hWnd, hMLEditWnd)) {
                strcpy(title,pNoteInfo->fileName);
                strcat(title," - notebook");
                SetWindowText(hWnd,title);
            };
            break;

        case IDM_SAVE:
            if(NBSave(pNoteInfo, hWnd, hMLEditWnd)) {
                strcpy(title,pNoteInfo->fileName);
                strcat(title," - notebook");
                SetWindowText(hWnd,title);
            };
            break;

        case IDM_SAVEAS:
            if(NBSaveAs(pNoteInfo, hWnd, hMLEditWnd))
            {
                strcpy(title,pNoteInfo->fileName);
                strcat(title," - notebook");
                SetWindowText(hWnd,title);
            };
            break;

        case IDM_PRINT:
            NBPrint(hMLEditWnd);
            break;

        case IDM_EXIT:
             PostMessage (hWnd, MSG_CLOSE,0,0);
        break;

        case IDM_40X15 ... IDM_96X52:
            SetWindowSize (hWnd, pNoteInfo, wParam);
        break;

        case IDM_DEFAULT... IDM_BIG5:
            SetCharset (hWnd, pNoteInfo, wParam);
            break;

        case IDM_ABOUT_THIS:
            AboutNotebook(hWnd);
        break;

        };

        if ((wParam>>16) == EN_CHANGE) {
            if (hMLEditWnd ==(HWND)lParam) {
                pNoteInfo->isChanged = TRUE;
            }
        };
        return 0;

    case MSG_CLOSE:
        if (pNoteInfo->isChanged) {
                int choise = MessageBox (hWnd,
                            NB_STR_SAVE_OR_NOT,
                            NB_STR_CAP,
                            MB_YESNOCANCEL | MB_ICONQUESTION);
                            
                if ( choise == IDYES)
                    NBSave(pNoteInfo, hWnd, hMLEditWnd);
                else if ( choise == IDCANCEL)
                    break;
        }
        DestroyWindow (hMLEditWnd);
        pNoteInfo->hMLEditWnd = HWND_INVALID;
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

#if 0
static HICON myLoadIcon (const char* filename)
{
    static char res_dir [MAX_PATH + 1] = {'\0'};
    char full_path [MAX_PATH + 1];

    if (res_dir [0] == '\0') {
        if (GetValueFromEtcFile (ETCFILEPATH, "appinfo", "apprespath",
            res_dir, MAX_PATH) < 0)
            strcpy (res_dir, "res/");
    }
    
    strcpy (full_path, res_dir);
    if (full_path [strlen (full_path) - 1] != '/')
        strcat (full_path, "/");
    strcat (full_path, "notebook/");
    strcat (full_path, filename);
    
    return LoadIconFromFile (HDC_SCREEN, full_path, 0); 
}
#endif

static void InitNoteBookInfo (PMAINWINCREATE pCreateInfo, PNOTEINFO pNoteInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER |
                        WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE | WS_SYSMENU;     
    pCreateInfo->dwExStyle = WS_EX_IMECOMPOSE;
    pCreateInfo->spCaption = NB_STR_CAP;
    pCreateInfo->hMenu = createmenu();
    pCreateInfo->hCursor = GetSystemCursor(0);
    pCreateInfo->hIcon = LoadIconFromFile (HDC_SCREEN, "res/notebook.ico", 0); 
    pCreateInfo->MainWindowProc = NoteBookWinProc;
    pCreateInfo->lx = pNoteInfo->lx; 
    pCreateInfo->ty = pNoteInfo->ty;
    pCreateInfo->rx
        = pNoteInfo->lx + ClientWidthToWindowWidth (WS_CAPTION | WS_BORDER,
                pNoteInfo->cols * GetSysCharWidth ());
    pCreateInfo->by
        = pNoteInfo->ty + ClientHeightToWindowHeight (WS_CAPTION | WS_BORDER,
                pNoteInfo->rows * GetSysCharHeight (), TRUE);
    pCreateInfo->iBkColor = COLOR_lightgray; 
    pCreateInfo->dwAddData = (DWORD)pNoteInfo;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void* NoteBook (void* data)
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;
    PNOTEINFO pNoteInfo;
    char currentpath [PATH_MAX + 1];
    static int x = 0, y = 0;

    getcwd(currentpath,PATH_MAX);
    if (data == NULL)
    {
        if(!(pNoteInfo = malloc(sizeof(NOTEINFO)))) return NULL;//error!!
        pNoteInfo->isChanged = FALSE;
        strcpy(pNoteInfo->fileName , "tmp.txt");
        //strcpy(pNoteInfo->fileName , "Î´ÃüÃû.txt");
        strcpy(pNoteInfo->filePath , currentpath);
        pNoteInfo->fileSize = 0;
        pNoteInfo->Buffer = NULL;
        pNoteInfo->hMLEditWnd = HWND_INVALID;

        pNoteInfo->lx = x;
        pNoteInfo->ty = y;
        x += 20; y += 20;
        pNoteInfo->cols = VGASTD_NUMBER_COLS;
        pNoteInfo->rows = VGASTD_NUMBER_ROWS;
        pNoteInfo->winType = IDM_80X25;
        pNoteInfo->editCharset = IDM_DEFAULT;
        pNoteInfo->log_font = NULL;
    }
    else
        pNoteInfo = (PNOTEINFO) data;

    if (pNoteInfo->filePath [strlen (pNoteInfo->filePath) - 1] != '/')
        strcat (pNoteInfo->filePath, "/");
    InitNoteBookInfo (&CreateInfo, pNoteInfo);

#if defined(MINIGUI_MAJOR_VERSION)&&(MINIGUI_MAJOR_VERSION <= 2)
    if ( !InitMiniGUIExt () ) 
        return NULL;
#endif

    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return NULL;

    while (GetMessage (&Msg, hMainWnd) ) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

#if defined(MINIGUI_MAJOR_VERSION)&&(MINIGUI_MAJOR_VERSION <= 2)
    MiniGUIExtCleanUp ();
#endif

    if (pNoteInfo->log_font)
       DestroyLogFont (pNoteInfo->log_font);
    free (pNoteInfo);
    return NULL;
}

#ifndef _LITE_VERSION
void* NewNoteBook (PNOTEINFO pNoteInfo)
{
    pthread_t th;
    
    CreateThreadForMainWindow (&th, NULL, NoteBook, pNoteInfo);

    return NULL;
}

int MiniGUIMain (int args, const char* arg[])
{
#ifdef _IME_GB2312
    GBIMEWindow (HWND_DESKTOP);
#endif

    NoteBook(NULL);
    return 0;
}

#include <minigui/dti.c>

#else

int MiniGUIMain (int args, const char* arg[])
{
#ifndef _STAND_ALONE
    int i;
    const char* layer = NULL;

    for (i = 1; i < args; i++) {
        if (strcmp (arg[i], "-layer") == 0) {
            layer = arg[i + 1];
            break;
        }
    }
    GetLayerInfo (layer, NULL, NULL, NULL);
    if (JoinLayer (layer, arg[0], 
                0 , 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif

    NoteBook (NULL);

    return 0;
}
#endif

