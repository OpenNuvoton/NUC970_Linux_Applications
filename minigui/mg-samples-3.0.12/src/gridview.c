/*
 * ** $Id: gridview.c 741 2009-03-31 07:16:18Z weiym $
 * **
 * ** gridview.c: Sample program for MiniGUI Programming Guide
 * **      Usage of GRID control.
 * **
 * ** Copyright (C) 2004 ~ 2007 Feynman Software.
 * **
 * ** License: GPL
 * */


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
#include "gridview_res_cn.h"
#elif defined _LANG_ZHTW
#include "gridview_res_tw.h"
#else
#include "gridview_res_en.h"
#endif

int ww = 400;
int wh = 240;

enum {
    IDC_GRIDVIEW,
};

static HWND hGVWnd;

static char* colnames[] = {
    GV_ST_CHINESE, 
    GV_ST_MATH, 
    GV_ST_ENGLISH, 
    GV_ST_TOTAL
};
static char* scores[] = {
    GV_ST_STUD1,
    GV_ST_STUD2,
    GV_ST_STUD3,
    GV_ST_STUD4,
    GV_ST_AVERAGE 
};

int total(GRIDCELLS* target, GRIDCELLS* source, DWORD dwAddData)
{
    int i, j;
    double value = 0;
    GRIDCELLDATA data;
    GRIDCELLS cells;
    GRIDCELLDATANUMBER num;
    memset(&data, 0, sizeof(data));
    memset(&num, 0, sizeof(num));
    data.mask = GVITEM_MAINCONTENT|GVITEM_STYLE;
    data.content = &num;
    data.style = GV_TYPE_NUMBER;
    cells.width = 1;
    cells.height = 1;
    for(i = 0; i<source->width; i++)
    {
        cells.column = source->column + i;
        for (j = 0; j<source->height; j++)
        {
            cells.row = source->row + j;
            SendMessage(hGVWnd, GRIDM_GETCELLPROPERTY, (WPARAM)&cells, (LPARAM)&data);
            value += num.number;
        }
    }
    num.number = value;
    num.len_format = -1;
    cells.row = target->row;
    cells.column = target->column;
    SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cells, (LPARAM)&data);
    
    return 0;
}

int averge(GRIDCELLS* target, GRIDCELLS* source, DWORD dwAddData)
{
    int i, j;
    int count = 0;
    double value = 0;
    GRIDCELLDATA data;
    GRIDCELLS cells;
    GRIDCELLDATANUMBER num;
    memset(&data, 0, sizeof(data));
    memset(&num, 0, sizeof(num));
    data.content = &num;
    data.style = GV_TYPE_NUMBER;
    cells.width = 1;
    cells.height = 1;
    for(i = 0; i<source->width; i++)
    {
        cells.column = source->column + i;
        for (j = 0; j<source->height; j++)
        {
            data.content = &num;
            data.style = GV_TYPE_NUMBER;
            cells.row = source->row + j;
            SendMessage(hGVWnd, GRIDM_GETCELLPROPERTY, (WPARAM)&cells, (LPARAM)&data);
            value += num.number;
            count++;
        }
    }
    data.mask = GVITEM_MAINCONTENT;
    num.number = value/count;
    cells.row = target->row;
    cells.column = target->column;
    SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cells, (LPARAM)&data);
    
    return 0;
}

