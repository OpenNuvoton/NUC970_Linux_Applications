#include "map.h"
#include "data.c"

ptagMap theMap; 

static ptagLevelCollection InitACollection(int * i);
static ptagLevel InitALevel(int *i); 
static int LineType(int i);
static inline int FindLineEnd(int j);
static inline void GotoNextLine(int *i);
static void DestroyCollection(ptagLevelCollection pCollection);
static void DestroyLevel(ptagLevel pLevel);
static void DestroyStep(void);
static BOOL FindPath(int * iGoRecord, int curx, int cury, int dx, int dy, int iRow, int iCol);

static int manGoInto[] = { B_SAVEMAN, B_FORBID, B_FORBID, B_FORBID, B_FORBID, B_FORBID, B_MAN };
static int manLeave[] = { B_FORBID, B_NOTHING, B_FORBID, B_GOAL, B_FORBID, B_FORBID, B_FORBID }; 
static int boxGoInto[] = { B_TREASURE, B_FORBID, B_FORBID, B_FORBID, B_FORBID, B_FORBID,B_OBJECT };
static int boxLeave[] = { B_FORBID, B_FORBID, B_NOTHING, B_FORBID, B_GOAL, B_FORBID,B_FORBID };

void InitMap(void) 
{
	int i = 0;
	int iLength = 0;
	ptagLevelCollection pCollection;
	iLength = sizeof(level_data_level);

	theMap = (ptagMap)calloc(1, sizeof(Map));

	theMap->iNoOfCollection = 0;
	theMap->head = NULL;
	theMap->tail = NULL;
	theMap->currentLevel = NULL;
	while( i < iLength ) {
		if (LineType(i) == LINE_TYPE_COLLECTION_BEGIN) {
			pCollection = InitACollection(&i);
			//Insert the struct in the doube list.
			if (theMap->head == NULL) {
				theMap->head = pCollection;
				theMap->tail = theMap->head;
				pCollection->next = pCollection;
				pCollection->prev = pCollection;
			} else {
				pCollection->next = theMap->tail->next;
				pCollection->next->prev = pCollection;
				pCollection->prev = theMap->tail;
				theMap->tail->next = pCollection;
				theMap->tail = pCollection;
			}
			theMap->iNoOfCollection++;
		} else 
			GotoNextLine(&i);
	}
	theMap->current = theMap->head;
	for (i = 0; i < BACK_STEP_SIZE; i++)
		theMap->pSteps[i] = NULL;
	PlayRestart();
}

//align at alphabit
static ptagLevelCollection InitACollection(int * i)
{
	ptagLevelCollection pCollection;
	char * name;
	int  iNameLength = 0;
	int  j = *i;
	char a;
	ptagLevel pLevel;
	int iLevel = 1;
	int iType;

	pCollection = (ptagLevelCollection)calloc(1, sizeof(LevelCollection));
	pCollection->iNoOfLevels = 0;
	pCollection->strName = (char *)calloc(MAX_COLLECTION_NAME, sizeof(char));
	name = pCollection->strName;
	pCollection->head = NULL;
	pCollection->tail = NULL;

	assert (LineType(j) == LINE_TYPE_COLLECTION_BEGIN); 

	//first get the name
	do {
		a = (char)level_data_level[j];
		if((isalpha(a)) || (a == '_')){
			name[iNameLength++] = a;
			j++;
		} 
		else
			break;	
	} while(1);

	GotoNextLine(&j);
	name[iNameLength] = '\0';


	while(1) {
		iType = LineType(j);
		if (iType == LINE_TYPE_COLLECTION_BEGIN) 
			break; //finished this collection
		
		if (iType == LINE_TYPE_LEVEL_BEGIN){  	
			pLevel = InitALevel(&j);
			pLevel->iNo = iLevel++;
			if (pCollection->head == NULL) {
				pCollection->head = pLevel;
				pCollection->tail = pCollection->head;
				pLevel->next = pLevel;
				pLevel->prev = pLevel;
			} else {
				pLevel->next = pCollection->tail->next;
				pLevel->next->prev = pLevel;
				pLevel->prev = pCollection->tail;
				pCollection->tail->next = pLevel;
				pCollection->tail = pLevel;
			}
			pCollection->iNoOfLevels++;
		} else
			GotoNextLine(&j);
	}	
	*i = j;
	pCollection->current = pCollection->head;
	return pCollection;
}

