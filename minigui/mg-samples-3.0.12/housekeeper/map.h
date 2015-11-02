#ifndef __MAP_H_
#define __MAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pwd.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <syslog.h>
#include <assert.h>
#include <fcntl.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

/*
 *  $		the box		B_OBJECT
 *  .		place		B_GOAL
 *  @		man		B_MAN
 *  *		box + place	B_TREASURE
 *  +		man + place	B_SAVEMAN
 *  #		wall		B_STONE
 */

#define B_FORBID -1
#define B_GOAL 	0
#define B_MAN	1
#define B_OBJECT 2 
#define B_SAVEMAN 3
#define B_TREASURE 4
#define B_STONE 5
#define B_NOTHING 6


#define MAX_COLLECTION_NAME 20

#define LINE_TYPE_COLLECTION_BEGIN 100
#define LINE_TYPE_LEVEL_BEGIN 200
#define LINE_TYPE_OTHER 300

#define STEP_TYPE_MOVE 100
#define STEP_TYPE_PUSH 200

#define DIR_UP  101
#define DIR_DOWN  201
#define DIR_LEFT 301
#define DIR_RIGHT 401

#define BACK_STEP_SIZE 50
typedef struct tagLevel{
	int iNo;
	int col;
	int row;
	int manx;
	int many;
	int *data;
	struct tagLevel *next;
	struct tagLevel *prev;
} Level;
typedef Level* ptagLevel;

typedef struct tagStep{
	int iType;
	int pt1[3];
	int pt2[3];
	int pt3[3]; 
} Step;
typedef Step* ptagStep;

typedef struct tagLevelCollection{
	int iNoOfLevels;
	char *strName;
	struct tagLevelCollection *next;
	struct tagLevelCollection *prev;
	ptagLevel current;
	ptagLevel head;
	ptagLevel tail;
} LevelCollection;
typedef LevelCollection * ptagLevelCollection;


typedef struct tagMap{
	int iNoOfCollection;
	ptagLevel currentLevel;
	ptagLevelCollection current;
	ptagLevelCollection head;
	ptagLevelCollection tail;
	ptagStep pSteps[BACK_STEP_SIZE];
	int shead;
} Map;
typedef Map* ptagMap;

void CovertCoord(HWND hWnd,int *px, int *py);
BOOL ptChoosePlace(HWND hWnd,int *px, int *py);
void DrawALittleBlock(HWND hWnd, int x, int y, int itype);	
//===============================================================================
//Init Functions:
void InitMap(void);
int GotoCollection(ptagLevelCollection pColl);
//===============================================================================
//Destroy Functions:
void DestroyMap(void);
extern ptagMap theMap; 
//===============================================================================
//Play Functions:
int PlayMove(HWND hwnd, int x, int y);
int PlayKeyboard(HWND hwnd, int iDir);
int PlayUndo(HWND hwnd);
void PlayRestart(void); 
ptagStep PopStep(void);
void PushStep(ptagStep pStep);
BOOL CheckMissionComplete(void);
#endif // __MAP_H_