static int
ControlTestWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
            {
                int i;
                GRIDCELLS cellsel;
                GRIDCELLDEPENDENCE dep;
                GRIDCELLDATA celldata;
                GRIDCELLDATAHEADER header;
                GRIDCELLDATANUMBER cellnum;
                GRIDVIEWDATA gvdata;

                gvdata.nr_rows = 4;
                gvdata.nr_cols = 3;
                gvdata.row_height = 20;
                gvdata.col_width  = 60;
                hGVWnd = CreateWindowEx (CTRL_GRIDVIEW, "Grid View",
                                           WS_CHILD | WS_VISIBLE | WS_VSCROLL |
                                           WS_HSCROLL | WS_BORDER, WS_EX_NONE, IDC_GRIDVIEW, 20, 20, 304,124, hWnd, (DWORD)&gvdata);
                memset(&header, 0, sizeof(header));
                memset(&celldata, 0, sizeof(celldata));
                //设置列表头的属性
                for (i = 1; i<= 3; i++)
                {
                    header.buff = colnames[i-1];
                    header.len_buff = -1;
                    celldata.content = &header;
                    celldata.mask = GVITEM_MAINCONTENT;
                    celldata.style = GV_TYPE_HEADER;
                    cellsel.row = 0;
                    cellsel.column = i;
                    cellsel.width = 1;
                    cellsel.height = 1;
                    SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cellsel, (LPARAM)&celldata);

                }

                //设置行表头的属性
                memset(&header, 0, sizeof(header));
                memset(&celldata, 0, sizeof(celldata));
                for (i = 1; i<= 4; i++)
                {
                    header.buff =  scores[i-1];
                    header.len_buff = -1;
                    celldata.content = &header;
                    celldata.mask = GVITEM_MAINCONTENT;
                    celldata.style = GV_TYPE_HEADER;
                    cellsel.row = i;
                    cellsel.column = 0;
                    cellsel.width = 1;
                    cellsel.height = 1;
                    SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cellsel, (LPARAM)&celldata);

                }

                //设置单元格的属性
                memset(&celldata, 0, sizeof(celldata));
                memset(&cellnum, 0, sizeof(cellnum));
                cellnum.number = 50;
                cellnum.format = NULL;
                celldata.content = &cellnum;
                celldata.mask = GVITEM_MAINCONTENT;
                celldata.style = GV_TYPE_NUMBER;
                cellsel.row = 1;
                cellsel.column = 1;
                cellsel.width = 3;
                cellsel.height = 4;
                SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cellsel, (LPARAM)&celldata);
                
                //增加一列的操作
                memset(&header, 0, sizeof(header));
                memset(&celldata, 0, sizeof(celldata));
                header.buff = GV_ST_TOTAL;
                header.size = -1;
                celldata.mask = GVITEM_MAINCONTENT;
                celldata.content = &header;
                celldata.style = GV_TYPE_HEADER;
                SendMessage(hGVWnd, GRIDM_ADDCOLUMN, 3, (LPARAM)&celldata);

                //增加一行的操作
                memset(&header, 0, sizeof(header));
                memset(&celldata, 0, sizeof(celldata));
                header.buff = GV_ST_AVERAGE;
                header.size = -1;
                celldata.mask = GVITEM_MAINCONTENT;
                celldata.content = &header;
                celldata.style = GV_TYPE_HEADER;
                SendMessage(hGVWnd, GRIDM_ADDROW, 4, (LPARAM)&celldata);

                memset(&celldata, 0, sizeof(celldata));
                memset(&cellnum, 0, sizeof(cellnum));
                cellnum.number = 0;
                cellnum.format = NULL;
                celldata.content = &cellnum;
                celldata.mask = GVITEM_MAINCONTENT;
                celldata.style = GV_TYPE_NUMBER;
                cellsel.row = 1;
                cellsel.column = 4;
                cellsel.width = 1;
                cellsel.height = 4;
                SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cellsel, (LPARAM)&celldata);

                cellsel.row = 5;
                cellsel.column = 1;
                cellsel.width = 4;
                cellsel.height = 1;
                SendMessage(hGVWnd, GRIDM_SETCELLPROPERTY, (WPARAM)&cellsel, (LPARAM)&celldata);


                // 给这个单元格设置求和函数.
                memset(&dep, 0, sizeof(dep));
                dep.callback = total;
                for (i = 1; i<= 4; i++)
                {
                    dep.source.row = i;
                    dep.source.column = 1;
                    dep.source.width = 3;
                    dep.source.height = 1;
                    dep.target.row = i;
                    dep.target.column = 4;
                    dep.target.width = 1;
                    dep.target.height = 1;
                    SendMessage(hGVWnd, GRIDM_ADDDEPENDENCE, 0, (LPARAM)&dep);
                }

                dep.callback = averge;
                // 给这个单元格设置求平均值函数.
                for (i = 1; i<= 4; i++)
                {
                    dep.source.row = 1;
                    dep.source.column = i;
                    dep.source.width = 1;
                    dep.source.height = 4;
                    dep.target.row = 5;
                    dep.target.column = i;
                    dep.target.width = 1;
                    dep.target.height = 1;
                    SendMessage(hGVWnd, GRIDM_ADDDEPENDENCE, 0, (LPARAM)&dep);
                }

                return 0;
            }
        case MSG_COMMAND:
            break;
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
    JoinLayer(NAME_DEF_LAYER , "gridview" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = GV_ST_CAP;
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


