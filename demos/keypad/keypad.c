/* Keybad Driver Test/Example Program
 *
 * Compile with:
 *  gcc -s -Wall -Wstrict-prototypes keybad.c -o keybad_demo
 *
 *
 * Note : 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>

#define DEVINPUT "/dev/input/event1"
#define NUM 1
int main(int argc, char **argv)
{	
	int fb = -1;
	ssize_t rb;
	
	struct input_event ev[NUM];	
	int yalv;
	
	if ((fb = open(DEVINPUT, O_RDONLY | O_NDELAY)) < 0) {
		printf("cannot open %s\n", DEVINPUT);
		exit(0);
    }
    
	 while (1) {
	 	if((rb = read(fb, ev, sizeof(struct input_event) * NUM))>0)
	 	{
	 		for (yalv = 0; yalv < (int) (rb / sizeof(struct input_event));yalv++) {
	 			switch (ev[yalv].type) {
		    case EV_SYN:
				printf("EV_TYPE = EV_SYN\n");
				break;
		    case EV_KEY:					
			    switch(ev[yalv].code)
			    {
			    	case BTN_TOUCH:
			    		printf("EV_KEY : BTN_TOUCH value %d \n",ev[yalv].value);
			    	break;
			    	default:
			    		printf("EV_KEY : code %d value %d \n", ev[yalv].code,ev[yalv].value);
			    	break;
			    }						
				break;
		    case EV_LED:
					printf("EV_TYPE = EV_LED\n");
				break;
		    case EV_REP:
					printf("EV_TYPE = EV_REP\n");
				break;
		    case EV_ABS:					
			    switch(ev[yalv].type)
			    {
			    	case ABS_X:
			    		printf("EV_ABS : ABS_X value %d \n",ev[yalv].value);
			    	break;
			    	case ABS_Y:
			    		printf("EV_ABS : ABS_Y value %d \n",ev[yalv].value);
			    	break;		    	
			    	default:
			    		printf("EV_ABS : code %d value %d \n",ev[yalv].code,ev[yalv].value);
			    	break;
			    }					
				break;					
		    default:
					printf("EV_TYPE = %x\n", ev[yalv].type);
				break;
		    }
		    //printf("time= %1d.%061d \n", ev[yalv].time.tv_sec,ev[yalv].time.tv_usec);		    		    
	 		}	 					
	 	}
	}
	return 0;
}
