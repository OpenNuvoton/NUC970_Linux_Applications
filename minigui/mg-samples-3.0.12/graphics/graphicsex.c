#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _LANG_ZHCN
#include "graphicsex_res_cn.h"
#elif defined _LANG_ZHTW
#include "graphicsex_res_tw.h"
#else
#include "graphicsex_res_en.h"
#endif

#define DEFAULT_X  320
#define DEFAULT_Y  200
#define DEFAULT_WIDTH  640
#define DEFAULT_HEIGHT 400

static void TellSpeed (HWND hwnd, unsigned int start_tick, unsigned int end_tick, const char* drawing, unsigned int count)
{
        char buff [1024];
        unsigned int seconds = (end_tick - start_tick)/100;

        sprintf (buff, GPEX_ST_TELLSPEED, 
                        count, drawing, seconds, (end_tick - start_tick) / (count * 100.0));

        MessageBox (hwnd, buff, drawing, MB_OK | MB_ICONINFORMATION);
}

static void Information(HWND hwnd)
{
    MessageBox (hwnd, GPEX_ST_DONE, GPEX_ST_GDI, MB_OK | MB_ICONINFORMATION);
}
static void GDIDemo_SplineTo(HWND hwnd,HDC hdc)
{
    static POINT pts [] ={
        {100,80},
        {200,120},
        {200,280},
        {100,320},
    };
    static POINT pts2 [] ={
        {500,50},
        {400,100},
        {400,300},
        {500,350},

    };

    /* Bezier spline */
    SetPenColor (hdc,PIXEL_red);
    SplineTo(hdc,pts);
    SetPenColor (hdc,PIXEL_black);
    SplineTo(hdc,pts2);

    Information(hwnd);
}

static void GDIDemo_PolyLineTo(HWND hwnd,HDC hdc)
{
    static POINT pts [] = {
        {10,10},
        {100,100},
        {300,70},
        {200,300},
        {600,280},
        {10,10},
    };

    SetBrushColor(hdc,PIXEL_red);
    PolyLineTo(hdc,pts,6);
    FloodFill(hdc,22,20);
    SetBrushColor(hdc,PIXEL_blue);
    FloodFill(hdc,220,270);
    SetBrushColor(hdc,PIXEL_black);
    FloodFill(hdc,299,71);

    Information(hwnd);
}


static void GDIDemo_Rectangle(HWND hwnd,HDC hdc)
{
    int x = DEFAULT_X, y = DEFAULT_Y;
    int tox = DEFAULT_WIDTH, toy = DEFAULT_HEIGHT;
    int count;
    unsigned int start_tick, end_tick;
    unsigned int nr_colors = GetGDCapability (hdc, GDCAP_COLORNUM);

    /* Rectangle */
    start_tick = GetTickCount();
    count = 10000;
    SetPenColor (hdc,PIXEL_red);
    while (count--){
        SetPenColor (hdc,rand() %nr_colors);
        Rectangle(hdc,x,y,tox,toy);
        tox = rand()%DEFAULT_WIDTH;
        toy = rand()%DEFAULT_HEIGHT;
    }
    end_tick =GetTickCount();
    TellSpeed (hwnd,start_tick,end_tick,GPEX_ST_RECT,10000);
}

static void GDIDemo_FocusRect(HWND hwnd,HDC hdc)
{
    int x = DEFAULT_X, y = DEFAULT_Y;
    int tox = DEFAULT_WIDTH, toy = DEFAULT_HEIGHT;
    int count;
    unsigned int start_tick, end_tick;
    unsigned int nr_colors = GetGDCapability (hdc, GDCAP_COLORNUM);

    /* Focus Rectangle  */
    start_tick = GetTickCount();
    count = 1000;
    SetPenColor (hdc,PIXEL_red);
    while (count--){
        SetPenColor (hdc,rand() %nr_colors);
        FocusRect(hdc,x,y,tox,toy);
        x   = rand()% DEFAULT_X;
        y   = rand()% DEFAULT_Y;
        tox = rand()% DEFAULT_WIDTH;
        toy = rand()% DEFAULT_HEIGHT;
    }
    end_tick =GetTickCount();
    TellSpeed (hwnd,start_tick,end_tick,GPEX_ST_FOCUSRECT,1000);

}

