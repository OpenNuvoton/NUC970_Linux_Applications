/* 
** $Id: look_feel.c 762 2009-10-26 12:00:31Z houhuihua $
**
** Listing 2.1
**
** scrollbar.c: Sample program for MiniGUI Programming Guide
**      Usage of look and feel.
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include <minigui/common.h>
#undef _USE_MINIGUIENTRY
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

static int _control_idc = 100;
#define    IDC_BOX1       200

HWND hwnd_my;
extern int tiny_iconview (HWND hwnd);
extern WINDOW_ELEMENT_RENDERER wnd_rdr_tiny;

/*initialize and terminate interface*/
#if 0
static int once_init_func (PWERENDERER renderer) 
{
    renderer->refcount = 0;

    /*we_fonts*/
    renderer->we_fonts[0] = GetSystemFont (SYSLOGFONT_CAPTION);
    renderer->we_fonts[1] = GetSystemFont (SYSLOGFONT_MENU);
    return 0;
}

static int once_release_func (PWERENDERER renderer)
{
    return 0;
}

static DWORD calc_3dbox_color (DWORD color, int flag)
{
    return PIXEL_black;
}

void (draw_3dbox) (HDC hdc, const RECT* pRect, DWORD color, DWORD flag)
{}
void (draw_radio) (HDC hdc, const RECT* pRect, DWORD color, int status)
{}
void (draw_checkbox) (HDC hdc, const RECT* pRect, DWORD color,
        int status)
{}
void (draw_checkmark) (HDC hdc, const RECT* pRect, DWORD color, 
        int status)
{}
void (draw_arrow) (HWND hWnd, HDC hdc, const RECT* pRect, DWORD color, int status)
{}
void (draw_fold) (HWND hWnd, HDC hdc, const RECT* pRect, DWORD color, 
        int status, int next)
{}
void (draw_focus_frame) (HDC hdc, const RECT *pRect, DWORD color)
{}
void (draw_normal_item) (HWND hWnd, HDC hdc, const RECT* pRect, 
        DWORD color)
{}
void (draw_hilite_item) (HWND hWnd, HDC hdc, const RECT* pRect, 
        DWORD color)
{}
void (draw_disabled_item) (HWND hWnd, HDC hdc, const RECT* pRect, 
        DWORD color)
{}
void (draw_significant_item) (HWND hWnd, HDC hdc, const RECT* pRect, 
        DWORD color)
{}
void (draw_push_button) (HWND hWnd, HDC hdc, const RECT* pRect,
        DWORD color1, DWORD color2, int status)
{}
void (draw_radio_button) (HWND hWnd, HDC hdc, const RECT* pRect, int status)
{}
void (draw_check_button) (HWND hWnd, HDC hdc, const RECT* pRect, int status)
{}
void (draw_border) (HWND hWnd, HDC hdc, BOOL is_active)
{}
void (draw_caption) (HWND hWnd, HDC hdc, BOOL is_active)
{}
void (draw_caption_button) (HWND hwnd, HDC hdc, int ht_code, int state)
{}
void (draw_scrollbar) (HWND hWnd, HDC hdc, int sb_pos)
{}
void (calc_trackbar_rect) (HWND hWnd, LFRDR_TRACKBARINFO *info, DWORD dwStyle,
                        const RECT* rcClient, RECT* rcRuler, 
                        RECT* rcBar, RECT* rcBorder)
{}
void (draw_trackbar) (HWND hWnd, HDC hdc, LFRDR_TRACKBARINFO *info)
{}
void (disabled_text_out) (HWND hWnd, HDC hdc, const char* spText,
                PRECT rc, DWORD dt_fmt)
{}
void (draw_tab) (HWND hWnd, HDC hdc, RECT *rect, char *title, 
            DWORD color, int flag, HICON icon)
{}
void (draw_progress) (HWND hWnd, HDC hdc, 
        int nMax, int nMin, int nPos, BOOL fVertical)
{}
void (draw_header) (HWND hWnd, HDC hdc, const RECT* pRect, DWORD color)
{}
void (erase_background) (HWND hWnd, HDC hdc, const RECT *rect)
{}

static int calc_we_area (HWND hWnd, int which, RECT* we_area)
{
    return 0;
}

static int calc_we_metrics (HWND hWnd, LFRDR_WINSTYLEINFO* style_info, int which)
{
    return 0;
}

static int hit_test (HWND hWnd, int x, int y)
{
    return 0;
}

static void calc_thumb_area (HWND hWnd, BOOL vertical, 
            LFSCROLLBARINFO* sb_info)
{
}

WINDOW_ELEMENT_RENDERER myrender =
{
    "myrender",
    once_init_func,
    once_release_func,
    calc_3dbox_color,
    draw_3dbox,
    draw_radio,
    draw_checkbox,
    draw_checkmark,
    draw_arrow,
    draw_fold,
    draw_focus_frame,

    draw_normal_item,
    draw_hilite_item,
    draw_disabled_item,
    draw_significant_item,

    draw_push_button,
    draw_radio_button,
    draw_check_button,

    draw_border,
    draw_caption,
    draw_caption_button,
    draw_scrollbar,

    calc_trackbar_rect,
    draw_trackbar,

    calc_we_area,
    calc_we_metrics,
    hit_test,
    NULL,
    NULL,

    calc_thumb_area,
    disabled_text_out,
 
    draw_tab,
    draw_progress,
    draw_header,

    NULL,
    NULL,
    NULL, //erase_background,
};

