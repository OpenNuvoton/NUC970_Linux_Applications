/*
** $Id: mginit.c 558 2008-03-04 09:02:19Z wangjian $
**
** The mginit program of MiniGUI Demostration Envirionment.
**
** Copyright (C) 2001, 2002 Wei Yongming.
** Copyright (C) 2003 ~ 2007 Feynman Software.
**
** Create date: 2001/09/05
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
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <minigui/fixedmath.h>

#ifdef _MGRM_THREADS
    #error "*******************************************************************"
    #error "**** Your MiniGUI is configured as MiniGUI-Threads.            ****"
    #error "**** This program is a server of MiniGUI-Processes,            ****"
    #error "****   it can only run on MiniGUI-Processes,                   ****"
    #error "****   not on MiniGUI-Threads.                                 ****"
    #error "**** If you want to configure MiniGUI as MiniGUI-Processes,    ****"
    #error "****   please run `./configure --enable-procs'                 ****"
    #error "****   when configuring MiniGUI                                ****"
    #error "****   and build and reinstall MiniGUI.                        ****"
    #error "**** Note that this is not a fatal error.                      ****"
    #error "*******************************************************************"
#else

#include "taskbar.h"
#include "desktop.h"

static HWND hTaskBar;

/*
static void AboutMiniGUI (void)
{
    char buff [1024];

    sprintf (buff, 
            "MiniGUI version %d.%d.%d.\n"
            "Copyright (C) 2002 ~ 2006 Feynman Software.\n"
            "Copyright (C) 1998 ~ 2002 Wei Yongming.\n"
            "All right reserved.\n\n"
            "MiniGUI is \"a cross-operating-system graphics user interface "
            "support system for embedded devices\", and is \"an embedded "
            "graphics middleware\". It aims to provide a compact, fast, "
            "stable, lightweight, and cross-OS GUI support system, which "
            "is especially fit for real-time embedded systems based-on "
            "Linux/uClinux, eCos, uC/OS-II, and other traditional RTOSes, "
            "such as VxWorks, ThreadX, and Nucleus.\n\n"
            "Please visit\n\n"
            "http://www.minigui.org\n"
            "http://www.minigui.com\n\n"
            "for more information.",
            MINIGUI_MAJOR_VERSION, 
            MINIGUI_MINOR_VERSION, 
            MINIGUI_MICRO_VERSION);

    MessageBox (HWND_DESKTOP, buff, 
            "About MiniGUI!", MB_OK | MB_ICONINFORMATION);
}

static void AboutMDE (void)
{
    char buff [1024];

    sprintf (buff, 
            "MDE version %d.%d.%d.\n"
            "Copyright (C) 2002 ~ 2006 Feynman Software.\n"
            "Copyright (C) 2001 ~ 2002 Wei Yongming.\n\n"
            "MDE (MiniGUI Demostration Envirionment) is a comprehensive "
            "demo system for MiniGUI Ver %d.%d.%d. \n\n"
            "It creates a task bar for you, and you can launch applications "
            "from the bar, and switch among different layers by click "
            "the layer-box on the bar.\n\n"
            "MDE is free software, covered by the GNU General Public License, "
            "and you are welcome to change it and/or distribute copies of it "
            "under certain conditions. Please see the file COPYING to know "
            "the details.\n\n"
            "There is absolutely no warranty for MDE.",
            MINIGUI_MAJOR_VERSION, 
            MINIGUI_MINOR_VERSION, 
            MINIGUI_MICRO_VERSION,
            MINIGUI_MAJOR_VERSION, 
            MINIGUI_MINOR_VERSION, 
            MINIGUI_MICRO_VERSION);

    MessageBox (HWND_DESKTOP, buff,
            "About MDE!", MB_OK | MB_ICONINFORMATION);
}
static void Usage (void)
{
    MessageBox (HWND_DESKTOP, 
            "Please start 'mginit' from the directory in which it located! "
            "Do not change to other directory to run it.\n\n"
            "Thanks a lot.",
            "Usage of MDE!",
            MB_OK | MB_ICONEXCLAMATION);
}
*/

static const char* new_del_client_info [] =
{
    NULL,
    "New comming in client: %s\n",
    "Disconnecting client: %s\n"
};

static void on_new_del_client (int op, int cli)
{
    static int nr_clients = 0;
    MG_Client* client = mgClients + cli;

    if (op > 0 && op <= LCO_DEL_CLIENT)
        printf (new_del_client_info [op], client?client->name:"NULL");

    if (op == LCO_NEW_CLIENT) {
        nr_clients ++;
    }
    else if (op == LCO_DEL_CLIENT) {
        nr_clients --;
        if (nr_clients == 0) {
#if 0
            SendMessage (hTaskBar, MSG_CLOSE, 0, 0);
#else
            printf ("There is no any client.\n");
#endif
        }
        else if (nr_clients < 0) {
            printf ("Serious error: nr_clients less than zero.\n");
        }
    }
    else
        printf ("Serious error: incorrect operations.\n");
}

