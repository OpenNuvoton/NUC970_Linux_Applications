/* 
 * $Id: desktop.h 382 2008-01-29 09:30:09Z wangjian $
 * 
 * The head file of desktop for mginit.
 */

#define ID_UP 0x101
#define ID_CB 0x102
#define ID_AB 0x103

#define ID_OP 0x111
#define ID_RN 0x112

#define APP_INFO_FILE   "mginit.rc"

typedef struct _Context
{
    PBITMAP bg;
    HMENU hmenu;
    HMENU icon_menu;
}Context;

typedef struct tagDspItem 
{
    BOOL    cdpath;
	RECT    hot_spot_rc;
	RECT    text_rc;
    char    path [PATH_MAX + 1];
    char    name [NAME_MAX + 1];
    char    layer [LEN_LAYER_NAME + 1];
    char    bmp_path [PATH_MAX + NAME_MAX + 1];
    BITMAP  bmp;
} DSPITEM;

typedef struct _ICON_INFO
{
    int      nr_apps;
    int      focus;
    char     bg_path [PATH_MAX + 1];
    DSPITEM* app_items;
}ICON_INFO;

#define START_X 20
#define START_Y 10
#define SPACE   30
#define DEF_WIDTH 64
#define DEF_HEIGHT 64

static ICON_INFO icon_info;

extern DESKTOPOPS mg_dsk_ops;
