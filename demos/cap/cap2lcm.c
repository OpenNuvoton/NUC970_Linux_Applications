/*
 *  V4L2 video capture example
 *
 *  This program can be used and distributed without restrictions.
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>             /* getopt_long() */
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>          
#include <linux/videodev2.h> 

#define CLEAR(x) memset (&(x), 0, sizeof (x))
//#define LCDRGB888
//#define LCDRGB565_1
//#define LCDRGB565_2
#define LCDPREVIEW

struct buffer {
	void * start;
	size_t length;
};

static char * dev_name = NULL;
static unsigned int dev_fmt = 0;
static int fd = -1;
static int lcmd = -1;
struct buffer * buffers = NULL;
static unsigned int n_buffers = 0;
static int bypass = 0;

#define LCD_WIDTH  800
#define LCD_HEIGHT 480

#define IN_WIDTH 640
#define IN_HEIGHT 480

unsigned long uVideoSize;
unsigned char *pVideoBuffer;
static void errno_exit(const char * s) {
	printf("%s error %d, %s\n", s, errno, strerror(errno));
	exit(EXIT_FAILURE);
}

static int xioctl(int fd, int request, void * arg) {
	int r;

	do {
		r = ioctl(fd, request, arg);
	} while (-1 == r && EINTR == errno);

	return r;
}

#define RGB565_MASK_RED 	0xF800 
#define RGB565_MASK_GREEN 0x07E0 
#define RGB565_MASK_BLUE 	0x001F 
static int read_frame(void) {	
	struct v4l2_buffer buf;	
	int error;
	int i,j,k,l;	
			
		CLEAR(buf); 	
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;

		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
			switch (errno) {
			case EAGAIN:			
				printf("errno = EAGAIN\n");
				return 0;				
			case EIO:
			default:
				errno_exit("VIDIOC_DQBUF");
			}
		}

			#ifdef LCDRGB888				
				unsigned short *pRGB16 = (unsigned short *)buffers[buf.index].start;		
				for(j=0;j<IN_HEIGHT;j++)			
				{
					l=j*LCD_WIDTH;
					for(i=0;i<IN_WIDTH;i++)	
					{
						 unsigned short RGB16 = *pRGB16; 
				     pVideoBuffer[(l+i)*4+2] = ((RGB16&RGB565_MASK_RED) >> 11)<<3;   
				     pVideoBuffer[(l+i)*4+1] = ((RGB16&RGB565_MASK_GREEN) >> 5)<<2; 
				     pVideoBuffer[(l+i)*4+0] = ((RGB16&RGB565_MASK_BLUE)<<3);				     
				     pRGB16++; 
					}				
				}
			#else
			  #if defined(LCDRGB565_2)
				memcpy(pVideoBuffer,buffers[buf.index].start,uVideoSize);
				#else
				for(j=0;j<IN_HEIGHT;j++)
				{
					memcpy(&pVideoBuffer[j*LCD_WIDTH*2],buffers[buf.index].start+j*IN_WIDTH*2,IN_WIDTH*2);
				}
				#endif
			#endif
		
		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			errno_exit("VIDIOC_QBUF");			  
	return 1;
}

static void mainloop(void) {
		while(1) {	
			read_frame();												
		}	
}

static void stop_capturing(void) {
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
			errno_exit("VIDIOC_STREAMOFF");
}

static void start_capturing(void) {
	unsigned int i;
	enum v4l2_buf_type type;
	for (i = 0; i < n_buffers; ++i) 
	{
		struct v4l2_buffer buf;		
		CLEAR(buf);		
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;		
		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			errno_exit("VIDIOC_QBUF");
	}		
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;		
	if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
		errno_exit("VIDIOC_STREAMON");
}

static void uninit_device(void) {
	int i;
  for (i = 0; i < n_buffers; ++i)  
		if (-1 == munmap(buffers[i].start, buffers[i].length))  
			errno_exit("munmap");    
  free(buffers); 
}

