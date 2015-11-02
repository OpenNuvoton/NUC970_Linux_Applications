/* 
** $Id: desktop.c 600 2008-08-14 07:22:04Z weiym $
**
** The desktop of MDE
**
** Copyright (c) 2001, Wei Yongming (ymwei@minigui.org)
** Copyright (C) 2003 ~ 2007 Feynman Software.
**
** Create date: 2008/01/28
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "desktop.h"

#ifdef _LANG_ZHCN
#include "desktop_res_cn.h"
#elif defined _LANG_ZHTW
#include "desktop_res_tw.h"
#else
#include "desktop_res_en.h"
#endif

static void free_dsp_app (void)
{
    int i;
    DSPITEM* item;

    item = icon_info.app_items;
    for (i = 0; i < icon_info.nr_apps; i++, item++) {
        if (item->bmp.bmBits) {
            UnloadBitmap (&item->bmp);
            item->bmp.bmBits = NULL;
        }
    }

    free (icon_info.app_items);
    icon_info.app_items = NULL;
}

static BOOL get_dsp_app (void)
{
    int i;
    DSPITEM* item;
    char section [10];
	int distance_x = START_X;
	int distance_y = START_Y;
    SIZE size;

    if (GetIntValueFromEtcFile (APP_INFO_FILE, "desktop", "app_nr", &icon_info.nr_apps) != ETC_OK)
        return FALSE;
    
    if (icon_info.nr_apps <= 0)
        return FALSE;
    
    if (GetValueFromEtcFile (APP_INFO_FILE, "desktop", "bg_pic", icon_info.bg_path, PATH_MAX) != ETC_OK)
        return FALSE;
    
    if ((icon_info.app_items = (DSPITEM*)calloc (icon_info.nr_apps, sizeof (DSPITEM))) == NULL) {
        return FALSE;
    }
    item = icon_info.app_items;
    for (i = 0; i < icon_info.nr_apps; i++, item++) {
        sprintf (section, "dsp-app%d", i);
        if (GetValueFromEtcFile (APP_INFO_FILE, section, "path", item->path, PATH_MAX) != ETC_OK)
            goto error;

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "name", item->name, NAME_MAX) != ETC_OK)
            goto error;

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "layer", item->layer, LEN_LAYER_NAME) != ETC_OK)
            goto error;

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "pictrue", item->bmp_path, PATH_MAX + NAME_MAX) != ETC_OK)
            goto error;

        if (LoadBitmap (HDC_SCREEN, &item->bmp, item->bmp_path) != ERR_BMP_OK) {
            fprintf (stderr, "desktop load resource:%s error. \n", item->bmp_path);
            goto error;
        }
        
        item->cdpath = TRUE;

		item->hot_spot_rc.left   = distance_x;
		item->hot_spot_rc.right  = item->hot_spot_rc.left + DEF_WIDTH;
		item->hot_spot_rc.top    = distance_y;
		item->hot_spot_rc.bottom = item->hot_spot_rc.top + DEF_HEIGHT;

        GetTextExtent(HDC_SCREEN, item->name, -1, &size);

        item->text_rc.top     = item->hot_spot_rc.bottom;
        item->text_rc.left    = (item->hot_spot_rc.right + item->hot_spot_rc.left - size.cx)/2;
        item->text_rc.bottom  = item->text_rc.top + size.cy;
        item->text_rc.right   = item->text_rc.left + size.cx;
		
        distance_y += SPACE + RECTH(item->hot_spot_rc);
        if(distance_y + DEF_HEIGHT >= g_rcDesktop.bottom)
        {
            distance_y = START_Y;
            distance_x += SPACE + DEF_WIDTH;
        }	
    }
    return TRUE;
error:
    free_dsp_app ();
    return FALSE;
}

static HMENU create_icon_menu (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)MGDT_ST_ICONMENU;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_OP;
    mii.typedata    = (DWORD)MGDT_ST_OPEN;
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_AB;
    mii.typedata    = (DWORD)MGDT_ST_ABOUT;
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    return hmnu;
}

static void* this_init(void)
{
    Context *con = malloc(sizeof(Context));

    get_dsp_app ();
    icon_info.focus = 0xFFFF;

    if(0 != strcmp(icon_info.bg_path, ""))
    {
        con->bg = (PBITMAP)malloc(sizeof(BITMAP));
        LoadBitmapFromFile(HDC_SCREEN, con->bg, icon_info.bg_path);
    }
    else
        con->bg = NULL; 

    con->icon_menu = create_icon_menu();   

    return (void *)con;
}

static void this_deinit(void* context)
{
    if(((Context *)context)->bg) 
        UnloadBitmap (((Context *)context)->bg);

    free_dsp_app ();

	if (context != NULL) free(context);

    return;
}

static void this_paint_desktop(void* context, HDC dc_desktop, const RECT* inv_rc)
{
    PBITMAP bg_bmp = NULL;
	int i = 0;
    DSPITEM* item;

    if(((Context *)context)->bg)
        bg_bmp = ((Context *)context)->bg;

    SelectClipRect (dc_desktop, inv_rc);
    if (bg_bmp) {
        FillBoxWithBitmap (dc_desktop, 0, 0,
                g_rcDesktop.right, g_rcDesktop.bottom, bg_bmp);
    }else {
        SetBrushColor (dc_desktop, PIXEL_blue);
        FillBox(dc_desktop, g_rcDesktop.left, g_rcDesktop.top,
                RECTW(g_rcDesktop), RECTH(g_rcDesktop));
    }
    
    item = icon_info.app_items;
    for(i = 0; i < icon_info.nr_apps; i++, item++)
	{
        if(i == icon_info.focus)
        {
            SetBrushColor (dc_desktop, PIXEL_darkblue);
            FillBox(dc_desktop, item->hot_spot_rc.left, 
                    item->hot_spot_rc.top,
                    RECTW(item->hot_spot_rc), 
                    RECTH(item->hot_spot_rc));
        }
        FillBoxWithBitmap (dc_desktop,
                item->hot_spot_rc.left,
                item->hot_spot_rc.top,
                RECTW(item->hot_spot_rc),
                RECTH(item->hot_spot_rc), 
                &item->bmp);
        SetBkMode(dc_desktop, BM_TRANSPARENT);
        TextOut(dc_desktop, 
                item->text_rc.left, item->text_rc.top, item->name);
	}
}

static void this_keyboard_handler(void* context, int message, 
		                               WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case MSG_DT_KEYDOWN:
            break;
        case MSG_DT_KEYUP:
            break;
    }
}

static void this_customize_desktop_menu (void* context, HMENU hmnu, int start_pos)
{
    ((Context *)context)->hmenu = hmnu;

    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = ID_UP;
    mii.typedata    = (DWORD)MGDT_ST_REFRESH;
    mii.hsubmenu    = 0;
    InsertMenuItem (hmnu, start_pos , TRUE, &mii);
#if 0
    mii.type        = MFT_STRING;
    mii.id          = ID_CB;
    mii.typedata    = (DWORD)"Change BkPicture";
    mii.hsubmenu    = 0;
    InsertMenuItem (hmnu, start_pos + 1, TRUE, &mii);
#endif
    mii.type        = MFT_STRING;
    mii.id          = ID_AB;
    mii.typedata    = (DWORD)MGDT_ST_ABOUTM;
    mii.hsubmenu    = 0;
    InsertMenuItem (hmnu, start_pos + 1, TRUE, &mii);

}

pid_t exec_app_name (const char* file_name, const char* app_name, const char * argv)
{
    pid_t pid = 0;

    if ((pid = vfork ()) > 0) {
        fprintf (stderr, "new child, pid: %d.\n", pid);
    }
    else if (pid == 0) {
        if (execl (file_name, app_name, argv, NULL) < 0)
			fprintf(stderr, "execl error\n");
        perror ("execl");
        _exit (1);
    }
    else {
        perror ("vfork");
    }

    return pid;
}

static void this_mouse_handler(void* context, int message, 
		                            WPARAM wParam, LPARAM lParam)
{
    int x, y;
    int i = 0;
    DSPITEM* item;
    static int old_x = 0, old_y = 0;

    x = LOSWORD (lParam);
    y = HISWORD (lParam);
    switch(message)
    {
        case MSG_DT_LBUTTONDOWN:
            {
                item = icon_info.app_items;
                for(i = 0; i < icon_info.nr_apps; i++, item++) 
                {
                    if (PtInRect(&item->hot_spot_rc, x, y)) 
                    {
                        icon_info.focus = i;
                        old_x = x;
                        old_y = y;
                        SendMessage(HWND_DESKTOP, MSG_ERASEDESKTOP, 
                                0, (LPARAM)&item->hot_spot_rc);
                        break;
                    }
                }
                break;
            }

        case MSG_DT_LBUTTONUP:
            {
                icon_info.focus = 0xFFFF;
                SendMessage(HWND_DESKTOP, MSG_ERASEDESKTOP, 0, 0);
                break;
            }
        case MSG_DT_LBUTTONDBLCLK:
            {
                char buff [PATH_MAX + NAME_MAX + 1];
                item = icon_info.app_items;
                for(i = 0; i < icon_info.nr_apps; i++, item++) 
                {
                    if (PtInRect(&item->hot_spot_rc, x, y)) 
                    {
                        if(item->cdpath)
                        {
                            chdir(item->path);
                        }
                        strcpy (buff, item->path);
                        strcat (buff, item->name);
                        exec_app_name(buff, item->name, "-layer");
                        break;
                    }
                }

                break;
            }

        case MSG_DT_MOUSEMOVE:
            {
                RECT rc;
                if (icon_info.focus == 0xFFFF) break;

                item = icon_info.app_items;
                for(i = 0; i < icon_info.nr_apps; i++, item++) 
                {
                    if (i == icon_info.focus ) 
                    {
                        GetBoundRect(&rc, &item->text_rc, &item->hot_spot_rc);
                        
                        item->hot_spot_rc.left      += x - old_x;
                        item->hot_spot_rc.right     += x - old_x;
                        item->hot_spot_rc.top       += y - old_y;
                        item->hot_spot_rc.bottom    += y - old_y;

                        item->text_rc.left      += x - old_x;
                        item->text_rc.right     += x - old_x;
                        item->text_rc.top       += y - old_y;
                        item->text_rc.bottom    += y - old_y;
                        
                        old_x = x;
                        old_y = y;
                        GetBoundRect(&rc, &rc, &item->hot_spot_rc);
                        GetBoundRect(&rc, &rc, &item->text_rc);
                        SendMessage(HWND_DESKTOP, MSG_ERASEDESKTOP, 0, (LPARAM)&rc);
                    }
                }
                break;
            }

        case MSG_DT_RBUTTONUP:
            {
                BOOL flag = FALSE;
                item = icon_info.app_items;
                for(i = 0; i < icon_info.nr_apps; i++, item++) 
                {
                    if (PtInRect(&item->hot_spot_rc, x, y)) 
                    {
                        icon_info.focus = i;
                        SendMessage(HWND_DESKTOP, MSG_ERASEDESKTOP, 
                                0, (LPARAM)&item->hot_spot_rc);
                        TrackPopupMenu (((Context *)context)->icon_menu, 
                                TPM_DEFAULT, x, y, HWND_DESKTOP);
                        flag = TRUE;
                        break;
                    }
                }
                if(!flag)
                    TrackPopupMenu (((Context *)context)->hmenu, TPM_DEFAULT, x, y, HWND_DESKTOP);
                break;
            }
    }
}

static void this_desktop_menucmd_handler (void* context, int id)
{
    if(!context)
        return;
    if(id == ID_UP)
    {
        DesktopUpdateAllWindow();
    }
#if 0
    else if(id == ID_CB)
    {
 		NEWFILEDLGDATA file_data;
		int choise = 0;
//		file_data.IsSave = FALSE;
		strcpy(file_data.filepath,".");
		choise = ShowOpenDialog(HWND_DESKTOP, 50, 50, 300, 200, &file_data);
		if(choise == IDOK)
		{
			if(access(file_data.filefullname, F_OK) < 0)
			{
				printf("file not exist!\n");
			}
			else
			{
				printf("ok!\n");
			}
		}
    }
#endif
    else if(id == ID_AB)
    {

#ifdef _MGMISC_ABOUTDLG
#ifdef _MGRM_THREADS
        OpenAboutDialog ();
#else
        OpenAboutDialog (HWND_DESKTOP);
#endif
#endif
    }
    else if(id == ID_OP)
    {
        int i = 0;
        DSPITEM* item;
        char buff [PATH_MAX + NAME_MAX + 1];
        item = icon_info.app_items;
        for(i = 0; i < icon_info.nr_apps; i++, item++) 
        {
            if (i == icon_info.focus) 
            {
                if(item->cdpath)
                {
                    chdir(item->path);
                }
                strcpy (buff, item->path);
                strcat (buff, item->name);
                exec_app_name(buff, item->name, "-layer");
                icon_info.focus = 0xFFFF;
                break;
            }
        }

    }
    else if(id == ID_RN)
    {
        icon_info.focus = 0xFFFF;
    }

    return;
}


DESKTOPOPS mg_dsk_ops =
{
    this_init,
    this_deinit,
    this_paint_desktop,
    this_keyboard_handler,
    this_mouse_handler,
    this_customize_desktop_menu,
    this_desktop_menucmd_handler,
};

