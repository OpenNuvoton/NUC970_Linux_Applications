/*
** $Id: gridview_ext.c 555 2008-03-04 05:52:43Z wangjian $
**
** gridview_ext.c: Sample program for MiniGUI Programming Guide
**      Usage of GRID control.
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

#ifdef _MGCTRL_GRIDVIEW

#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _LANG_ZHCN
#include "gridview_ext_res_cn.h"
#elif defined _LANG_ZHTW
#include "gridview_ext_res_tw.h"
#else
#include "gridview_ext_res_en.h"
#endif

int ww = 700;
int wh = 580;

enum {
    ID_ADDCOLUMN = 100,
    ID_DELCOLUMN,
    ID_ADDROW,
    ID_DELROW,
    ID_GETCOLNUM,
    ID_GETROWNUM,
    ID_SETPROPERTY,
    ID_SETCOLWIDTH,
    ID_SETROWHEIGHT,
    ID_GETCOLWIDTH,
    ID_GETROWHEIGHT,
    ID_SETSELECTED,
    ID_SETNUMBERCELL,
    ID_SETCHECKBOX,
    ID_SETSELECTION,
};

enum {
    IDC_GRIDVIEW,
};

static HWND hGVWnd;
static HWND hMainWnd;

static int button_x = 20;
static int button_y = 340;

void add_button(const char* text, int id)
{
    SIZE size;
    GetTextExtent(HDC_SCREEN, text, -1, &size);
    if(button_x + size.cx + 10 > 600)
    {
        button_x = 20;
        button_y += 40;
    }
    CreateWindow(CTRL_BUTTON, text, WS_CHILD | WS_VISIBLE | WS_GROUP, 
                id, button_x, button_y, size.cx + 10, 30, hMainWnd, 0);
    
    button_x += size.cx + 20;
}

static int
ControlTestWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
        {
            GRIDVIEWDATA gvdata;
            gvdata.nr_rows = 30;
            gvdata.nr_cols = 30;
            gvdata.row_height = 20;
            gvdata.col_width = 80;
            hGVWnd = CreateWindowEx (CTRL_GRIDVIEW, "Grid View",
                                       WS_CHILD | WS_VISIBLE | WS_VSCROLL |
                                       WS_HSCROLL | WS_BORDER, WS_EX_NONE, IDC_GRIDVIEW, 20, 20, 600,
                                       300, hWnd, (DWORD)&gvdata);
            hMainWnd = hWnd;
            add_button(GVEX_ST_ADDROW, ID_ADDROW);
            add_button(GVEX_ST_DELROW, ID_DELROW);
            add_button(GVEX_ST_ADDCOL, ID_ADDCOLUMN);
            add_button(GVEX_ST_DELCOL, ID_DELCOLUMN);
            add_button(GVEX_ST_GETROWNUM, ID_GETROWNUM);
            add_button(GVEX_ST_GETCOLNUM, ID_GETCOLNUM);
            add_button(GVEX_ST_SETGRID, ID_SETPROPERTY);
            add_button(GVEX_ST_SETCOLW, ID_SETCOLWIDTH);
            add_button(GVEX_ST_SETROWH, ID_SETROWHEIGHT);
            add_button(GVEX_ST_GETCOLW, ID_GETCOLWIDTH);
            add_button(GVEX_ST_GETROWH, ID_GETROWHEIGHT);
            add_button(GVEX_ST_SETSEL, ID_SETSELECTED);
            add_button(GVEX_ST_SETNUMCELL, ID_SETNUMBERCELL);
            add_button(GVEX_ST_SETCHECKBOX, ID_SETCHECKBOX);
            add_button(GVEX_ST_SETSELECTION, ID_SETSELECTION);
            return 0;
        }
        case MSG_COMMAND:
            {
                int id = LOWORD(wParam);
                switch(id)
                {
                    case ID_ADDCOLUMN:
                        SendMessage(hGVWnd, GRIDM_ADDCOLUMN, 0 ,0);
                        break;
                    case ID_DELCOLUMN:
                        SendMessage(hGVWnd, GRIDM_DELCOLUMN, 0 ,0);
                        break;
                    case ID_ADDROW:
                        SendMessage(hGVWnd, GRIDM_ADDROW, 0 ,0);
                        break;
                    case ID_DELROW:
                        SendMessage(hGVWnd, GRIDM_DELROW, 0 ,0);
                        break;
                    case ID_GETROWNUM:
                        {
                            char buf[64];
                            int num = SendMessage(hGVWnd, GRIDM_GETROWCOUNT, 0 ,0);
                            sprintf(buf, GVEX_ST_ROWNUMS, num);
                            MessageBox( hGVWnd, buf, GVEX_ST_ROWNUM, MB_OK );
                            break;
                        }
                    case ID_GETCOLNUM:
                        {
                            char buf[64];
                            int num = SendMessage(hGVWnd, GRIDM_GETCOLCOUNT, 0 ,0);
                            sprintf(buf, GVEX_ST_COLNUMS, num);
                            MessageBox( hGVWnd, buf, GVEX_ST_COLNUM, MB_OK );
                            break;
                        }
                    case ID_SETPROPERTY:
                        {
                            GRIDCELLS cells;
                            GRIDCELLDATATEXT text;
                            GRIDCELLDATA data;
                            text.buff = GVEX_ST_TEXT;
                            text.len_buff = 6;
                            data.style = GV_TYPE_TEXT | GV_HALIGN_LEFT | GV_VALIGN_CENTER;
                            data.mask = GVITEM_MAINCONTENT | GVITEM_STYLE | GVITEM_BGCOLOR | GVITEM_FGCOLOR;
                            data.color_bg = PIXEL_blue;
                            data.color_fg = PIXEL_red;
                            data.content = &text;
                            cells.row = 2;
                            cells.column = 2;
                            cells.width = 1;
                            cells.height = 1;
                            SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cells, (LPARAM)&data);
                            break;
                        }
                    case ID_SETNUMBERCELL:
                        {
                            GRIDCELLS cells;
                            GRIDCELLDATANUMBER number;
                            GRIDCELLDATA data;
                            number.number = 98.2323232;
                            number.format = "$%.2lf";
                            number.len_format = -1;
                            data.style = GV_TYPE_NUMBER | GV_HALIGN_LEFT | GV_VALIGN_CENTER;
                            data.mask = GVITEM_MAINCONTENT | GVITEM_STYLE;
                            data.content = &number;
                            cells.row = 3;
                            cells.column = 3;
                            cells.height = 1;
                            cells.width = 2;
                            SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cells, (LPARAM)&data);
                            break;
                        }
                    case ID_SETCHECKBOX:
                        {
                            GRIDCELLS cells;
                            GRIDCELLDATACHECKBOX checkbox;
                            GRIDCELLDATA data;
                            checkbox.checked = FALSE;
                            checkbox.text = GVEX_ST_CHEKTEXT;
                            checkbox.len_text = -1;
                            data.style = GV_TYPE_CHECKBOX | GV_HALIGN_LEFT | GV_VALIGN_CENTER;
                            data.mask = GVITEM_MAINCONTENT | GVITEM_STYLE | GVITEM_BGCOLOR | GVITEM_FGCOLOR;
                            data.color_bg = PIXEL_blue;
                            data.color_fg = PIXEL_red;
                            data.content = &checkbox;
                            cells.row = 1;
                            cells.column = 6;
                            cells.height = 5;
                            cells.width = 1;
                            SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cells, (LPARAM)&data);
                            break;
                        }
                    case ID_SETSELECTION:
                        {
                            GRIDCELLS cells;
                            GRIDCELLDATASELECTION selection;
                            GRIDCELLDATA data;
                            selection.cur_index = 0;
                            selection.selections = "1\n2\n3\n";
                            selection.len_sel = -1;
                            data.style = GV_TYPE_SELECTION| GV_HALIGN_LEFT | GV_VALIGN_CENTER;
                            data.mask = GVITEM_MAINCONTENT | GVITEM_STYLE | GVITEM_BGCOLOR | GVITEM_FGCOLOR;
                            data.color_bg = PIXEL_blue;
                            data.color_fg = PIXEL_red;
                            data.content = &selection;
                            cells.row = 6;
                            cells.column = 6;
                            cells.height = 5;
                            cells.width = 1;
                            SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cells, (LPARAM)&data);
                            break;
                        }
                    case ID_SETCOLWIDTH:
                        {
                            SendMessage(hGVWnd, GRIDM_SETCOLWIDTH, 2, 150);
                            break;
                        }
                    case ID_SETROWHEIGHT:
                        {
                            SendMessage(hGVWnd, GRIDM_SETROWHEIGHT, 2, 150);
                            break;
                        }
                    case ID_GETCOLWIDTH:
                        {
                            char buf[64];
                            int num = SendMessage(hGVWnd, GRIDM_GETCOLWIDTH, 0 ,2);
                            sprintf(buf, GVEX_ST_2NDCOLW, num);
                            MessageBox( hGVWnd, buf, GVEX_ST_COLW, MB_OK );
                            
                            break;
                        }
                    case ID_GETROWHEIGHT:
                        {
                            char buf[64];
                            int num = SendMessage(hGVWnd, GRIDM_GETROWHEIGHT, 0 ,2);
                            sprintf(buf, GVEX_ST_2NDROWH, num);
                            MessageBox( hGVWnd, buf, GVEX_ST_ROWH, MB_OK );
                            
                            break;
                        }
                    case ID_SETSELECTED:
                        {
                            GRIDCELLS cells;
                            cells.row = 2;
                            cells.column = 2;
                            cells.width = 2;
                            cells.height = 2;
                            SendMessage(hGVWnd, GRIDM_SETSELECTED, 0 , (LPARAM)&cells);
                            break;
                        }
                }
                return 0;
            }


//        case IDC_SETPROPERTY:
//            GRIDITEMDATA data;
//            data.mask = GVITEM_BGCOLOR | GVITEM_FGCOLOR | GVITEM_FONT;
//            data.bgColor = "";
//            data.fgColor = "";
//            data.font = "";
//            SendMessage(hGVWnd, GRIDM_SETPROPERTY, (WPARAM)"D", (LPARAM)&data);
//            return 0;
//            
//        case IDC_SETCALFUNC:
//            SendMessage(hGVWnd, GRIDM_SETCALFUNC, (WPARAM)"D4", (LPARAM)pfnCaculate);
//            return 0;
//
//        case IDC_SETNUMFORMAT:
//            SendMessage(hGVWnd, GRIDM_SETNUMFORMAT, (WPARAM)"C2", (LPARAM)"%1.2f");
//            return 0;

        case MSG_CLOSE:
            PostQuitMessage (hWnd);
            DestroyMainWindow (hWnd);
            return 0;
            
    }
    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "gridview_ext" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = GVEX_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = ControlTestWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = ww;
    CreateInfo.by = wh;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

#else
int MiniGUIMain (int argc, const char* argv[])
{
    printf ("The GridView control is not enabled in your MiniGUI\n");
    return -1;
}
#endif /*_MGCTRL_GRIDVIEW*/