static void init_mmap(void) {
	
	struct v4l2_requestbuffers req;
	
#if defined(LCDPREVIEW)
  enum v4l2_buf_type type;
	struct v4l2_framebuffer preview;
#endif
	lcmd = open("/dev/fb0", O_RDWR);
#ifdef LCDRGB888
	uVideoSize = LCD_WIDTH * LCD_HEIGHT * 4;
#else
	uVideoSize = LCD_WIDTH * LCD_HEIGHT * 2;
#endif
  pVideoBuffer = mmap(NULL, uVideoSize, PROT_READ|PROT_WRITE, MAP_SHARED, lcmd, 0);
  
#if defined(LCDPREVIEW)
   preview.base=pVideoBuffer;
   preview.flags=V4L2_FBUF_FLAG_OVERLAY;
  if (-1 == xioctl(fd, VIDIOC_S_FBUF, &preview)) {
		if (EINVAL == errno) {
			printf("%s does not support memory mapping\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_S_FBUF");
		}
	}
	
	type =V4L2_BUF_TYPE_VIDEO_OVERLAY;
	if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
		errno_exit("VIDIOC_STREAMON");
	while(1);
#endif
	CLEAR(req);

	req.count = 4;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			printf("%s does not support memory mapping\n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_REQBUFS");
		}
	}
	
	if (req.count < 2) {
		printf("Insufficient buffer memory on %s", dev_name);
		exit(EXIT_FAILURE);
	}

	buffers = calloc(req.count, sizeof(*buffers));

	if (!buffers) {
		printf("Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;		
		if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");		
		buffers[n_buffers].length = buf.length;				
		buffers[n_buffers].start = mmap(NULL /* start anywhere */, 
																	  buf.length,
																		PROT_READ | PROT_WRITE /* required */,
																	  MAP_SHARED /* recommended */, 
																	  fd, 
																	  buf.m.offset);
		if (MAP_FAILED == buffers[n_buffers].start)
			errno_exit("mmap");
	}
}

static void init_device(void) {
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	unsigned int min;
	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			printf("%s is no V4L2 device/n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_QUERYCAP");
		}
	}
  	
	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		printf("%s is no video capture device/n", dev_name);
		exit(EXIT_FAILURE);
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		printf("%s does not support streaming i/o/n", dev_name);
			exit(EXIT_FAILURE);
	}
	/* Select video input, video standard and tune here. */
	CLEAR(cropcap);
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect; /* reset to default */
		if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
			switch (errno) {
			case EINVAL:
				/* Cropping not supported. */
				break;
			default:
				/* Errors ignored. */
				break;
			}
		}
	} else {
		/* Errors ignored. */
	}

	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = IN_WIDTH;
	fmt.fmt.pix.height = IN_HEIGHT;
	fmt.fmt.pix.pixelformat = dev_fmt;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
#ifndef LCDRGB888
  #if defined(LCDRGB565_2) | defined(LCDPREVIEW)
	fmt.fmt.pix.bytesperline = LCD_WIDTH*2;
	#endif
#endif
	if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
		errno_exit("VIDIOC_S_FMT");
	
	/* Buggy driver paranoia. */
	min = fmt.fmt.pix.width * 2;
#if defined(LCDRGB888) | defined(LCDRGB565_1)
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
#elif defined(LCDRGB565_2)  | defined(LCDPREVIEW)
	fmt.fmt.pix.bytesperline = LCD_WIDTH*2;
#endif
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;
  init_mmap();  
}

static void close_device(void) {
	if (-1 == close(fd))
		errno_exit("close");

	fd = -1;
}

static void open_device(void) {
	struct stat st;

	if (-1 == stat(dev_name, &st)) {
		printf("Cannot identify '%s': %d, %s/n", dev_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (!S_ISCHR(st.st_mode)) {
		printf("%s is no device/n", dev_name);
		exit(EXIT_FAILURE);
	}

	fd = open(dev_name, O_RDWR /* required */| O_NONBLOCK, 0);	

	if (-1 == fd) {
		printf("Cannot open '%s': %d, %s/n", dev_name, errno,strerror(errno));
		exit(EXIT_FAILURE);
	}
}
				
int main(int argc, char ** argv) {    
	dev_name = "/dev/video0";
	dev_fmt = V4L2_PIX_FMT_RGB565;

	open_device();
		
	init_device();
		
	start_capturing();

	mainloop();

	stop_capturing();

	uninit_device();

	close_device();

	exit(EXIT_SUCCESS);

	return 0;
}