WINDOW_ELEMENT_ATTR myattrs[] =
{
    {-1, 0}
};
#endif

static int LookFeelWinProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_CREATE:
            {
                HWND    ctrl_hwnd, combox_hwnd;
                DIR*    dir;
                struct  dirent* dir_ent;

                CreateWindow (CTRL_STATIC, 
                        "list box demo", 
                        WS_CHILD | SS_NOTIFY | SS_SIMPLE | WS_VISIBLE,
                        ++_control_idc, 
                        10, 55, 200, 15, hwnd, 0);

                ctrl_hwnd = CreateWindow (CTRL_LISTBOX,
                        "files in current dir", 
                        WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_SORT | LBS_MULTIPLESEL | WS_VSCROLL, 
                        ++_control_idc, 
                        10, 75, 200, 100, hwnd, 0);

                combox_hwnd = CreateWindow (CTRL_COMBOBOX, 
                        "combobox",
                        WS_VISIBLE | CBS_SIMPLE | CBS_SORT | WS_TABSTOP,
                        //WS_VISIBLE | CBS_DROPDOWNLIST | CBS_NOTIFY | CBS_READONLY,
                        IDC_BOX1,
                        10, 190, 160, 24,
                        hwnd, 100);

                dir = opendir ("./");
                while ( (dir_ent = readdir ( dir )) != NULL ) {
                    SendMessage (ctrl_hwnd, LB_ADDSTRING, 0, (LPARAM)dir_ent->d_name);
                    SendMessage (combox_hwnd, CB_ADDSTRING, 0, (LPARAM)dir_ent->d_name);
                }
                closedir (dir);
                /*CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE |
                        SBS_VERT | SBS_LEFTALIGN
                        | SBS_NOTNOTIFYPARENT,
                        0,
                        ++_control_idc,
                        20, 50, 30, 150, hwnd, 
                        "tiny", 0,
                        0);

                CreateWindowEx2 (CTRL_SCROLLBAR, "",
                        WS_VISIBLE |
                        SBS_VERT | SBS_LEFTALIGN
                        | SBS_NOTNOTIFYPARENT,
                        0,
                        ++_control_idc,
                        50, 50, 30, 150, hwnd, 
                        "tiny", 0,
                        0);*/
                CreateWindow (CTRL_BUTTON,
                        "icon_view",
                        WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
                        IDOK,
                        175, 218, 60, 30, hwnd, 0);
                CreateWindow (CTRL_STATIC,
                        "simple edit demo",
                        WS_CHILD | WS_VISIBLE | SS_SIMPLE, 
                        ++_control_idc, 
                        10, 5, 200, 15, hwnd, 0);
                CreateWindow (CTRL_EDIT, 
                        "this is a simple edit", 
                        WS_CHILD | WS_VISIBLE | WS_BORDER, 
                        ++_control_idc, 
                        10, 23, 200, 24, hwnd, 0);
                CreateWindow (CTRL_BUTTON,
                        "close",
                        WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
                        IDCANCEL,
                        175, 268, 60, 30, hwnd, 0);
            }
            break;

        case MSG_PAINT:
            {
                /*HDC hdc;
                hdc = BeginPaint (hwnd);
                //TextOut (hdc, 100, 100, "This is a window using the empty LF RDR.");
                TextOut (hdc, 100, 100, "This is a window using the tiny LF RDR.");
                EndPaint (hwnd, hdc);*/
            }
            break;

        case MSG_COMMAND:
            {
                int id = LOWORD(wParam);
                
                switch (id)
                {
                    case IDOK:
                        //ShowWindow (hwnd, SW_HIDE);
                        tiny_iconview (hwnd_my);
                        break;
                    case IDCANCEL:
                        PostMessage(hwnd,MSG_CLOSE,0,0);
                 }
            }
            break;
            //return 0;

        case MSG_CLOSE:
            {
                DestroyAllControls(hwnd);
                DestroyMainWindow (hwnd);
                PostQuitMessage (hwnd);
            }
            return 0;
    }
    return DefaultMainWinProc(hwnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;
    const char* old_renderer;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "look_feel" , 0 , 0);
#endif

    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "look_feel";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = 0;
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = LookFeelWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 240;
    CreateInfo.by = 320;
    //CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;

    /*myrender.calc_we_area = calc_we_area;
    myrender.calc_we_metrics = calc_we_metrics;
    myrender.hit_test = hit_test;
    myrender.calc_thumb_area = calc_thumb_area;*/

    /* Add render */
    //if (!AddWindowElementRenderer ("myrender", &myrender))
    //    return -1;
    if (!AddWindowElementRenderer ("tiny", &wnd_rdr_tiny))
    {
        printf("AddWindowElementRenderer error!\n");
        return -1;
    }
    old_renderer =SetDefaultWindowElementRenderer("tiny");

    //hMainWnd = CreateMainWindowEx (&CreateInfo, "myrender", myattrs, NULL, NULL);
    hwnd_my =hMainWnd = CreateMainWindowEx (&CreateInfo, "tiny", NULL, NULL, NULL);
    
    if (hMainWnd == HWND_INVALID)
    {
        //RemoveWindowElementRenderer ("myrender");    
        SetDefaultWindowElementRenderer(old_renderer);
        RemoveWindowElementRenderer ("tiny");    
        return -1;
    }

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    /* remove render */
    //RemoveWindowElementRenderer ("myrender");    
    SetDefaultWindowElementRenderer(old_renderer);
    RemoveWindowElementRenderer ("tiny");    

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

