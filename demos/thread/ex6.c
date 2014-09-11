#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void *
test_thread (void *v_param)
{
  return NULL;
}
void *
test_demo (void * count)
{
//printf("pthread[%d]\n",count);
  return NULL;
}

int
main (void)
{
  unsigned long count;

  setvbuf (stdout, NULL, _IONBF, 0);

  for (count = 0; count < 200; ++count)
    {
      pthread_t thread;
      int status;

      status = pthread_create (&thread, NULL, test_thread, NULL);
      if (status != 0)
	{
	  printf ("status = %d, count = %lu: %s\n", status, count,
		  strerror (errno));
	  return 1;
	}
      else
	{
	  printf ("count = %lu\n", count);
	}
      /* pthread_detach (thread); */
      pthread_join (thread, NULL);
      sched_yield();
      //usleep (10);
    }

{
    pthread_t *thread_ = (pthread_t *)malloc(sizeof(pthread_t) * 100);
    for (count = 0; count < 100; ++count)
    	{
	    	int status;

  	      status = pthread_create (&(thread_[count]), NULL, test_demo, (void *)count);
	      if (status != 0)
		{
			  printf ("status = %d, count = %lu: %s\n", status, count, strerror (errno));
			  printf ("System can only create more than %d threads within one process.\n", count + 2);
			  return 1;
		}
	      
    	}
    free(thread_);
}
  return 0;
}