//we begin at the begin of a line
static ptagLevel InitALevel(int *i) 
{
	ptagLevel pLevel;
	int j = *i;
	int iLineBegin = 0;
	int iLineEnd = 0;
	int iRow = 0;
	int iCol = 0;
	char a;
	int *pInt;
	int k;
	int b;

	assert (LineType(j) == LINE_TYPE_LEVEL_BEGIN); 

	//first decide how many line rows
	iLineBegin = j;
	while(1) {
		//we are now at the begin of the line
		
		a = (char)level_data_level[j];
		while (a != 0x0a) {
			if (a == '#')
				iLineEnd = j;
			a = (char)level_data_level[++j];
		}
		
		if (iLineEnd == 0) //There are no '#'s in this line.
			break;

		if (iLineEnd -iLineBegin + 1 > iCol)
			iCol = iLineEnd - iLineBegin + 1;

		iRow ++; 
		j++; 
		iLineEnd = 0;
		iLineBegin = j;
	}


	//Init a structure
	pLevel = (ptagLevel)calloc(1, sizeof(Level));
	pLevel->row = iRow;
	pLevel->col = iCol;
	pLevel->data = (int *)calloc(iRow*iCol, sizeof(int));
	pInt = pLevel->data;
	
	//set value to pLevel->data	
	j = *i;

	for(iRow = 0; iRow < pLevel->row; iRow++)
	{
		iLineEnd = FindLineEnd(j);
		for(k = 0; k <= iLineEnd; k++) 
		{
			b = level_data_level[j++];
			switch (b) {
			case '@':
				pLevel->manx = k;
				pLevel->many = iRow;	
				pInt[iRow*iCol+k] = B_MAN;
				break;
			case '$':
				pInt[iRow*iCol+k] = B_OBJECT;
				break;
			case '.':
				pInt[iRow*iCol+k] = B_GOAL;
				break;
			case '*':
				pInt[iRow*iCol+k] = B_TREASURE;
				break;
			case '+':
				pLevel->manx = k;
				pLevel->many = iRow;	
				pInt[iRow*iCol+k] = B_SAVEMAN;
				break;
			case '#':
				pInt[iRow*iCol+k] = B_STONE;
				break;
			default:
				pInt[iRow*iCol+k] = B_NOTHING;
			}
		}
		for ( ; k < iCol; k++)
			pInt[iRow*iCol+k] = B_NOTHING;
		GotoNextLine(&j);
	}
	
	*i = j;
	return pLevel;
}

static int LineType(int i)
{
	char a;
	a = (char)level_data_level[i++];
	while(a != 0x0a) {
		if (a == '#') return LINE_TYPE_LEVEL_BEGIN;
		if (isalpha(a)) return LINE_TYPE_COLLECTION_BEGIN;
		a = (char)level_data_level[i++];
	}
	return LINE_TYPE_OTHER;
}

static inline int FindLineEnd(int j)
{
	int iLineEnd = 0;
	int i = 0;
	char a;

	do {
		a = (char)level_data_level[j++];
		if (a == '#')
			iLineEnd = i;
		i ++;
	} while (a != 0x0a);

	assert(iLineEnd);
	return iLineEnd;
}
static inline void GotoNextLine(int *i)
{
	int j = *i;
	int a;
	do {
		a = level_data_level[j++];
	} while (a != 0x0a);
	*i = j;
}

int GotoCollection(ptagLevelCollection pColl)
{
	if (theMap->current == pColl)
		return 0;
	theMap->current = pColl;
	PlayRestart();
	return 1;
}
//===============================================================================
//Destroy Functions:

void DestroyMap(void)
{
	ptagLevelCollection pCollection1;
	ptagLevelCollection pCollection2;
	ptagLevelCollection pCollection3;

	pCollection1 = theMap->head;
	if (pCollection1) {
		pCollection2 = pCollection1->next;
		while ((pCollection2) && (pCollection2 != pCollection1)) {
			pCollection3 = pCollection2;
			DestroyCollection(pCollection3);
			pCollection2 = pCollection2->next;
		}
		DestroyCollection(pCollection1);
	}
	DestroyLevel(theMap->currentLevel);
	DestroyStep();
	free(theMap);
}

