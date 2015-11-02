
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include "lf_skin.h"

#if ((MINIGUI_MAJOR_VERSION >= 2 && MINIGUI_MICRO_VERSION >= 2 && MINIGUI_MINOR_VERSION >= 2) \
        || (MINIGUI_MAJOR_VERSION >= 3))
#define ARRAY_LEN(array) \
    (sizeof(array)/sizeof(array[0]))

static char* all_pic[] = {
    LFSKIN_ARROWS,            
    LFSKIN_ARROW_SHAFT,       
    LFSKIN_CAPTION,           
    LFSKIN_CAPTIONBTN,        
    LFSKIN_CONFIG,            
    LFSKIN_DISVOLUME,         
    LFSKIN_HPROGRESSBAR,      
    LFSKIN_HTRACKBAR,         
    LFSKIN_HELP,              
    LFSKIN_HELPBACK,          
    LFSKIN_MAIN,              
    LFSKIN_PAUSE,             
    LFSKIN_PROTRACK,          
    LFSKIN_SB_HSHAFT,         
    LFSKIN_SB_HTHUMB,         
    LFSKIN_SB_VSHAFT,         
    LFSKIN_SB_VTHUMB,         
    LFSKIN_SB_ARROWS,         
    LFSKIN_START,             
    LFSKIN_STOP,              
    LFSKIN_TAB,               
    LFSKIN_TRACKBAR,          
    LFSKIN_TREE,              
    LFSKIN_VOLUME,            
    LFSKIN_BOTTOMBORDER,      
    LFSKIN_LEFTBORDER,        
    LFSKIN_RIGHTBORDER,       
    LFSKIN_OTHER_ARROW_SHAFT, 
    LFSKIN_OTHER_BOTTOMBORDER,
    LFSKIN_OTHER_CAPTION,     
    LFSKIN_OTHER_CAPTIONBTN,  
    LFSKIN_OTHER_CONFIG,      
    LFSKIN_OTHER_DISVOLUME,   
    LFSKIN_OTHER_HPROGRESSBAR,
    LFSKIN_OTHER_HTRACKBAR,   
    LFSKIN_OTHER_HELP,        
    LFSKIN_OTHER_HELPBACK,    
    LFSKIN_OTHER_HELPTREEBACK,
    LFSKIN_OTHER_LEFTBORDER,  
    LFSKIN_OTHER_MAIN,        
    LFSKIN_OTHER_PAUSE,       
    LFSKIN_OTHER_PROTRACK,    
    LFSKIN_OTHER_RIGHTBORDER, 
    LFSKIN_OTHER_SB_HSHAFT,   
    LFSKIN_OTHER_SB_HTHUMB,   
    LFSKIN_OTHER_SB_VSHAFT,   
    LFSKIN_OTHER_SB_VTHUMB,   
    LFSKIN_OTHER_SB_ARROWS,   
    LFSKIN_OTHER_START,       
    LFSKIN_OTHER_STOP,        
    LFSKIN_OTHER_TAB,         
    LFSKIN_OTHER_TRACKBAR,    
    LFSKIN_OTHER_TREE,        
    LFSKIN_OTHER_VOLUME,      
    LFSKIN_OTHER_ARROWS      
};

BOOL register_all_pic (void)
{
    int i;

   // SetResPath ("/usr/local/lib/minigui/res/bmp/");
    //SetResPath ("./res/systemres/");
    SetResPath ("./res/skin_res/");

    for (i = 0; i < ARRAY_LEN(all_pic); i++)
    {
        if (RegisterResFromFile (HDC_SCREEN, all_pic[i]) == FALSE)
        {
            fprintf (stderr, "can't register %s\n", all_pic[i]);
            return FALSE;
        }
    }
    return TRUE;
}

void unregister_all_pic (void)
{
    int i;

    for (i = 0; i < ARRAY_LEN(all_pic); i++)
    {
        UnregisterRes(all_pic[i]);
    }
}
#else
BOOL register_all_pic (void)
{
    return FALSE;
}

void unregister_all_pic (void)
{
}
#endif
