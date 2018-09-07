
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GUI.h"


#include "N9H30TouchPanel.h"
#include "tslib.h"


void TouchTask(void)
{
    static U16 xOld;
    static U16 yOld;
    static U8  PressedOld = 0;
    int x, y, xDiff, yDiff;
    int  Pressed;

    int ifirst;
    struct tsdev *ts;
    char *tsdevice=NULL;


    printf("Touch Task start\n");

//  signal(SIGSEGV, sig);
//  signal(SIGINT, sig);
//  signal(SIGTERM, sig);

    if ((tsdevice = getenv("TSLIB_TSDEVICE")) == NULL)
    {
#ifdef USE_INPUT_API
        tsdevice = strdup ("/dev/input/event0");
#else
        tsdevice = strdup ("/dev/adc");
#endif /* USE_INPUT_API */
    }

    ts = ts_open (tsdevice, 0);

    if (!ts)
    {
        perror (tsdevice);
        exit(1);
    }

    if (ts_config(ts))
    {
        perror("ts_config");
        exit(1);
    }

    ifirst = 1;
    while (1)
    {
        struct ts_sample samp;
        int ret;

        usleep(10000);  //delay 10 ms
        ret = ts_read(ts, &samp, 1);
//  printf("X=%d, Y=%d, ret=%d, pres=%d\n",samp.x,samp.y,ret,samp.pressure);
        if ( ( ret == 1) && ( samp.pressure != 0 ) )
            Pressed = 1;// TBD: Insert function which returns:
        else
            Pressed = 0;
        //      1, if the touch screen is pressed
        //      0, if the touch screen is released
        //
        // Touch screen is pressed
        //
        if (Pressed)
        {
            x =  samp.x;// TBD: Insert function which reads current x value
            y =  samp.y;// TBD: Insert function which reads current y value
            //
            // The touch has already been pressed
            //
            if (PressedOld == 1)
            {
                //
                // Calculate difference between new and old position
                //
                xDiff = (x > xOld) ? (x - xOld) : (xOld - x);
                yDiff = (y > yOld) ? (y - yOld) : (yOld - y);
                //
                // Store state if new position differs significantly from old position
                //
                if (xDiff + yDiff > 2)
                {
                    xOld = x;
                    yOld = y;
                    GUI_TOUCH_StoreState(x, y);
//  printf("Chnage X=%d, Y=%d\n",x,y);
                }
            }
            //
            // The touch was previously released
            // Store state regardless position
            //
            else
            {
                if ((x != 0) && (y != 0))
                {
                    if ( ifirst <= 5 )
                    {
                        ifirst++;
                        continue;
                    }
                    xOld = x;
                    yOld = y;
                    PressedOld = 1;
                    GUI_TOUCH_StoreState(x, y);
//  printf("X=%d, Y=%d\n",x,y);
                }
            }
        }
        else
        {
            if (PressedOld == 1)
            {
                PressedOld = 0;
                GUI_TOUCH_StoreState(-1, -1);
                ifirst = 0;
//  printf("X= -1, Y=-1\n");
            }
        }
    }
    //
    // Make sure
    //
}