static void GDIDemo_DrawHDotLine(HWND hwnd, HDC hdc)
{
    int x = DEFAULT_X, y = DEFAULT_Y;
    int tox = DEFAULT_WIDTH, toy = DEFAULT_HEIGHT;
    int count;
    unsigned int start_tick, end_tick;
    unsigned int nr_colors = GetGDCapability (hdc, GDCAP_COLORNUM);

    /* Horizontal dot dash line */
    start_tick = GetTickCount ();
    count = 100000;
    SetPenColor (hdc, PIXEL_yellow);
    while (count--) {
        SetPenColor (hdc, rand() % nr_colors);
        DrawHDotLine(hdc,tox,toy,300);
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_HEIGHT;
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GPEX_ST_HLINE, 100000);

    /* Vertial dot dash line */
    start_tick = GetTickCount ();
    count = 100000;
    SetPenColor (hdc, PIXEL_yellow);
    while (count--) {
        SetPenColor (hdc, rand() % nr_colors);
        MoveTo (hdc, x, y);
        DrawVDotLine(hdc,tox,toy,300);
        tox = rand() % DEFAULT_WIDTH;
        toy = rand() % DEFAULT_HEIGHT;
    }
    end_tick = GetTickCount ();
    TellSpeed (hwnd, start_tick, end_tick, GPEX_ST_VLINE, 100000);
}

static void GDIDemo_FillPolygon(HWND hwnd,HDC hdc)
{
    static POINT pts [] ={
        {320,20},
        {100,200},
        {320,350},
        {480,350},
        {480,150},
    };
    SetBrushColor(hdc,PIXEL_red);

    FillPolygon(hdc,pts,5);

    Information(hwnd);
}

static void draw_circle(void* context,int x1,int x2,int y)
{
    HDC hdc = (HDC) context;
    SetBrushColor(hdc,rand() % PIXEL_yellow);
    FillCircle(hdc,x1,y,1);
    FillCircle(hdc,x2,y,3);

}

static void GDIDemo_CircleGenerator(HWND hwnd,HDC hdc)
{

    int x = 400, y = 250,r=150;
    int count;
    count = 50;
    while (count--) {
        CircleGenerator((void*) hdc,x,y,r,draw_circle);
        x = rand() % 400;
        y = rand() % 250;
        r = rand() % 150;
    }

    Information(hwnd);

}

static void draw_ellipse(void* context,int x1,int x2,int y)
{
    HDC hdc = (HDC) context;
    FillCircle(hdc,x1,y,1);
    FillCircle(hdc,x2,y,3);
}

static void GDIDemo_EllipseGenerator(HWND hwnd,HDC hdc)
{

    int x = 100, y = 100,rx=100,ry=160;
    int count;
    count = 100;
    while (count--) {
        SetBrushColor(hdc,rand() % PIXEL_red);
        EllipseGenerator((void*) hdc,x,y,rx,ry,draw_ellipse);
        x  = x+10;
        y  = y+5;
        rx = rand() % 100;
        ry = rand() % 160;
    }

    Information(hwnd);
}

static void draw_circlearc(void* context,int x,int y)
{
    HDC hdc = (HDC) context;
    SetBrushColor(hdc,rand() % PIXEL_red);
    FillCircle(hdc,x,y,1);
}

static void GDIDemo_CircleArcGenerator(HWND hwnd,HDC hdc)
{
    int x=0,y=0,r=100,ang1=3200,ang2=17180;
    int count;
    count =100;
    while (count--){
        SetBrushColor(hdc,rand()% PIXEL_red);
        CircleArcGenerator((void*) hdc,x,y,r,ang1,ang2,draw_circlearc);
        x = x+20;
        y = y+10;
    }
    Information(hwnd);
}

static void draw_polygon(void* context,int x1,int x2,int y)
{
    HDC hdc = (HDC) context;
    SetBrushColor(hdc,rand() % PIXEL_yellow);
    FillCircle(hdc,x1,y,5);
    FillCircle(hdc,x2,y,5);
}

static void GDIDemo_PolygonGenerator(HWND hwnd,HDC hdc)
{
    static POINT pts [] ={
        {320,20},
        {100,200},
        {320,340},
        {480,360},
        {480,150},
    };
    SetPenColor(hdc,PIXEL_yellow);
    PolygonGenerator((void*) hdc,pts,5,draw_polygon);
    Information(hwnd);
}

