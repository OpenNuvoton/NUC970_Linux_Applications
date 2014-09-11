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

#include <asm/types.h>          /* for videodev2.h */

#include <linux/videodev2.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

//#define DBG printf
#define DBG

typedef enum {
	IO_METHOD_READ, IO_METHOD_MMAP, 
} io_method;

struct buffer {
	void * start;
	size_t length;
};

static char * dev_name = NULL;
static unsigned int dev_fmt = 0;
static io_method io = IO_METHOD_MMAP;
static int fd = -1;
static int lcmd = -1;
struct buffer * buffers = NULL;
static unsigned int n_buffers = 0;
static int bypass = 0;
FILE *fp;
char *filename = "test.yuv";

#define LCD_WIDTH  320
#define LCD_HEIGHT 240

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

static void process_image(const void * p, int size) {
	DBG("image size=%d\n",size);
	fwrite(p, size, 1, fp);
}

static int read_frame(void) {	
	struct v4l2_buffer buf;
	int error;
	int i;	

	switch (io) {  
    case IO_METHOD_READ:  
				while(read(fd, buffers[0].start, buffers[0].length)>0)
				{
	        if (error==-1) {  
	            switch (errno) {  
	            case EAGAIN:  
	                return 0;  
	  
	            case EIO:  
	                /* Could ignore EIO, see spec. */  
	                /* fall through */  
	            default:  
	                errno_exit("read");  
	            }  
	        }
      	}		
        process_image(buffers[0].start, buffers[0].length);  
  
        break; 
        			
		case IO_METHOD_MMAP:  
			
		CLEAR(buf); 	
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
	
		if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
			switch (errno) {
			case EAGAIN:
			{
				printf("errno = EAGAIN\n");
				return 0;
			}
	
			case EIO:
				/* Could ignore EIO, see spec. */
					/* fall through */
				default:
				errno_exit("VIDIOC_DQBUF");
			}
		}
		DBG("uVideoSize=%d\n",uVideoSize);
		if(bypass==1)
		{
			memcpy(pVideoBuffer,buffers[buf.index].start,uVideoSize);		
		}else{
			assert(buf.index < n_buffers);
			process_image(buffers[buf.index].start, buf.length);
		}
		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			errno_exit("VIDIOC_QBUF");		
		break;			  
	}
	return 1;
}

static void mainloop(void) {
	unsigned int count;

		count = 10;
		while(count) {
			fd_set fds;
			struct timeval tv;
			int r;

			FD_ZERO(&fds);
			FD_SET(fd, &fds);

			/* Timeout. */
			tv.tv_sec = 2;
			tv.tv_usec = 0;

			r = select(fd + 1, &fds, NULL, NULL, &tv);

			if (-1 == r) {
				if (EINTR == errno)
					continue;
				errno_exit("select");
			}

			if (0 == r) {
				fprintf(stderr, "select timeout/n");
				exit(EXIT_FAILURE);
			}			
			
			read_frame();					
			if(bypass==0)
			{
				DBG("count=%d\n",count);
				count--;
			}
				
			/* EAGAIN - continue select loop. */
		}	
}

static void stop_capturing(void) {
	enum v4l2_buf_type type;
	
		switch (io) 
		{  
	    case IO_METHOD_READ:  
	        /* Nothing to do. */  
	        break;  
	    case IO_METHOD_MMAP: 	
				type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
					errno_exit("VIDIOC_STREAMOFF");
					break;  
	}
}