static void DestroyCollection(ptagLevelCollection pCollection)
{
	ptagLevel pLevel1;
	ptagLevel pLevel2;
	ptagLevel pLevel3;

	pLevel1 = pCollection->head;
	if (pLevel1) {
		pLevel2 = pLevel1->next;
		while ((pLevel2) && (pLevel2 != pLevel1)) {
			pLevel3 = pLevel2;
			DestroyLevel(pLevel3);
			pLevel2 = pLevel2->next;
		}
		DestroyLevel(pLevel1);
	}
	free(pCollection);
	
}

static void DestroyLevel(ptagLevel pLevel)
{
	if (!pLevel)
		return;
	free(pLevel->data);
	free(pLevel);
}

static void DestroyStep(void)
{
	int i;
	ptagStep * pSteps;

	pSteps = theMap->pSteps;

	for (i = 0; i < BACK_STEP_SIZE; i++){
		if (pSteps[i]) {
			free(pSteps[i]);
			pSteps[i] = NULL;
		}
	}
	theMap->shead = -1;
}
//===============================================================================
//Play Functions:
int PlayMove(HWND hwnd, int dx, int dy) 
{
	
	ptagLevel cur_level;
	int curx,cury;
	int iRow, iCol;	
	int *iGoRecord;
	int a;
	BOOL bRet;
	int curbefore,curafter;
	int nextbefore,nextafter;
	ptagStep pAStep;

	cur_level = theMap->currentLevel;
	iCol = cur_level->col;

	a = cur_level->data[dy * iCol + dx];
	if ((a != B_GOAL) && (a != B_NOTHING))
		return 0;
	
	iRow = cur_level->row;
	curx = cur_level->manx;
	cury = cur_level->many;
	iGoRecord = (int *) calloc (iRow * iCol, sizeof(int));
	memcpy(iGoRecord, cur_level->data, iRow * iCol * sizeof(int));
	bRet = FindPath(iGoRecord, curx, cury, dx, dy, iRow, iCol);
	free(iGoRecord);
	if (!bRet) 
		return 0;
	
	curbefore = cur_level->data[cury  * iCol + curx];
	curafter  = manLeave[curbefore];
	nextbefore = cur_level->data[dy * iCol + dx];
	nextafter  = manGoInto[nextbefore];
	assert(curafter != B_FORBID);
	assert(nextafter != B_FORBID);
	//for UNDO
	pAStep = (ptagStep) calloc (1, sizeof(Step));
	pAStep->iType  = STEP_TYPE_MOVE;
	pAStep->pt1[0] = curx;
	pAStep->pt1[1] = cury;
	pAStep->pt1[2] = curbefore;
	pAStep->pt2[0] = dx;
	pAStep->pt2[1] = dy;
	pAStep->pt2[2] = nextbefore;
	PushStep(pAStep);
	//act
	cur_level->data[cury  * iCol + curx]  = curafter;
	cur_level->data[dy * iCol + dx] = nextafter;
	DrawALittleBlock(hwnd, curx, cury, curafter);	
	DrawALittleBlock(hwnd, dx, dy, nextafter);	

	cur_level->manx = dx;
	cur_level->many = dy;	

	return STEP_TYPE_MOVE;
}