static void GDIDemo_MonotoneVerticalPolygonGenerator(HWND hwnd,HDC hdc)
{
    static POINT pts [] ={
        {320,100},
        {100,200},
        {320,340},
        {480,360},
        {480,150},
    };
    MonotoneVerticalPolygonGenerator((void*) hdc,pts,5,draw_polygon);
    Information(hwnd);
}

static void draw_line(void* context,int stepx,int stepy)
{
    HDC hdc = (HDC) context;
    static int x = 0,y = 0;
    SetPenWidth(hdc,5);
    SetPenColor(hdc,rand() % PIXEL_red);
    LineEx(hdc,x,y,stepx,stepy);
    x += stepx;
    y += stepy;
}

static void GDIDemo_LineGenerator(HWND hwnd,HDC hdc)
{
    SetPenColor(hdc,PIXEL_red);
    LineGenerator((void*) hdc,0,0,400,400,draw_line);
    Information(hwnd);
} 

static void  GDIDemo_PolyArcEx(HWND hwnd,HDC hdc)
{
    ARC arc;
    arc.x = 150;
    arc.y = 150;
    arc.width = 100;
    arc.height = 100;
    arc.angle1 = 45*64;
    arc.angle2 = 270*64;
    PolyArcEx(hdc,&arc,1);
    Information(hwnd);
}

static void GDIDemo_PolyFillArcEx(HWND hwnd,HDC hdc)
{
    ARC arc;
    arc.x = 300;
    arc.y = 0;
    arc.width  = 200;
    arc.height = 300;
    arc.angle1 = 45*64;
    arc.angle2 = 135*64;
    SetBrushColor(hdc,rand() % PIXEL_red);
    PolyFillArcEx(hdc,&arc,1);
    Information(hwnd);
}

static int TestWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message) {
        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
            SetTextColor (hdc, PIXEL_black);
            SetBkColor (hdc, PIXEL_lightwhite);
            SetBkMode (hdc, BM_TRANSPARENT);
            TabbedTextOut (hdc, 50, 20, GPEX_ST_PROMPT);
            EndPaint (hWnd, hdc);
            return 0;

        case MSG_CHAR:
            hdc =  GetClientDC(hWnd);
            switch (wParam) {
            case 'a':
            case 'A':
                GDIDemo_SplineTo(hWnd,hdc);
                GDIDemo_PolyLineTo(hWnd,hdc);
                GDIDemo_Rectangle(hWnd,hdc);
                break;

            case 'b':
            case 'B':
                GDIDemo_DrawHDotLine(hWnd,hdc);
                break;

            case 'c':
            case 'C':
                GDIDemo_FocusRect(hWnd,hdc);
                InvalidateRect (hWnd, NULL, TRUE);
                GDIDemo_FillPolygon(hWnd,hdc);
                break;

            case 'd':
            case 'D':
                GDIDemo_CircleGenerator(hWnd,hdc);
                InvalidateRect (hWnd, NULL, TRUE);
                GDIDemo_EllipseGenerator(hWnd,hdc);
                InvalidateRect (hWnd, NULL, TRUE);
                GDIDemo_CircleArcGenerator(hWnd,hdc);  
                break;

            case 'e':
            case 'E':
                GDIDemo_PolygonGenerator(hWnd,hdc);
                GDIDemo_MonotoneVerticalPolygonGenerator(hWnd,hdc);
                break;

            case 'f':
            case 'F':
                GDIDemo_LineGenerator(hWnd,hdc);
                GDIDemo_PolyArcEx(hWnd,hdc);
                GDIDemo_PolyFillArcEx(hWnd,hdc);
                break;

            case 'q':
            case 'Q':
                PostMessage(hWnd,MSG_CLOSE,0,0);
          }
            ReleaseDC(hdc);
            InvalidateRect (hWnd, NULL, TRUE);
            break;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }
    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}


static int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
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

    if (JoinLayer (layer, argv[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = GPEX_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = TestWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = DEFAULT_WIDTH;
    CreateInfo.by = DEFAULT_HEIGHT;
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

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}