static void start_capturing(void) {
	unsigned int i;
	enum v4l2_buf_type type;
	
	switch (io)
	{ 
		 case IO_METHOD_READ:  
		 	DBG("\nIO_METHOD_READ\n");
	        /* Nothing to do. */  
	   break;  
	   case IO_METHOD_MMAP:  
	   	DBG("\nIO_METHOD_MMAP\n");
			for (i = 0; i < n_buffers; ++i) {
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
			break;
	}
}

static void uninit_device(void) {
    unsigned int i;  
    switch (io) {  
    case IO_METHOD_READ:  
        free(buffers[0].start);  
        break;  
  
    case IO_METHOD_MMAP:  
        for (i = 0; i < n_buffers; ++i)  
            if (-1 == munmap(buffers[i].start, buffers[i].length))  
                errno_exit("munmap");  
        break;  
    }    
    free(buffers); 
}

static void init_read(unsigned int buffer_size) {  
    buffers = calloc(1, sizeof(*buffers));    
    if (!buffers) {  
        printf("Out of memory/n");  
        exit(EXIT_FAILURE);  
    }    
    buffers[0].length = buffer_size;  
    buffers[0].start = malloc(buffer_size);    
    if (!buffers[0].start) {  
        printf("Out of memory/n");  
        exit(EXIT_FAILURE);  
    }  
}  
static void init_mmap(void) {
	
	struct v4l2_requestbuffers req;
	lcmd = open("/dev/fb0", O_RDWR);
	uVideoSize = LCD_WIDTH * LCD_HEIGHT * 2;
  pVideoBuffer = mmap(NULL, uVideoSize, PROT_READ|PROT_WRITE, MAP_SHARED, lcmd, 0);
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
	DBG("reg.count=%d\n",req.count);
	DBG("reg.type=%d\n",req.type);
	DBG("reg.memory=%d\n",req.memory);
	
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
		DBG("buffers[%d].length=0x%08x\n",n_buffers,buffers[n_buffers].length);
		DBG("buffers[%d].m.offset=0x%08x\n",n_buffers,buf.m.offset);
		DBG("buffers[%d].start=0x%08x\n",n_buffers,buffers[n_buffers].start);
		if (MAP_FAILED == buffers[n_buffers].start)
			errno_exit("mmap");
	}
}
void support_format(unsigned int format)
{
  if(format & V4L2_CAP_VIDEO_CAPTURE)  				DBG("\t\t\t V4L2_CAP_VIDEO_CAPTURE\n");
  if(format & V4L2_CAP_VIDEO_OUTPUT)   				DBG("\t\t\t V4L2_CAP_VIDEO_OUTPUT\n");
  if(format & V4L2_CAP_VIDEO_OVERLAY)  				DBG("\t\t\t V4L2_CAP_VIDEO_OVERLAY\n");
  if(format & V4L2_CAP_VBI_CAPTURE)    				DBG("\t\t\t V4L2_CAP_VBI_CAPTURE\n");
  if(format & V4L2_CAP_VBI_OUTPUT)   					DBG("\t\t\t V4L2_CAP_VBI_OUTPUT\n");
  if(format & V4L2_CAP_SLICED_VBI_CAPTURE) 		DBG("\t\t\t V4L2_CAP_SLICED_VBI_CAPTURE\n");
  if(format & V4L2_CAP_SLICED_VBI_OUTPUT) 			DBG("\t\t\t V4L2_CAP_SLICED_VBI_OUTPUT\n");
  //if(format & V4L2_CAP_RDS_CAPTURE)   					DBG("\t\t\t V4L2_CAP_RDS_CAPTURE\n");
  if(format & V4L2_CAP_VIDEO_OUTPUT_OVERLAY)		DBG("\t\t\t V4L2_CAP_VIDEO_OUTPUT_OVERLAY\n");
  //if(format & V4L2_CAP_HW_FREQ_SEEK)   				DBG("\t\t\t V4L2_CAP_HW_FREQ_SEEK\n");
  //if(format & V4L2_CAP_RDS_OUTPUT)   					DBG("\t\t\t V4L2_CAP_RDS_OUTPUT\n");
  //if(format & V4L2_CAP_VIDEO_CAPTURE_MPLANE)		DBG("\t\t\t V4L2_CAP_VIDEO_CAPTURE_MPLANE\n");
  //if(format & V4L2_CAP_VIDEO_OUTPUT_MPLANE) 		DBG("\t\t\t V4L2_CAP_VIDEO_OUTPUT_MPLANE\n");
  if(format & V4L2_CAP_TUNER )    							DBG("\t\t\t V4L2_CAP_TUNER \n");
  if(format & V4L2_CAP_AUDIO)    							DBG("\t\t\t V4L2_CAP_AUDIO\n");
  if(format & V4L2_CAP_RADIO)    							DBG("\t\t\t V4L2_CAP_RADIO\n");
  //if(format & V4L2_CAP_MODULATOR)   						DBG("\t\t\t V4L2_CAP_MODULATOR\n");
  if(format & V4L2_CAP_READWRITE)   						DBG("\t\t\t V4L2_CAP_READWRITE\n");
  if(format & V4L2_CAP_ASYNCIO)    						DBG("\t\t\t V4L2_CAP_ASYNCIO\n");
  if(format & V4L2_CAP_STREAMING)   						DBG("\t\t\t V4L2_CAP_STREAMING\n");  	
  	
}
static void init_device(void) {
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	unsigned int min;

	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device/n", dev_name);
			exit(EXIT_FAILURE);
		} else {
			errno_exit("VIDIOC_QUERYCAP");
		}
	}
	DBG("cap.driver=%s\n",cap.driver);
	DBG("cap.card=%s\n",cap.card);
	DBG("cap.bus_info=%s\n",cap.bus_info);
	DBG("cap.version=%d\n",cap.version);
	DBG("cap.capabilities=%d\n",cap.capabilities);
  support_format(cap.capabilities);
  	
	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		printf("%s is no video capture device/n", dev_name);
		exit(EXIT_FAILURE);
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		DBG("%s does not support streaming i/o/n", dev_name);
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
	fmt.fmt.pix.width = 320;
	fmt.fmt.pix.height = 240;
	fmt.fmt.pix.pixelformat = dev_fmt;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
		errno_exit("VIDIOC_S_FMT");


	DBG("fmt.type=%d\n",fmt.type);
	DBG("fmt.fmt.pix.width=%d\n",fmt.fmt.pix.width);
	DBG("fmt.fmt.pix.height=%d\n",fmt.fmt.pix.height);	
	DBG("fmt.fmt.pix.pixelformat=0x%08x\n",fmt.fmt.pix.pixelformat);
	DBG("fmt.fmt.pix.field=%d\n",fmt.fmt.pix.field);
	/* Note VIDIOC_S_FMT may change width and height. */

	/* Buggy driver paranoia. */
	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;
	switch (io) {  
    case IO_METHOD_READ:  
        init_read(fmt.fmt.pix.sizeimage);  
        break;  
  
    case IO_METHOD_MMAP:  
        init_mmap();  
        break;      
    }  	
}