static BOOL FindPath(int * iGoRecord, int curx, int cury, int dx, int dy, int iRow, int iCol)
{
	int nextx, nexty;
	int a;
	
	if ((curx == dx) && (cury == dy))
		return TRUE;

	//try go right;
	nextx = curx + 1;
	nexty = cury;
	if (nextx < iCol) {
		a = iGoRecord[nexty * iCol + nextx];
		if ((a == B_NOTHING) || (a==B_GOAL)) {
			iGoRecord[nexty *iCol + nextx] = B_MAN;
			if(FindPath(iGoRecord, nextx, nexty, dx, dy, iRow, iCol))
				return TRUE;

			iGoRecord[nexty *iCol + nextx] = B_NOTHING;
		}
	}
	//try go left;
	nextx = curx - 1;
	nexty = cury;
	if (nextx >= 0) {
		a = iGoRecord[nexty * iCol + nextx];
		if ((a == B_NOTHING) || (a==B_GOAL)) {
			iGoRecord[nexty *iCol + nextx] = B_MAN;
			if(FindPath(iGoRecord, nextx, nexty, dx, dy, iRow, iCol))
				return TRUE;
			iGoRecord[nexty *iCol + nextx] = B_NOTHING;
		}
	}
	//try go down;
	nextx = curx;
	nexty = cury + 1;
	if (nexty < iRow) {
		a = iGoRecord[nexty * iCol + nextx];
		if ((a == B_NOTHING) || (a==B_GOAL)) {
			iGoRecord[nexty *iCol + nextx] = B_MAN;
			if(FindPath(iGoRecord, nextx, nexty, dx, dy, iRow, iCol))
				return TRUE;
			iGoRecord[nexty *iCol + nextx] = B_NOTHING;
		}
	}
	//try go up;
	nextx = curx;
	nexty = cury - 1;
	if (nexty >= 0) {
		a = iGoRecord[nexty * iCol + nextx];
		if ((a == B_NOTHING) || (a==B_GOAL)) {
			iGoRecord[nexty *iCol + nextx] = B_MAN;
			if(FindPath(iGoRecord, nextx, nexty, dx, dy, iRow, iCol))
				return TRUE;
			iGoRecord[nexty *iCol + nextx] = B_NOTHING;
		}
	}
	return FALSE;

}
int PlayKeyboard(HWND hwnd, int iDir)
{
	int curx,cury;
	int nextx,nexty;
	int farx,fary;
	int curbefore,curafter;
	int nextbefore,nextafter;
	int farbefore, farafter;
	int iRow, iCol;	
	
	ptagStep pAStep;

	ptagLevel cur_level;
	cur_level = theMap->currentLevel;
	iRow = cur_level->row;
	iCol = cur_level->col;

	curx = cur_level->manx;
	cury = cur_level->many;

	switch (iDir) {
		case DIR_UP:
			nextx = curx;
			nexty = cury - 1;
			farx = nextx;
			fary = nexty - 1;
			break;
		case DIR_DOWN:
			nextx = curx;
			nexty = cury + 1;
			farx = nextx;
			fary = nexty + 1;
			break;

		case DIR_LEFT:
			nextx = curx - 1;
			nexty = cury;
			farx = nextx - 1;
			fary = nexty;
			break;

		case DIR_RIGHT:
			nextx = curx + 1;
			nexty = cury;
			farx = nextx + 1;
			fary = nexty;
			break;
		default:
			return 0;
	}
	curbefore = cur_level->data[cury  * iCol + curx];
	curafter  = manLeave[curbefore];
	nextbefore = cur_level->data[nexty * iCol + nextx];
	nextafter  = manGoInto[nextbefore];
	if (nextafter != B_FORBID) {//move to
		assert(curafter != B_FORBID);

		//for UNDO
		pAStep = (ptagStep) calloc (1, sizeof(Step));
		pAStep->iType  = STEP_TYPE_MOVE;
		pAStep->pt1[0] = curx;
		pAStep->pt1[1] = cury;
		pAStep->pt1[2] = curbefore;
		pAStep->pt2[0] = nextx;
		pAStep->pt2[1] = nexty;
		pAStep->pt2[2] = nextbefore;
		PushStep(pAStep);
		//act
		cur_level->data[cury  * iCol + curx]  = curafter;
		cur_level->data[nexty * iCol + nextx] = nextafter;
		DrawALittleBlock(hwnd, curx, cury, curafter);	
		DrawALittleBlock(hwnd, nextx, nexty, nextafter);	

		cur_level->manx = nextx;
		cur_level->many = nexty;	
		return STEP_TYPE_MOVE;
	}
	if ((nextbefore == B_OBJECT) || (nextbefore == B_TREASURE))//try pull box
	{
		farbefore = cur_level->data[fary * iCol + farx];
		farafter  = boxGoInto[farbefore];
		
		if (farafter == B_FORBID)
			return 0;
		nextafter = boxLeave[nextbefore];
		assert(nextafter != B_FORBID);
		nextafter = manGoInto[nextafter];
		assert(nextafter != B_FORBID);

		//for UNDO
		pAStep = (ptagStep) calloc (1, sizeof(Step));
		pAStep->iType  = STEP_TYPE_PUSH;
		pAStep->pt1[0] = curx;
		pAStep->pt1[1] = cury;
		pAStep->pt1[2] = curbefore;
		pAStep->pt2[0] = nextx;
		pAStep->pt2[1] = nexty;
		pAStep->pt2[2] = nextbefore;
		pAStep->pt3[0] = farx;
		pAStep->pt3[1] = fary;
		pAStep->pt3[2] = farbefore;
		PushStep(pAStep);
		//act
		cur_level->data[cury  * iCol + curx]  = curafter;
		cur_level->data[nexty * iCol + nextx] = nextafter;
		cur_level->data[fary * iCol + farx] = farafter;

		DrawALittleBlock(hwnd, curx, cury, curafter);	
		DrawALittleBlock(hwnd, nextx, nexty, nextafter);	
		DrawALittleBlock(hwnd, farx, fary, farafter);	
		
		cur_level->manx = nextx;
		cur_level->many = nexty;	
		return STEP_TYPE_PUSH;
	}
	return 0;
}

