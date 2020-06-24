/*********************************************************************
*                 SEGGER Software GmbH                               *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.10 - Graphical user interface for embedded applications **
All  Intellectual Property rights in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with the following terms:

The  software has  been licensed by SEGGER Software GmbH to Nuvoton Technology Corporationat the address: No. 4, Creation Rd. III, Hsinchu Science Park, Taiwan
for the purposes  of  creating  libraries  for its 
Arm Cortex-M and  Arm9 32-bit microcontrollers, commercialized and distributed by Nuvoton Technology Corporation
under  the terms and conditions  of  an  End  User  
License  Agreement  supplied  with  the libraries.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              Nuvoton Technology Corporation, No. 4, Creation Rd. III, Hsinchu Science Park, 30077 Hsinchu City, Taiwan
Licensed SEGGER software: emWin
License number:           GUI-00735
License model:            emWin License Agreement, signed February 27, 2018
Licensed platform:        Cortex-M and ARM9 32-bit series microcontroller designed and manufactured by or for Nuvoton Technology Corporation
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2018-03-26 - 2020-03-27
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : ROTARY.h
Purpose     : ROTARY include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef ROTARY_H
#define ROTARY_H

#include "WM.h"
#include "DIALOG_Type.h"      // Req. for Create indirect data structure
#include "WIDGET.h"

#if (GUI_SUPPORT_MEMDEV && GUI_WINSUPPORT)

#if defined(__cplusplus)
  extern "C" {                  // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Public Types
*
**********************************************************************
*/
typedef WM_HMEM ROTARY_Handle;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/
ROTARY_Handle ROTARY_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int Id);
ROTARY_Handle ROTARY_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int Id, int NumExtraBytes);
ROTARY_Handle ROTARY_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void ROTARY_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/
void ROTARY_AddValue     (ROTARY_Handle hObj, I32 Delta);
void ROTARY_AddAngle     (ROTARY_Handle hObj, I32 Delta);
I32  ROTARY_GetAngle     (ROTARY_Handle hObj);
int  ROTARY_GetImageSize (ROTARY_Handle hObj, int * pxSize, int * pySize);
int  ROTARY_GetMarkerSize(ROTARY_Handle hObj, int * pxSize, int * pySize);
int  ROTARY_GetUserData  (ROTARY_Handle hObj, void * pDest, int NumBytes);
I32  ROTARY_GetValue     (ROTARY_Handle hObj);
void ROTARY_SetAngle     (ROTARY_Handle hObj, I32 Pos);
void ROTARY_SetBitmap    (ROTARY_Handle hObj, const GUI_BITMAP * pBitmap);
void ROTARY_SetBitmapObj (ROTARY_Handle hObj, GUI_DRAW_HANDLE hDrawObj);
void ROTARY_SetDoRotate  (ROTARY_Handle hObj, U8 DoRotate);
void ROTARY_SetMarker    (ROTARY_Handle hObj, const GUI_BITMAP * pBitmap, int Radius, I32 AngleOffset, U8 DoRotate);
void ROTARY_SetMarkerObj (ROTARY_Handle hObj, GUI_DRAW_HANDLE hDrawObjMarker, GUI_DRAW_HANDLE hDrawObjMarkerHR, int Radius, I32 Offset, U8 DoRotate);
void ROTARY_SetOffset    (ROTARY_Handle hObj, int Offset);
void ROTARY_SetPeriod    (ROTARY_Handle hObj, I32 Period);
void ROTARY_SetRadius    (ROTARY_Handle hObj, int Radius);
void ROTARY_SetRange     (ROTARY_Handle hObj, U32 AngPositiv, U32 AngNegativ);
void ROTARY_SetSnap      (ROTARY_Handle hObj, I32 Snap);
void ROTARY_SetTickSize  (ROTARY_Handle hObj, I32 TickSize);
int  ROTARY_SetUserData  (ROTARY_Handle hObj, const void * pSrc, int NumBytes);
void ROTARY_SetValue     (ROTARY_Handle hObj, I32 Value);
int  ROTARY_SetValueRange(ROTARY_Handle hObj, I32 Min, I32 Max);

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

#if defined(__cplusplus)
  }
#endif

#endif  // (GUI_SUPPORT_MEMDEV && GUI_WINSUPPORT)
#endif  // ROTARY_H

/*************************** End of file ****************************/
