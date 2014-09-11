#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/soundcard.h>
#include <string.h>

enum mode { PLAY, REC };

void usage(void) {
	fprintf(stderr,
	"usage: demo <options> sample_rate channel filename\n"
	"options:\n"
	"	play\n"
	"	rec\n");
	return;
}

int main(int argc, char *argv[]) 
{

	int audio_fd, mixer_fd;
	int omode;
	int mode, samplerate;
	char para;
  	char filename[30] ;

	if (argc < 3)
		usage();

	if (strcmp(argv[1], "play") == 0) 
	{	
		mode = PLAY;
		omode = O_WRONLY;
		strcpy(filename, argv[4]);

	} 
	else if (strcmp(argv[1], "rec") == 0)
	{	
		mode = REC;
		omode = O_RDONLY;
		strcpy(filename, argv[4]);
	} 
	else 
		usage(); 
		
	samplerate = atoi(argv[2]);
	
	// open device file 
	if ((audio_fd = open("/dev/dsp",omode,0)) == -1) 
	{
		perror("/dev/dsp");
		return;
	}
	
	if ((mixer_fd = open("/dev/mixer", O_WRONLY)) == -1) 
    {
      	perror("open /dev/mixer error");
        return;
    }

	/*  Setting Sampling Parameters
		1. sample format
		2. number of channels
		3. sampling rate
	*/
	int format;
	format = AFMT_S16_LE;
	if (ioctl(audio_fd,SNDCTL_DSP_SETFMT, &format) == -1) 
	{
		perror("SNDCTL_DSP_SETFMT");
		return;	
	}

	int channels = atoi(argv[3]);
	if (ioctl(audio_fd, SNDCTL_DSP_CHANNELS, & channels) == -1) 
	{
		perror("SNDCTL_DSP_CHANNELS");
		return;
	}

		
	int speed = samplerate;  //44.1 KHz
	if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &speed) == -1) 
	{
		perror("SNDCTL_DSP_SPEED");
		return;
	} 
	else 
		printf("Actual Speed : %d \n",speed);

	/*  it won¡¦t be actually 44.1 KHz , codec devices usually generate the
		sampling clock by dividing the frequency of crystal the oscillator
		inside soundcard.
	*/
	
	int volume = 0x6060;	
	printf("Set SOUND_MIXER_ALTPCM ..\n");
	ioctl(mixer_fd, MIXER_WRITE(SOUND_MIXER_ALTPCM), &volume);
	
	printf("Set SOUND_MIXER_PCM ..\n");
	ioctl(mixer_fd, MIXER_WRITE(SOUND_MIXER_PCM), &volume);
	
	int music_fd;	
    signed short applicbuf[2048];         
	int count;

	if (mode == PLAY) 
	{	
		if ((music_fd = open(filename, O_RDONLY, 0)) == -1) 
		{
			perror(filename);
			return;
		}
	
		while ((count = read(music_fd, applicbuf, 2048)) > 0) 
			write(audio_fd, applicbuf, count);
	} 
	else 
	{  
		//recording
		if ((music_fd = open(filename, O_WRONLY | O_CREAT, 0)) == -1) 
		{
            perror(filename);
            return;
        }      		
 
		//set the recording source (MIC)
        int recsrc = SOUND_MASK_MIC;
      
   	    if (ioctl(mixer_fd, SOUND_MIXER_WRITE_RECSRC,&recsrc) == -1) 
   	    {
        	perror("CD");
			return;
		}
        
		/* 	recording three minutes
			
			sample format :16bit
			channel num: 2 channels
			sample rate = speed                                                             
			data rate = speed * 2 * 2	bytes/sec
		*/

		int totalbyte= speed * channels * 2 * 60 * 3;
		int totalword = totalbyte/2;
		int total = 0;

        while (total != totalword)
        {
			if(totalword - total >= 2048)
				count = 2048;
			else 
				count = totalword - total;
			
       		read(audio_fd, applicbuf, count);
            write(music_fd, applicbuf, count);
			total += count;   		
		}

		close(mixer_fd);

	}
	close(audio_fd);
	close(music_fd);
	return 0;
}