ptagStep PopStep(void) 
{
	ptagStep pStep1;
	int i = theMap->shead;
	if (i == -1) 
		return NULL;

	pStep1 = theMap->pSteps[i];
	theMap->pSteps[i] = NULL;
	i --;
	if ((i < 0) && (theMap->pSteps[BACK_STEP_SIZE - 1]))
		i = BACK_STEP_SIZE - 1;

	theMap->shead = i;
	return pStep1;
}

void PushStep(ptagStep pStep)
{
	theMap->shead++;
	theMap->shead %= 50;
	
	if(theMap->pSteps[theMap->shead])
		free(theMap->pSteps[theMap->shead]);
	theMap->pSteps[theMap->shead] = pStep;
}

void PlayRestart(void) 
{	
	ptagLevel pLevel1, pLevel2;

	DestroyLevel(theMap->currentLevel);
	DestroyStep();
	//duplcate the current level
	pLevel1 = theMap->current->current;
	pLevel2 = (ptagLevel)calloc(1, sizeof(Level));
	pLevel2->iNo = pLevel1->iNo;
	pLevel2->row = pLevel1->row;
	pLevel2->col = pLevel1->col;
	pLevel2->manx = pLevel1->manx;
	pLevel2->many = pLevel1->many;
	pLevel2->next = pLevel1->next;
	pLevel2->prev = pLevel1->prev;
	pLevel2->data = (int *)calloc(pLevel2->row*pLevel2->col, sizeof(int));
	memcpy(pLevel2->data, pLevel1->data, pLevel2->row * pLevel2->col * sizeof(int));
	theMap->currentLevel = pLevel2;
	//
}

BOOL CheckMissionComplete(void)
{
	int i,j;
	int iRow, iCol;	
	int a;
		
	ptagLevel cur_level;
	cur_level = theMap->currentLevel;
	iRow = cur_level->row;
	iCol = cur_level->col;

	for (i = 0; i < iRow; i++)
		for (j = 0; j < iCol; j++) {
			a = cur_level->data[i*iCol+j]; 
			if ( (a == B_OBJECT) || (a == B_GOAL))
				return FALSE;

		}
	return TRUE;
}

int PlayUndo(HWND hwnd)
{
	ptagStep pAStep;
	ptagLevel cur_level;
	int itype;
	int x, y, value;
	int iRow, iCol;
	
	pAStep = PopStep();
	if (!pAStep)
		return -1;

	itype = pAStep->iType;
	assert((itype == STEP_TYPE_PUSH) || (itype == STEP_TYPE_MOVE));

	cur_level = theMap->currentLevel;
	iCol = cur_level->col;
	iRow = cur_level->row;

	x = pAStep->pt1[0];
	y = pAStep->pt1[1];
	value = pAStep->pt1[2];
	cur_level->data[y * iCol + x] = value;
	DrawALittleBlock(hwnd,x, y, value);	
	cur_level->manx = x;
	cur_level->many = y;

	x = pAStep->pt2[0];
	y = pAStep->pt2[1];
	value = pAStep->pt2[2];
	cur_level->data[y * iCol + x] = value;
	DrawALittleBlock(hwnd,x, y, value);	

	if(itype == STEP_TYPE_PUSH) {
		x = pAStep->pt3[0];
		y = pAStep->pt3[1];
		value = pAStep->pt3[2];
		cur_level->data[y * iCol + x] = value;
		DrawALittleBlock(hwnd,x, y, value);	
	}
	free(pAStep);
	return 0;
}