static void close_device(void) {
	if (-1 == close(fd))
		errno_exit("close");

	fd = -1;
}

static void open_device(void) {
	struct stat st;

	if (-1 == stat(dev_name, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s/n", dev_name, errno,
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device/n", dev_name);
		exit(EXIT_FAILURE);
	}

	fd = open(dev_name, O_RDWR /* required */| O_NONBLOCK, 0);

	if (-1 == fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s/n", dev_name, errno,
				strerror(errno));
		exit(EXIT_FAILURE);
	}
}
static void usage(FILE * fp, int argc, char ** argv) {  
    fprintf(fp, "Usage: %s [options]\n\n"  
            "Options:\n"  
            "-d | --device name   Video device name [/dev/video]\n"  
            "-h | --help          Print this message\n"  
            "-m | --mmap          Use memory mapped buffers\n"  
            "-r | --read          Use read() calls\n"  
            "-u | --userp         Use application allocated buffers\n"  
            "-f | --fmt           Output format(yuv422,rgb555,rgb565,grey,yuv422p,yuv411p\n"            
            "-b | --bypass        Capture image data to LCM \n"
            "", argv[0]);  
}  

static const char short_options[] = "d:hmrf:b";

static const struct option long_options[] = { 
		{ "device", required_argument, NULL, 'd' }, 
		{ "help",   no_argument,       NULL, 'h' }, 
		{ "mmap",   no_argument,       NULL, 'm' }, 
		{ "read",   no_argument,       NULL, 'r' }, 		
		{ "format", required_argument, NULL, 'f' },
		{ "bypass", no_argument,       NULL, 'b' },
		{ 0, 0, 0, 0 } };
			
void setting_options(int argc, char ** argv)
{
	int i;
 for (;;) {  
        int index;  
        int c;  
  
        c = getopt_long(argc, argv, short_options, long_options, &index);  
        if (-1 == c)  
            break;  
  
        switch (c) {  
        case 0: /* getopt_long() flag */  
            break;  
  
        case 'd':  
            dev_name = optarg;  
            break;  
  
        case 'h':  
            usage(stdout, argc, argv);  
            exit(EXIT_SUCCESS);  
  
        case 'm':  
            io = IO_METHOD_MMAP;  
            break;  
  
        case 'r':  
            io = IO_METHOD_READ;  
            break;  
             
        case 'f':              		
						if(strcmp("yuv422",  optarg)==0) dev_fmt=V4L2_PIX_FMT_YUYV;
            if(strcmp("rgb555",  optarg)==0) dev_fmt=V4L2_PIX_FMT_RGB555;
            if(strcmp("rgb565",  optarg)==0) dev_fmt=V4L2_PIX_FMT_RGB565;
            if(strcmp("grey",    optarg)==0) dev_fmt=V4L2_PIX_FMT_GREY;
            if(strcmp("yuv422p", optarg)==0) dev_fmt=V4L2_PIX_FMT_YUV422P;
            if(strcmp("yuv411p", optarg)==0) dev_fmt=V4L2_PIX_FMT_YUV411P;            
            break; 
        case 'b':
        		bypass = 1;
        		break;
        default:  
            usage(stderr, argc, argv);  
            exit(EXIT_FAILURE);  
        }  
    }  	
}			

int main(int argc, char ** argv) {    
	dev_name = "/dev/video0";
	dev_fmt = V4L2_PIX_FMT_YUYV;
	io = IO_METHOD_MMAP;  
			
	setting_options(argc,argv);

	open_device();
		
	init_device();
		
	start_capturing();

	fp = fopen(filename, "wa+");
	mainloop();
	
	fclose(fp);

	stop_capturing();

	uninit_device();

	close_device();

	exit(EXIT_SUCCESS);

	return 0;
}

