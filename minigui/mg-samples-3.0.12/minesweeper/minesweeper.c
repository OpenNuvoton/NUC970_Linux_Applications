/*
** $Id: minesweeper.c 609 2008-08-20 09:51:23Z weiym $
**
** bomb.c: The MineSweeper game.
**
** Copyright (C) 1999 ~ 2002 Zheng Xiang and others.
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
#include <time.h>
#include <unistd.h>
#include <pwd.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _LANG_ZHCN
#include "minesweeper_res_cn.h"
#elif defined _LANG_ZHTW
#include "minesweeper_res_tw.h"
#else
#include "minesweeper_res_en.h"
#endif

typedef struct 
{	
       	int flag;
      	int value;
       	BOOL test;
       	BOOL hit;
        BOOL bombout;
        BOOL error;
}BOM;

typedef struct  
{
	int x;
 	int y;
	BOOL NY;
}NO;

typedef struct
{
    int highscore;
    char name[20];
}SCORE;

static BOM bom[30][16];
static NO NoAdr[540];
static SCORE score[3];
static int itime, leftbombnum;
static int bombnum = 99;
void SearchGround(HWND hWnd,int x,int y);

int Open(HWND hWnd,int x,int y);

int TestMyWinProc(HWND hWnd,int message,WPARAM wParam,LPARAM lParam);

void InitMyWinCreateInfo(PMAINWINCREATE pCreateInfo);
void InitAbHostedCreateInfo(HWND hHosting, PMAINWINCREATE  pCreateInfo); 
void InitHighScoreCreateInfo (HWND hHosting, PMAINWINCREATE pCreateInfo);
void InitCongratulationCreateInfo (HWND hHosting, PMAINWINCREATE pCreateInfo);

void* TestMyWindow(void* data);

void BombGame(HWND hWnd, int x, int y);

void Finished(HWND hWnd);

void Cancel3DFrame(HDC hdc, int l,int t,int r,int b);

void TextValue(HDC hdc, int x,int y,int value);

void BombOut(HWND hWnd);

void BothButtonDownProc(HWND hWnd, int x, int y);

void DrawDigit(HDC hdc, char* buffer, int CLOCK);

#define  ID_ABOUT  300
#define  ID_NEW    301  
#define  ID_LARGE  302
#define  ID_MIDDLE 303
#define  ID_SMALL  304
#define  ID_CLOSE  305
#define  ID_HIGHSCORE 306
#define  ID_CLOCK  400

#define  WIDTH_LARGEWIN   544
#define  HEIGHT_LARGEWIN  470
#define  WIDTH_MIDDLEWIN  292
#define  HEIGHT_MIDDLEWIN 430
#define  WIDTH_SMALLWIN   178
#define  HEIGHT_SMALLWIN  270

#define  WIDTH_FACE      30
#define  HEIGHT_FACE     30

#define  WIDTH_DIGIT     14
#define  WIDTH_BOMNUM    (2*WIDTH_DIGIT)
#define  HEIGHT_BOMNUM   30

#define  WIDTH_CLOCK     (3*WIDTH_DIGIT)
#define  HEIGHT_CLOCK    30

#define  WIDTH_BOX       18
#define  HEIGHT_BOX      18

static int sg_boxnumx = 30;
static int sg_boxnumy = 16;
static BITMAP sg_bmpDigit[10];

static int oldx, oldy, adrx, adry;
static int flag_bombout, flag_finished, flag_size = 2;
static int second = 0;
static BITMAP bitmap1, bmpflag;
static BOOL fValid1 = FALSE;
static BOOL fValidflag = FALSE;
static BOOL bTimer;
static HWND hHighscore,hCongratuate; 

static int winwidth = WIDTH_LARGEWIN;
static int offsetx;
static int x_face, x_bomnum, x_clock;

void DrawMyBitmap(HDC hdc, PBITMAP bmp, int x, int y, int w, int h)
{
    if (bmp)
        FillBoxWithBitmap(hdc, x, y, w, h, bmp);
    else
        TextOut (hdc, x, y, "a");
}

void Cancel3DFrame(HDC hdc, int  l, int  t, int w, int h)
{
    SetPenColor (hdc,COLOR_darkgray);
    Rectangle(hdc, l, t, l + w, t + h);
    SetBrushColor(hdc,COLOR_lightgray);
    FillBox(hdc, l+1, t+1, w-1, h-1); 
} 

void TextValue(HDC hdc, int x, int y, int value)
 {
    int color = 0;
    char   va[20]; 
    switch(value)
    {
       case 1:  color=COLOR_blue;       break;
       case 2:  color=COLOR_green;      break;   
       case 3:  color=COLOR_red;        break;
       case 4:  color=COLOR_magenta;    break;
       case 5:  color=COLOR_yellow;     break;
       case 6:  color=COLOR_cyan;       break;
       case 7:  color=COLOR_darkred;    break;
       case 8:  color=COLOR_darkgreen;  break; 
    }
    SetBkColor(hdc,COLOR_lightgray);
    SetTextColor(hdc,color);
    sprintf(va,"%d",value);
    
    TextOut(hdc, x + ((WIDTH_BOX - GetSysCharWidth ()) >> 1), 
        y + ((HEIGHT_BOX - GetSysCharHeight ()) >> 1), va);
}  

void SearchGround(HDC hdc,int x,int y)
{
     int x1=0,y1=0;
     int i=1;
     
     bom[x][y].test=TRUE;
     NoAdr[itime].x=x;
     NoAdr[itime].y=y;
     NoAdr[itime].NY=FALSE;
     itime++;
     Cancel3DFrame(hdc,x*WIDTH_BOX+offsetx, y*HEIGHT_BOX+HEIGHT_FACE,
                        WIDTH_BOX, HEIGHT_BOX);
     while( i <= 8 )
     {
         switch(i) 
         {
             case  1:  x1=x-1;  y1=y;    break;
             case  2:  x1=x-1;  y1=y-1;  break;
             case  3:  x1=x-1;  y1=y+1;  break;
             case  4:  x1=x+1;  y1=y;    break;
             case  5:  x1=x+1;  y1=y-1;  break;
             case  6:  x1=x+1;  y1=y+1;  break;
             case  7:  y1=y-1;  x1=x;    break;
             case  8:  y1=y+1;  x1=x;    break;   
         }
         if( x1>=0 && y1>=0 && x1<sg_boxnumx && y1<sg_boxnumy
                 &&!bom[x1][y1].hit&& !bom[x1][y1].test && !bom[x1][y1].value )
               SearchGround(hdc,x1,y1);
                     
         if( x1>=0 && y1>=0 && x1<sg_boxnumx && y1<sg_boxnumy
                            &&!bom[x1][y1].hit
                            &&!bom[x1][y1].test && bom[x1][y1].value!=0 )
         {
             bom[x1][y1].test=TRUE;
             NoAdr[itime].x=x1;
             NoAdr[itime].y=y1;
             NoAdr[itime].NY=TRUE;
             itime++;
             Cancel3DFrame(hdc, x1*WIDTH_BOX+offsetx, y1*HEIGHT_BOX+HEIGHT_FACE,
                                 WIDTH_BOX, HEIGHT_BOX);
             TextValue(hdc, x1*WIDTH_BOX+offsetx, y1*HEIGHT_BOX+HEIGHT_FACE, 
                                 bom[x1][y1].value);
         } 
         i++;
     }
 }
 
BOOL Open(HWND hWnd,int x,int y)
{ 
    int x1=0,y1=0;
    int i=1;
    HDC hdc;

    hdc = GetClientDC(hWnd);
    while( i <= 8 )
    {
        switch( i ) {
            case  1:  x1=x-1;  y1=y;    break;
            case  2:  x1=x-1;  y1=y-1;  break;
            case  3:  x1=x-1;  y1=y+1;  break;
            case  4:  x1=x+1;  y1=y;    break;
            case  5:  x1=x+1;  y1=y-1;  break;
            case  6:  x1=x+1;  y1=y+1;  break;
            case  7:  y1=y-1;  x1=x;    break;
            case  8:  y1=y+1;  x1=x;    break;   
        }

        if( x1>=0 && y1>=0 && x1<sg_boxnumx && y1<sg_boxnumy
            && !bom[x1][y1].hit && bom[x1][y1].flag) {    
            ReleaseDC(hdc);
            return FALSE;
        }
                      
        if( x1>=0 && y1>=0 && x1<sg_boxnumx && y1<sg_boxnumy
                && !bom[x1][y1].test && !bom[x1][y1].value
                && !bom[x1][y1].flag ){
            SearchGround(hdc,x1,y1);
        }   
                     
        if( x1>=0 && y1>=0 && x1<sg_boxnumx && y1<sg_boxnumy
                  && !bom[x1][y1].test && bom[x1][y1].value!=0 )
        {
            bom[x1][y1].test=TRUE;
            NoAdr[itime].x=x1;
            NoAdr[itime].y=y1;
            NoAdr[itime].NY=TRUE;
            itime++;
             Cancel3DFrame(hdc, x1*WIDTH_BOX+offsetx, y1*HEIGHT_BOX+HEIGHT_FACE,
                                 WIDTH_BOX, HEIGHT_BOX);
             TextValue(hdc, x1*WIDTH_BOX+offsetx, y1*HEIGHT_BOX+HEIGHT_FACE, 
                                 bom[x1][y1].value);
        }
        i++; 
    }
    
    ReleaseDC(hdc);
    return  TRUE; 
}

static BITMAP bmpbom, bmpface, bmphitfalse;
static BOOL fValidbom = FALSE,fValidface = FALSE, fValidhitfalse = FALSE;
void BombOut(HWND hWnd)
{ 
  int i,j;
  HDC hdc;
  
  hdc=GetClientDC(hWnd);
  
  for (i = 0; i < sg_boxnumx; i++) {
      for (j = 0; j < sg_boxnumy; j++) {
         if (bom[i][j].flag && !bom[i][j].hit) {
            Cancel3DFrame(hdc,i*WIDTH_BOX+offsetx,j*HEIGHT_BOX+HEIGHT_FACE,
                    WIDTH_BOX,HEIGHT_BOX);
                    
            DrawMyBitmap(hdc, fValidbom?&bmpbom:NULL,
                        i*WIDTH_BOX+offsetx+1,
                        j*HEIGHT_BOX+1+HEIGHT_FACE, 0, 0);
            bom[i][j].bombout = TRUE;            
          }
         if (!bom[i][j].flag && bom[i][j].hit){
            Cancel3DFrame(hdc,i*WIDTH_BOX+offsetx,j*HEIGHT_BOX+HEIGHT_FACE,
                    WIDTH_BOX,HEIGHT_BOX);
                    
            DrawMyBitmap(hdc, fValidhitfalse?&bmphitfalse:NULL,
                        i*WIDTH_BOX+offsetx+1,
                        j*HEIGHT_BOX+1+HEIGHT_FACE, 0, 0);
            bom[i][j].error = TRUE;            
                        
          }   
       }
   }

   DrawMyBitmap (hdc, fValidface?&bmpface:NULL, x_face, 0, 0, 0);
   flag_bombout = 1;
   bTimer = FALSE;
   ReleaseDC(hdc);
}

static BITMAP bmpfinalface;
static BOOL fValidfinalface = FALSE;

void Finished(HWND hWnd)
{ 
    int i,j;
    HDC hdc;
    RECT bombnumber;
    MAINWINCREATE CreateInfo;
  
    hdc = GetClientDC(hWnd);
  
    for (i = 0; i < sg_boxnumx; i++) {
        for (j = 0; j < sg_boxnumy; j++) {
            if (bom[i][j].flag && !bom[i][j].hit) {
                    
                DrawMyBitmap(hdc, fValidflag?&bmpflag:NULL,
                                 i*WIDTH_BOX+offsetx+3,
                                 j*HEIGHT_BOX+3+HEIGHT_FACE,
                                 WIDTH_BOX - 5,
                                 HEIGHT_BOX - 5);
                bom[i][j].hit = TRUE;                     
            }
        }
    }

    DrawMyBitmap (hdc, fValidfinalface?&bmpfinalface:NULL, x_face+1, 1, 0, 0);
    flag_finished = 1;
    bTimer = FALSE;
    ReleaseDC(hdc);
   
    leftbombnum = 0;
                     
    SetRect (&bombnumber, x_bomnum, 0,
                         x_bomnum + WIDTH_BOMNUM, HEIGHT_BOMNUM);
    InvalidateRect (hWnd, &bombnumber, FALSE);
   
    if (second < score[flag_size].highscore){
        InitCongratulationCreateInfo(hWnd, &CreateInfo);
        hCongratuate = CreateMainWindow (&CreateInfo);
    }    
}    

HMENU createpmenuabout(void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)MS_ST_ABOUT;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_ABOUT;
    mii.typedata    = (DWORD)MS_ST_ABOUTTHIS;
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    return hmnu;
}

HMENU createpmenustart(void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)MS_ST_START;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_NEW;
    mii.typedata    = (DWORD)MS_ST_NEW;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_HIGHSCORE;
    mii.typedata    = (DWORD)MS_ST_HISCORE;
    InsertMenuItem(hmnu, 1, TRUE, &mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_CLOSE;
    mii.typedata    = (DWORD)MS_ST_EXIT;
    InsertMenuItem(hmnu, 2, TRUE, &mii);
    
    return hmnu;
}

HMENU createpmenulevel(void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)MS_ST_LEVEL;
    hmnu = CreatePopupMenu (&mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_LARGE;
    mii.typedata    = (DWORD)MS_ST_LARGE;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
     
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_MIDDLE;
    mii.typedata    = (DWORD)MS_ST_MIDDLE;
    InsertMenuItem(hmnu, 1, TRUE, &mii);  
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_SMALL;
    mii.typedata    = (DWORD)MS_ST_SMALL;
    InsertMenuItem(hmnu, 2, TRUE, &mii);  

    return hmnu;
}

HMENU createmenu1(void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)MS_ST_START;
    mii.hsubmenu    = createpmenustart();

    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)MS_ST_LEVEL;
    mii.hsubmenu    = createpmenulevel();
    InsertMenuItem(hmnu, 1, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.id          = 120;
    mii.typedata    = (DWORD)MS_ST_ABOUT;
    mii.hsubmenu    = createpmenuabout();
    InsertMenuItem(hmnu, 2, TRUE, &mii);
                   
    return hmnu;
}

void BothButtonDownProc(HWND hWnd,int adrx,int adry)
      
{
    int test_open = 0; 
    int i = 1;
    int adrx1 = 0, adry1 = 0;
    int flag_opened = 1;
    if (!bom[adrx][adry].test)
    return;
    if (!bom[adrx][adry].value)
    return;
    if (bom[adrx][adry].hit)
    return;
    while (i <= 8)
    {
       switch (i)
       {
        case 1:
           adrx1 = adrx-1;
           adry1 = adry;
        break;
                        
        case 2:
           adrx1 = adrx-1;
           adry1 = adry-1;
        break;

        case 3:
           adrx1 = adrx-1;
           adry1 = adry+1;
        break;
        
        case 4:
           adrx1 = adrx;
           adry1 = adry-1;
        break;
        
        case 5:
            adrx1 = adrx;
            adry1 = adry+1;
        break;
        
        case 6:
            adrx1 = adrx+1;
            adry1 = adry; 
        break; 
        
        case 7:
            adrx1 = adrx+1;
            adry1 = adry+1;
        break; 
        
        case 8:
            adrx1 = adrx+1;
            adry1 = adry-1;
        break;
       }
       
       if (adrx1>=0 && adry1>=0 && adrx1<sg_boxnumx && adry1<sg_boxnumy
                            && bom[adrx1][adry1].hit)
           test_open++;
       else
            if(adrx>=0 && adry1>=0 && adrx1<sg_boxnumx && adry1<sg_boxnumy
                            && !bom[adrx1][adry1].test)
                flag_opened = 0;            
       i++;
    }
    if ((test_open == bom[adrx][adry].value) && !flag_opened)
    {
        if (!Open (hWnd, adrx, adry))
            BombOut (hWnd);
        if (itime == (sg_boxnumx*sg_boxnumy-bombnum))
            Finished(hWnd);
             
    }
            
}

void DrawDigit(HDC hdc, char* buffer, int CLOCK)
{
    int x;
    PBITMAP bmp;
    
    if (CLOCK)
        x = x_clock;
    else
        x = x_bomnum;
        
    while(*buffer){
        bmp = sg_bmpDigit + (*buffer - '0');
        FillBoxWithBitmap(hdc, x, 0, 0, 0, bmp);
        x += WIDTH_DIGIT;
        buffer++;
    }    
}

static int myLoadBitmap (BITMAP* bm, const char* filename)
{
	char full_path [MAX_PATH + 1];

	strcpy (full_path, "res/");
    strcat (full_path, filename);
	return LoadBitmap (HDC_SCREEN, bm, full_path);
}

static char* get_record_file (void)
{
	static char record_file [MAX_PATH + 1] = {'\0'};

	if (record_file [0] != '\0')
		return record_file;

	strcpy (record_file, getpwuid (getuid ()) ->pw_dir);
	if (record_file [strlen (record_file) - 1] != '/')
		strcat (record_file, "/");
	strcat (record_file, ".highscore.bomb");
		return record_file;
}

int TestMyWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC             hdc;
    char            bomn[30], seconds[30];
    char            buffer[256];
    int             i, j;
    int             ran1, ran2;
    static RECT     bombregion, face, onerect, bombnumber, clock; 
    static RECT     winposition;
    FILE*           pHighscore;
    MAINWINCREATE   CreateInfo;

    switch (message) {
      
        case MSG_CREATE:
            if( myLoadBitmap(&bmpbom,"lei.bmp")<0)
                fprintf(stderr,"bitmap error\n");
            else
                fValidbom = TRUE;
  
            if( myLoadBitmap(&bmpface,"face1.bmp")<0)
                fprintf(stderr,"bitmap error\n");
            else
                fValidface = TRUE;
                
            if( myLoadBitmap(&bitmap1,"face.bmp")<0)
                fprintf(stderr,"bitmap error\n");
            else
                fValid1 = TRUE;
                
            if( myLoadBitmap(&bmpflag,"flag.bmp")<0)
                fprintf(stderr,"bitmap error\n");
            else
                fValidflag = TRUE;   
                
            if( myLoadBitmap(&bmpfinalface,"finished.bmp")<0)
                fprintf(stderr,"bitmap error\n");
            else
                fValidfinalface = TRUE;   
                
            if( myLoadBitmap(&bmphitfalse,"hitfalse.bmp")<0)
                fprintf(stderr,"bitmap error\n");
            else
                fValidhitfalse = TRUE;   
                
            for (i = 0; i < 10; i++){
                sprintf(buffer, "%d.bmp", i);
                myLoadBitmap(sg_bmpDigit + i, buffer);
            }

            if ((pHighscore = fopen(get_record_file(),"r"))){
                for (i = 0; i < 3; i++)
                    fscanf(pHighscore, "%d, %s",
                            &score[i].highscore, score[i].name);
                fclose(pHighscore);
            }
            else
                for (i = 0; i < 3; i++){
                    score[i].highscore = 999;
                    strcpy(score[i].name, "unknown");
                }    
            
            SetTimer(hWnd, ID_CLOCK, 100); /* 1s */
            PostMessage(hWnd,MSG_COMMAND,ID_NEW,0);
        break;

        case MSG_COMMAND:
            if (LOWORD(wParam) == ID_ABOUT)
            {
               InitAbHostedCreateInfo(hWnd,&CreateInfo);
               CreateMainWindow(&CreateInfo);  
            }
            
            if (LOWORD(wParam) == ID_CLOSE)
            {
                PostMessage(hWnd, MSG_CLOSE, 0, 0);
            }
            
            if (LOWORD(wParam) == ID_HIGHSCORE)
            {
               InitHighScoreCreateInfo(hWnd,&CreateInfo);
               hHighscore =  CreateMainWindow(&CreateInfo);  
               ShowWindow(hHighscore,SW_SHOW); 
            }
            
            if (LOWORD(wParam) == ID_LARGE)
            {
                bombnum = 99;
                sg_boxnumx = 30;
                sg_boxnumy = 16;
                winwidth = WIDTH_LARGEWIN;
                flag_size = 2;
                GetWindowRect(hWnd, &winposition);
                MoveWindow(hWnd, winposition.left, winposition.top, 
                           WIDTH_LARGEWIN, HEIGHT_LARGEWIN, FALSE);
                PostMessage(hWnd, MSG_COMMAND, ID_NEW, 0);
            }    
            if (LOWORD(wParam) == ID_MIDDLE)
            {
                bombnum = 40;
                sg_boxnumx = 16;
                sg_boxnumy = 16;
                winwidth = WIDTH_MIDDLEWIN;
                flag_size = 1;
                GetWindowRect(hWnd, &winposition);
                MoveWindow(hWnd, winposition.left, winposition.top, 
                           WIDTH_MIDDLEWIN, HEIGHT_MIDDLEWIN, FALSE);
                PostMessage(hWnd, MSG_COMMAND, ID_NEW, 0);
            }
            
            if (LOWORD(wParam) == ID_SMALL)
            {
                bombnum = 10;
                sg_boxnumx = 8;
                sg_boxnumy = 8;
                winwidth = WIDTH_SMALLWIN;
                flag_size = 0;
                GetWindowRect(hWnd, &winposition);
                MoveWindow(hWnd, winposition.left, winposition.top, 
                           WIDTH_SMALLWIN, HEIGHT_SMALLWIN, FALSE);
                PostMessage(hWnd, MSG_COMMAND, ID_NEW, 0);
            }    
                
            if (LOWORD(wParam) == ID_NEW)
            {
                bTimer = FALSE;
                second = 0;
                itime = 0;
                leftbombnum = bombnum;
                flag_bombout = 0;
                flag_finished = 0;
                x_bomnum = winwidth / 6;
                x_face = (winwidth*2) / 5;
                x_clock = (winwidth*3) / 5;
                offsetx = (winwidth - WIDTH_BOX*sg_boxnumx)/2-2;
                SetRect (&clock, x_clock, 0, 
                            x_clock + WIDTH_CLOCK, HEIGHT_CLOCK);
                            
                SetRect (&face, x_face, 0,
                            x_face + WIDTH_FACE, HEIGHT_FACE);
                            
                SetRect (&bombregion, offsetx, HEIGHT_FACE,
                           WIDTH_BOX*sg_boxnumx+offsetx,
                           HEIGHT_BOX*sg_boxnumy+HEIGHT_FACE);
                     
                SetRect (&bombnumber, x_bomnum, 0,
                     x_bomnum + WIDTH_BOMNUM, HEIGHT_BOMNUM);
                     
               //**************initial bom value**************
               
              for (i = 0; i < sg_boxnumx; i++)
                for (j = 0; j < sg_boxnumy; j++)
                { bom[i][j].flag  = 0;
                  bom[i][j].hit   = FALSE;
                  bom[i][j].value = 0;
                  bom[i][j].test  = FALSE;
                  bom[i][j].bombout = FALSE;
                  bom[i][j].error = FALSE;
                 }; 
              for (i = 0; i < (sg_boxnumx*sg_boxnumy); i++)
                  NoAdr[i].NY = FALSE; 
                   
              srandom( time(NULL));
              i = 0;
              while( i < bombnum )
               {
                  ran1 = random()%sg_boxnumx;
                  ran2 = random()%sg_boxnumy;
                  if(!bom[ran1][ran2].flag)
                     { 
                        bom[ran1][ran2].flag = 1;
                        i++;
                     } 
                } ;     
 
               for (i = 0; i < sg_boxnumx; i++)         
                  for (j = 0; j < sg_boxnumy; j++)
                    {
                if (!bom[i][j].flag) {
                if(i-1>=0&&j-1>=0&&bom[i-1][j-1].flag)
                        bom[i][j].value++;
                        
                if(i-1>=0&&bom[i-1][j].flag)
                        bom[i][j].value++;
                        
                if(i-1>=0&&j+1<sg_boxnumy&&bom[i-1][j+1].flag)
                        bom[i][j].value++;
                        
                if(j-1>=0&&bom[i][j-1].flag)
                        bom[i][j].value++;
                        
                if(j+1<sg_boxnumy&&bom[i][j+1].flag)
                        bom[i][j].value++;
                        
                if(i+1<sg_boxnumx&&j+1<sg_boxnumy&&bom[i+1][j+1].flag)
                        bom[i][j].value++; 
                        
                if(i+1<sg_boxnumx&&bom[i+1][j].flag) 
                        bom[i][j].value++;
                        
                if(i+1<sg_boxnumx&&j-1>=0&&bom[i+1][j-1].flag)
                        bom[i][j].value++;
                } 
                     }    
               InvalidateRect (hWnd, NULL, TRUE);
               }
        break;
        
        case MSG_TIMER:
            if (wParam == ID_CLOCK)
                if (bTimer){
                    if (second < 1000){
                        second++;
                        InvalidateRect(hWnd, &clock, FALSE);
                    }
                }
        break;        

        case MSG_LBUTTONDOWN:

            oldx = LOWORD (lParam);
            oldy = HIWORD (lParam);

            adrx = (oldx-offsetx)/WIDTH_BOX;
            adry = (oldy-HEIGHT_FACE)/HEIGHT_BOX;
            
            if (hCongratuate | hHighscore)
                break;
                
            if (!PtInRect (&bombregion, oldx, oldy)) {
                if (PtInRect (&face, oldx, oldy)){
                    PostMessage (hWnd, MSG_COMMAND, ID_NEW, 0);
                    break;
                }    
                else
                    break;
            }
            if (flag_bombout)
                break;
            if (flag_finished)
                break;
            if (!bTimer)
                bTimer = TRUE;
                
            if (bom[adrx][adry].hit)
                break;

            if (wParam & KS_RIGHTBUTTON) {
                BothButtonDownProc(hWnd,adrx,adry);
                break;
            }
            
                
            if (bom[adrx][adry].test)
                break;
                
            if (bom[adrx][adry].flag) {
                BombOut(hWnd);   
                break;
            }    
            if (bom[adrx][adry].value != 0)
            {
                    NoAdr[itime].x  = adrx;
                    NoAdr[itime].y  = adry;
                    NoAdr[itime].NY = TRUE;
                    itime++;
                    bom[adrx][adry].test = TRUE;
                    
                    SetRect (&onerect, adrx*WIDTH_BOX+offsetx,
                           adry*HEIGHT_BOX+HEIGHT_FACE,
                           (adrx+1)*WIDTH_BOX+offsetx,
                           (adry+1)*HEIGHT_BOX+HEIGHT_FACE);
                    InvalidateRect (hWnd, &onerect, FALSE);
             }
             else {
                    hdc = GetClientDC(hWnd);
                    SearchGround (hdc, adrx, adry);
                    ReleaseDC(hdc);
             }
             
             if (itime == (sg_boxnumx*sg_boxnumy - bombnum))
                Finished(hWnd);
             
        break;

        case MSG_RBUTTONDOWN:
           //  SetCapture (hWnd);
             oldx = LOWORD (lParam);
             oldy = HIWORD (lParam);
             adrx = (oldx-offsetx)/WIDTH_BOX;
             adry = (oldy-HEIGHT_FACE)/HEIGHT_BOX;  

            if (!PtInRect (&bombregion, oldx, oldy))
                break;
            if (flag_bombout)
                break;
            if (flag_finished)
                break;
                
            if (!bTimer)
                bTimer = TRUE;
                
           /*****RBUTTONDOWN & LBUTTONDOWN******/
            
            if (wParam & KS_LEFTBUTTON){
                BothButtonDownProc(hWnd,adrx,adry); 
                break; 
            }    
           /*******  only  rbuttondown   *******/
                
            if (bom[adrx][adry].test)
               break;
            if (!bom[adrx][adry].hit )
               {
                     bom[adrx][adry].hit = TRUE;
                     leftbombnum--;    
               }
            else
               { bom[adrx][adry].hit = FALSE;
                 leftbombnum++;
               }
            SetRect (&onerect, adrx*WIDTH_BOX+offsetx,
                      adry*HEIGHT_BOX+HEIGHT_FACE, 
                      (adrx+1)*WIDTH_BOX+offsetx,
                      (adry+1)*HEIGHT_BOX+HEIGHT_FACE);
            InvalidateRect (hWnd, &onerect, FALSE);
            InvalidateRect (hWnd, &bombnumber, FALSE);
        break;
      
       
        case MSG_RBUTTONUP:
        break;

        case MSG_LBUTTONUP:
        break;
        
        case MSG_PAINT:
        {
            RECT        rc;
            WINDOWINFO  *info;

            hdc = BeginPaint (hWnd);
           
            sprintf(seconds, "%03d", second);
            DrawDigit(hdc, seconds, TRUE);

            if (leftbombnum >= 0){                     
                sprintf(bomn,"%02d",leftbombnum);
                DrawDigit(hdc, bomn, FALSE);
            }
            
            if (flag_finished)
                DrawMyBitmap (hdc, fValidfinalface?&bmpfinalface:NULL,
                                    x_face+1, 1, 0, 0);
            else 
                if (flag_bombout)
                    DrawMyBitmap (hdc, fValidface?&bmpface:NULL,
                                    x_face, 0, 0, 0);
                else
                    DrawMyBitmap(hdc, fValid1?&bitmap1:NULL,
                                    x_face, 0, 0, 0);
            
            info = (WINDOWINFO *)GetWindowInfo (hWnd);
            for (i = 0; i < sg_boxnumx; i++)
                for (j = 0; j < sg_boxnumy; j++)
            {
                if (!bom[i][j].test && !bom[i][j].bombout){
                    SetPenColor (hdc,COLOR_darkgray);
                    Rectangle(hdc, 
                            i*WIDTH_BOX+offsetx,
                            j*HEIGHT_BOX+HEIGHT_FACE,
                            (i+1)*WIDTH_BOX+offsetx,
                            (j+1)*HEIGHT_BOX+HEIGHT_FACE);

                    rc.left = i*WIDTH_BOX+1+offsetx;
                    rc.right = (i+1)*WIDTH_BOX+offsetx;
                    rc.top = j*HEIGHT_BOX+1+HEIGHT_FACE;
                    rc.bottom = (j+1)*HEIGHT_BOX+HEIGHT_FACE;

                    info->we_rdr->draw_3dbox (hdc, &rc, 
                            GetWindowElementAttr (hWnd, WE_MAINC_THREED_BODY),
                            LFRDR_BTN_STATUS_NORMAL | LFRDR_3DBOX_FILLED);
                }
              
           
                if ( bom[i][j].hit) {
                    DrawMyBitmap(hdc, fValidflag?&bmpflag:NULL,
                                 i*WIDTH_BOX+offsetx+3,
                                 j*HEIGHT_BOX+3+HEIGHT_FACE,
                                 WIDTH_BOX - 5,
                                 HEIGHT_BOX - 5);
                }
            
                if (bom[i][j].error) {
                    Cancel3DFrame(hdc,i*WIDTH_BOX+offsetx,
                                 j*HEIGHT_BOX+HEIGHT_FACE,
                                 WIDTH_BOX,HEIGHT_BOX);
                    
                    DrawMyBitmap(hdc, fValidhitfalse?&bmphitfalse:NULL,
                       i*WIDTH_BOX+offsetx+1, j*HEIGHT_BOX+1+HEIGHT_FACE, 0, 0);
                }
                
                if (bom[i][j].bombout) {
                    Cancel3DFrame(hdc,i*WIDTH_BOX+offsetx,
                                        j*HEIGHT_BOX+HEIGHT_FACE,
                                        WIDTH_BOX,HEIGHT_BOX);
                    
                    DrawMyBitmap(hdc, fValidbom?&bmpbom:NULL,
                       i*WIDTH_BOX+offsetx+1, j*HEIGHT_BOX+1+HEIGHT_FACE, 0, 0);
                }
            }
            
            for ( i = 0; i < itime; i++ )
            {
             Cancel3DFrame(hdc,(NoAdr[i].x)*WIDTH_BOX+offsetx,
                        (NoAdr[i].y)*HEIGHT_BOX+HEIGHT_FACE,
                        WIDTH_BOX, HEIGHT_BOX);
             if(NoAdr[i].NY) 
                 TextValue(hdc,(NoAdr[i].x)*WIDTH_BOX+offsetx,
                          (NoAdr[i].y)*HEIGHT_BOX+HEIGHT_FACE, 
                          bom[NoAdr[i].x][NoAdr[i].y].value);
            }
            EndPaint (hWnd, hdc);
            return 0;    
        }

        case MSG_CLOSE:
            UnloadBitmap(&bmpbom);
            UnloadBitmap(&bmpface);
            UnloadBitmap(&bitmap1);
            UnloadBitmap(&bmpfinalface);
            UnloadBitmap(&bmphitfalse);
            UnloadBitmap(&bmpflag);
            
            KillTimer(hWnd, ID_CLOCK);  
            for (i = 0; i < 10; i++)
                UnloadBitmap(sg_bmpDigit + i);

            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
        return 0;
    }
    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

void InitMyWinCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_SYSMENU |  WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption= MS_ST_CAPTION;
    pCreateInfo->hMenu = createmenu1();
    pCreateInfo->hCursor = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = TestMyWinProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = WIDTH_LARGEWIN;
    pCreateInfo->by = HEIGHT_LARGEWIN;
    pCreateInfo->iBkColor = COLOR_lightgray; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

#ifndef _MGRM_THREADS
int MiniGUIMain(int args, const char* arg[])
#else
void* TestMyWindow(void* data) 
#endif
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

    if (JoinLayer (layer, arg[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif

    InitMyWinCreateInfo(&CreateInfo);

    hMainWnd = CreateMainWindow(&CreateInfo);

    if (hMainWnd == HWND_INVALID)
#ifndef _MGRM_THREADS
        return 0;
#else
        return NULL;
#endif

    ShowWindow(hMainWnd,SW_SHOWNORMAL);
    while( GetMessage(&Msg, hMainWnd) ) {
        TranslateMessage (&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup(hMainWnd);
#ifndef _MGRM_THREADS
        return 0;
#else
    return NULL;
#endif
}

#ifdef _MGRM_THREADS
int MiniGUIMain(int args, const char* arg[])
{
    TestMyWindow (NULL);
    return 0;
}
#endif

/**********   create a hosted about window****/
int AbHostedWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message) {
        case MSG_CREATE:
        break;

        case MSG_PAINT:
        {
            HDC hdc;

            hdc = BeginPaint (hWnd);
            SetBkColor(hdc,COLOR_lightgray);
            TextOut (hdc, 10, 25, MS_ST_VERSION); 
            TextOut (hdc, 10, 55, MS_ST_AUTHOR1);
            TextOut (hdc, 10, 75, MS_ST_AUTHOR2);
            EndPaint (hWnd, hdc);
        }
        return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}
void InitAbHostedCreateInfo (HWND hHosting, PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle   = WS_BORDER | WS_CAPTION | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = MS_ST_ABWCAP;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor   = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = AbHostedWinProc;
    pCreateInfo->lx = 100; 
    pCreateInfo->ty = 200;
    pCreateInfo->rx = 540;
    pCreateInfo->by = 390;
    pCreateInfo->iBkColor  = COLOR_lightgray; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting  = hHosting;
}

/*************************** High Scores Window ******************************/
#define IDC_RESET  110
int HighScoreWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static HWND hRESET, hOK;
    
    switch (message) {
        case MSG_CREATE:
            hRESET = CreateWindow ("button",
                        MS_ST_RESET,
                        WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
                        IDC_RESET,
                        100, 145, 60, 26, hWnd, 0);
                        
            hOK    = CreateWindow ("button",
                        MS_ST_OK,
                        WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
                        IDOK,
                        220, 145, 60, 26, hWnd, 0);
        break;

        case MSG_COMMAND:
        {
            int id = LOWORD(wParam);
            FILE* pHighscore;
            int i;

            if (id == IDC_RESET && HIWORD(wParam) == BN_CLICKED) {
                for (i = 0; i < 3; i++){
                    score[i].highscore = 999;
                    strcpy(score[i].name, "unknown");
                }    
                
                if ((pHighscore = fopen(get_record_file(), "w+"))){
                    for (i = 0; i < 3; i++)
                        fprintf(pHighscore, "%d, %s\n", 
                                    score[i].highscore,score[i].name);
                    fclose(pHighscore);
                }    
                InvalidateRect(hWnd, NULL, TRUE);
            }
            if (id == IDOK && HIWORD(wParam) == BN_CLICKED)
                PostMessage(hWnd, MSG_CLOSE, 0, 0);
        }
        break;
        
        case MSG_PAINT:
        {
            HDC hdc;
            char buffer[50];
            int i;
            
            hdc = BeginPaint (hWnd);
            SetBkColor(hdc,COLOR_lightgray);
            TextOut(hdc, 130, 15, MS_ST_HERO);
            TextOut(hdc, 15, 45, MS_ST_SMALL);
            TextOut(hdc, 15, 75, MS_ST_MIDDLE);
            TextOut(hdc, 15, 105, MS_ST_LARGE);
            for (i = 0; i < 3; i++){
                sprintf(buffer, "%3d          %s", 
                            score[i].highscore, score[i].name);
                TextOut(hdc, 150, 45+i*30, buffer);
            }    
            EndPaint (hWnd, hdc);
        }
        return 0;

        case MSG_CLOSE:
            DestroyWindow (hRESET);
            DestroyWindow (hOK);
            DestroyMainWindow (hWnd);
            hHighscore = HWND_NULL;
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

void InitHighScoreCreateInfo (HWND hHosting, PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle   = WS_BORDER | WS_CAPTION;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption = MS_ST_HSCAP;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor   = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = HighScoreWinProc;
    pCreateInfo->lx = 100; 
    pCreateInfo->ty = 200;
    pCreateInfo->rx = 470;
    pCreateInfo->by = 410;
    pCreateInfo->iBkColor  = COLOR_lightgray; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting  = hHosting;
}

/************************* Congratulation Window *****************************/

#define IDC_CTRL_NAME       100

int CongratulationWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static HWND hPrompt, hName, hOK;
       
    switch (message) {
        case MSG_CREATE:
            hPrompt = CreateWindow ("static",
                          MS_ST_INPUTNAME,
                          SS_SIMPLE | WS_VISIBLE,
                          IDC_STATIC,
                          10, 10, 185, 24, hWnd, 0);
            hName   = CreateWindow ("edit",
                          getlogin(),
                          WS_CHILD | WS_VISIBLE | WS_BORDER,
                          IDC_CTRL_NAME,
                          10, 40, 175, 26, hWnd, 0);
            hOK     = CreateWindow ("button",
                          MS_ST_OK,
                          WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
                          IDOK,
                          110, 75, 75, 26, hWnd, 0);
        break;

        case MSG_COMMAND:
        {
            int id = LOWORD(wParam);
            FILE* pHighscore;
            int i;

            if (id == IDOK && HIWORD(wParam) == BN_CLICKED) {
                score[flag_size].highscore = second;
                SendMessage (hName, MSG_GETTEXT,
                                19, (LPARAM)score[flag_size].name);
                if ((pHighscore = fopen(get_record_file(), "w+"))){
                    for (i = 0; i < 3; i++)
                        fprintf(pHighscore, "%d, %s\n", 
                                    score[i].highscore,score[i].name);
                    fclose(pHighscore);
                }    
                PostMessage (hWnd, MSG_CLOSE, 0, 0);
            }
        }
        break;
        case MSG_CLOSE:
            DestroyWindow (hPrompt);
            DestroyWindow (hName);
            DestroyWindow (hOK);
            DestroyMainWindow (hWnd);
            hCongratuate = HWND_NULL;
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

void InitCongratulationCreateInfo (HWND hHosting, PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle   = WS_BORDER | WS_CAPTION | WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_IMECOMPOSE;
    pCreateInfo->spCaption = MS_ST_CONGCAP ;
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor   = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = CongratulationWinProc;
    pCreateInfo->lx = 50; 
    pCreateInfo->ty = 60;
    pCreateInfo->rx = 255;
    pCreateInfo->by = 200;
    pCreateInfo->iBkColor  = COLOR_lightgray; 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting  = hHosting;
}

#ifndef _MGRM_THREADS
#include <minigui/dti.c>
#endif

