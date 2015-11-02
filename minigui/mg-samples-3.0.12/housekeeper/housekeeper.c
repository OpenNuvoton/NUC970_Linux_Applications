/*
 * $Id: housekeeper.c 420 2008-02-02 02:48:59Z wangjian $
 *
 *little game housekeeper, also named sokoban. 
 *written by Song Lixin(zjujoe@263.net) 2001.3.6
 */
#include "map.h"

#ifdef _LANG_ZHCN
#include "housekeeper_res_cn.h"
#elif defined _LANG_ZHTW
#include "housekeeper_res_tw.h"
#else
#include "housekeeper_res_en.h"
#endif

#define ID_GOTO		7980
#define ID_NEXT		7990
#define ID_PREVIOUS     8000 
#define ID_RESTART	8010
#define ID_UNDO		8020
#define ID_EXIT		8040

#define	ID_MAP_A	8050
#define	ID_MAP_B	8060
#define	ID_MAP_C	8070
#define	ID_MAP_D	8080
#define	ID_MAP_E	8090
#define	ID_MAP_F	8100
#define	ID_MAP_G	8110
#define	ID_MAP_H	8120
#define ID_ABOUT	8130
	
#define  WIDTH_LARGEWIN   600
#define  HEIGHT_LARGEWIN  400

int TestMyWinProc(HWND hWnd,int message,WPARAM wParam,LPARAM lParam);
void InitMyWinCreateInfo(PMAINWINCREATE pCreateInfo);
HMENU createmenu1(void);
HMENU createpmenugame(void);
HMENU createpmenucollection(void);
HMENU createpmenuabout(void);
BOOL InitializeApp(void);
void OnClose(HWND hWnd);
void OnDraw(HWND hWnd,HDC hDC);

static BITMAP bitmapAll[6];

#define IDC_LEVELINPUT 9000
DLGTEMPLATE DlgInputLevel = 
{
    WS_BORDER | WS_CAPTION,
    WS_EX_IMECOMPOSE,
    70, 80, 400, 158,
    HK_ST_CHOOSELEVEL,
    0, 0,
    4, NULL,
    0
};

CTRLDATA CtrlInputLevel [] = 
{
    {
        "static",
        WS_VISIBLE | SS_RIGHT,
        14, 30, 150, 18,
        IDC_STATIC, 
        HK_ST_INPUTLEVEL,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP, 
        180, 30, 100, 22, 
        IDC_LEVELINPUT, 
        NULL,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 
        80, 94, 100, 28, 
        IDOK, 
        HK_ST_OK,
        0 
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        256, 94, 100, 28, 
        IDCANCEL, 
        HK_ST_CANCEL, 
        0
    }
};
  
static int 
DialogInputLevelProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	char strText[4];
	int i;

	switch (message) {
	case MSG_INITDIALOG:
        	return 1;
        
    	case MSG_COMMAND:
		switch (wParam) {
		case IDOK:
			GetDlgItemText(hDlg,IDC_LEVELINPUT,strText,3);
			i = atoi(strText);
			EndDialog(hDlg,i);
			break;
		case IDCANCEL:
		    EndDialog (hDlg, 0);
		    break;
		}
		break;
    }
    
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}
HMENU createmenu1(void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    hmnu = CreateMenu();

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 100;
    mii.typedata    = (DWORD)HK_ST_GAME;
    mii.hsubmenu    = createpmenugame();

    InsertMenuItem(hmnu, 0, TRUE, &mii);

    mii.type        = MFT_STRING;
    mii.id          = 110;
    mii.typedata    = (DWORD)HK_ST_COLL;
    mii.hsubmenu    = createpmenucollection();
    InsertMenuItem(hmnu, 1, TRUE, &mii);
    
    mii.type        = MFT_STRING;
    mii.id          = 120;
    mii.typedata    = (DWORD)HK_ST_ABOUT;
    mii.hsubmenu    = createpmenuabout();
    InsertMenuItem(hmnu, 2, TRUE, &mii);
                   
    return hmnu;
}

HMENU createpmenuabout(void)
{
    HMENU hmnu;
    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)HK_ST_ABOUT;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_ABOUT;
    mii.typedata    = (DWORD)HK_ST_ABOUTTHIS;
    InsertMenuItem(hmnu, 0, TRUE, &mii);
    return hmnu;
}

