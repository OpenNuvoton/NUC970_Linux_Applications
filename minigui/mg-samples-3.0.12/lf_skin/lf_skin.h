/*
 *
 * lf_skin.h
 *
 * wangjian<wangjian@minigui.org>
 *
 * 2008-8-20
*/

#define SKINS_BLUE          0
#define SKINS_BLACK         1

#define IDC_STATICBACK      200
#define IDC_STATICHEAD      201
#define IDC_START           202
#define IDC_STOP            203
#define IDC_VOLUME          204
#define IDC_TRACKBAR        205
#define IDC_PROGRESS        206
#define IDC_CONFIG          207
#define IDC_TMIN            208
#define IDC_SMIN            209
#define IDC_TSEC            210
#define IDC_SSEC            211
#define IDC_SUBHELP         212
#define IDC_PROPSHEET       213
#define IDC_HELPINFO        214
#define PAGE_HELPLIST       215
#define PAGE_INFO           216
#define IDC_TREEVIEW        217
#define IDC_SPINBOX         218
#define IDC_SKIN_BUTTON     219
#define IDC_HELPLIST        220
#define IDC_INFO            221

#define TIMER_SEC           1000

#define MSG_CHANGE_SKINS    (MSG_USER + 10)

#define CHAPTER_NUM         4

#define TIME_POS_X          120
#define TIME_POS_Y          20

#define LFSKIN_ARROWS                  "lfskin_Arrows.gif"
#define LFSKIN_ARROW_SHAFT             "lfskin_Arrow_Shaft.gif"
#define LFSKIN_CAPTION                 "lfskin_Caption.gif"
#define LFSKIN_CAPTIONBTN              "lfskin_Captionbtn.gif"
#define LFSKIN_CONFIG                  "lfskin_Config.gif"
#define LFSKIN_DISVOLUME               "lfskin_DisVolume.gif"
#define LFSKIN_HPROGRESSBAR            "lfskin_HProgressbar.gif"
#define LFSKIN_HTRACKBAR               "lfskin_HTrackBar.gif"
#define LFSKIN_HELP                    "lfskin_Help.gif"
#define LFSKIN_HELPBACK                "lfskin_HelpBack.gif"
#define LFSKIN_MAIN                    "lfskin_Main.gif"
#define LFSKIN_PAUSE                   "lfskin_Pause.gif"
#define LFSKIN_PROTRACK                "lfskin_ProTrack.gif"
#define LFSKIN_SB_HSHAFT               "lfskin_SB_HShaft.bmp"
#define LFSKIN_SB_HTHUMB               "lfskin_SB_Hthumb.gif"
#define LFSKIN_SB_VSHAFT               "lfskin_SB_VShaft.bmp"
#define LFSKIN_SB_VTHUMB               "lfskin_SB_Vthumb.gif"
#define LFSKIN_SB_ARROWS               "lfskin_sb_arrows.gif"
#define LFSKIN_START                   "lfskin_Start.gif"
#define LFSKIN_STOP                    "lfskin_Stop.gif"
#define LFSKIN_TAB                     "lfskin_Tab.gif"
#define LFSKIN_TRACKBAR                "lfskin_TrackBar.gif"
#define LFSKIN_TREE                    "lfskin_Tree.gif"
#define LFSKIN_VOLUME                  "lfskin_Volume.gif"
#define LFSKIN_BOTTOMBORDER            "lfskin_bottomborder.gif"
#define LFSKIN_LEFTBORDER              "lfskin_leftborder.bmp"
#define LFSKIN_RIGHTBORDER             "lfskin_rightborder.bmp"
#define LFSKIN_OTHER_ARROW_SHAFT       "lfskin_other_Arrow_Shaft.bmp"
#define LFSKIN_OTHER_BOTTOMBORDER      "lfskin_other_BottomBorder.bmp"
#define LFSKIN_OTHER_CAPTION           "lfskin_other_Caption.gif"
#define LFSKIN_OTHER_CAPTIONBTN        "lfskin_other_Captionbtn.gif"
#define LFSKIN_OTHER_CONFIG            "lfskin_other_Config.gif"
#define LFSKIN_OTHER_DISVOLUME         "lfskin_other_DisVolume.gif"
#define LFSKIN_OTHER_HPROGRESSBAR      "lfskin_other_HProgressbar.gif"
#define LFSKIN_OTHER_HTRACKBAR         "lfskin_other_HTrackBar.gif"
#define LFSKIN_OTHER_HELP              "lfskin_other_Help.gif"
#define LFSKIN_OTHER_HELPBACK          "lfskin_other_HelpBack.gif"
#define LFSKIN_OTHER_HELPTREEBACK      "lfskin_other_HelpTreeBack.bmp"
#define LFSKIN_OTHER_LEFTBORDER        "lfskin_other_LeftBorder.bmp"
#define LFSKIN_OTHER_MAIN              "lfskin_other_Main.gif"
#define LFSKIN_OTHER_PAUSE             "lfskin_other_Pause.gif"
#define LFSKIN_OTHER_PROTRACK          "lfskin_other_ProTrack.gif"
#define LFSKIN_OTHER_RIGHTBORDER       "lfskin_other_RightBorder.bmp"
#define LFSKIN_OTHER_SB_HSHAFT         "lfskin_other_SB_HShaft.bmp"
#define LFSKIN_OTHER_SB_HTHUMB         "lfskin_other_SB_Hthumb.gif"
#define LFSKIN_OTHER_SB_VSHAFT         "lfskin_other_SB_VShaft.bmp"
#define LFSKIN_OTHER_SB_VTHUMB         "lfskin_other_SB_Vthumb.gif"
#define LFSKIN_OTHER_SB_ARROWS         "lfskin_other_SB_arrows.gif"
#define LFSKIN_OTHER_START             "lfskin_other_Start.gif"
#define LFSKIN_OTHER_STOP              "lfskin_other_Stop.gif"
#define LFSKIN_OTHER_TAB               "lfskin_other_Tab.bmp"
#define LFSKIN_OTHER_TRACKBAR          "lfskin_other_TrackBar.gif"
#define LFSKIN_OTHER_TREE              "lfskin_other_Tree.bmp"
#define LFSKIN_OTHER_VOLUME            "lfskin_other_Volume.gif"
#define LFSKIN_OTHER_ARROWS            "lfskin_other_arrows.gif"
   
BOOL register_all_pic (void);
void unregister_all_pic (void);