static const char* change_layer_info [] =
{
        NULL,
        "New layer created: %s; client: %s\n",
        "Layer deleted: %s; client: %s\n",
        "New client joined layer: (%s, %s)\n",
        "Remove client from layer: (%s, %s)\n",
        "Change topmost layer to %s; client: %s\n",
        "Change active client: (%s, %s)\n",
};

static void adjust_boxes (int width, MG_Layer* layer)
{
    int left = _LEFT_BOXES;
    MG_Layer* cur_layer = mgLayers;

    while (cur_layer) {
        if (cur_layer != layer) {
            MoveWindow ((HWND)cur_layer->dwAddData, left, _MARGIN,
                            width, _HEIGHT_CTRL, TRUE);
            left += width;
        }

        cur_layer = cur_layer->next;
    }
}

static void on_change_topmost (MG_Layer* layer)
{
    MG_Layer* cur_layer = mgLayers;

    if (layer)
        SendMessage ((HWND)layer->dwAddData, BM_SETCHECK, BST_CHECKED, 0);

    while (cur_layer) {
        if (cur_layer != layer) {
            SendMessage ((HWND)cur_layer->dwAddData, BM_SETCHECK, 0, 0);
        }

        cur_layer = cur_layer->next;
    }
}

static void on_change_layer (int op, MG_Layer* layer, MG_Client* client)
{
    static int nr_boxes = 0;
    static int box_width = _MAX_WIDTH_LAYER_BOX;
    int new_width;

    if (op > 0 && op <= LCO_ACTIVE_CHANGED)
        printf (change_layer_info [op], layer?layer->name:"NULL", 
                        client?client->name:"NULL");

    switch (op) {
    case LCO_NEW_LAYER:
        nr_boxes ++;
        if (box_width * nr_boxes > _WIDTH_BOXES) {
            new_width = _WIDTH_BOXES / nr_boxes;
            if (new_width < _MIN_WIDTH_LAYER_BOX) {
                new_width = _MIN_WIDTH_LAYER_BOX;
            }

            if (new_width != box_width) {
                adjust_boxes (new_width, layer);
                box_width = new_width;
            }
        }

        layer->dwAddData = (DWORD)CreateWindow (CTRL_BUTTON, layer->name,
                 WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_PUSHLIKE | BS_CENTER,
                 _ID_LAYER_BOX,
                 _LEFT_BOXES + box_width * (nr_boxes - 1), _MARGIN,
                 box_width, _HEIGHT_CTRL, hTaskBar, (DWORD)layer);
        break;

    case LCO_DEL_LAYER:
        DestroyWindow ((HWND)(layer->dwAddData));
        layer->dwAddData = 0;
        nr_boxes --; 
        if (box_width * nr_boxes < _WIDTH_BOXES) {
            if (nr_boxes != 0)
                new_width = _WIDTH_BOXES / nr_boxes;
            else
                new_width = _MAX_WIDTH_LAYER_BOX;

            if (new_width > _MAX_WIDTH_LAYER_BOX)
                new_width = _MAX_WIDTH_LAYER_BOX;

            adjust_boxes (new_width, layer);
            box_width = new_width;
        }
        break;

    case LCO_JOIN_CLIENT:
        break;
    case LCO_REMOVE_CLIENT:
        break;
    case LCO_TOPMOST_CHANGED:
        on_change_topmost (layer);
        break;
    case LCO_ACTIVE_CHANGED:
        break;
    default:
        printf ("Serious error: incorrect operations.\n");
    }
}

static void child_wait (int sig)
{
    int pid;
    int status;

    while ((pid = waitpid (-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED (status))
            printf ("--pid=%d--status=%x--rc=%d---\n", 
                            pid, status, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf ("--pid=%d--signal=%d--\n", pid, WTERMSIG (status));
    }
}

int MiniGUIMain (int args, const char* arg[])
{
    struct sigaction siga;
    MSG msg;

    OnNewDelClient = on_new_del_client;
    OnChangeLayer = on_change_layer;

    if (!ServerStartup (0, 0, 0)) {
        fprintf (stderr, 
                 "Can not start the server of MiniGUI-Processes: mginit.\n");
        return 1;
    }
    
    SetCustomDesktopOperationSet(&mg_dsk_ops);

 //   AboutMiniGUI ();
 //   AboutMDE ();

    if ((hTaskBar = create_task_bar ()) == HWND_INVALID) {
        fprintf (stderr, "Can not create task bar.\n");
        return 2;
    }

    siga.sa_handler = child_wait;
    siga.sa_flags  = 0;
    memset (&siga.sa_mask, 0, sizeof(sigset_t));

    sigaction (SIGCHLD, &siga, NULL);

    while (GetMessage (&msg, hTaskBar)) {
        DispatchMessage (&msg);
    }

    return 0;
}

#endif

