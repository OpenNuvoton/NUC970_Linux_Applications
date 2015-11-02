/* 
** $Id: scrollbar.c 570 2008-03-18 03:54:25Z zhounuohua $
**
** Listing 2.1
**
** scrollbar_ctrl.c: Sample program for MiniGUI Programming Guide
**      Usage of scrollbar control.
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _LANG_ZHCN
#include "scrollbar_ctrl_res_cn.h"
#elif defined _LANG_ZHTW
#include "scrollbar_ctrl_res_tw.h"
#else
#include "scrollbar_ctrl_res_en.h"
#endif

/** define scrollbar data */
#define SB_MAX 20
#define SB_MIN 0
#define SB_PAGE 6

/** define circle data */
#define CC_CENTER_X    400
#define CC_CENTER_Y    100
#define CC_RADIUS_MAX   50
#define CC_RADIUS_MIN   0

/** define box data */
#define BOX_WIDTH   40
#define BOX_HEIGHT  20
#define BOX_Y_MAX   300
#define BOX_Y_MIN   200

/** x position of separator */
#define SEP   300

/** radius of circle */
int _radius;

/** y position of box */
int _box_pos_y;

/** invalidate rect */
RECT _rect_invalid; 

void calc_circle_pos (int curPos)
{
    _radius = (CC_RADIUS_MAX - CC_RADIUS_MIN + 1) * 
        (curPos - SB_MIN) / (SB_MAX - SB_MIN) + CC_RADIUS_MIN;
}

void calc_box_pos (int curPos)
{
    _box_pos_y = (BOX_Y_MAX - BOX_Y_MIN + 1 - BOX_HEIGHT) * 
        (curPos - SB_MIN) / (SB_MAX - SB_MIN) + BOX_Y_MIN;
}

void draw_shape (HWND hwnd, int curPos)
{
    if (curPos < SB_MIN)
        curPos = SB_MIN;
    if (curPos > SB_MAX)
        curPos = SB_MAX;

    calc_circle_pos (curPos);
    calc_box_pos (curPos);
    InvalidateRect (hwnd, &_rect_invalid, TRUE);
}

