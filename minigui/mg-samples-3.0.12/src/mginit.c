/* 
** $Id: mginit.c 322 2007-08-30 01:20:10Z xwyan $
**
** Listing 31.1
**
** mginit.c: Sample program for MiniGUI Programming Guide
**      A simple mginit program.
**
** Copyright (C) 2003 ~ 2007 Feynman Software.
**
** License: GPL
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

static BOOL quit = FALSE;

static void on_new_del_client (int op, int cli)
{       
    static int nr_clients = 0;

    if (op == LCO_NEW_CLIENT) {
        nr_clients ++;
    }   
    else if (op == LCO_DEL_CLIENT) {
        nr_clients --;
        if (nr_clients == 0) {
            printf ("There is no any client, I will quit.\n");
            quit = TRUE;
        }               
        else if (nr_clients < 0) {
            printf ("Serious error: nr_clients less than zero.\n");
        }
    }
    else
        printf ("Serious error: incorrect operations.\n");
}

static pid_t exec_app (const char* file_name, const char* app_name)
{
    pid_t pid = 0;

    if ((pid = vfork ()) > 0) {
        fprintf (stderr, "new child, pid: %d.\n", pid);
    }
    else if (pid == 0) {
        execl (file_name, app_name, NULL);
        perror ("execl");
        _exit (1);
    }
    else {
        perror ("vfork");
    }

    return pid;
}

static unsigned int old_tick_count;

static pid_t pid_scrnsaver = 0;

static int my_event_hook (PMSG msg)
{
    old_tick_count = GetTickCount ();

    if (pid_scrnsaver) {
        kill (pid_scrnsaver, SIGINT);
        ShowCursor (TRUE);
        pid_scrnsaver = 0;
    }

    if (msg->message == MSG_KEYDOWN) {
        switch (msg->wParam) {
            case SCANCODE_F1:
               exec_app ("./edit", "edit");
               break;
            case SCANCODE_F2:
               exec_app ("./timeeditor", "timeeditor");
               break;
            case SCANCODE_F3:
               exec_app ("./propsheet", "propsheet");
               break;
            case SCANCODE_F4:
               exec_app ("./bmpbkgnd", "bmpbkgnd");
               break;
	}
    }

    return HOOK_GOON;
}

static void child_wait (int sig)
{
    int pid;
    int status;

    while ((pid = waitpid (-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED (status))
            printf ("--pid=%d--status=%x--rc=%d---\n", pid, status, WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf ("--pid=%d--signal=%d--\n", pid, WTERMSIG (status));
    }
}

int MiniGUIMain (int args, const char* arg[])
{
    MSG msg;
    struct sigaction siga;

    siga.sa_handler = child_wait;
    siga.sa_flags  = 0;
    memset (&siga.sa_mask, 0, sizeof(sigset_t));
    sigaction (SIGCHLD, &siga, NULL);

    OnNewDelClient = on_new_del_client;

    if (!ServerStartup (0 , 0 , 0)) {
        fprintf (stderr, 
                 "Can not start the server of MiniGUI-Processes: mginit.\n");
        return 1;
    }


    SetServerEventHook (my_event_hook);

    if (args > 1) {
        if (exec_app (arg[1], arg[1]) == 0)
            return 3;
    }
    else {
        if (exec_app ("./helloworld", "helloworld") == 0)
            return 3;
    }

    old_tick_count = GetTickCount ();

    while (!quit && GetMessage (&msg, HWND_DESKTOP)) {
        if (pid_scrnsaver == 0 && GetTickCount () > old_tick_count + 1000) {
            ShowCursor (FALSE);
            pid_scrnsaver = exec_app ("./scrnsaver", "scrnsaver");
        }
        DispatchMessage (&msg);
    }

    return 0;
}

