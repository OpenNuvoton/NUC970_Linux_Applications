/*********************************************************************
*                 SEGGER Software GmbH                               *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2018  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.48 - Graphical user interface for embedded applications **
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
SUA period:               2018-03-26 - 2019-03-27
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include <stddef.h>
#include <stdio.h>
#include "GUI.h"
#include "GUIDRV_Lin.h"
//#include "GUIDRV_FlexColor.h"
//#include "N9H30TouchPanel.h"
#include "LCDConf.h"


/*********************************************************************
*
*       Layer configuration
*
**********************************************************************
*/
//
// Physical display size
//

//
// Color conversion
//
//#define COLOR_CONVERSION GUICC_M888

//
// Display driver
//
//#define DISPLAY_DRIVER GUIDRV_FLEXCOLOR
//#define DISPLAY_DRIVER GUIDRV_LIN_32
//
// Buffers / VScreens
//
#define NUM_BUFFERS  1 // Number of multiple buffers to be used
#define NUM_VSCREENS 1 // Number of virtual screens to be used

//
// Orientation
//
//#define DISPLAY_ORIENTATION (0)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_Y | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y | GUI_SWAP_XY)


/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
//#ifndef   VXSIZE_PHYS
//#define VXSIZE_PHYS XSIZE_PHYS
//#endif
//#ifndef   VYSIZE_PHYS
//#define VYSIZE_PHYS YSIZE_PHYS
//#endif
//#ifndef   XSIZE_PHYS
//#error Physical X size of display is not defined!
//#endif
//#ifndef   YSIZE_PHYS
//#error Physical Y size of display is not defined!
//#endif
//#ifndef   COLOR_CONVERSION
//#error Color conversion not defined!
//#endif
//#ifndef   DISPLAY_DRIVER
//#error No display driver defined!
//#endif
#ifndef   DISPLAY_ORIENTATION
#define DISPLAY_ORIENTATION 0
#endif

// Touch panel
//#define GUI_TOUCH_AD_LEFT   72
//#define GUI_TOUCH_AD_TOP    926  //82
//#define GUI_TOUCH_AD_RIGHT  938
//#define GUI_TOUCH_AD_BOTTOM 82  //926
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
extern unsigned char *pVideoBuffer;
//extern int ts_phy2log(int *sumx, int *sumy);

void GUI_TOUCH_X_ActivateX(void)
{
}

void GUI_TOUCH_X_ActivateY(void)
{
}



int  GUI_TOUCH_X_MeasureX(void)
{
#if 0
    int sumx;
    int sumy;
    if (Read_TouchPanel(&sumx, &sumy))
    {
        sysprintf("X = %d\n", sumx);
        ts_phy2log(&sumx, &sumy);
        return sumx;
    }
#endif	
    return -1;
}

int  GUI_TOUCH_X_MeasureY(void)
{
#if 0
    int sumx;
    int sumy;
    if ( Read_TouchPanel(&sumx, &sumy) )
    {
        sysprintf("Y = %d\n", sumy);
        ts_phy2log(&sumx, &sumy);
        return sumy;
    }
#endif	
    return -1;
}
/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
extern g_xres;
extern g_yres;
extern g_bits_per_pixel;

void LCD_X_Config(void)
{

    // LCD calibration
#if (NUM_BUFFERS > 1)
    GUI_MULTIBUF_Config(NUM_BUFFERS);
#endif
    //
    // Set display driver and color conversion for 1st layer
    //
    //printf("### g_xres=%d g_yres=%d g_bits_per_pixel=%d\n", g_xres, g_yres, g_bits_per_pixel);
    if (g_bits_per_pixel == 32)
    GUI_DEVICE_CreateAndLink(GUIDRV_LIN_32, GUICC_M888, 0, 0);
    else
    GUI_DEVICE_CreateAndLink(GUIDRV_LIN_16, GUICC_M565, 0, 0);
    if (LCD_GetSwapXY())
    {
        LCD_SetSizeEx (0, g_yres, g_xres);
        LCD_SetVSizeEx(0, g_yres * NUM_VSCREENS, g_xres);
    }
    else
    {
        LCD_SetSizeEx (0, g_xres, g_yres);
        LCD_SetVSizeEx(0, g_xres, g_yres * NUM_VSCREENS);
    }
    LCD_SetVRAMAddrEx(0, (void *)pVideoBuffer);

    //
    // Set user palette data (only required if no fixed palette is used)
    //
#if defined(PALETTE)
    LCD_SetLUTEx(0, PALETTE);
#endif

// LCD calibration
//
// Calibrate touch screen
//
    GUI_TOUCH_Calibrate(GUI_COORD_X, 0, g_xres, 0, g_xres);
    GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, g_yres, 0, g_yres);

}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData)
{
    int r;

    switch (Cmd)
    {
    //
    // Required
    //
    case LCD_X_INITCONTROLLER:
    {
        //
        // Called during the initialization process in order to set up the
        // display controller and put it into operation. If the display
        // controller is not initialized by any external routine this needs
        // to be adapted by the customer...
        //
//        LCD_ON();
        return 0;
    }
    case LCD_X_SETVRAMADDR:
    {
        //
        // Required for setting the address of the video RAM for drivers
        // with memory mapped video RAM which is passed in the 'pVRAM' element of p
        //
        LCD_X_SETVRAMADDR_INFO * p;
        p = (LCD_X_SETVRAMADDR_INFO *)pData;
        pData=(void*)pVideoBuffer;
        //...
        return 0;
    }
    case LCD_X_SETORG:
    {
        //
        // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
        //
        LCD_X_SETORG_INFO * p;
        p = (LCD_X_SETORG_INFO *)pData;
        pData=(void*)pVideoBuffer;
        //...
        return 0;
    }
    case LCD_X_SHOWBUFFER:
    {
        //
        // Required if multiple buffers are used. The 'Index' element of p contains the buffer index.
        //
        LCD_X_SHOWBUFFER_INFO * p;
        p = (LCD_X_SHOWBUFFER_INFO *)pData;
        //...
        return 0;
    }
    case LCD_X_SETLUTENTRY:
    {
        //
        // Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
        //
        LCD_X_SETLUTENTRY_INFO * p;
        p = (LCD_X_SETLUTENTRY_INFO *)pData;
        //...
        return 0;
    }
    case LCD_X_ON:
    {
        //
        // Required if the display controller should support switching on and off
        //

        return 0;
    }
    case LCD_X_OFF:
    {
        //
        // Required if the display controller should support switching on and off
        //
        // ...
        return 0;
    }
    default:
        r = -1;
    }
    return r;
}

/*************************** End of file ****************************/
