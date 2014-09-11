/* Creates two threads, one printing 10000 "a"s, the other printing
   10000 "b"s.
   Illustrates: thread creation, thread joining. */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread.h"

#define MAX_MALLOC 64
void * process(void * arg)
{
  int i, j;
  void *pp[MAX_MALLOC];
  
  fprintf(stderr, "Starting process %s\n", (char *) arg);
  
  for(j = 0;j < 1000; j++)
  {
	  for (i = 0; i < MAX_MALLOC; i++) 
	  {	    
	    pp[i] = malloc(1024);	
	    printf("[%s]--[%d]malloced @ [0x%x]\n",(char *) arg, i, pp[i]);
	    usleep(100);
	  }
	  
	  
	  for (i = 0; i < MAX_MALLOC; i++) 
	  {	   
	    printf("[%s]--[%d] before_free@ [0x%x]\n",(char *) arg, i, pp[i]);
	    free(pp[i]);    	     
	  }	  
 }
 	
 	printf("process over\n");
  return NULL;
}

void do_func()
{
	int retcode;
	pthread_t th_c;
	void * retval;
	
	retcode = pthread_create(&th_c, NULL, process, (void *)"c");
	retcode = pthread_join(th_c, &retval);
}

int main(void)
{
  int retcode;
  pthread_t th_a, th_b;
   
  void * retval;

  printf("hello\n");
  //usleep(0);
  retcode = pthread_create(&th_a, NULL, process, (void *) "a");

  if (retcode != 0) fprintf(stderr, "create a failed %d\n", retcode);
  else fprintf(stderr, "create a succeeded %d\n", retcode);
  //
  retcode = pthread_create(&th_b, NULL, process, (void *) "b");

  if (retcode != 0) fprintf(stderr, "create b failed %d\n", retcode);
  else fprintf(stderr, "create b succeeded %d\n", retcode);

  //join all
  retcode = pthread_join(th_a, &retval);

  if (retcode != 0) fprintf(stderr, "join a failed %d\n", retcode);
  else fprintf(stderr, "join a succeeded %d\n", retcode);

  retcode = pthread_join(th_b, &retval);
  if (retcode != 0) fprintf(stderr, "join b failed %d\n", retcode);
  else fprintf(stderr, "join b succeeded %d\n", retcode);

	//do_func();
  return 0;
}