HMENU createpmenugame(void)
{
    HMENU hmnu;

    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)HK_ST_GAME;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_GOTO;
    mii.typedata    = (DWORD)HK_ST_GOTOLEVEL;
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_NEXT;
    mii.typedata    = (DWORD)HK_ST_NEXTLEVEL;
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_PREVIOUS;
    mii.typedata    = (DWORD)HK_ST_PREVLEVEL;
    InsertMenuItem(hmnu, 2, TRUE, &mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_RESTART;
    mii.typedata    = (DWORD)HK_ST_RESTART;
    InsertMenuItem(hmnu, 3, TRUE, &mii);
    
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_UNDO;
    mii.typedata    = (DWORD)HK_ST_UNDO;
    InsertMenuItem(hmnu, 4, TRUE, &mii);


    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_EXIT;
    mii.typedata    = (DWORD)HK_ST_EXIT;
    InsertMenuItem(hmnu, 5, TRUE, &mii);
    
    return hmnu;
}

HMENU createpmenucollection(void)
{
    HMENU hmnu;

    MENUITEMINFO mii;
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)HK_ST_COLL;
    hmnu = CreatePopupMenu (&mii);
    

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_MAP_A;
    mii.typedata    = (DWORD)HK_ST_COLL_A;
    InsertMenuItem(hmnu, 0, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_MAP_B;
    mii.typedata    = (DWORD)HK_ST_COLL_B;
    InsertMenuItem(hmnu, 1, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_MAP_C;
    mii.typedata    = (DWORD)HK_ST_COLL_C;
    InsertMenuItem(hmnu, 2, TRUE, &mii);


    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_MAP_D;
    mii.typedata    = (DWORD)HK_ST_COLL_D;
    InsertMenuItem(hmnu, 3, TRUE, &mii);


    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_MAP_E;
    mii.typedata    = (DWORD)HK_ST_COLL_E;
    InsertMenuItem(hmnu, 4, TRUE, &mii);


    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_MAP_F;
    mii.typedata    = (DWORD)HK_ST_COLL_F;
    InsertMenuItem(hmnu, 5, TRUE, &mii);


    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_MAP_G;
    mii.typedata    = (DWORD)HK_ST_COLL_G;
    InsertMenuItem(hmnu, 6, TRUE, &mii);


    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_MAP_H;
    mii.typedata    = (DWORD)HK_ST_COLL_H;
    InsertMenuItem(hmnu, 7, TRUE, &mii);

    return hmnu;
}

int TestMyWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int iRet;
	int x,y;
	ptagLevelCollection pTempColl;
	ptagLevel pTempLev;
	int iDir = 0;
	//char strTemp[50];
	switch(message){
	case MSG_CREATE:
		if(!InitializeApp()){
            return 1;
		}

    	DlgInputLevel.controls = CtrlInputLevel;
		break;
	case MSG_COMMAND:
		switch(LOWORD(wParam)){	
		case ID_GOTO:
    			iRet = DialogBoxIndirectParam(&DlgInputLevel, hWnd, DialogInputLevelProc, 0L);
			if ((iRet > 0) && (iRet <= theMap->current->iNoOfLevels)) {
				pTempLev = theMap->current->head;
				if (pTempLev->iNo != iRet) {
					pTempLev = pTempLev->next;
					while(pTempLev != theMap->current->head) {
						if(pTempLev->iNo == iRet) 
							break;
						pTempLev = pTempLev->next;
					}
				}	
			        if ((pTempLev->iNo == iRet) && (pTempLev != theMap->currentLevel)) {
					theMap->current->current = pTempLev;
					PlayRestart();
					InvalidateRect(hWnd, NULL, TRUE);
				}		
			}
			break;
		case ID_NEXT:
			theMap->current->current = theMap->current->current->next;
			PlayRestart();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_PREVIOUS:
			theMap->current->current = theMap->current->current->prev;
			PlayRestart();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_RESTART:
			PlayRestart();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_UNDO:
			PlayUndo(hWnd); 
			break;
		case ID_EXIT:
		 	OnClose(hWnd);
			break;
		case ID_MAP_A:
			pTempColl = theMap->head;
			if (GotoCollection(pTempColl)) InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MAP_B:
			pTempColl = theMap->head->next;
			if (GotoCollection(pTempColl)) InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MAP_C:
			pTempColl = theMap->head->next->next;
			if (GotoCollection(pTempColl)) InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MAP_D:
			pTempColl = theMap->head->next->next->next;
			if (GotoCollection(pTempColl)) InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MAP_E:
			pTempColl = theMap->tail->prev->prev->prev;
			if (GotoCollection(pTempColl)) InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MAP_F:
			pTempColl = theMap->tail->prev->prev;
			if (GotoCollection(pTempColl)) InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MAP_G:
			pTempColl = theMap->tail->prev;
			if (GotoCollection(pTempColl)) InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MAP_H:
			pTempColl = theMap->tail;
			if (GotoCollection(pTempColl)) InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_ABOUT:				
			MessageBox(HWND_DESKTOP, HK_ST_ABOUTBOX, HK_ST_ABOUT, MB_OK);
			break;
		}		
		return 0;
	case MSG_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		if (!ptChoosePlace(hWnd, &x, &y))
			return 0;
		//snprintf(strTemp, 50, "%d:%d", x, y);
		//MessageBox(HWND_DESKTOP, strTemp, strTemp, MB_OK);
		PlayMove(hWnd, x, y);
		return 0;
	case MSG_RBUTTONDOWN:
		PlayUndo(hWnd); 
		return 0;
	case MSG_PAINT:
		hdc = BeginPaint(hWnd);
		OnDraw(hWnd,hdc);
		EndPaint(hWnd,hdc);
		return 0;

	case MSG_KEYDOWN:
		switch (wParam) {
			case SCANCODE_CURSORBLOCKUP:     
				iDir = DIR_UP;
				break;
			case SCANCODE_CURSORBLOCKLEFT:        
				iDir = DIR_LEFT;
				break;
			case SCANCODE_CURSORBLOCKRIGHT:      
				iDir = DIR_RIGHT;
				break;
			case SCANCODE_CURSORBLOCKDOWN:        
				iDir = DIR_DOWN;
				break;
			default:
				iDir = -1;
		}
		
		if ((iDir != -1) && (PlayKeyboard(hWnd, iDir) == STEP_TYPE_PUSH)){
			if (CheckMissionComplete()) {
				MessageBox(HWND_DESKTOP, HK_ST_COMPLETED, HK_ST_GOOD, MB_OK);
				theMap->current->current = theMap->current->current->next;
				PlayRestart();
				InvalidateRect(hWnd, NULL, TRUE);
			}
			return 0;
		}
		
		if (wParam == SCANCODE_U) 
			PlayUndo(hWnd); 
		return 0;			
	case MSG_TIMER:
		return 0;			
    case MSG_CLOSE:
		 OnClose(hWnd);
	    	 return 0;
	}
	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

void OnDraw(HWND hWnd,HDC hDC) 
{
	RECT rect;
	POINT poStart1;
	int iSideLength;
	ptagLevel cur_level;
	char strText[50];
	int iRow;
	int iCol;
	int i, j;
	int iBmp;

	GetClientRect(hWnd,&rect);
	SetBkColor(hDC,RGB2Pixel(HDC_SCREEN, 102, 102, 102));
	//SetTextColor(hDC,COLOR_green);	
	SetPenColor(hDC,COLOR_black);
	poStart1.x = rect.left;
	poStart1.y = rect.top;
	
	cur_level = theMap->currentLevel;
	snprintf(strText, 50, "%s %s, %s %d ",
           HK_ST_COLL, theMap->current->strName, 
           HK_ST_LEVEL, cur_level->iNo);	
	TextOut(hDC, rect.left + 40, rect.bottom - 20, strText);
	rect.bottom -= 20;	
	if (rect.bottom - rect.top < 100)
		return;
	iRow = cur_level->row;
	iCol = cur_level->col;

	i = RECTH(rect)/iRow;
	j = RECTW(rect)/iCol;
	if (i <= j) 
		iSideLength = i;
	else 	
		iSideLength = j;	

	for (i = 0; i < iRow; i++) {
		for (j = 0; j < iCol; j++) {
			iBmp = cur_level->data[i * iCol + j];
			if (iBmp < B_NOTHING) {
				FillBoxWithBitmap(hDC, poStart1.x + iSideLength * j \
						, poStart1.y + iSideLength * i, iSideLength, iSideLength, &bitmapAll[iBmp]);
            }
		}
    }
}

void InitMyWinCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_CAPTION | WS_BORDER | WS_SYSMENU |  WS_VISIBLE;
    pCreateInfo->dwExStyle = WS_EX_NONE;
    pCreateInfo->spCaption= HK_ST_CAP;
    pCreateInfo->hMenu = createmenu1();
    pCreateInfo->hCursor = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = TestMyWinProc;
    pCreateInfo->lx = 0;
    pCreateInfo->ty = 0;
    pCreateInfo->rx = WIDTH_LARGEWIN;
    pCreateInfo->by = HEIGHT_LARGEWIN;
    pCreateInfo->iBkColor = RGB2Pixel(HDC_SCREEN, 102, 102, 102); 
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

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
    if (JoinLayer (layer, arg[0], 0, 0) == INV_LAYER_HANDLE) {
        printf ("JoinLayer: invalid layer handle.\n");
        exit (1);
    }
#endif

    InitMyWinCreateInfo(&CreateInfo);

    hMainWnd = CreateMainWindow(&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd,SW_SHOWNORMAL);
    while( GetMessage(&Msg, hMainWnd) ) {
        TranslateMessage (&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup(hMainWnd);
    return 0;
}

BOOL InitializeApp(void)
{

	if(LoadBitmap(HDC_SCREEN, &bitmapAll[B_GOAL], "res/goal.gif"))
		return FALSE;
	if(LoadBitmap(HDC_SCREEN, &bitmapAll[B_MAN], "res/man.gif"))
		return FALSE;
	if(LoadBitmap(HDC_SCREEN, &bitmapAll[B_OBJECT], "res/object.gif"))
		return FALSE;
	if(LoadBitmap(HDC_SCREEN, &bitmapAll[B_SAVEMAN], "res/saveman.gif"))
		return FALSE;
	if(LoadBitmap(HDC_SCREEN, &bitmapAll[B_STONE], "res/stone.gif"))
		return FALSE;
	if(LoadBitmap(HDC_SCREEN, &bitmapAll[B_TREASURE], "res/treasure.gif"))
		return FALSE;
	InitMap();
	return TRUE;
}

void OnClose(HWND hWnd)
{
	DestroyMap();
	UnloadBitmap(&bitmapAll[B_GOAL]);
	UnloadBitmap(&bitmapAll[B_MAN]);
	UnloadBitmap(&bitmapAll[B_OBJECT]);
	UnloadBitmap(&bitmapAll[B_SAVEMAN]);
	UnloadBitmap(&bitmapAll[B_STONE]);
	UnloadBitmap(&bitmapAll[B_TREASURE]);
	DestroyMainWindow (hWnd);
	PostQuitMessage (hWnd);
}

void CovertCoord(HWND hWnd,int *px, int *py)
{
	RECT rect;
	int iSideLength;
	ptagLevel cur_level;
	int iRow;
	int iCol;
	int i, j;

	GetClientRect(hWnd,&rect);
	rect.bottom -= 20;	
	if (rect.bottom - rect.top < 100)
		return;
	cur_level = theMap->currentLevel;
	iRow = cur_level->row;
	iCol = cur_level->col;
	i = RECTH(rect)/iRow;
	j = RECTW(rect)/iCol;
	if (i <= j) 
		iSideLength = i;
	else 	
		iSideLength = j;	

	i = *px;
	j = *py;

	*px = rect.left + i * iSideLength;
	*py = rect.top  + j * iSideLength;
}

BOOL ptChoosePlace(HWND hWnd, int *px, int *py) 
{
	RECT rect;
	int iSideLength;
	ptagLevel cur_level;
	int iRow;
	int iCol;
	int i, j;

	GetClientRect(hWnd,&rect);
	rect.bottom -= 20;	
	if (rect.bottom - rect.top < 100)
		return FALSE;
	cur_level = theMap->currentLevel;
	iRow = cur_level->row;
	iCol = cur_level->col;
	i = RECTH(rect)/iRow;
	j = RECTW(rect)/iCol;
	if (i <= j) 
		iSideLength = i;
	else 	
		iSideLength = j;	

	i = *px;
	j = *py;

	if ( (i > rect.left) && (i < rect.left + iSideLength * iCol) &&
	     (j > rect.top ) && (j < rect.top  + iSideLength * iRow) ) {
		 *px = (i - rect.left) / iSideLength;
		 *py = (j - rect.top ) / iSideLength;
		 return TRUE;
	}
	return FALSE;
}

void DrawALittleBlock(HWND hWnd, int x, int y, int itype)	
{
	HDC hdc;
	RECT rect;
	int iSideLength;
	ptagLevel cur_level;
	int iRow;
	int iCol;
	int i, j;

	GetClientRect(hWnd,&rect);
	rect.bottom -= 20;	
	if (rect.bottom - rect.top < 100)
		return;
	cur_level = theMap->currentLevel;
	iRow = cur_level->row;
	iCol = cur_level->col;
	i = RECTH(rect)/iRow;
	j = RECTW(rect)/iCol;
	if (i <= j) 
		iSideLength = i;
	else 	
		iSideLength = j;	

	i = x;
	j = y;

	x = rect.left + i * iSideLength;
	y = rect.top  + j * iSideLength;

	hdc = GetClientDC(hWnd);
	if(itype <= B_NOTHING) {
		if(itype == B_NOTHING) {
			SetBrushColor(hdc,RGB2Pixel(HDC_SCREEN, 102, 102, 102));
			FillBox(hdc, x, y, iSideLength, iSideLength);
		}
		else {
			FillBoxWithBitmap(hdc, x, y, iSideLength, iSideLength, &bitmapAll[itype]);
        }

	}
	ReleaseDC(hdc);
}
