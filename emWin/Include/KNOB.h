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
File        : KNOB.h
Purpose     : KNOB include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef KNOB_H
#define KNOB_H

#include "WM.h"
#include "DIALOG_Type.h"      // Req. for Create indirect data structure
#include "WIDGET.h"
#include "GUI_Debug.h"

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
typedef WM_HMEM KNOB_Handle;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/
KNOB_Handle KNOB_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int Id);
KNOB_Handle KNOB_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int Id, int NumExtraBytes);
KNOB_Handle KNOB_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void KNOB_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/
void KNOB_AddValue     (KNOB_Handle hObj, I32 Value);
int  KNOB_GetUserData  (KNOB_Handle hObj, void * pDest, int NumBytes);               // Gets user defined data
I32  KNOB_GetValue     (KNOB_Handle hObj);                                           // Returns the position of the knob in tenth of degree
int  KNOB_OwnerDraw    (const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
void KNOB_SetBkColor   (KNOB_Handle hObj, GUI_COLOR Color);                          // Sets the BK color of the widget
void KNOB_SetBkDevice  (KNOB_Handle hObj, GUI_MEMDEV_Handle hMemBk);                 // Sets the background device
void KNOB_SetDevice    (KNOB_Handle hObj, GUI_MEMDEV_Handle hMemSrc);                // Sets the memory device with the drawn knob
void KNOB_SetInvert    (KNOB_Handle hObj, U8 Invert);                                // Inverts value of the knob widget
void KNOB_SetKeyValue  (KNOB_Handle hObj, I32 KeyValue);                             // Sets the value the knob will rotate on one key press
void KNOB_SetOffset    (KNOB_Handle hObj, I32 Offset);                               // Sets the offset angle of the knob
void KNOB_SetOwnerDraw (KNOB_Handle hObj, WIDGET_DRAW_ITEM_FUNC * pfOwnerDraw);
void KNOB_SetPeriod    (KNOB_Handle hObj, I32 Period);                               // Sets the period in which the knob stops
void KNOB_SetPos       (KNOB_Handle hObj, I32 Pos);                                  // Sets position of the knob in tenth of degree
void KNOB_SetRange     (KNOB_Handle hObj, I32 MinRange, I32 MaxRange);               // Sets the usable range of the knob widget
void KNOB_SetSnap      (KNOB_Handle hObj, I32 Snap);                                 // Sets Snap positions where the knob stops
void KNOB_SetRotateHQ  (KNOB_Handle hObj);
void KNOB_SetRotateLQ  (KNOB_Handle hObj);
void KNOB_SetTickSize  (KNOB_Handle hObj, I32 TickSize);                             // Sets the ticksize, in 1/10 of degree
int  KNOB_SetUserData  (KNOB_Handle hObj, const void * pSrc, int NumBytes);          // Sets user defined data
void KNOB_SetValueRange(KNOB_Handle hObj, I32 Min, I32 Max);

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
#endif  // KNOB_H

/*************************** End of file ****************************/
