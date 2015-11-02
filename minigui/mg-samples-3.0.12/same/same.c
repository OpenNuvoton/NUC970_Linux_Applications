/*
** $Id: same.c 759 2009-09-21 05:22:47Z dongjunjie $
**
** Same: the Same game.
** (C) 1997 the Free Software Foundation
**
** Author: Miguel de Icaza.
**         Federico Mena.
**         Horacio Peña.
**
** The idea is originally from KDE's same game program.
**
** Ported to MiniGUI by Wei Yongming
**
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

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _LANG_ZHCN
#include "same_res_cn.h"
#elif defined _LANG_ZHTW
#include "same_res_tw.h"
#else
#include "same_res_en.h"
#endif

#define IDM_NEW         100
#define IDM_SCORES        110
#define IDM_EXIT        120

#define IDM_PREF        200

#define IDM_ABOUT        300

#define STONE_SIZE        40
#define STONE_COLS        15
#define STONE_LINES        10

#define ID_TIMER        100

#define _(x) (x)

static RECT rcBoard;
static HWND hwnd_score = HWND_INVALID, hwnd_menu;
static BITMAP stones;
static int tagged_count = 0;
static int ball_timeout_id = -1;
static int old_x = -1, old_y = -1;
static int score;

static int cur_sel_scen = 0;

static struct ball {
    int color;
    int tag;
    int frame;
} field [STONE_COLS][STONE_LINES];

static int nstones;
static int ncolors;
static int sync_stones = 0;

#define mapx(x) (x)
#define mapy(y) (STONE_LINES-1-(y))

static void
draw_ball (HDC hdc, int x, int y)
{
    int bx, by;

    if (field [x][y].color){
        by = STONE_SIZE * (field [x][y].color - 1);
        bx = STONE_SIZE * (field [x][y].frame);
        
        FillBoxWithBitmapPart (hdc, x * STONE_SIZE, y * STONE_SIZE, 
                 STONE_SIZE, STONE_SIZE, 0, 0, &stones,
                 bx, by);
    } else {
        SetBrushColor (hdc, PIXEL_black);
        FillBox (hdc, x * STONE_SIZE, y * STONE_SIZE,
                       STONE_SIZE, STONE_SIZE);
    }
}

static void paint (HWND hwnd, HDC hdc)
{
    RECT rc;
    int x1, y1, x2, y2, x, y;
    
    GetClientRect (hdc, &rc);
    x1 = rc.left / STONE_SIZE;
    y1 = rc.top / STONE_SIZE;
    x2 = rc.right / STONE_SIZE;
    y2 = rc.bottom / STONE_SIZE;

    for (x = 0; x < STONE_COLS; x++){
        for (y = 0; y < STONE_LINES; y++){
            draw_ball (hdc, x, y);
        }
    }
}

static void untag_all (HWND hwnd)
{
    int x, y;
    HDC hdc = GetClientDC (hwnd);

    for (x = 0; x < STONE_COLS; x++)
        for (y = 0; y < STONE_LINES; y++){
            field [x][y].tag   = 0;
            if (sync_stones && field [x][y].frame != 0) {
                field [x][y].frame = 0;
                draw_ball (hdc, x, y);
            }
        }

    ReleaseDC (hdc);
}

static int flood_fill (int x, int y, int color)
{
    int c = 0;
    
    if (field [x][y].color != color)
        return c;
    
    if (field [x][y].tag)
        return c;

    c = 1;
    field [x][y].tag = 1;
    
    if (x+1 < STONE_COLS)
        c += flood_fill (x+1, y, color);
    if (x)
        c += flood_fill (x-1, y, color);
    if (y+1 < STONE_LINES)
        c += flood_fill (x, y+1, color);
    if (y)
        c += flood_fill (x, y-1, color);
    return c;
}

static int move_tagged_balls (HDC hdc)
{
    int x, y;
    
    for (x = 0; x < STONE_COLS; x++)
        for (y = 0; y < STONE_LINES; y++){
            if (!field [x][y].tag)
                continue;
            field [x][y].frame = (field [x][y].frame + 1) % nstones;
            draw_ball (hdc, x, y);
        }
    return 1;
}

static void disable_timeout (HWND hwnd)
{
    if (ball_timeout_id != -1){
        KillTimer (hwnd, ball_timeout_id);
        ball_timeout_id = -1;
    }
}

static void mark_balls (HWND hwnd, int x, int y)
{
    if (x == old_x && y == old_y)
        return;

    old_x = x;
    old_y = y;

    untag_all (hwnd);
    disable_timeout (hwnd);
    if (!field [x][y].color)
        return;
    
    tagged_count = flood_fill (x, y, field [x][y].color);
    
    if (tagged_count > 1) {
        SetTimer (hwnd, ID_TIMER, 10); 
        ball_timeout_id = ID_TIMER;
    }
}

static void unmark_balls (HWND hWnd)
{
    old_x = -1;
    old_y = -1;
    disable_timeout (hWnd);
    untag_all (hWnd);
}

static void compress_column (int x)
{
    int y, ym;
    
    for (y = STONE_LINES - 1; y >= 0; y--){
        if (!field [mapx(x)][mapy(y)].tag)
            continue;
        for (ym = y; ym < STONE_LINES - 1; ym++)
            field [mapx(x)][mapy(ym)] = field [mapx(x)][mapy(ym+1)];
        field [mapx(x)][mapy(ym)].color = 0;
        field [mapx(x)][mapy(ym)].tag   = 0;
    }
}

static void compress_y (void)
{
    int x;

    for (x = 0; x < STONE_COLS; x++)
        compress_column (x);
}

static void copy_col (int dest, int src)
{
    int y;
    
    for (y = 0; y < STONE_LINES; y++)
        field [mapx(dest)][mapy(y)] = field [mapx(src)][mapy(y)];
}

static void clean_last_col (void)
{
    int y;

    for (y = 0; y < STONE_LINES; y++){
        field [mapx(STONE_COLS-1)][mapy(y)].color = 0;
        field [mapx(STONE_COLS-1)][mapy(y)].tag   = 0;
    }
}

static void compress_x (void)
{
    int x, xm, l;

    for (x = 0; x < STONE_COLS; x++){
        for (l = STONE_COLS; field [mapx(x)][mapy(0)].color == 0 && l; l--){
            for (xm = x; xm < STONE_COLS-1; xm++)
                copy_col (xm, xm+1);
            clean_last_col ();
        } 
    }
}

static void set_score (int new_score)
{
    char b [40];
    
    score = new_score;
    sprintf (b, "%s %.5d", SM_ST_SETSCORE, score);
    if (hwnd_score != HWND_INVALID)
        SetWindowText (hwnd_score, b);
}


static void end_of_game (HWND hWnd, char *title)
{
    
    char b [256];

    sprintf (b, " %s %.5d", SM_ST_ENDGAME, score);

    MessageBox (hWnd, b, title, 
            MB_OK | MB_ICONINFORMATION | MB_BASEDONPARENT);
}

static void check_game_over (HWND hwnd)
{
    int cleared=1;
    int x,y;
    
    for(x = 0; x < STONE_COLS; x++)
        for(y = 0 ; y < STONE_LINES; y++) {
            if (!field [x][y].color)
                continue;
            cleared = 0;
            if(x+1 < STONE_COLS) 
                if(field[x][y].color == field[x+1][y].color)
                    return;
            if(y+1 < STONE_LINES) 
                if(field[x][y].color == field[x][y+1].color)
                    return;
        }
    if (cleared)
        set_score (score+1000);
    end_of_game(hwnd, _(SM_ST_SAMEGAME));
}

static void kill_balls (HWND hwnd, int x, int y)
{
    if (!field [x][y].color)
        return;
    
    if (tagged_count < 2)
        return;

    set_score (score + (tagged_count - 2) * (tagged_count - 2));
    compress_y ();
    compress_x ();
    InvalidateRect (hwnd, &rcBoard, FALSE);
    check_game_over (hwnd);
}

static void fill_board (void)
{
    int x, y;

    for (x = 0; x < STONE_COLS; x++)
        for (y = 0; y < STONE_LINES; y++){
            field [x][y].color = 1 + (rand () % ncolors);
            field [x][y].tag   = 0;
            field [x][y].frame = sync_stones ? 0 : (rand () % nstones);
        }
}

static void new_game (HWND hwnd)
{
    fill_board ();
    set_score (0);
    InvalidateRect (hwnd, &rcBoard, FALSE);
}

static void configure_sync (const char *fname)
{
    if (strstr (fname, "-sync.gif"))
        sync_stones = 1;
    else
        sync_stones = 0;
}


static void load_scenario (const char *fname)
{
    UnloadBitmap (&stones);
    if (LoadBitmap (HDC_SCREEN, &stones, fname)) {
        printf ("Loading scenario failed: %s.\n", fname);
        exit (1);
    }

    configure_sync (fname);

    nstones = stones.bmWidth / STONE_SIZE;
    ncolors = 3;
}

static void create_same_board (const char *fname)
{
    load_scenario (fname);
    fill_board ();
}

static void sel_scenario (HWND hwnd, int sel)
{
    MENUBUTTONITEM mbi;

    mbi.which = MB_WHICH_TEXT;
    if (sel != cur_sel_scen) {
        SendMessage (hwnd_menu, MBM_GETITEMDATA, sel, (LPARAM)&mbi);
        load_scenario (mbi.text);
        new_game (hwnd);
        cur_sel_scen = sel;
    }
}

static int fill_menu (HWND hwnd_menu)
{
    int i = 0;
    MENUBUTTONITEM mbi;
    struct dirent *e;
    char *dname = "res/";
    DIR *dir;
    char buff [30];
    
    dir = opendir (dname);

    if (!dir)
        return -1;
    
    while ((e = readdir (dir)) != NULL){
        if (!strstr (e->d_name, ".gif")) {
            continue;
        }

        strcpy (buff, "res/");
        strcat (buff, e->d_name);
        mbi.text = buff;
        mbi.bmp = NULL;
        mbi.data = 0;
        SendMessage (hwnd_menu, MBM_ADDITEM, -1, (LPARAM) &mbi);
        i ++;
    }
    closedir (dir);

    return i;
}

static HMENU createpmenugame (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)SM_ST_GAME;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_NEW;
    mii.typedata    = (DWORD)SM_ST_NEWGAME;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
    
#if 0
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_SCORES;
    mii.typedata    = (DWORD)SM_ST_SCORE;
    InsertMenuItem(hmnu, 1, TRUE, &mii);
#endif
    
    mii.type        = MFT_STRING;
    mii.state       = 0;
    mii.id          = IDM_EXIT;
    mii.typedata    = (DWORD)SM_ST_EXIT;
    InsertMenuItem(hmnu, 2, TRUE, &mii);
    
    return hmnu;
}

#if 0
static HMENU createpmenusettings (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)SM_ST_SET;
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_PREF;
    mii.typedata    = (DWORD)SM_ST_PREF;
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    return hmnu;
}

static HMENU createpmenuabout (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)SM_ST_ABOUT;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = IDM_ABOUT;
    mii.typedata    = (DWORD)SM_ST_ABOUTTHIS;
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    return hmnu;
}
#endif

static HMENU createmenu (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)SM_ST_GAME;
    mii.hsubmenu    = createpmenugame ();

    InsertMenuItem(hmnu, 0, TRUE, &mii);

#if 0
    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)SM_ST_SET;
    mii.hsubmenu    = createpmenusettings ();
    InsertMenuItem(hmnu, 1, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.id          = 120;
    mii.typedata    = (DWORD)SM_ST_ABOUT;
    mii.hsubmenu    = createpmenuabout ();
    InsertMenuItem(hmnu, 2, TRUE, &mii);
#endif
                   
    return hmnu;
}

int SameGameProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
    {
        RECT rcClient;
        MENUBUTTONITEM mbi;

        GetClientRect (hWnd, &rcClient);

        CreateWindow (CTRL_STATIC, SM_ST_SCENARIO, 
                    WS_CHILD | WS_VISIBLE | SS_RIGHT, 0,
                    0, rcClient.bottom - GetSysCharHeight () - 4,
                    100, GetSysCharHeight () + 4, hWnd, 0);

        hwnd_menu = CreateWindow ("menubutton",
                     SM_ST_MNB,
                     WS_CHILD | WS_VISIBLE, 100,
                     100, rcClient.bottom - GetSysCharHeight () - 4, 
                     100, GetSysCharHeight () + 4, hWnd, 0);
        if (fill_menu (hwnd_menu) <= 0) {
            fprintf (stderr, "Can not find scenario.\n");
            return -1;
        }

        SendMessage (hwnd_menu, MBM_SETCURITEM, 0, 0);

        hwnd_score = CreateWindow (CTRL_STATIC, SM_ST_SETSCORE, 
                    WS_CHILD | WS_VISIBLE | SS_RIGHT, 0,
                    200, rcClient.bottom - GetSysCharHeight () - 4,
                    rcClient.right - 200, GetSysCharHeight () + 4, hWnd, 0);

        mbi.which = MB_WHICH_TEXT;
        SendMessage (hwnd_menu, MBM_GETITEMDATA, 0, (LPARAM)&mbi);
        create_same_board (mbi.text);
        new_game (hWnd);
    }
    break;
        
    case MSG_COMMAND:
        if (LOWORD(wParam) == 100 && HIWORD (wParam) == MBN_CHANGED) {
            int sel = SendMessage (hwnd_menu, MBM_GETCURITEM, 0, 0);
            if (sel >= 0)
                sel_scenario (hWnd, sel);
            break;
        }

        switch (wParam) {
        case IDM_NEW:
            new_game (hWnd);
        break;

        case IDM_SCORES:
        break;
            
        case IDM_EXIT:
            SendMessage (hWnd, MSG_CLOSE, 0, 0L);
        break;

        case IDM_PREF:
        break;
            
        case IDM_ABOUT:
        break;
    }
    break;


    case MSG_PAINT:
    {
        HDC hdc;
        hdc = BeginPaint (hWnd);
        paint (hWnd, hdc);
        EndPaint (hWnd, hdc);
        return 0;
    }

    case MSG_LBUTTONDOWN:
    {
        int x = LOWORD (lParam);
        int y = HIWORD (lParam);

        kill_balls (hWnd, x / STONE_SIZE, y / STONE_SIZE);
        old_x = -1;
        old_y = -1;
    }
    break;

    case MSG_MOUSEMOVE:
    {
        int x = LOWORD (lParam);
        int y = HIWORD (lParam);
        
        if (PtInRect (&rcBoard, x, y))
            mark_balls (hWnd, x / STONE_SIZE, y / STONE_SIZE);
        else
            unmark_balls (hWnd);
    }
    break;

    case MSG_NCMOUSEMOVE:
        if (wParam != HT_CLIENT)
            unmark_balls (hWnd);
    break;

    case MSG_MOUSEMOVEIN:
        if (!wParam)
            unmark_balls (hWnd);
        break;

    case MSG_TIMER:
        if (wParam == ID_TIMER) {
            HDC hdc = GetClientDC (hWnd);
            move_tagged_balls (hdc);
            ReleaseDC (hdc);
        }
        break;

    case MSG_CLOSE:
        if (MessageBox (hWnd, 
                    SM_ST_QUIT, 
                    SM_ST_SAME, 
                    MB_YESNO | MB_ICONQUESTION |
                    MB_BASEDONPARENT) != IDYES)
            return 0;

        DestroyAllControls (hWnd);
           DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

static void InitCreateInfo (PMAINWINCREATE pCreateInfo)
{
    rcBoard.left = 0;
    rcBoard.top = 0;
    rcBoard.right = STONE_COLS * STONE_SIZE;
    rcBoard.bottom = STONE_LINES * STONE_SIZE;
    
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = SM_ST_SAME;
    pCreateInfo->hMenu = createmenu ();
    pCreateInfo->hCursor = GetSystemCursor (IDC_ARROW);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = SameGameProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = pCreateInfo->lx + 
            ClientWidthToWindowWidth (WS_CAPTION | WS_BORDER, 
                            STONE_COLS * STONE_SIZE);
    pCreateInfo->by = pCreateInfo->ty + 
            ClientHeightToWindowHeight (WS_CAPTION | WS_BORDER, 
                            STONE_LINES * STONE_SIZE, TRUE);
    pCreateInfo->by += GetSysCharHeight () + 4;
    pCreateInfo->iBkColor = PIXEL_black;
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

int MiniGUIMain (int argc, const char *argv [])
{
    HWND hMainWnd;
    MSG Msg;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    int i;
    const char* layer = NULL;

    for (i = 1; i < argc; i++) {
        if (strcmp (argv[i], "-layer") == 0) {
            layer = argv[i + 1];
            break;
        }
    }
    GetLayerInfo (layer, NULL, NULL, NULL);
    //if (JoinLayer ("same","same"/* argv[0]*/, 0, 0) == INV_LAYER_HANDLE) {
    if (JoinLayer (layer, argv[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif

    InitCreateInfo (&CreateInfo);
    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    srand (time (NULL));

    ShowWindow (hMainWnd, SW_SHOWNORMAL);

    while( GetMessage (&Msg, hMainWnd) ) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
