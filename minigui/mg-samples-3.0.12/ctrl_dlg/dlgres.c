/*
** $Id: dlgres.c 549 2008-02-29 08:10:52Z malin $
**
** dlgres.c: The dialog box templates.
**
** Copyright (C) 2001 ~ 2002 Wei Yongming.
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

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "resouce.h"

#ifdef _LANG_ZHCN
#include "dlgdemo_res_cn.h"
#elif defined _LANG_ZHTW
#include "dlgdemo_res_tw.h"
#else
#include "dlgdemo_res_en.h"
#endif

// 初始化进程对话框
DLGTEMPLATE DlgInitProgress =
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    120, 150, 400, 130, 
    initializing,
    0, 0,
    3, NULL,
    0
};

CTRLDATA CtrlInitProgress [] =
{ 
    {
        "static",
        WS_VISIBLE | SS_SIMPLE,
        10, 10, 380, 16, 
        IDC_PROMPTINFO, 
        doing,
        0
    },
    {
        CTRL_PROGRESSBAR,
        WS_VISIBLE,
        10, 40, 380, 20,
        IDC_PROGRESS,
        NULL,
        0
    },
    {
        "button",
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON, 
        170, 70, 60, 25,
        IDOK, 
        sure,
        0
    }
};

DLGTEMPLATE DlgStartupMode =
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    120, 100, 480, 334,
    please_select_mode,
    0, 0,
    13, NULL,
    0
};

CTRLDATA CtrlStartupMode[] =
{ 
    {
        "static",
        WS_VISIBLE | SS_GROUPBOX, 
        16, 10, 200, 160,
        IDC_STATIC,
        mode,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON | WS_TABSTOP | WS_GROUP,
        30, 38, 180, 20,
        IDC_MODE_MDI,
        mode_mdi,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON, 
        30, 64, 180, 20, 
        IDC_MODE_APP, 
        mode_bg,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        30, 90, 180, 20,
        IDC_MODE_IIIE,
        mode_moving,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON | WS_DISABLED,
        30, 116, 180, 20,
        IDC_MODE_SETTINGS,
        mode_param,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTORADIOBUTTON | WS_DISABLED,
        30, 142, 180, 20,
        IDC_MODE_PLC,
        mode_helper,
        0
    },
    {
        "combobox",
        WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP | CBS_READONLY,
        240, 10, 184, 24,
        IDC_BOX1,
        combox,
        0
    },
    {
        "combobox",
        WS_VISIBLE | CBS_SPINLIST | WS_TABSTOP,
        240, 40, 184, 24,
        IDC_BOX2,
        combox,
        0
    },
    {
        "combobox",
        WS_VISIBLE | CBS_AUTOSPIN | WS_TABSTOP,
        240, 70, 184, 24,
        IDC_BOX3,
        "0",
        0
    },
    {
        "combobox",
        WS_VISIBLE | CBS_SIMPLE | WS_TABSTOP,
        240, 100, 184, 24,
        IDC_BOX4,
        "0",
        100
    },
    {
        "button",
        WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP,
        172, 242, 95, 28,
        IDOK, 
        sure,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        283, 242, 95, 28,
        IDCANCEL,
        Exit,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_LEFT | WS_GROUP,
        18, 280, 360, 16,
        IDC_PROMPTINFO,
        prompt,
        0
    }
};

DLGTEMPLATE DlgStructParams = 
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    70, 50, 550, 320,
    machine_setting,
    0, 0,
    26, NULL
};

CTRLDATA CtrlStructParams[] = 
{
    {
        "static",
        WS_VISIBLE | SS_RIGHT,
        4, 14, 280, 22, 
        IDC_STATIC, 
        fixpla_top_radius,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        295, 14, 103, 22,
        IDC_EDIT1,
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT, 
        5, 36, 280, 22,
        IDC_STATIC, 
        fixpla_bot_radius,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        295, 36, 103, 22,
        IDC_EDIT2,
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT, 
        5, 58, 280, 22, 
        IDC_STATIC, 
        fixpla_height,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP, 
        295, 58, 103, 22, 
        IDC_EDIT3,
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT, 
        5, 80, 280, 22, 
        IDC_STATIC, 
        fixpla_angle,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP, 
        295, 80, 103, 22, 
        IDC_EDIT4, 
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT, 
        5, 102, 280, 22, 
        IDC_STATIC, 
        movepla_top_radius,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP, 
        295, 102, 103, 22, 
        IDC_EDIT5, 
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT, 
        5, 124, 280, 22, 
        IDC_STATIC,
        movepla_bot_radius,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        295, 124, 103, 22, 
        IDC_EDIT6, 
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT, 
        5, 146, 280, 22, 
        IDC_STATIC, 
        movepla_height,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        295, 146, 103, 22,
        IDC_EDIT7,
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT,
        5, 168, 280, 22,
        IDC_STATIC,
        movepla_angle,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        295, 168, 103, 22,
        IDC_EDIT8, 
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT, 
        5, 190, 280, 22, 
        IDC_STATIC,
        platform_init_diff,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        295, 190, 103, 22,
        IDC_EDIT9, 
        NULL, 
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT,
        5, 218, 280, 22, 
        IDC_STATIC,
        min_leg,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        295, 218, 103, 22, 
        IDC_EDITA, 
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT, 
        5, 242, 280, 22, 
        IDC_STATIC, 
        max_leg,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        295, 242, 103, 22,
        IDC_EDITB,
        NULL,
        0
    },
    {
        "static",
        WS_VISIBLE | SS_RIGHT,
        5, 266, 280, 22,
        IDC_STATIC,
        fixpla_distance,
        0
    },
    {
        "edit",
        WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        295, 266, 103, 22,
        IDC_EDITC,
        NULL,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
        410, 14, 100, 28,
        IDOK, 
        sure,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        410, 58, 100, 28, 
        IDCANCEL, 
        cancel,
        0
    }
};
      
DLGTEMPLATE DlgPassword = 
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    70, 80, 460, 158,
    password_setting,
    0, 0,
    5, NULL,
    0
};

CTRLDATA CtrlPassword [] = 
{
    {
        "static",
        WS_VISIBLE | SS_RIGHT,
        14, 30, 150, 18,
        IDC_STATIC, 
        enter_new_pwd,
        0
    },
    {
        "edit",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD | WS_TABSTOP, 
        180, 30, 200, 24, 
        IDC_NEWPASSWORD, 
        NULL,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP,
        180, 70, 140, 22, 
        IDC_PASSWORDVALID, 
        pwd_valid,
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 
        80, 94, 100, 28, 
        IDOK, 
        sure,
        0 
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        276, 94, 100, 28, 
        IDCANCEL, 
        cancel,
        0
    }
};
 
DLGTEMPLATE DlgPropertySheet = 
{
    WS_BORDER | WS_CAPTION,
    WS_EX_NONE,
    20, 20, 580, 450,
    property_caption,
    0, 0,
    4, NULL,
    0
};

CTRLDATA CtrlPropertySheet [] = 
{
    {
        CTRL_PROPSHEET,
        WS_VISIBLE | WS_TABSTOP | PSS_COMPACTTAB,
        10, 10, 560, 360,
        IDC_PROPSHEET, 
        "Property sheet control",
        0
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 
        200, 380, 100, 28, 
        IDC_APPLY, 
        apply,
        0 
    },
    {
        "button",
        WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP, 
        340, 380, 100, 28, 
        IDOK, 
        sure,
        0 
    },
    {
        "button",
        WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        460, 380, 100, 28, 
        IDCANCEL, 
        cancel,
        0
    }
};
 