static int ScrollbarProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    /** IDC of SCROLLBAR control */
    static int _my_scroll_idc = 100;

    /** scrollbar handle of main window  */
    static HWND hwnd_sb_main;

    /** scrollbar handle of control */
    HWND hwnd_sb_ctrl;

    switch (message)
    {
        case MSG_CREATE:
            {
                _rect_invalid.left   = SEP;
                _rect_invalid.top    = 0;
                _rect_invalid.right  = g_rcScr.right;
                _rect_invalid.bottom = g_rcScr.bottom;

                SCROLLINFO scinfo = {0};
                scinfo.fMask = SIF_ALL;
                scinfo.nMin = SB_MIN;
                scinfo.nMax = SB_MAX;
                scinfo.nPage = SB_PAGE;
                scinfo.nPos = SB_MIN;

                calc_circle_pos (scinfo.nPos);
                calc_box_pos (scinfo.nPos);

                /** classic VSCROLL with SBS_NOTNOTIFYPARENT */

                hwnd_sb_main = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE |
                        SBS_VERT | SBS_LEFTALIGN
                        | SBS_NOTNOTIFYPARENT
                       ,
                        0,
                        ++_my_scroll_idc,
                        20, 50, 20, 150, hwnd, 
                        "classic", 0,
                        0);

                SendMessage (hwnd_sb_main, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_main, MSG_SETFOCUS, 0, 0);

                /** flat VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE |
                        SBS_VERT | SBS_LEFTALIGN
                       ,
                        0,
                        ++_my_scroll_idc,
                        43, 50, 20, 150, hwnd, 
                        "flat", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** fashion VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE |
                        SBS_VERT | SBS_LEFTALIGN
                       ,
                        0,
                        ++_my_scroll_idc,
                        66, 50, 20, 150, hwnd, 
                        "fashion", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** tiny VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE |
                        SBS_VERT | SBS_LEFTALIGN
                       ,
                        0,
                        ++_my_scroll_idc,
                        92, 50, 20, 150, hwnd, 
                        "tiny", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** classic NOSHAFT VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_VERT | SBS_NOSHAFT | SBS_LEFTALIGN 
                       ,
                        0,
                        ++_my_scroll_idc,
                        120, 50, 20, 34, hwnd, 
                        "classic", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** flat NOSHAFT VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_VERT | SBS_NOSHAFT | SBS_LEFTALIGN 
                       ,
                        0,
                        ++_my_scroll_idc,
                        140, 50, 20, 34, hwnd, 
                        "flat", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** fashion NOSHAFT VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_VERT | SBS_NOSHAFT | SBS_LEFTALIGN 
                       ,
                        0,
                        ++_my_scroll_idc,
                        160, 50, 20, 34, hwnd, 
                        "fashion", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** tiny NOSHAFT VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_VERT | SBS_NOSHAFT | SBS_LEFTALIGN 
                       ,
                        0,
                        ++_my_scroll_idc,
                        184, 50, 20, 34, hwnd, 
                        "tiny", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** classic NOARROW VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_VERT | SBS_NOARROW | SBS_LEFTALIGN 
                       ,
                        0,
                        ++_my_scroll_idc,
                        210, 50, 20, 150, hwnd, 
                        "classic", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** flat NOARROW VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_VERT | SBS_NOARROW | SBS_LEFTALIGN 
                       ,
                        0,
                        ++_my_scroll_idc,
                        232, 50, 20, 150, hwnd, 
                        "flat", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** fashion NOARROW VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_VERT | SBS_NOARROW | SBS_LEFTALIGN 
                       ,
                        0,
                        ++_my_scroll_idc,
                        254, 50, 20, 150, hwnd, 
                        "fashion", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** tiny NOARROW VSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_VERT | SBS_NOARROW | SBS_LEFTALIGN 
                       ,
                        0,
                        ++_my_scroll_idc,
                        276, 50, 20, 150, hwnd, 
                        "tiny", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** classic HSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_HORZ 
                        | SBS_TOPALIGN
                       ,
                        0,
                        ++_my_scroll_idc,
                        20, 220, 150, 20, hwnd, 
                        "classic", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** flat HSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_HORZ 
                        | SBS_TOPALIGN
                       ,
                        0,
                        ++_my_scroll_idc,
                        20, 240, 150, 20, hwnd, 
                        "flat", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** fashion HSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_HORZ 
                        | SBS_TOPALIGN
                       ,
                        0,
                        ++_my_scroll_idc,
                        20, 260, 150, 20, hwnd, 
                        "fashion", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);

                /** tiny HSCROLL */

                hwnd_sb_ctrl = CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE | SBS_HORZ 
                        | SBS_TOPALIGN
                       ,
                        0,
                        ++_my_scroll_idc,
                        20, 280, 150, 20, hwnd, 
                        "tiny", 0,
                        0);

                SendMessage (hwnd_sb_ctrl, SBM_SETSCROLLINFO, (WPARAM)&scinfo, TRUE);
                SendMessage (hwnd_sb_ctrl, MSG_SETFOCUS, 0, 0);
            }
            break;

        case MSG_COMMAND:
            {
                int code = HIWORD(wParam);
                HWND scroll = (HWND)lParam;
                int pos = 0;

                switch (code) 
                {
                    case SB_LINELEFT:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);

                            SendMessage (scroll, SBM_SETPOS, --pos, TRUE);
                        }

                        break;

                    case SB_LINERIGHT:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);

                            SendMessage (scroll, SBM_SETPOS, ++pos, TRUE);
                        }
                        break;

                    case SB_PAGELEFT:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);

                            pos -= SB_PAGE;
                            SendMessage (scroll, SBM_SETPOS, pos, TRUE);
                        }
                        break;

                    case SB_PAGERIGHT:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);

                            pos += SB_PAGE;
                            SendMessage (scroll, SBM_SETPOS, pos, TRUE);
                        }
                        break;

                    case SB_LINEUP:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);

                            SendMessage (scroll, SBM_SETPOS, --pos, TRUE);
                        }

                        break;

                    case SB_LINEDOWN:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);

                            SendMessage (scroll, SBM_SETPOS, ++pos, TRUE);
                        }
                        break;

                    case SB_PAGEUP:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);

                            pos -= SB_PAGE;
                            SendMessage (scroll, SBM_SETPOS, pos, TRUE);
                        }
                        break;

                    case SB_PAGEDOWN:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);

                            pos += SB_PAGE;
                            SendMessage (scroll, SBM_SETPOS, pos, TRUE);
                        }
                        break;

                    case SB_THUMBPOSITION:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);
                        }
                        break;

                    case SB_THUMBTRACK:
                        {
                            pos = SendMessage (scroll, SBM_GETPOS, 0, 0);
                        }
                        break;

                    case SB_TOP:
                        {
                            pos = SB_MIN;
                        }
                        break;

                    case SB_BOTTOM:
                        {
                            pos = SB_MAX;
                        }
                        break;

                    default:
                        break;
                }

                draw_shape (hwnd, pos);
            }
            break;

        case MSG_HSCROLL:
            {
                int pos = 0;
                switch (wParam)
                {
                    case SB_LINELEFT:
                        {
                            pos = SendMessage (hwnd_sb_main, SBM_GETPOS, 0, 0);

                            SendMessage (hwnd_sb_main, SBM_SETPOS, --pos, TRUE);
                        }

                        break;

                    case SB_LINERIGHT:
                        {
                            pos = SendMessage (hwnd_sb_main, SBM_GETPOS, 0, 0);

                            SendMessage (hwnd_sb_main, SBM_SETPOS, ++pos, TRUE);
                        }
                        break;

                    case SB_PAGELEFT:
                        {
                            pos = SendMessage (hwnd_sb_main, SBM_GETPOS, 0, 0);

                            pos -= SB_PAGE;
                            SendMessage (hwnd_sb_main, SBM_SETPOS, pos, TRUE);
                        }
                        break;

                    case SB_PAGERIGHT:
                        {
                            pos = SendMessage (hwnd_sb_main, SBM_GETPOS, 0, 0);

                            pos += SB_PAGE;
                            SendMessage (hwnd_sb_main, SBM_SETPOS, pos, TRUE);
                        }
                        break;
                    case SB_THUMBPOSITION:
                        {
                            pos = (int)lParam;
                        }
                        break;

                    case SB_THUMBTRACK:
                        {
                            pos = (int)lParam;
                        }
                        break;

                    case SB_TOP:
                        {
                            pos = SB_MIN;
                        }
                        break;

                    case SB_BOTTOM:
                        {
                            pos = SB_MAX;
                        }
                        break;

                    default:
                        break;
                }
                draw_shape (hwnd, pos);
            }
            break;

        case MSG_VSCROLL:
            {
                int pos = 0;
                switch (wParam)
                {
                    case SB_LINEUP:
                        {
                            pos = SendMessage (hwnd_sb_main, SBM_GETPOS, 0, 0);

                            SendMessage (hwnd_sb_main, SBM_SETPOS, --pos, TRUE);
                            
                        }

                        break;

                    case SB_LINEDOWN:
                        {
                            pos = SendMessage (hwnd_sb_main, SBM_GETPOS, 0, 0);

                            SendMessage (hwnd_sb_main, SBM_SETPOS, ++pos, TRUE);
                        }
                        break;

                    case SB_PAGEUP:
                        {
                            pos = SendMessage (hwnd_sb_main, SBM_GETPOS, 0, 0);

                            pos -= SB_PAGE;
                            SendMessage (hwnd_sb_main, SBM_SETPOS, pos, TRUE);
                        }
                        break;

                    case SB_PAGEDOWN:
                        {
                            pos = SendMessage (hwnd_sb_main, SBM_GETPOS, 0, 0);

                            pos += SB_PAGE;
                            SendMessage (hwnd_sb_main, SBM_SETPOS, pos, TRUE);
                        }
                        break;
                    case SB_THUMBPOSITION:
                        {
                            pos = (int)lParam;
                        }
                        break;

                    case SB_THUMBTRACK:
                        {
                            pos = (int)lParam;
                        }
                        break;

                    case SB_TOP:
                        {
                            pos = SB_MIN;
                        }
                        break;

                    case SB_BOTTOM:
                        {
                            pos = SB_MAX;
                        }
                        break;
                    default:
                        break;
                }
                draw_shape (hwnd, pos);
            }
            break;

        case MSG_PAINT:
            {
                HDC hdc = BeginPaint(hwnd); 

                /** separator */
                MoveTo (hdc, SEP, 0);
                LineTo (hdc, SEP, g_rcScr.bottom);

                /** circle */
                Circle (hdc, CC_CENTER_X, CC_CENTER_Y, _radius);

                /** top and bottom line of box */
                MoveTo (hdc, SEP + 20, BOX_Y_MIN);
                LineTo (hdc, SEP + 20 + BOX_WIDTH + 40, BOX_Y_MIN);

                MoveTo (hdc, SEP + 20, BOX_Y_MAX);
                LineTo (hdc, SEP + 20 + BOX_WIDTH + 40, BOX_Y_MAX);

                /** box */
                SetBrushColor (hdc, PIXEL_black);
                FillBox (hdc, SEP + 40, _box_pos_y, BOX_WIDTH, BOX_HEIGHT);

                EndPaint (hwnd, hdc);
            }
            break;

        case MSG_CLOSE:
            {
                DestroyMainWindow (hwnd);
                PostQuitMessage (hwnd);
                return 0;
            }
    }

    return DefaultMainWinProc(hwnd, message, wParam, lParam);
}


int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "scrollbar" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = SCB_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = ScrollbarProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    hMainWnd = CreateMainWindowEx (&CreateInfo, "flat", 0, 0, 0);

    if (hMainWnd == HWND_INVALID)
    {
        return -1;
    }

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

