/* 
** $Id: scrollbar.c 759 2009-09-21 05:22:47Z dongjunjie $
**
** Listing 8.1
**
** scrollbar.c: Sample program for MiniGUI Programming Guide
**      Demo for scrollbar
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

#ifdef _LANG_ZHCN
#include "scrollbar_res_cn.h"
#elif defined _LANG_ZHTW
#include "scrollbar_res_tw.h"
#else
#include "scrollbar_res_en.h"
#endif

static int step = 12;

static int ScrollWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static int iStart = 0;
    static int iStartPos = 0; 
    
    switch (message) {
        case MSG_CREATE:
            SetScrollRange (hWnd, SB_VERT, 0, 20*step);
            EnableScrollBar (hWnd, SB_HORZ, FALSE);
            if (!CreateCaret (hWnd, NULL, 8, 14))
                fprintf (stderr, "Create caret error!\n");
        break;
        
        case MSG_SHOWWINDOW:
/*		printf("------------\n");
            SetScrollRange (hWnd, SB_VERT, 0, 20*step);
            EnableScrollBar (hWnd, SB_HORZ, FALSE);
			ShowScrollBar(hWnd, SB_VERT, TRUE);
			ShowScrollBar(hWnd, SB_HORZ, FALSE);
  */          ShowCaret (hWnd);
        break;
		

        case MSG_SETFOCUS:
            ActiveCaret (hWnd);
            ShowCaret (hWnd);
        break;

        case MSG_KILLFOCUS:
            HideCaret (hWnd);
        break;
        
        case MSG_LBUTTONDOWN:
            SetCaretPos (hWnd, LOWORD (lParam), HIWORD (lParam));
        break;
        
        case MSG_LBUTTONDBLCLK:
            ShowScrollBar (hWnd, SB_HORZ, FALSE);
        break;

        case MSG_RBUTTONDBLCLK:
            ShowScrollBar (hWnd, SB_HORZ, TRUE);
        break;
       
        case MSG_HSCROLL:
            if (wParam == SB_LINERIGHT) {
                
                if (iStartPos < 5) {
                    iStartPos ++;
                    ScrollWindow (hWnd, -GetSysCharWidth (), 0, NULL, NULL);
                }
            }
            else if (wParam == SB_LINELEFT) {
                if (iStartPos > 0) {
                    iStartPos --;

                    ScrollWindow (hWnd, GetSysCharWidth (), 0, NULL, NULL);
                }
            }
            SetScrollPos (hWnd, SB_HORZ, iStartPos*20);
        break;
        
        case MSG_VSCROLL:
        {
            SCROLLINFO vscroll;
            if (wParam == SB_LINEDOWN) {
                vscroll.fMask = SIF_ALL;
                GetScrollInfo(hWnd, SB_VERT, &vscroll);
                if (vscroll.nPos + vscroll.nPage >= vscroll.nMax)
                    break;
                if (iStart < step) {
                    iStart ++;
                    ScrollWindow (hWnd, 0, -20, NULL, NULL);
                }
            }
            else if (wParam == SB_LINEUP) {
                if (iStart > 0) {
                    iStart --;

                    ScrollWindow (hWnd, 0, 20, NULL, NULL);
                }
            }
            SetScrollPos (hWnd, SB_VERT, iStart*20);
        }
        break;
        case MSG_PAINT:
        {
            HDC hdc;
            int i;
            RECT rcClient;
            
            GetClientRect (hWnd, &rcClient);
            
            hdc = BeginPaint (hWnd);
            for (i = 0; i < 17 - iStart; i++) {
                rcClient.left = 0;
                rcClient.right = (strlen (strLine [i + iStart]) - iStartPos)
                                 * GetSysCharWidth ();
                rcClient.top = i*20;
                rcClient.bottom = rcClient.top + 20;

                TextOut (hdc, 0, i*20, strLine [i + iStart] + iStartPos);
            }

            EndPaint (hWnd, hdc);
        }
        return 0;

        case MSG_CLOSE:
            DestroyCaret (hWnd);
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_BORDER | WS_CAPTION | WS_HSCROLL | WS_VSCROLL;
    pCreateInfo->dwExStyle = WS_EX_NONE | WS_EX_IMECOMPOSE;
    pCreateInfo->spCaption = CAPTION;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = ScrollWinProc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 400;
    pCreateInfo->by = 280;
    pCreateInfo->iBkColor = COLOR_lightwhite; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain(int args, const char* arg[])
{
    MSG Msg;
    MAINWINCREATE CreateInfo;
    HWND hMainWnd;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "scrollbar" , 0 , 0);
#endif

    InitCreateInfo(&CreateInfo);

    hMainWnd = CreateMainWindow(&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return 1;

    ShowWindow(hMainWnd, SW_SHOW);

    while (GetMessage(&Msg, hMainWnd) ) {
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup(hMainWnd);
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

