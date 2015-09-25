#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
#include <asm/ioctl.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/fb.h>
#include <unistd.h>
#include <dirent.h>

#include "nuc970_jpeg.h"

#define TEST_NORMAL_DECODE        1
#define TEST_NORMAL_ENCODE        2
#define TEST_DECODE_DOWNSCALE     3
#define TEST_ENCODE_UPSCALE       4
#define TEST_DECODE_OUTPUTWAIT    7
#define TEST_ENCODE_RESERVED      8 
#define TEST_DECODE_DOWNSCALE_FB  9
 
#define RETRY_NUM                 100
#define EXIF_SIZE                 0x84


#define NUC970_VA_JPEG 	   	      (0xF000A000)

#define JPEG_DEMO_DIR    "/mnt/mmcblk0p1"


jpeg_param_t jpeg_param;
static int fb_fd=0;
int fd;
static struct fb_var_screeninfo var;
static char *fbdevice = "/dev/fb0";
unsigned char *pVideoBuffer, *backupVideoBuffer;
unsigned int fb_paddress;
__u32 xres, yres,jpeg_buffer_size;
__u32 appOffset[10], appSize[10], appNUm;
jpeg_info_t *jpeginfo;  
unsigned char *pJpegBuffer = NULL, *pSRCbuffer = NULL, *pJpegSWaddr = NULL; 
char device[] = "/dev/video0";
unsigned long uVideoSize;

__u8 g_qtflag = 0;

__u8 g_au8QT0[64] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
      g_au8QT1[64] = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};


static       int       s_ai32RToY[256];
static       int       s_ai32GToY[256];
static       int       s_ai32BToY[256];
static       int       s_ai32RToCb[256];
static       int       s_ai32GToCb[256];
static       int       s_ai32BToCb[256];
static       int       s_ai32RToCr[256];
static       int       s_ai32GToCr[256];
static       int       s_ai32BToCr[256];

extern void create_simple_EXIF(char *Buffer, int thumbnail_offset,int thumbnail_size);

int Write2File(char *filename, char *buffer, int len)
{
    int fd;
    unsigned long totalcnt, wlen;
    int ret = 0;
    
    fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC);
    if (fd < 0)
    {
        ret = -1;
        printf("Write2File - open file fail\n");
        goto out;
    }

    totalcnt = 0;
    while (totalcnt < len)
    {
        wlen = write(fd, buffer + totalcnt, len - totalcnt);
        if(wlen < 0)
        {
            printf("Write2File - write file error, errno=%d, len=%x\n", errno, wlen);
            ret = -1;
            break;
        }
        totalcnt += wlen;
    }
    close(fd);
out:    
    return ret;
}

int ParsingJPEG(unsigned char *Buffer, int Length, int *Width, int *Height)
{
    //HByte,LByte:For JPEG Marker decode
    //MLength:Length of Marker (all data in the marker)

    unsigned char HByte,LByte;
    unsigned short int MLength;     
    int index;      
    appNUm = 0;
    index = 0;          

    while(index < Length)
    {
        HByte = Buffer[index++];

        if (HByte == 0xFF)
        {
            LByte = Buffer[index++];
            
            switch(LByte)
            {
                case 0xD8:      /* SOI Marker (Start Of Image) */
                    break;
                case 0xC1:
                case 0xC2:
                case 0xC3:
                case 0xC5:
                case 0xC6:
                case 0xC7:
                case 0xC8:
                case 0xC9:
                case 0xCA:
                case 0xCB:
                case 0xCD:
                case 0xCE:
                case 0xCF:  /* SOF Marker(Not Baseline Mode) */
                    return -1;
                    break;
                
                case 0xDB:      /* DQT Marker (Define Quantization Table) */
                    if (index + 2 > Length)
                        return -1;
                    HByte = Buffer[index++];
                    LByte = Buffer[index++];
                    MLength = (HByte << 8) + LByte - 2;
                        
                    if (MLength != 65)  /* Has more than 1 Quantization Table */
                    {
                        if (MLength % 65)
                            return -1;
                        index += MLength;           /* Skip Data */
                    }
                    else    /* Maybe has 1 Quantization Table */
                        index += 64;
                        
                    if (index > Length)
                        return -1;
                    break;
                    
                case 0xC0:      /* SOF Marker (Baseline mode Start Of Frame) */                 
                    if(index + 7 > Length)
                        return -1;
                    HByte = Buffer[index++];
                                LByte = Buffer[index++];
                    MLength = (HByte << 8) + LByte;                     
                    index++;    /* Sample precision */                      
                                HByte = Buffer[index++];
                                LByte = Buffer[index++];
                    *Height = (HByte << 8) + LByte;                                         
                                HByte = Buffer[index++];
                                LByte = Buffer[index++];
                    *Width = (HByte << 8) + LByte;  
                    return 0;
                    break;
                
                case 0xDA:      /* Scan Header */
                    if (index + 2 > Length)
                        return -1;
                    HByte = Buffer[index++];
                    LByte = Buffer[index++];
                    MLength = (HByte << 8) + LByte;
                                index += MLength - 2;       /* Skip Scan Header Data */ 
                       
                    if (index > Length)
                        return -1;
                    break;
                
                case 0xC4:      /* DHT Marker (Define Huffman Table) */
                    if (index + 2 > Length)
                        return -1;
                    HByte = Buffer[index++];
                    LByte = Buffer[index++];
                    MLength = (HByte << 8) + LByte;
                    index += MLength - 2;
                    if (index > Length)
                        return -1;
                                break;
                
                case 0xE0:
                case 0xE1:
                case 0xE2:
                case 0xE3:
                case 0xE4:
                case 0xE5:
                case 0xE6:
                case 0xE7:
                case 0xE8:
                case 0xE9:
                case 0xEA:
                case 0xEB:
                case 0xEC:
                case 0xED:
                case 0xEE:
                case 0xEF:
                case 0xFE:      /*  Application Marker && Comment */
                    if (index + 2 > Length)
                        return -1;                      
                    HByte = Buffer[index++];
                    LByte = Buffer[index++];

                    MLength = (HByte << 8) + LByte;                 
                    appOffset[appNUm] = index - 4;
                    appSize[appNUm] = MLength + 2;
                    appNUm++;
                    index += MLength - 2;       /* Skip Application or Comment Data */                          
                    if (index > Length)
                        return -1;      
                    break;
			}
        }
    }
    return -1;      // Wrong file format
}


int jpegCodec(int mode)
{
    unsigned long BufferSize, bufferCount,readSize;
    int enc_reserved_size;
    int ret = 0;
    int i,len, jpeginfo_size;   
    int width,height, parser;
    FILE *fp;

    memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
    jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
    jpeginfo = malloc(jpeginfo_size);
    
    if (mode == TEST_NORMAL_DECODE || mode == TEST_DECODE_DOWNSCALE || mode == TEST_DECODE_OUTPUTWAIT || mode == TEST_DECODE_DOWNSCALE_FB)   /* Decode operation Test */
    {       
        char *filename = "./jpegDec.jpg";
        char *filenameOPW = "./jpegOPW.jpg";
        int DecOPWbuffersize;

        jpeg_param.encode = 0;          /* Decode Operation */
        jpeg_param.src_bufsize = 100*1024;  /* Src buffer size (Bitstream buffer size for JPEG engine) */
        jpeg_param.dst_bufsize = 640*480*2; /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
        jpeg_param.decInWait_buffer_size = 0;   /* Decode input Wait buffer size (Decode input wait function disable when                                  decInWait_buffer_size is 0) */
        jpeg_param.decopw_en = 0;
        jpeg_param.windec_en = 0;

        if (mode == TEST_DECODE_DOWNSCALE || mode == TEST_DECODE_DOWNSCALE_FB)   /* Decode Downscale */
        {
            jpeg_param.scale = 1;       /* Enable scale function */
            jpeg_param.scaled_width = 320;  /* width after scaling */
            jpeg_param.scaled_height = 240; /* height after scaling */
            jpeg_param.dec_stride = xres;   /* Enable stride function */            
        }
        else if (mode == TEST_DECODE_OUTPUTWAIT)
        {
            jpeg_param.src_bufsize = 600*1024;  /* Src buffer size (Bitstream buffer size for JPEG engine) */
            jpeg_param.dst_bufsize = 200*1024;  /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
            jpeg_param.decInWait_buffer_size = 0;   /* Decode input Wait buffer size (Decode input wait function disable when
                                   decInWait_buffer_size is 0) */
            jpeg_param.decopw_en = 1;
            jpeg_param.scale = 0;       /* Scale function is disabled when scale is 0 */
        }
        else                    /* Normal Decode */
        {
            jpeg_param.dec_stride = 0;  /* Stride function is disabled when dec_stride is 0 */
            jpeg_param.scale = 0;       /* Scale function is disabled when scale is 0 */
        }

        /* Total buffer size for JPEG engine */

        BufferSize = (jpeg_param.src_bufsize + jpeg_param.dst_bufsize);

        if (BufferSize > jpeg_buffer_size)
        {
        	printf("Required buffer size is 0x%x, but JPEG buffer size is 0x%x\n", BufferSize, jpeg_buffer_size);
            printf("JPEG Engine Buffer isn't enough. Please enlarge JPEG buffer via menuconfig setting.\n");
            goto out;
        }

        /* Clear buffer */
        if (mode == TEST_DECODE_DOWNSCALE || mode == TEST_DECODE_DOWNSCALE_FB)
        {
            memset(pJpegBuffer, 0x77, BufferSize);  
            memset(pVideoBuffer, 0x77, BufferSize); 
        }
        
        /* Open JPEG file */
        if (mode == TEST_DECODE_OUTPUTWAIT)
            fp = fopen(filenameOPW, "r+");
        else
            fp = fopen(filename, "r+");
        
        if (fp == NULL)
        {
                printf("open %s error!\n", fp);
                return 0;
        }

        pSRCbuffer = pJpegBuffer;
        bufferCount = 0;
        parser = 0;
        printf("JPEG Header Parser:\n");
        
        /* Read Bitstream to JPEG engine src buffer */
        while (!feof(fp))    
        {   
            fd_set writefds;
            struct timeval tv;
            int result;
            tv.tv_sec       = 0;
            tv.tv_usec      = 0;
            FD_ZERO( &writefds );
            FD_SET( fd , &writefds );
            tv.tv_sec       = 0;
            tv.tv_usec      = 0;
        
            select( fd + 1 , NULL , &writefds , NULL, &tv );
            if ( FD_ISSET( fd, &writefds ))
            {                   
                readSize = fread(pSRCbuffer, 1, 4096 , fp);
                pSRCbuffer += readSize; 
                bufferCount += readSize;
            }  
             
            if (!parser)
            {
                result = ParsingJPEG(pJpegBuffer, bufferCount, &width, &height);
                if(!result)
                {
                    printf("\t->Width %d, Height %d\n", width,height);
                    parser = 1;
                }
                else
                    printf("\t->Can't get image siz in %5d byte bistream\n", bufferCount);
            }

            if ( bufferCount > jpeg_param.src_bufsize)
            {
                printf("Bitstream size is larger than src buffer, %d!!\n",bufferCount);         
                return 0;
            }
        }
        printf("Bitstream is 0x%X Bytes\n",bufferCount);

        if (bufferCount % 4)
            bufferCount = (bufferCount & ~0x3) + 4; 

        printf("Set Src Buffer is 0x%X Bytes\n",bufferCount);
        
        jpeg_param.src_bufsize = bufferCount;   /* Src buffer size (Bitstream buffer size for JPEG engine) */
        jpeg_param.dst_bufsize = BufferSize - bufferCount;  /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
                
        if (mode == TEST_DECODE_OUTPUTWAIT)
        {   
            if(width % 2)
                width = (width & 0xFFFFFFFE) + 2; 

            jpeg_param.dec_stride = width;  /* Set Decode Output width */   

            /* Buffer for target image */   
            DecOPWbuffersize = width * height * 4;
 
            pJpegSWaddr = malloc(sizeof(char) * DecOPWbuffersize);

            jpeg_param.decopw_TargetBuffersize = DecOPWbuffersize;

            if(pJpegBuffer < 0)
            {
                printf("Malloc Failed!\n");
                ret = -1;
                goto out;
            }
            jpeg_param.decopw_vaddr = (__u32)pJpegSWaddr;   
            printf("jpeg_param.decopw_vaddr 0x%X\n", jpeg_param.decopw_vaddr );

        }

        jpeg_param.buffersize = 0;      /* only for continuous shot */
        jpeg_param.buffercount = 1;

        /* Set decode output format: RGB555/RGB565/RGB888/YUV422/PLANAR_YUV */
        jpeg_param.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_RGB888;        
        
        if (mode == TEST_DECODE_DOWNSCALE_FB)
        {
            __u32 u32OutputOffset = 0;          
            u32OutputOffset = (__u32)((xres * ((__u32)(yres - 240) / 2))) + (__u32)((xres - 320) / 2);  
            if(jpeg_param.decode_output_format == DRVJPEG_DEC_PRIMARY_PACKET_RGB888)
                u32OutputOffset *= 4;   
            else
                u32OutputOffset *= 2;
            jpeg_param.paddr_dst = fb_paddress + u32OutputOffset;
            jpeg_param.vaddr_dst = (__u32)pVideoBuffer + u32OutputOffset;
        }

        /* Set operation property to JPEG engine */
        if ((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
        {
            fprintf(stderr,"set jpeg param failed:%d\n",errno);
            ret = -1;
            goto out;
        }       

        
        /* Trigger JPEG engine */
        if ((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
        {
            fprintf(stderr,"trigger jpeg failed:%d\n",errno);
            ret = -1;
            goto out;
        }
        
        /* Get JPEG decode information */
        len = read(fd, jpeginfo, jpeginfo_size);

        if (len<0) 
        {
            fprintf(stderr, "read data error errno=%d\n", errno);
            ret = -1;
            goto out;
        }
        printf("JPEG: Width %d, Height %d\n",jpeginfo->width, jpeginfo->height);

        if (jpeginfo->state == JPEG_DECODED_IMAGE)
        {
            printf("Decode Complete\n");
            if (mode == TEST_DECODE_DOWNSCALE)       
            {       
                printf("Stride %d\n", jpeginfo->dec_stride);
                printf("Output size is %d x %d\n", xres, jpeginfo->height);
                memcpy((void*)pVideoBuffer, (char*)(pJpegBuffer + jpeg_param.src_bufsize), jpeginfo->image_size[0]);
                ret = Write2File("./DecodeDownscale.dat", pJpegBuffer + jpeg_param.src_bufsize, 
                    jpeginfo->image_size[0]);
            }
            else if (mode == TEST_DECODE_DOWNSCALE_FB)
            {       
                printf("Stride %d\n", jpeginfo->dec_stride);
                printf("Output size is %d x %d\n", xres, yres);
                ret = Write2File("./DecodeDownscaleFB.dat", pVideoBuffer,uVideoSize);
            }
            else if(mode == TEST_DECODE_OUTPUTWAIT) 
            {   
                printf("Output size is %d x %d\n", jpeginfo->width, jpeginfo->height);
                    ret = Write2File("./DecodeOPW.dat", (unsigned char *)((__u32)pJpegSWaddr), jpeginfo->image_size[0]);
            }   
            else    
            {   
                printf("Output size is %d x %d\n", jpeginfo->width, jpeginfo->height);
                    ret = Write2File("./NormalDecode.dat", pJpegBuffer + jpeg_param.src_bufsize, 
                    jpeginfo->image_size[0]);
            
            }
                if(ret < 0)
            {
                printf("write to file, errno=%d\n", errno);
            }
        }
        else if (jpeginfo->state == JPEG_DECODE_ERROR)
            printf("Decode Error\n");
        else if (jpeginfo->state == JPEG_MEM_SHORTAGE)
            printf("Memory Shortage\n");    
        else if (jpeginfo->state == JPEG_DECODE_PARAM_ERROR)
            printf("Decode Parameter Error\n");         
        
    }
    else        /* Encode operation Test */
    {
        char *filename = "./jpegEnc.dat";

        jpeg_param.encode = 1;          /* Encode */
        jpeg_param.src_bufsize = 640*480*2; /* Src buffer (Raw Data) */
        jpeg_param.dst_bufsize = 100*1024;  /* Dst buffer (Bitstream) */
        jpeg_param.encode_width = 640;      /* JPEG width */
        jpeg_param.encode_height = 480;     /* JPGE Height */
        jpeg_param.encode_source_format = DRVJPEG_ENC_SRC_PACKET;   /* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
        jpeg_param.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;    /* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

        jpeg_param.buffersize = 0;      /* only for continuous shot */
        jpeg_param.buffercount = 1;     

        if (mode == TEST_ENCODE_RESERVED)
        {
            __u32 u32BitstreamAddr,i;
            __u8 *u8Ptr;
            /* Reserve memory space for user application 
               # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically. 
               # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space). 
               # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530).     
               # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
                Byte 0 & 1 :  FF D8
                Byte 2 & 3 :  FF E0
                Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
                Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
           
                Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0); 
                        FF D8 FF E0 04 02 [1024 bytes]         
            */
            printf("Enable Reserved space function\n");
            enc_reserved_size = 1024;       /* JPGE Reserved size for user application */
            printf("Reserved space size %d\n", enc_reserved_size);
            u32BitstreamAddr = (__u32) pJpegBuffer + jpeg_param.src_bufsize;        /* Bistream Address */          
            u8Ptr =(__u8 *) (u32BitstreamAddr + 6);     /* Pointer for Reserved memory */    

            for(i=0;i<enc_reserved_size;i++)    /* Set data to Reserved Buffer */
                *(u8Ptr + i) = i & 0xFF;
            
            ioctl(fd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);
            printf("Bitstream address : 0x%X\n", u32BitstreamAddr);
            printf("Reserved memory : 0x%X~ 0x%X\n", u32BitstreamAddr+ 6, (u32BitstreamAddr + 6 + enc_reserved_size));          
        }

        if (mode == TEST_ENCODE_UPSCALE)    /* Upscale function */
        {
            jpeg_param.scaled_width = 800;  /* Width after scaling */
            jpeg_param.scaled_height = 600; /* Height after scaling */
            jpeg_param.scale = 1;       /* Enable scale function */ 
        }
        else
            jpeg_param.scale = 0;       /* Scale function is disabled when scale 0 */       

        if (g_qtflag)
        {
            ioctl(fd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
            ioctl(fd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
            ioctl(fd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
        }
        else
            ioctl(fd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

        /* Set operation property to JPEG engine */
        if ((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
        {
            fprintf(stderr,"set jpeg param failed:%d\n",errno);
            ret = -1;
            goto out;
        }
        
        /* Total buffer size for JPEG engine */
        BufferSize = (jpeg_param.src_bufsize + jpeg_param.dst_bufsize);

        if (BufferSize > jpeg_buffer_size)
        {
        	printf("Required buffer size is 0x%x, but JPEG buffer size is 0x%x\n", BufferSize, jpeg_buffer_size);
            printf("JPEG Engine Buffer isn't enough. Please enlarge JPEG buffer via menuconfig setting.\n");
            goto out;
        }
                
        /* Open JPEG file */
        fp = fopen(filename, "r+");
        
        if (fp == NULL)
        {
            printf("open %s error!\n", fp);
            return 0;
        }

        pSRCbuffer = pJpegBuffer;
        bufferCount = 0;
    
        /* Read Bitstream to JPEG engine src buffer */
        while (!feof(fp))    
        {   
            fd_set writefds;
            struct timeval tv;
            tv.tv_sec       = 0;
            tv.tv_usec      = 0;
            FD_ZERO( &writefds );
            FD_SET( fd , &writefds );
            tv.tv_sec       = 0;
            tv.tv_usec      = 0;
        
            select( fd + 1 , NULL , &writefds , NULL, &tv );
            
            if ( FD_ISSET( fd, &writefds ))
            {                   
                readSize = fread(pSRCbuffer, 1, 4096 , fp);
        
                pSRCbuffer += readSize; 
                bufferCount += readSize;
            }

            if ( bufferCount > jpeg_param.src_bufsize)
            {
                printf("Raw Data size is larger than src buffer, %d!!\n",bufferCount);          
                return 0;
            }
        }

        /* Trigger JPEG engine */
        if ((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
        {
            fprintf(stderr,"trigger jpeg failed:%d\n",errno);
            ret = -1;

            goto out;
        }

        /* Get JPEG Encode information */

        len = read(fd, jpeginfo, jpeginfo_size);
        printf("Encode Complete\n");
        if (len < 0) 
        {
            fprintf(stderr, "No data can get\n");
            if (jpeginfo->state == JPEG_MEM_SHORTAGE)
                printf("Memory Stortage\n");    
            ret = -1;
            goto out;
        }
        
        if (mode == TEST_ENCODE_UPSCALE)
            ret = Write2File("./EncodeUpscale.jpg", pJpegBuffer + jpeg_param.src_bufsize, 
                jpeginfo->image_size[0]);       
        else if (mode == TEST_ENCODE_RESERVED)
        {
            __u32 u32BitstreamAddr, appMarkerSize;
            __u8 *u8BistreamPtr;
            printf("Reserved space information from bitstream\n");
            u8BistreamPtr = pJpegBuffer + jpeg_param.src_bufsize;   /* Bistream Address */      
            appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
            printf("App Marker size (Byte 4 & Byte 5) : %d\n",appMarkerSize);
            printf("Reserved size (App Marker size -2): %d\n",appMarkerSize - 2); 
            printf("Reserved buffer : Byte 6 ~ %d\n", (6 + appMarkerSize - 2));

            ret = Write2File("./NormalEncodeReserved.jpg", pJpegBuffer + jpeg_param.src_bufsize, 
                jpeginfo->image_size[0]);               

        }
        else
            ret = Write2File("./NormalEncode.jpg", pJpegBuffer + jpeg_param.src_bufsize, 
                jpeginfo->image_size[0]);
        if (ret < 0)
        {
            printf("write to file, errno=%d\n", errno);
        }
    }
out:
    free(jpeginfo);
    if (mode == TEST_DECODE_OUTPUTWAIT)
        free(pJpegSWaddr);
    return 0;

}



int jpegCodecIPW(int mode)
{
    unsigned long BufferSize,BitstreamCount, readSize;
    int ret = 0;
    int i,len, jpeginfo_size;
    __u32 jpeg_buffersize,jpeg_buffer_state;
    jpeg_state_t state;
    char *filename = "./jpegIPW.jpg";   
    FILE *fp;
    char *buffer;
    __u32 bTrigger;

    memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
    jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
    jpeginfo = malloc(jpeginfo_size);
    
    jpeg_param.encode = 0;              /* Decode Operation */
    jpeg_param.src_bufsize = 16*1024;       /* Src buffer size (Bitstream buffer size for JPEG engine) */
    jpeg_param.dst_bufsize = 640*480*2;     /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
    jpeg_param.decInWait_buffer_size = 16 * 1024;   /* Decode input Wait buffer size (Decode input wait function disable when                                  decInWait_buffer_size is 0) */
    
    /* Open JPEG file */
    fp = fopen(filename, "r+");
        
    if (fp == NULL)
    {
            printf("open %s error!\n", fp);
            return 0;
    }
    
    /* allocate tmp buffer for file read */
    buffer = (char *)malloc(jpeg_param.decInWait_buffer_size);

    if (mode == TEST_DECODE_DOWNSCALE)
    {
        jpeg_param.scale = 1;       /* Enable scale function */
        jpeg_param.scaled_width = 320;  /* width after scaling */
        jpeg_param.scaled_height = 240; /* height after scaling */
        jpeg_param.dec_stride = xres;   /* Enable stride function */
    }
    else
    {
        jpeg_param.dec_stride = 0;  /* Stride function is disabled when dec_stride is 0 */
        jpeg_param.scale = 0;       /* Scale function is disabled when scale is 0 */
    }
    jpeg_param.buffersize = 0;//only for continuous shot
    jpeg_param.buffercount = 1;

    /* Set decode output format: RGB555/RGB565/RGB888/YUV422 */
    jpeg_param.decode_output_format =DRVJPEG_DEC_PRIMARY_PACKET_RGB888;
    
    /* Set operation property to JPEG engine */
    if ((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
    {
        fprintf(stderr,"set jpeg param failed:%d\n",errno);
        ret = -1;
        goto out;
    }
    
    /* Total buffer size for JPEG engine */
    BufferSize = (jpeg_param.src_bufsize + jpeg_param.dst_bufsize);

    if(BufferSize > jpeg_buffer_size)
    {
      	printf("Required buffer size is 0x%x, but JPEG buffer size is 0x%x\n", BufferSize, jpeg_buffer_size);
        printf("JPEG Engine Buffer isn't enough. Please enlarge JPEG buffer via menuconfig setting.\n");
        goto out;
    }

    /* Clear buffer */
    if(mode == TEST_DECODE_DOWNSCALE)
    {
        memset(pJpegBuffer, 0x77, BufferSize);  
        memset(pVideoBuffer, 0x77, BufferSize); 
    }
    
    bTrigger = 0;       /* For flow control */
    BitstreamCount = 0; /* Bitstream count for application */
    readSize = 0;

    printf("Bitstream Buffer for Decode Input Wait is %dKB\n",jpeg_param.src_bufsize/1024);

    while (!feof(fp) || state != JPEG_DECODED_IMAGE || state != JPEG_MEM_SHORTAGE)   
    {   
        /* Check Buffer status */           
        do
        {         
            /* Get Decode Input Wait Buffer State */
            ioctl(fd , JPEG_DECIPW_BUFFER_STATE , &jpeg_buffer_state);
            /* Get Decode state */
            ioctl(fd, JPEG_STATE, (__u32)&state);       
        } while(jpeg_buffer_state != JPEG_DECIPW_BUFFER_EMPTY  && state != JPEG_DECODED_IMAGE && state != JPEG_MEM_SHORTAGE);
                
        if (state == JPEG_DECODED_IMAGE || state == JPEG_MEM_SHORTAGE)   /* Decode Complete!! */
            break;

        /* Get available Buffer size */
        if ((ioctl(fd, JPEG_G_DECIPW_BUFFER_SIZE, &jpeg_buffersize)) < 0)
        {
            fprintf(stderr,"set jpeg param failed:%d\n",errno);
            ret = -1;
            goto out;
        }
        
        fd_set writefds;
        struct timeval tv;
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
        FD_ZERO( &writefds );
        FD_SET( fd , &writefds );
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
        
        select( fd + 1 , NULL , &writefds , NULL, &tv );
        if ( FD_ISSET( fd, &writefds ))
        {   
            /* Read "jpeg_buffersize" Bytes Bitstream from file to tmp buffer "buffer" */
            readSize = fread(buffer, 1, jpeg_buffersize, fp);
            
            if (readSize != 0)
            {
                /* Write "jpeg_buffersize" Bytes Bitstream from tmp buffer "buffer" to JPEG engine buffer */
                write(fd, buffer, readSize);  

                BitstreamCount = BitstreamCount + readSize; 
                printf("Fill %5d bytes to JPEG engine, ",readSize);
                printf("Bitstream in JPEG engine %6d Bytes\n",BitstreamCount);
            }
        }           
    
        if (!bTrigger)
        {
            /* Trigger JPEG engine */
            if ((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
            {
                fprintf(stderr,"trigger jpeg failed:%d\n",errno);
                ret = -1;
                goto out;
            }   
            bTrigger = 1;   
        }
        else
            ioctl(fd, JPEG_DECODE_RESUME, NULL);    /* Resume the JPEG decode operation */
    }

    /* Get JPEG decode information */
    len = read(fd, jpeginfo, jpeginfo_size);

    if (len < 0) 
    {
        fprintf(stderr, "read data error errno=%d\n", errno);
        ret = -1;
        goto out;
    }
    printf("JPEG: Width %d, Height %d\n",jpeginfo->width, jpeginfo->height);

    if (jpeginfo->state == JPEG_DECODED_IMAGE)
    {
        printf("Decode Complete\n");
        if (mode == TEST_DECODE_DOWNSCALE)       
        {       
            printf("Stride %d\n", jpeginfo->dec_stride);
            memcpy((void*)pVideoBuffer, (char*)(pJpegBuffer + jpeg_param.src_bufsize), jpeginfo->image_size[0]);        
            printf("Output Size is %d x %d\n",jpeginfo->dec_stride, jpeginfo->height);
            ret = Write2File("./decIPW_DecodeDownscale.dat", pJpegBuffer + jpeg_param.src_bufsize, 
                jpeginfo->image_size[0]);
        }
        else    
        {
            printf("Output Size is %d x %d\n",jpeginfo->width, jpeginfo->height);   
                ret = Write2File("./decIPW_NormalDecode.dat", pJpegBuffer + jpeg_param.src_bufsize,jpeginfo->image_size[0]);
        }
        
        if (ret < 0)
            printf("write to file, errno=%d\n", errno);
    }
    else if (jpeginfo->state == JPEG_DECODE_ERROR)
        printf("Decode Error\n");
    else if (jpeginfo->state == JPEG_MEM_SHORTAGE)
        printf("Memory Shortage\n");            
    else if (jpeginfo->state == JPEG_DECODE_PARAM_ERROR)
        printf("Decode Parameter Error\n"); 
        
out:
    fclose(fp);
    free(buffer);
    free(jpeginfo);

    return 0;

}
 

int jpegCodecIOPW(void)
{
    unsigned long BufferSize,BitstreamCount, bufferCount, readSize;
    int ret = 0;
    int i,len, jpeginfo_size;
    __u32 jpeg_buffersize,jpeg_buffer_state;
    jpeg_state_t state;
    char *filename = "./jpegIOPW.jpg";  
    FILE *fp;
    char *buffer;
    __u32 bTrigger;
    int width,height, parser;
    int DecOPWbuffersize;

    memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
    jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
    jpeginfo = malloc(jpeginfo_size);
    
    jpeg_param.encode = 0;              /* Decode Operation */
    jpeg_param.src_bufsize = 16*1024;       /* Src buffer size (Bitstream buffer size for JPEG engine) */
    jpeg_param.dst_bufsize = 200*1024;      /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
    jpeg_param.decInWait_buffer_size = 16 * 1024;   /* Decode input Wait buffer size (Decode input wait function disable when                                  decInWait_buffer_size is 0) */   
    jpeg_param.decopw_en = 1;
    jpeg_param.scale = 0;       /* Scale function is disabled when scale is 0 */
    jpeg_param.buffersize = 0;//only for continuous shot
    jpeg_param.buffercount = 1;

    /* Open JPEG file */
    fp = fopen(filename, "r+");
        
    if(fp == NULL)
        {
            printf("open %s error!\n", fp);
            return 0;
    }

    pSRCbuffer = pJpegBuffer;
    bufferCount = 0;
    parser = 0;
    printf("JPEG Header Parser:\n");
    /* Read Bitstream to JPEG engine src buffer */
    while (!feof(fp))    
    {   
        fd_set writefds;
        struct timeval tv;
        int result;
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
        FD_ZERO( &writefds );
        FD_SET( fd , &writefds );
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
    
        select( fd + 1 , NULL , &writefds , NULL, &tv );
        if ( FD_ISSET( fd, &writefds ))
        {                   
            readSize = fread(pSRCbuffer, 1, 4096 , fp);
            pSRCbuffer += readSize; 
            bufferCount += readSize;
        }   
        if (!parser)
        {
            result = ParsingJPEG(pJpegBuffer, bufferCount, &width, &height);
            if(!result)
            {
                printf("\t->Width %d, Height %d\n", width,height);
                parser = 1;
            }
            else
                printf("\t->Can't get image siz in %5d byte bistream\n", bufferCount);
        }
        if ( bufferCount > jpeg_buffer_size)
        {
            printf("Bitstream size is larger than jpeg buffer size, %d!!\n",bufferCount);           
            return 0;
        }
    }
    fclose(fp);

    printf("Bitstream is 0x%X Bytes\n",bufferCount);
    if (bufferCount % 4)
        bufferCount = (bufferCount & ~0x3) + 4; 
    printf("Set Src Buffer is 0x%X Bytes\n",bufferCount);
        
    if (width % 2)
        width = (width & 0xFFFFFFFE) + 2; 

    jpeg_param.dec_stride = width;  /* Set Decode Output width */   

    /* Buffer for target image */   
    DecOPWbuffersize = width * height * 4;
 
    pJpegSWaddr = malloc(sizeof(char) * DecOPWbuffersize);

    jpeg_param.decopw_TargetBuffersize = DecOPWbuffersize;

    if(pJpegBuffer < 0)
    {
        printf("Malloc Failed!\n");
        ret = -1;
        goto out;
    }
    jpeg_param.decopw_vaddr = (__u32)pJpegSWaddr;   
    printf("jpeg_param.decopw_vaddr 0x%X\n", jpeg_param.decopw_vaddr );
    
    /* allocate tmp buffer for file read */
    buffer = (char *)malloc(jpeg_param.decInWait_buffer_size);

    /* Set decode output format: RGB555/RGB565/RGB888/YUV422 */
    jpeg_param.decode_output_format =DRVJPEG_DEC_PRIMARY_PACKET_RGB888;
    
    /* Set operation property to JPEG engine */
    if ((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
    {
        fprintf(stderr,"set jpeg param failed:%d\n",errno);
        ret = -1;
        goto out;
    }
    
    /* Total buffer size for JPEG engine */
    BufferSize = (jpeg_param.src_bufsize + jpeg_param.dst_bufsize);

    if (BufferSize > jpeg_buffer_size)
    {
       	printf("Required buffer size is 0x%x, but JPEG buffer size is 0x%x\n", BufferSize, jpeg_buffer_size);
        printf("JPEG Engine Buffer isn't enough. Please enlarge JPEG buffer via menuconfig setting.\n");
        goto out;
    }
    
    bTrigger = 0;       /* For flow control */
    BitstreamCount = 0; /* Bitstream count for application */
    readSize = 0;

    printf("Bitstream Buffer for Decode Input Wait is %dKB\n",jpeg_param.src_bufsize/1024);

    /* Open JPEG file */
    fp = fopen(filename, "r+");
    if (fp == NULL)
    {
            printf("open %s error!\n", fp);
            return 0;
    }

    while (!feof(fp) || state != JPEG_DECODED_IMAGE || state != JPEG_MEM_SHORTAGE)   
    {   
        /* Check Buffer status */           
        do {         
            /* Get Decode Input Wait Buffer State */
            ioctl(fd , JPEG_DECIPW_BUFFER_STATE , &jpeg_buffer_state);
            /* Get Decode state */
            ioctl(fd, JPEG_STATE, (__u32)&state);       
        } while(jpeg_buffer_state != JPEG_DECIPW_BUFFER_EMPTY  && state != JPEG_DECODED_IMAGE && state != JPEG_MEM_SHORTAGE);
                
        if (state == JPEG_DECODED_IMAGE || state == JPEG_MEM_SHORTAGE)   /* Decode Complete!! */
            break;

        /* Get available Buffer size */
        if ((ioctl(fd, JPEG_G_DECIPW_BUFFER_SIZE, &jpeg_buffersize)) < 0)
        {
            fprintf(stderr,"set jpeg param failed:%d\n",errno);
            ret = -1;
            goto out;
        }
        
        fd_set writefds;
        struct timeval tv;
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
        FD_ZERO( &writefds );
        FD_SET( fd , &writefds );
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
        
        select( fd + 1 , NULL , &writefds , NULL, &tv );
        if ( FD_ISSET( fd, &writefds ))
        {   
            /* Read "jpeg_buffersize" Bytes Bitstream from file to tmp buffer "buffer" */
            readSize = fread(buffer, 1, jpeg_buffersize, fp);
            
            if (readSize != 0)
            {
                /* Write "jpeg_buffersize" Bytes Bitstream from tmp buffer "buffer" to JPEG engine buffer */
                write(fd, buffer, readSize);  

                BitstreamCount = BitstreamCount + readSize; 
                printf("Fill %5d bytes to JPEG engine, ",readSize);
                printf("Bitstream in JPEG engine %6d Bytes\n",BitstreamCount);
            }
        }           
    
        if (!bTrigger)
        {
            /* Trigger JPEG engine */
            if ((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
            {
                fprintf(stderr,"trigger jpeg failed:%d\n",errno);
                ret = -1;
                goto out;
            }   
            bTrigger = 1;   
        }
        else
            ioctl(fd, JPEG_DECODE_RESUME, NULL);    /* Resume the JPEG decode operation */
    }

    /* Get JPEG decode information */
    len = read(fd, jpeginfo, jpeginfo_size);

    if (len < 0) 
    {
        fprintf(stderr, "read data error errno=%d\n", errno);
        ret = -1;
        goto out;
    }
    printf("JPEG: Width %d, Height %d\n",jpeginfo->width, jpeginfo->height);
    
    if (jpeginfo->state == JPEG_DECODED_IMAGE)
    {       
        printf("Decode Complete\n");
        printf("Output Size is %d x %d\n",jpeginfo->width, jpeginfo->height);
        ret = Write2File("./decIOPW_NormalDecode.dat", (unsigned char *)((__u32)pJpegSWaddr), jpeginfo->image_size[0]);
        if(ret < 0)
            printf("write to file, errno=%d\n", errno);
    }
    else if (jpeginfo->state == JPEG_DECODE_ERROR)
        printf("Decode Error\n");
    else if (jpeginfo->state == JPEG_MEM_SHORTAGE)
        printf("Memory Shortage\n");            
    else if (jpeginfo->state == JPEG_DECODE_PARAM_ERROR)
        printf("Decode Parameter Error\n"); 
        
out:
    fclose(fp);
    free(buffer);
    free(pJpegSWaddr);
    free(jpeginfo);

    return 0;

}
 
int jpegWindowCodec(void)
{
    unsigned long BufferSize, bufferCount,readSize;
    int ret = 0;
    int i,len, jpeginfo_size,u32WindowX,u32WindowY,direction,loop;
    FILE *fp;
    char *filename = "./jpegWIN.jpg";
    int width,height, parser,result;

    memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
    jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
    jpeginfo = malloc(jpeginfo_size);

    jpeg_param.encode = 0;          /* Decode Operation */
    jpeg_param.src_bufsize = 300*1024;  /* Src buffer size (Bitstream buffer size for JPEG engine) */
    jpeg_param.dst_bufsize = xres*yres*2;   /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
    jpeg_param.decInWait_buffer_size = 0;   /* Decode input Wait buffer size (Decode input wait function disable when                              decInWait_buffer_size is 0) */
    jpeg_param.decopw_en = 0;       /* Disable Decode Output Wait */

    BufferSize = jpeg_param.src_bufsize + jpeg_param.dst_bufsize;

    if(BufferSize > jpeg_buffer_size)
    {
       	printf("Required buffer size is 0x%x, but JPEG buffer size is 0x%x\n", BufferSize, jpeg_buffer_size);
        printf("JPEG Engine Buffer isn't enough. Please enlarge JPEG buffer via menuconfig setting.\n");
        goto out;
    }

    /* Open JPEG file */
    fp = fopen(filename, "r+");
    if (fp == NULL)
    {
        printf("open %s error!\n", fp);
        return 0;
    }
    
    pSRCbuffer = pJpegBuffer;
    bufferCount = 0;
    parser = 0;
    /* Read Bitstream to JPEG engine src buffer */
    printf("JPEG Header Parser:\n");
    while (!feof(fp))    
    {   
        fd_set writefds;
        struct timeval tv;
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
        FD_ZERO( &writefds );
        FD_SET( fd , &writefds );
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
    
        select( fd + 1 , NULL , &writefds , NULL, &tv );
        if ( FD_ISSET( fd, &writefds ))
        {                   
            readSize = fread(pSRCbuffer, 1, 4096 , fp);
            pSRCbuffer += readSize; 
            bufferCount += readSize;
        }   
        if (!parser)
        {
            result = ParsingJPEG(pJpegBuffer, bufferCount, &width, &height);
            if (!result)
            {
                printf("\t->Width %d, Height %d\n", width,height);
                parser = 1;
            }
            else
                printf("\t->Can't get image siz in %5d byte bistream\n", bufferCount);
        }

        if ( bufferCount > jpeg_param.src_bufsize)
        {
            printf("Bitstream size is larger than src buffer, %d!!\n",bufferCount);         
            return 0;
        }
    }

    printf("Bitstream is 0x%X Bytes\n",bufferCount);

    if (bufferCount % 4)
        bufferCount = (bufferCount & ~0x3) + 4; 

    printf("Set Src Buffer is 0x%X Bytes\n",bufferCount);
        
    jpeg_param.src_bufsize = bufferCount;   /* Src buffer size (Bitstream buffer size for JPEG engine) */
    jpeg_param.dst_bufsize = BufferSize - bufferCount;  /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
        

    u32WindowX = width / 16 - 1; 
    u32WindowY = height / 16 - 1; 
    jpeg_param.windec_mcux_start = 0;
    jpeg_param.windec_mcux_end = xres / 16 -1;
    jpeg_param.windec_mcuy_start = 0;
    jpeg_param.windec_mcuy_end = yres / 16 -1;  

    direction = 0;
    loop = 1;

    printf("Total MCU (0,0) - (%2d,%2d)\n",u32WindowX ,u32WindowY);

    while (loop)
    {       
        jpeg_param.encode = 0;          /* Decode Operation */
        jpeg_param.src_bufsize = 300*1024;  /* Src buffer size (Bitstream buffer size for JPEG engine) */
        jpeg_param.dst_bufsize = xres*yres*2;   /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
        jpeg_param.decInWait_buffer_size = 0;   /* Decode input Wait buffer size (Decode input wait function disable when                                  decInWait_buffer_size is 0) */

        jpeg_param.windec_stride = xres;    /* Set Window Decode Stride */      

        jpeg_param.dec_stride = 0;      /* Stride function is disabled when dec_stride is 0 */
        jpeg_param.scale = 0;           /* Scale function is disabled when scale is 0 */

        jpeg_param.buffersize = 0;      /* only for continuous shot */
            jpeg_param.buffercount = 1;
        jpeg_param.windec_en = 1;       /* Enable Window Decode */

        /* Set decode output format: RGB555/RGB565/RGB888/YUV422  */
        jpeg_param.decode_output_format =DRVJPEG_DEC_PRIMARY_PACKET_RGB888;

        /* Set operation property to JPEG engine */
        if ((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
        {
            fprintf(stderr,"set jpeg param failed:%d\n",errno);
            ret = -1;
            goto out;
        }
        
        /* Total buffer size for JPEG engine */
        BufferSize = (jpeg_param.src_bufsize + jpeg_param.dst_bufsize);

        /* Trigger JPEG engine */
        if ((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
        {
            fprintf(stderr,"trigger jpeg failed:%d\n",errno);
            ret = -1;
            goto out;
        }
        
        /* Get JPEG decode information */
        len = read(fd, jpeginfo, jpeginfo_size);
        if (len < 0) 
        {
            fprintf(stderr, "read data error errno=%d\n", errno);
            ret = -1;
            goto out;
        }

        if (jpeginfo->state == JPEG_DECODED_IMAGE)
        {
            memcpy((void*)pVideoBuffer, (char*)(pJpegBuffer + jpeg_param.src_bufsize), xres * yres * 2);
        }       
        usleep(100000);
        
        if ((jpeg_param.windec_mcux_start == 0 || jpeg_param.windec_mcuy_start == 0) && direction == 1)      
            loop = 0;

        if (direction)
        {
            jpeg_param.windec_mcux_start--;
            jpeg_param.windec_mcux_end--;
            jpeg_param.windec_mcuy_start--;
            jpeg_param.windec_mcuy_end--;   
        }
        else
        {
            jpeg_param.windec_mcux_start++;
            jpeg_param.windec_mcux_end++;
            jpeg_param.windec_mcuy_start++;
            jpeg_param.windec_mcuy_end++;   
        }

        if (u32WindowX < jpeg_param.windec_mcux_end || u32WindowY < jpeg_param.windec_mcuy_end)
        {
            jpeg_param.windec_mcux_start--;
            jpeg_param.windec_mcux_end--;
            jpeg_param.windec_mcuy_start--;
            jpeg_param.windec_mcuy_end--;   
            direction = 1;  
        }

        if (loop)
            printf("Decode MCU -> (%2d,%2d)-(%2d,%2d)\n",jpeg_param.windec_mcux_start,jpeg_param.windec_mcuy_start, jpeg_param.windec_mcux_end,jpeg_param.windec_mcuy_end );
    }

out:
    free(jpeginfo);
    printf("Test End\n");

    return 0;

}

int jpeg_resize(void)
{
    unsigned long BufferSize, bufferCount,readSize,enc_source_width;
    int appOption = 0, AppTotalSize = 0, ReservedAppOffset,QTOption = 0;
    int ret = 0;
    int i,len, jpeginfo_size, max_width, max_height, min_width, min_height,scale_width, scale_height;   
    int width,height, parser;
    FILE *fp;
    char inputfilename[50];
    char outputfilename[50];
    unsigned char *AppInfo;
    int dec_mode = 0,u32jpeg_Format,enc_source_datasize;
    memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
    jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
    jpeginfo = malloc(jpeginfo_size);

/******************************************************************/
/*                       Get JPEG File name                       */
/******************************************************************/

    printf("Please input jpeg file name (ex.resize.jpg)\n");
    scanf("%s", &inputfilename);


/******************************************************************/
/*                    Get JPEG Width & Height                     */
/******************************************************************/

    /* Open JPEG file */
    fp = fopen(inputfilename, "r+");
        
    if (fp == NULL)
    {
        printf("open %s error!\n", fp);
        return 0;
    }

    pSRCbuffer = pJpegBuffer;
    bufferCount = 0;
    parser = 0;
    printf("-> JPEG Parser - %s\n", inputfilename);
    strcpy(outputfilename, inputfilename);
    
    /* Read Bitstream to JPEG engine src buffer */
    while (!feof(fp))    
    {   
        fd_set writefds;
        struct timeval tv;
        int result;
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
        FD_ZERO( &writefds );
        FD_SET( fd , &writefds );
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
        
        select( fd + 1 , NULL , &writefds , NULL, &tv );
        if ( FD_ISSET( fd, &writefds ))
        {                   
            readSize = fread(pSRCbuffer, 1, 4096 , fp);
            pSRCbuffer += readSize; 
            bufferCount += readSize;
        }   
        if (!parser)
        {
            result = ParsingJPEG(pJpegBuffer, bufferCount, &width, &height);
            if(!result)
            {
                printf("\t-> Width %d, Height %d\n", width,height);
                parser = 1;
            }
        }
        if ( bufferCount > jpeg_buffer_size)
        {
            printf("\t-> Bitstream size is larger than buffer size, %d!!\n",bufferCount);           
            return 0;
        }
    }
    printf("\t-> Bitstream is 0x%X Bytes\n",bufferCount);

    if(bufferCount % 4)
        bufferCount = (bufferCount & ~0x3) + 4; 
        
    jpeg_param.src_bufsize = bufferCount;   /* Src buffer size (Bitstream buffer size for JPEG engine) */
    jpeg_param.dst_bufsize = jpeg_buffer_size - bufferCount;    /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
    
/******************************************************************/
/*                Get JPEG Sclaed Width & Height                  */
/******************************************************************/
    max_width = width * 8;
    min_width = width / 6;
    if (max_width > 4095)
        max_width = 4095;
    if (min_width < 1)
        min_width = 1;
    
    max_height = height * 8;
    min_height = height / 6 ;

    if (max_height > 4095)
        max_height = 4095;
    if (min_height < 1)
        min_height = 1;
        
    printf("Please input resize encode width (%d < width < %d)\n", min_width, max_width);
    while (1)
    {
        scanf("%d", &scale_width);
        if (scale_width <= max_width && scale_width >= min_width)
            break;
        else
            printf("-> Invalid value\n");
    }
    printf("Please input resize encode height (%d < height < %d)\n", min_height, max_height);
    
    while (1)
    {
        scanf("%d", &scale_height);
        if (scale_height <= max_height && scale_height >= min_height)
            break;
        else
            printf("-> Invalid value\n");
    }

    printf("Resize from %dx%d to %dx%d\n", width, height, scale_width,scale_height);
    
    if (scale_width <= width || scale_height <= height)
        dec_mode = 1; /* Need to downscale */

    printf("Please selsect encode format (0=YUV422,1=YUV420)\n" );
    scanf("%d", &u32jpeg_Format);

    printf("Please selsect Application Marker Option(0=Disable,1=Enable)\n" );
    scanf("%d", &appOption);

    printf("Keep Original Q-Table (0=No,1=Yes)\n" );
    scanf("%d", &QTOption);     
/******************************************************************/
/*               Create Encode Source for Resizing                */
/******************************************************************/
    printf("Create Encode Source Raw Data for Resizing\n");
    
    jpeg_param.scale = 1;       /* Enable scale function */
    
    if (scale_width <= width)
        jpeg_param.scaled_width = scale_width;  /* width after scaling */
    else
        jpeg_param.scaled_width = width;    /* width after scaling */

    if (scale_height <= height)
        jpeg_param.scaled_height = scale_height;/* height after scaling */
    else
        jpeg_param.scaled_height = height;  /* height after scaling */

    if (jpeg_param.scaled_width % 2)
        jpeg_param.scaled_width++;

    jpeg_param.dec_stride = 0;      /* Enable stride function */            
    jpeg_param.decopw_en = 0;
    jpeg_param.windec_en = 0;
    jpeg_param.buffersize = 0;      /* only for continuous shot */
        jpeg_param.buffercount = 1;

    /* Set decode output format: RGB555/RGB565/RGB888/YUV422/PLANAR_YUV */
    jpeg_param.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_YUV422;
        
    /* Set operation property to JPEG engine */
    if ((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
    {
        fprintf(stderr,"\t-> set jpeg param failed:%d\n",errno);
        ret = -1;
        goto out;
    }       

    /* Trigger JPEG engine */
    if ((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
    {
        fprintf(stderr,"\t-> trigger jpeg failed:%d\n",errno);
        ret = -1;
        goto out;
    }
        
    /* Get JPEG decode information */
    len = read(fd, jpeginfo, jpeginfo_size);
    if (len < 0) 
    {
        fprintf(stderr, "\t-> read data error errno=%d\n", errno);
        ret = -1;
        goto out;
    }
    
    if (jpeginfo->state == JPEG_DECODED_IMAGE)
    {
        if (appOption)                           /* Get Application Marker from soruce jpeg file */
        {
            int i, offset = 0;
            
            AppTotalSize = 0;
            for (i = 0; i < appNUm; i++)
                AppTotalSize += appSize[i];         
            AppInfo =  malloc(sizeof(char) * AppTotalSize);
            ReservedAppOffset = AppTotalSize;

            if (ReservedAppOffset % 4)
                ReservedAppOffset = (ReservedAppOffset & ~0x3) + 4; 
            /* Collect all Application marker to Buffer AppInfo */
            for (i = 0; i < appNUm; i++)
            {
                memcpy((void*)AppInfo + offset, (char*)(pJpegBuffer + appOffset[i]), appSize[i]);
                offset += appSize[i];
            }
        }

        /* Copy decode data from destination buffer to source buffer for Resizing encode */
        memcpy((void*)pJpegBuffer, (char*)(pJpegBuffer + jpeg_param.src_bufsize), jpeginfo->image_size[0]);
        printf("\t-> Encode Source: Width %d, Height %d\n",jpeginfo->width, jpeginfo->height);
            if(ret < 0)
            printf("\t-> write to file, errno=%d\n", errno);
    }
    else if (jpeginfo->state == JPEG_DECODE_ERROR)
        printf("\t-> Decode Error\n");
    else if (jpeginfo->state == JPEG_MEM_SHORTAGE)
        printf("\t-> Memory Shortage\n");       
    else if (jpeginfo->state == JPEG_DECODE_PARAM_ERROR)
        printf("Decode Parameter Error\n"); 

/******************************************************************/
/*                         Encode Resizing                        */
/******************************************************************/
    printf("Create Encode JPEG for Resizing\n");
    if (appOption)
        enc_source_datasize = jpeginfo->image_size[0] + ReservedAppOffset;
    else
        enc_source_datasize = jpeginfo->image_size[0];  

    enc_source_width = jpeginfo->width;
    memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
    
    jpeg_param.encode = 1;                      /* Encode */
    jpeg_param.src_bufsize = enc_source_datasize;           /* Src buffer (Raw Data) */
    jpeg_param.dst_bufsize = jpeg_buffer_size - enc_source_datasize;/* Dst buffer (Bitstream) */
    jpeg_param.encode_source_format = DRVJPEG_ENC_SRC_PACKET;   /* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
    
    if (width > scale_width)
        jpeg_param.encode_width = scale_width;          /* JPEG width (Encode Source is downscaled from source jpeg */
    else
        jpeg_param.encode_width = width;            /* JPEG width */

    if (height > scale_height)
        jpeg_param.encode_height = scale_height;        /* JPEG Height (Encode Source is downscaled from source jpeg */
    else
        jpeg_param.encode_height = height;          /* JPGE Height */

    jpeg_param.scaled_width = scale_width;              /* width after scaling */
    jpeg_param.scaled_height = scale_height;            /* height after scaling */
    
    jpeg_param.scale = 1;   

    if (u32jpeg_Format)
        jpeg_param.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;    
    else
        jpeg_param.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;
    
    jpeg_param.buffersize = 0;      /* only for continuous shot */
    jpeg_param.buffercount = 1; 
        
    if (!QTOption)   
    {
        if (g_qtflag)
        {
            ioctl(fd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
            ioctl(fd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
            ioctl(fd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
        }
        else
            ioctl(fd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);
    }

    /* Set operation property to JPEG engine */
    if ((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
    {
        fprintf(stderr,"\t-> set jpeg param failed:%d\n",errno);
        ret = -1;
        goto out;
    }
        
    /* Set encode source stride (Must calling after IOCTL - JPEG_S_PARAM) */
    ioctl(fd, JPEG_SET_ENC_STRIDE, enc_source_width);
    
    /* Trigger JPEG engine */
    if ((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
    {
        fprintf(stderr,"\t-> trigger jpeg failed:%d\n",errno);
        ret = -1;
        goto out;
    }
    
    /* Get JPEG Encode information */
    len = read(fd, jpeginfo, jpeginfo_size);
    printf("\t-> Encode Complete\n");
    if (len < 0) 
    {
        fprintf(stderr, "No data can get\n");
        if (jpeginfo->state == JPEG_MEM_SHORTAGE)
            printf("Memory Stortage\n");    
        ret = -1;
        goto out;
    }
    else
    {   
        int index,i;
        char extenname[50], width_buf[10], height_buf[10];

        for (i = 0; i < sizeof(outputfilename); i++)
        {
            if(outputfilename[i] == '.')
            {
                index = i;
                break;
            }
        }       
        strcpy(extenname, (outputfilename + i +1)); 
        outputfilename[i] = '\0';
        sprintf(width_buf,"%d", scale_width);
        sprintf(height_buf,"%d", scale_height);
        strcat(outputfilename, "_");
        strcat(outputfilename, width_buf);
        strcat(outputfilename, "x");
        strcat(outputfilename, height_buf);
        strcat(outputfilename, ".");
        strcat(outputfilename,extenname);
        printf("Output to file %s\n",outputfilename);

        if (appOption)
        {
            __u8 *u8Ptr;
            /* The address to put the Application Marker (-2 : Original FF D8) */
            jpeg_param.src_bufsize  = jpeg_param.src_bufsize - (AppTotalSize - 2);
            /* Copy Application Marker data from source jpeg to the resized jpeg */
            memcpy((void*)pJpegBuffer + jpeg_param.src_bufsize, (char*)AppInfo, AppTotalSize);          
            /* Set New FF D8 to the resized jpeg */
            u8Ptr =(__u8 *) (pJpegBuffer + jpeg_param.src_bufsize - 2);  
            *(u8Ptr) = 0xFF;
            *(u8Ptr + 1) = 0xD8;
            ret = Write2File(outputfilename, pJpegBuffer + jpeg_param.src_bufsize - 2, jpeginfo->image_size[0] + AppTotalSize - 2); 
        }
        else
            ret = Write2File(outputfilename, pJpegBuffer + jpeg_param.src_bufsize, jpeginfo->image_size[0]);    
        
        if (ret < 0)
            printf("write to file, errno=%d\n", errno);
    }

out:
    free(jpeginfo);
    return 0;
}


int jpegWindecDownscale(void)
{
    unsigned long BufferSize, bufferCount,readSize;
    int ret = 0;
    int i,len, jpeginfo_size,u32WindowX,u32WindowY,MCU_ShiftX,MCU_ShiftY;
    int target_width = 120, target_height = 120;
    int target_offsetX[2], target_offsetY[2];
    FILE *fp;
    int width,height, parser,result;
    __u32 u32OutputOffset[4];   
    char *filename[4] = {"./thumb0.jpg","./thumb1.jpg","./thumb2.jpg","./thumb3.jpg"};

    memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
    jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
    jpeginfo = malloc(jpeginfo_size);
    
    /* Clear Buffer */
    memset(pVideoBuffer, 0x77, xres * yres * 2);    

    /* Calculate output position and offset */
    target_offsetX[0] = (xres - target_width * 2) / 3;
    target_offsetX[1] = target_offsetX[0] * 2 + target_width;
    target_offsetY[0] = (yres - target_height * 2) / 3;
    target_offsetY[1] = target_offsetY[0] * 2 + target_height;
    u32OutputOffset[0] = (xres * target_offsetY[0]  + target_offsetX[0]) * 2;   
    u32OutputOffset[1] = (xres * target_offsetY[0]  + target_offsetX[1]) * 2;   
    u32OutputOffset[2] = (xres * target_offsetY[1]  + target_offsetX[0]) * 2;   
    u32OutputOffset[3] = (xres * target_offsetY[1]  + target_offsetX[1]) * 2;   

    for (i = 0; i < 4; i++)
    {   
        /* Open JPEG file */
        fp = fopen(filename[i], "r+");
    
        if (fp == NULL)
        {
            printf("open %s error %d!\n", filename[i], fp);
            return 0;
        }
        else            
            printf("open %s\n",filename[i]);

        pSRCbuffer = pJpegBuffer;
        bufferCount = 0;
        parser = 0;
        /* Read Bitstream to JPEG engine src buffer */
        printf("JPEG Header Parser bitstream %d:\n",i);
        while (!feof(fp))    
        {   
            fd_set writefds;
            struct timeval tv;
            tv.tv_sec       = 0;
            tv.tv_usec      = 0;
            FD_ZERO( &writefds );
            FD_SET( fd , &writefds );
            tv.tv_sec       = 0;
            tv.tv_usec      = 0;
    
            select( fd + 1 , NULL , &writefds , NULL, &tv );
            if ( FD_ISSET( fd, &writefds ))
            {                   
                readSize = fread(pSRCbuffer, 1, 4096 , fp);
                pSRCbuffer += readSize; 
                bufferCount += readSize;
            }   
            if (!parser)
            {
                result = ParsingJPEG(pJpegBuffer, bufferCount, &width, &height);
                if(!result)
                {
                    printf("\t->Width %d, Height %d\n", width,height);
                    parser = 1;
                }
            }

            if( bufferCount > jpeg_buffer_size)
            {
                printf("Bitstream size is larger than jpeg engine buffer, %d!!\n",bufferCount);         
                return 0;
            }
        }

        printf("Bitstream %d is 0x%X Bytes\n",i, bufferCount);

        if (bufferCount % 4)
            bufferCount = (bufferCount & ~0x3) + 4; 

        printf("Set Src Buffer is 0x%X Bytes\n",bufferCount);
        
        jpeg_param.encode = 0;  
        jpeg_param.src_bufsize = bufferCount;           /* Src buffer size (Bitstream buffer size for JPEG engine) */
        jpeg_param.dst_bufsize = jpeg_buffer_size - bufferCount;/* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
        jpeg_param.decInWait_buffer_size = 0;           /* Decode input Wait buffer size (Decode input wait function disable when                                   decInWait_buffer_size is 0) */
        jpeg_param.decopw_en = 0;               /* Disable Decode Output Wait */

        BufferSize = jpeg_param.src_bufsize + jpeg_param.dst_bufsize;

        if (BufferSize > jpeg_buffer_size)
        {   
        	printf("Required buffer size is 0x%x, but JPEG buffer size is 0x%x\n", BufferSize, jpeg_buffer_size);
            printf("JPEG Engine Buffer isn't enough. Please enlarge JPEG buffer via menuconfig setting.\n");
            goto out;
        }
        MCU_ShiftX = 0;
        MCU_ShiftY = 0;
        u32WindowX = width / 16 - 1; 
        u32WindowY = height / 16 - 1; 
        
        /* Get 1:1 size */
        if (u32WindowX  > u32WindowY)
        {           
            MCU_ShiftX = (u32WindowX - u32WindowY) / 2;
            u32WindowX = u32WindowY;
        }
        else
        {
            MCU_ShiftY = (u32WindowY - u32WindowX) / 2;
            u32WindowY = u32WindowX;
        }
        jpeg_param.buffersize = 0;          /* only for continuous shot */
        jpeg_param.buffercount = 1;
        jpeg_param.windec_en = 1;           /* Enable Window Decode */
        jpeg_param.windec_mcux_start = 0 + MCU_ShiftX;
        jpeg_param.windec_mcux_end = u32WindowX + MCU_ShiftX;
        jpeg_param.windec_mcuy_start = 0 + MCU_ShiftY;
        jpeg_param.windec_mcuy_end = u32WindowY + MCU_ShiftY;   
        jpeg_param.windec_stride = xres;        /* Set Window Decode Stride */  
        jpeg_param.scale = 1;               /* Enable scale function */
        jpeg_param.scaled_width = target_width;     /* width after scaling */
        jpeg_param.scaled_height = target_height;   /* height after scaling */
            
        /* Set output offset */ 
        jpeg_param.paddr_dst = fb_paddress + u32OutputOffset[i];
        jpeg_param.vaddr_dst = (__u32)pVideoBuffer + u32OutputOffset[i];

        printf("Decode thumbnail %d MCU(%d,%d)-(%2d,%2d) and Downscale to %d x %d -> (X,Y) = (%4d,%4d)\n",i,jpeg_param.windec_mcux_start,jpeg_param.windec_mcuy_start, jpeg_param.windec_mcux_end ,jpeg_param.windec_mcuy_end,target_width,target_height,target_offsetX[i%2],target_offsetY[i/2]);

        /* Set decode output format: RGB555/RGB565/RGB888/YUV422  */
        jpeg_param.decode_output_format =DRVJPEG_DEC_PRIMARY_PACKET_RGB888;

        /* Set operation property to JPEG engine */
        if ((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
        {
            fprintf(stderr,"set jpeg param failed:%d\n",errno);
            ret = -1;
            goto out;
        }
        
        /* Trigger JPEG engine */
        if ((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
        {
            fprintf(stderr,"trigger jpeg failed:%d\n",errno);
            ret = -1;
            goto out;
        }
        
        /* Get JPEG decode information */
        len = read(fd, jpeginfo, jpeginfo_size);
        if (len < 0) 
        {
            fprintf(stderr, "read data error errno=%d\n", errno);
            ret = -1;
            goto out;
        }

        if (jpeginfo->state == JPEG_DECODED_IMAGE)
            printf("Decode Complete\n");
        else if(jpeginfo->state == JPEG_DECODE_ERROR)
            printf("Decode Error\n");
        else if(jpeginfo->state == JPEG_MEM_SHORTAGE)
            printf("Memory Shortage\n");    
        else if(jpeginfo->state == JPEG_DECODE_PARAM_ERROR)
            printf("Decode Parameter Error\n"); 

    }
    
    if (jpeginfo->state != JPEG_DECODE_PARAM_ERROR)
    {
        printf("Output Size is %d x %d\n", xres, yres); 
        ret = Write2File("./DecodeWinDownscale.dat", pVideoBuffer,uVideoSize);
    }
out:
    free(jpeginfo);
    printf("Test End\n");

    return 0;

}

void InitColorTable(void)
{
        int       i;
        int       nScale = 1L << 16;             //equal to power(2,16)
        int       CBCR_OFFSET = 128 << 16;
        /*
        nHalf is for (y, cb, cr) rounding, equal to (1L<<16)*0.5
        If (R,G,B)=(0,0,1), then Cb = 128.5, should round to 129
        Using these tables will produce 129 too:
        Cb      = (INT32)((RToCb[0] + GToCb[0] + BToCb[1]) >> 16)
                = (INT32)(( 0 + 0 + 1L<<15 + 1L<<15 + 128 * 1L<<16 ) >> 16)
                = (INT32)(( 1L<<16 + 128 * 1L<<16 ) >> 16 )
                = 129
        */
        int       nHalf = nScale >> 1;

        for ( i = 0; i < 256; i += 1 )
        {
                s_ai32RToY[i] = (int)(0.29900 * nScale + 0.5) * i;
                s_ai32GToY[i] = (int)(0.58700 * nScale + 0.5) * i;
                s_ai32BToY[i] = (int)(0.11400 * nScale + 0.5) * i + nHalf;

                s_ai32RToCb[i] = (int)(0.16874 * nScale + 0.5) * (-i);
                s_ai32GToCb[i] = (int)(0.33126 * nScale + 0.5) * (-i);
                s_ai32BToCb[i] = (int)(0.50000 * nScale + 0.5) * i + CBCR_OFFSET + nHalf - 1;

                s_ai32RToCr[i] = s_ai32BToCb[i];
                s_ai32GToCr[i] = (int)(0.41869 * nScale + 0.5) * (-i);
                s_ai32BToCr[i] = (int)(0.08131 * nScale + 0.5) * (-i);
        }
}

void _BGR565ToYCbCr(__u16 *pu16SrcBuf,__u16 *pu16DestBuf,__u16 u16Width, __u16 u16Height)
{
    __u16    u16X, u16Y, u16SrcBuf;
    __u8      u8R, u8G, u8B;
    __u8      u8Y0, u8U0, u8V0, u8Y1;
    __u8 *pu8DestBufY;
    __u8 *pu8DestBufU;
    __u8 *pu8DestBufV;

    pu8DestBufY = (__u8 *)  pu16DestBuf;
    pu8DestBufU = pu8DestBufY + u16Width * u16Height;
    pu8DestBufV = pu8DestBufU + u16Width * u16Height / 2;

    for ( u16Y = 0; u16Y < u16Height; u16Y += 1 )
    {
        for ( u16X = 0; u16X < u16Width; u16X += 2 )
        {
        	u16SrcBuf = pu16SrcBuf[u16Y * u16Width + u16X];
            u8B = (__u8)(u16SrcBuf & 0x001F);
            u8B = (u8B << 3) | (u8B >> 2);
            u16SrcBuf >>= 5;
            u8G = (__u8)(u16SrcBuf & 0x003F);
            u8G = (u8G << 2) | (u8G >> 4);
            u16SrcBuf >>= 6;
            u8R = (__u8)(u16SrcBuf & 0x001F);
            u8R = (u8R << 3) | (u8R >> 2);

            u8Y0 = (s_ai32RToY[u8R] + s_ai32GToY[u8G] + s_ai32BToY[u8B]) >> 16;    // -128
            u8U0 = (s_ai32RToCb[u8R] + s_ai32GToCb[u8G] + s_ai32BToCb[u8B]) >> 16;     // -128
            u8V0 = (s_ai32RToCr[u8R] + s_ai32GToCr[u8G] + s_ai32BToCr[u8B]) >> 16;       // -128

            u16SrcBuf = pu16SrcBuf[u16Y * u16Width + u16X + 1];
            u8B = (__u8)(u16SrcBuf & 0x001F);
            u8B = (u8B << 3) | (u8B >> 2);
            u16SrcBuf >>= 5;
            u8G = (__u8)(u16SrcBuf & 0x003F);
            u8G = (u8G << 2) | (u8G >> 4);
            u16SrcBuf >>= 6;
            u8R = (__u8)(u16SrcBuf & 0x001F);
            u8R = (u8R << 3) | (u8R >> 2);

            u8Y1 = (s_ai32RToY[u8R] + s_ai32GToY[u8G] + s_ai32BToY[u8B]) >> 16;    // -128

            pu8DestBufY[u16Y * u16Width + u16X] = u8Y0;
            pu8DestBufY[u16Y * u16Width + u16X + 1] = u8Y1;

            pu8DestBufU[u16Y * u16Width/2 + u16X/2] = u8U0;
            pu8DestBufV[u16Y * u16Width/2 + u16X/2] = u8V0;
		}
    }
    return;
}


int jpegEncFB(void)
{
    __u32 u32BitstreamAddr,u32ExifAddr;
    __u8 *u8Ptr;
    __u8 *u8BistreamPtr;
    int enc_reserved_size;
    int ret = 0;
    int i,len, jpeginfo_size;
    int i32ThumbnailOffset, i32ThumbnailSize, i32BitstreamSize, i32PrimaryOffset;   
    FILE *fp;

    /* Convert RGB565 (Frame buffer) to Planar YUV422 (JPEG engine buffer) */
    _BGR565ToYCbCr((__u16 *)pVideoBuffer, (__u16 *)pJpegBuffer,(__u16)xres, (__u16)yres);

    memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
    jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
    jpeginfo = malloc(jpeginfo_size);

/* Encode Thumbnail First (Downscale 1/2) */
    jpeg_param.encode = 1;          /* Encode */
    jpeg_param.src_bufsize = xres*yres*2;   /* Src buffer (Raw Data) */
    jpeg_param.dst_bufsize = jpeg_buffer_size - jpeg_param.src_bufsize; /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
    jpeg_param.encode_width = xres;     /* JPEG width */
    jpeg_param.encode_height = yres;    /* JPGE Height */
    jpeg_param.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;   /* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
    jpeg_param.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;    /* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
    jpeg_param.buffersize = 0;      /* only for continuous shot */
    jpeg_param.buffercount = 1; 
    jpeg_param.scale = 1;           /* Scale function is disabled when scale 0 */   
    jpeg_param.scaled_width = xres / 2; /* width after scaling */
    jpeg_param.scaled_height = yres / 2;    /* height after scaling */

    
    /* Reserve memory space for user application 
       # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user   
       # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
        Byte 0 & 1 :  FF D8
        Byte 2 & 3 :  FF E0
        Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
        Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
           
        Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 134,0); 
                FF D8 FF E0 00 88 [134 bytes for EXIF][Bit stream without FF D8]
    */
    
    /* Reserved space for EXIF size */
    enc_reserved_size = EXIF_SIZE + 2;      /* JPGE Reserved size for user application (2 is FF D8 for Thumbnail) */

    if(enc_reserved_size % 4)
        enc_reserved_size = (enc_reserved_size + 0x03) & ~0x03;

    ioctl(fd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);    

    ioctl(fd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

    /* Set operation property to JPEG engine */
    if((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
    {
        fprintf(stderr,"set jpeg param failed:%d\n",errno);
        ret = -1;
        goto out;
    }
                
    /* Trigger JPEG engine */
    if((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
    {
        fprintf(stderr,"trigger jpeg failed:%d\n",errno);
        ret = -1;
        goto out;
    }

    /* Get JPEG Encode information */

    len = read(fd, jpeginfo, jpeginfo_size);
    printf("Encode Thumbnail Complete\n");

    if(len<0) {
        fprintf(stderr, "No data can get\n");
        if(jpeginfo->state == JPEG_MEM_SHORTAGE)
            printf("Memory Stortage\n");    
        ret = -1;
        goto out;
    }

    /*
            FF D8 FF E0 00 88 [134 bytes for EXIF + 2 Bytes for Thumbnail FF D8][Bit stream without FF D8]
        |<-------6------->|<---------------enc_reserved_size ------------->|<----i32ThumbnailSize--->|
        |<---------------------------------jpeginfo->image_size[0]---------------------------------->|
    */

    i32ThumbnailSize = jpeginfo->image_size[0] - EXIF_SIZE - 6;
    i32ThumbnailOffset = enc_reserved_size + 6 - 2;             /* 2 Bytes for Thumbnail FF D8 */

    /* Set New FF D8 to the Thumbnail jpeg */
    u8Ptr =(__u8 *) (pJpegBuffer + jpeg_param.src_bufsize + i32ThumbnailOffset);     
    *(u8Ptr) = 0xFF;
    *(u8Ptr + 1) = 0xD8;

    u32ExifAddr = (__u32) pJpegBuffer + jpeg_param.src_bufsize + 6;     /* Bistream Address */          

    /* Set EXIF Header */
    create_simple_EXIF((char *)u32ExifAddr, i32ThumbnailOffset, i32ThumbnailSize);

    printf("Thumbnail Offst 0x%X Size 0x%X\n", i32ThumbnailOffset,i32ThumbnailSize);

    i32PrimaryOffset = jpeginfo->image_size[0];
    
/* Encode Primary */

    /*
            FF D8 FF E0 00 88 [134 bytes for EXIF + 2 Bytes for Thumbnail FF D8][Bit stream without FF D8][Primary without FF D8]
        Thumbnail setting  
        |<-------6------->|<------enc_reserved_size for Thumbnail--------->|<----i32ThumbnailSize--->|
        |<---------------------------------jpeginfo->image_size[0]---------------------------------->|
        Primary setting
        |<-------6------->|<-------------------enc_reserved_size for Primary------------------------>|
        |<------------------------------jpeginfo->image_size[0](Thumbnail)-------------------------->|  
    */
    
    enc_reserved_size = i32PrimaryOffset - 6;

    if(enc_reserved_size % 4)
        enc_reserved_size = (enc_reserved_size + 0x03) & ~0x03;

    jpeg_param.encode = 1;          /* Encode */
    jpeg_param.src_bufsize = xres*yres*2;   /* Src buffer (Raw Data) */
    jpeg_param.dst_bufsize = jpeg_buffer_size - jpeg_param.src_bufsize; /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
    jpeg_param.encode_width = xres;     /* JPEG width */
    jpeg_param.encode_height = yres;    /* JPGE Height */
    jpeg_param.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;   /* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
    jpeg_param.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;    /* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
    jpeg_param.buffersize = 0;      /* only for continuous shot */
    jpeg_param.buffercount = 1; 
    jpeg_param.scale = 0;           /* Scale function is disabled when scale 0 */   

    ioctl(fd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);    

    ioctl(fd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

    /* Set operation property to JPEG engine */
    if((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
    {
        fprintf(stderr,"set jpeg param failed:%d\n",errno);
        ret = -1;
        goto out;
    }
    
    /* Trigger JPEG engine */
    if((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
    {
        fprintf(stderr,"trigger jpeg failed:%d\n",errno);
        ret = -1;
        goto out;
    }

    /* Get JPEG Encode information */

    len = read(fd, jpeginfo, jpeginfo_size);
    printf("Primary Encode Complete\n");

    i32BitstreamSize = jpeginfo->image_size[0];

    /* Set New FF E1 for EXIF app marker */
    u8Ptr =(__u8 *) (pJpegBuffer + jpeg_param.src_bufsize + 3);  
    *(u8Ptr) = 0xE1;

    if(len<0) {
        fprintf(stderr, "No data can get\n");
        if(jpeginfo->state == JPEG_MEM_SHORTAGE)
            printf("Memory Stortage\n");    
        ret = -1;
        goto out;
    }

    ret = Write2File("./Snapshot.jpg", pJpegBuffer + jpeg_param.src_bufsize, 
    i32BitstreamSize);              

    if(ret < 0)
    {
        printf("write to file, errno=%d\n", errno);
    }
    printf("Please check ./Snapshot.jpg\n");


out:
    free(jpeginfo);

}


int  jpegDeocdeToFB(char *filename)
{
    unsigned long BufferSize, bufferCount,readSize;
    int enc_reserved_size;
    int ret = 0;
    int i,len, jpeginfo_size;   
    int width,height, parser;
    FILE *fp;

    memset((void*)&jpeg_param, 0, sizeof(jpeg_param_t));
    jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
    jpeginfo = malloc(jpeginfo_size);
    
    jpeg_param.encode = 0;          /* Decode Operation */
    jpeg_param.src_bufsize = 0x380000;  /* Src buffer size (Bitstream buffer size for JPEG engine) */
    jpeg_param.dst_bufsize = 0x80000; /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
    jpeg_param.decInWait_buffer_size = 0;   /* Decode input Wait buffer size (Decode input wait function disable when                                  decInWait_buffer_size is 0) */
    jpeg_param.decopw_en = 0;
    jpeg_param.windec_en = 0;

    jpeg_param.scale = 1;       /* Enable scale function */
    jpeg_param.scaled_width = 800;  /* width after scaling */
    jpeg_param.scaled_height = 480; /* height after scaling */
    jpeg_param.dec_stride = xres;   /* Enable stride function */            

    /* Set output offset */ 
    jpeg_param.paddr_dst = 0;
    jpeg_param.vaddr_dst = (__u32)pVideoBuffer;

    BufferSize = (jpeg_param.src_bufsize + jpeg_param.dst_bufsize);

    if (BufferSize > jpeg_buffer_size)
    {
    	printf("Required buffer size is 0x%x, but JPEG buffer size is 0x%x\n", BufferSize, jpeg_buffer_size);
        printf("JPEG Engine Buffer isn't enough. Please enlarge JPEG buffer via menuconfig setting.\n");
        goto out;
    }

	printf("pJpegBuffer = 0x%x, size = 0x%x\n", (int)pJpegBuffer, BufferSize);

    /* Clear buffer */
    //memset(pJpegBuffer, 0x77, BufferSize);  
    //memset(pVideoBuffer, 0x77, 800*480*4); 
    
    printf("Open file %s...\n", filename);
    
    /* Open JPEG file */
    fp = fopen(filename, "r+");
    if (fp == NULL)
    {
            printf("open %s error!\n", fp);
            return 0;
    }

    pSRCbuffer = pJpegBuffer;
    bufferCount = 0;
    parser = 0;
    printf("JPEG Header Parser:\n");
    
    /* Read Bitstream to JPEG engine src buffer */
    while (!feof(fp))    
    {   
        fd_set writefds;
        struct timeval tv;
        int result;
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
        FD_ZERO( &writefds );
        FD_SET( fd , &writefds );
        tv.tv_sec       = 0;
        tv.tv_usec      = 0;
    
        select( fd + 1 , NULL , &writefds , NULL, &tv );
        if ( FD_ISSET( fd, &writefds ))
        {                   
            readSize = fread(pSRCbuffer, 1, 4096 , fp);
            pSRCbuffer += readSize; 
            bufferCount += readSize;
        }  
         
        if (!parser)
        {
            result = ParsingJPEG(pJpegBuffer, bufferCount, &width, &height);
            if(!result)
            {
                printf("\t->Width %d, Height %d\n", width,height);
                parser = 1;
            }
            else
                printf("\t->Can't get image siz in %5d byte bistream\n", bufferCount);
        }

        if ( bufferCount > jpeg_param.src_bufsize)
        {
            printf("Bitstream size is larger than src buffer, %d!!\n",bufferCount);         
            return 0;
        }
    }
    printf("Bitstream is 0x%X Bytes\n",bufferCount);

    if (bufferCount % 4)
        bufferCount = (bufferCount & ~0x3) + 4; 

    printf("Set Src Buffer is 0x%X Bytes\n",bufferCount);
    
    jpeg_param.src_bufsize = bufferCount;   /* Src buffer size (Bitstream buffer size for JPEG engine) */
    jpeg_param.dst_bufsize = BufferSize - bufferCount;  /* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */

    jpeg_param.buffersize = 0;      /* only for continuous shot */
    jpeg_param.buffercount = 1;

    /* Set decode output format: RGB555/RGB565/RGB888/YUV422/PLANAR_YUV */
    jpeg_param.decode_output_format = DRVJPEG_DEC_PRIMARY_PACKET_RGB888;        
    
    /* Set operation property to JPEG engine */
    if ((ioctl(fd, JPEG_S_PARAM, &jpeg_param)) < 0)
    {
        fprintf(stderr,"set jpeg param failed:%d\n",errno);
        ret = -1;
        goto out;
    }       

    /* Ask JPEG engine direct decode output to frame buffer */
    if ((ioctl(fd, JPEG_DECODE_TO_FRAME_BUFFER, 1)) < 0)
    {
        fprintf(stderr,"ioctl JPEG_DECODE_TO_FRAME_BUFFER failed:%d\n",errno);
        ret = -1;
        goto out;
    }       
    
    /* Trigger JPEG engine */
    if ((ioctl(fd, JPEG_TRIGGER, NULL)) < 0)
    {
        fprintf(stderr,"trigger jpeg failed:%d\n",errno);
        ret = -1;
        goto out;
    }
    
    /* Get JPEG decode information */
    len = read(fd, jpeginfo, jpeginfo_size);

    if (len<0) 
    {
        fprintf(stderr, "read data error errno=%d\n", errno);
        ret = -1;
        goto out;
    }
    printf("JPEG: Width %d, Height %d\n",jpeginfo->width, jpeginfo->height);

    if (jpeginfo->state == JPEG_DECODED_IMAGE)
    {
        printf("Decode Complete\n");
        printf("Stride %d\n", jpeginfo->dec_stride);
        printf("Output size is %d x %d\n", xres, jpeginfo->height);
        //memcpy((void*)pVideoBuffer, (char*)(pJpegBuffer + jpeg_param.src_bufsize), 800*480*4);
    }
    else if (jpeginfo->state == JPEG_DECODE_ERROR)
        printf("Decode Error\n");
    else if (jpeginfo->state == JPEG_MEM_SHORTAGE)
        printf("Memory Shortage\n");    
    else if (jpeginfo->state == JPEG_DECODE_PARAM_ERROR)
        printf("Decode Parameter Error\n");         

out:
    free(jpeginfo);
    return 0;

}

void slide_show()
{
 	struct stat attr;
 	struct dirent *entry = NULL;
 	DIR 	*jpeg_dir;
 	int     n;
 	char    filename[256];
 	

    while (1)
	{
		jpeg_dir = opendir(JPEG_DEMO_DIR);
 		stat(JPEG_DEMO_DIR, &attr); 

  		while ((entry = readdir(jpeg_dir)))
     	{
         	printf ("Filename = %s\n" ,entry->d_name );
         	
         	n = strlen(entry->d_name);
         	
         	if ((entry->d_name[n-4] == '.') &&
         		((entry->d_name[n-3] == 'j') || (entry->d_name[n-3] == 'J')) &&
         		((entry->d_name[n-2] == 'p') || (entry->d_name[n-2] == 'P')) &&
         		((entry->d_name[n-1] == 'g') || (entry->d_name[n-1] == 'G')))
         	{
         		strcpy(filename, JPEG_DEMO_DIR);
         		strcat(filename, "/");
         		strcat(filename, entry->d_name);
         		jpegDeocdeToFB(filename);
         		sleep(1);
         	}
     	}
    }
}


int number;
int main()
{
    unsigned int select = 0;
    char *p;
    int i;

    fb_fd = open(fbdevice, O_RDWR);
    if (fb_fd == -1) {
        perror("open fbdevice");
        return -1;
    }

    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &var) < 0) {
        perror("ioctl FBIOGET_VSCREENINFO");
        close(fb_fd);
        return -1;
    }

    xres = var.xres;
    yres = var.yres;

    //ioctl(fb_fd,LCD_ENABLE_INT);
    uVideoSize = xres * yres * 2;
    pVideoBuffer = mmap(NULL, uVideoSize, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
    printf("LCD resolution is %dx%d, pVideoBuffer = 0x%x\n", xres, yres, pVideoBuffer);
    if(pVideoBuffer == MAP_FAILED)
    {
        printf("LCD Video Map Failed!\n");
        exit(0);
    }
    backupVideoBuffer = malloc(uVideoSize);

    if(backupVideoBuffer < 0)
    {
        printf("Malloc fail\n");
        return -1;
    }
    memcpy((void*)backupVideoBuffer, (char*)pVideoBuffer, uVideoSize);

	fb_paddress = NUC970_VA_JPEG;

	/* Check device for jpegcodec "/dev/video0" or "/dev/video1" */
    printf("Open jpegcodec device\n");

    for (i = 0; i < RETRY_NUM; i++)
    {
        fd = open(device, O_RDWR);
        if (fd < 0)
        {           
            continue;   
        }
        break;
    }
    if (fd < 0)
    {
        printf("JPEG Engine is busy\n");
        return -1; 
    }
    /* allocate memory for JPEG engine */
    ioctl(fd, JPEG_GET_JPEG_BUFFER, (__u32)&jpeg_buffer_size);
    
    printf("\tjpeg engine memory buffer: 0x%X\n", jpeg_buffer_size);

    pJpegBuffer = mmap(NULL, jpeg_buffer_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    if(pJpegBuffer == MAP_FAILED)
    {
        printf("JPEG Map Failed!\n");
        goto end;
    }
    else
        printf("\tGet memory from jpeg engine: 0x%X\n",jpeg_buffer_size);

    InitColorTable();
    
    while(select != 0xFF)
    {
    	printf("\n\n\n\n");
    	printf("+---------------------------------------------------------------+\n");
		printf("|  NUC970 JPEG demo                                             |\n");
    	printf("+---------------------------------------------------------------+\n");
    	printf("| [0] Jpeg Decode Input & Output                                |\n");
    	printf("| [1] Jpeg Normal Encode (VGA)                                  |\n");
    	printf("| [2] Jpeg Normal Decode                                        |\n");
    	printf("| [3] Jpeg Decode Downscale & Stride                            |\n");
    	printf("| [4] Jpeg Encode Upscale (VGA to 800x600)                      |\n");
    	printf("| [5] Jpeg [Decode Input Wait] Normal Decode                    |\n");
    	printf("| [6] Jpeg [Decode Input Wait] Decode Downscale & Stride        |\n");
    	printf("| [7] Jpeg Decode Output Wait                                   |\n"); 
    	printf("| [8] Jpeg Window Decode                                        |\n"); 
    	printf("| [9] Jpeg Encode File with Reserved size for Application       |\n"); 
    	printf("| [A] Jpeg Resizing from one jpeg file to another jpeg file     |\n"); 
    	printf("| [B] Active Default Q-Table                                    |\n"); 
    	printf("| [C] Active User-defined Q-Table                               |\n"); 
    	printf("| [S] Slide show on panel                                       |\n"); 
    	printf("| [X] Exit                                                      |\n"); 
    	printf("+---------------------------------------------------------------+\n");
        printf("Select : \n"); 
  again:
        select = getchar();
        if(select == '\n' || select == '\r')
            goto again;
        switch(select)
        {
            case '0':
                printf("\n  [jpegIOPW.jpg => decIOPW_NormalDecode.jpg]\n");
                jpegCodecIOPW();
                break;
            
            case '1':
                printf("\n  [jpegEnc.dat  => NormalEncode.jpg]\n");
                jpegCodec(TEST_NORMAL_ENCODE);
                break; 
                
            case '2':
                printf("\n  [jpegDec.jpg  -> NormalDecode.dat]\n");   
                jpegCodec(TEST_NORMAL_DECODE);
                break;
            
            case '3':
                printf("\n[jpegDec.jpg  -> DecodeDownscale.dat & Panel]\n");
                jpegCodec(TEST_DECODE_DOWNSCALE);
                break;
                 
            case '4':
                printf("\n  [jpegEnc.dat  -> EncodeUpscale.jpg]\n");  
                jpegCodec(TEST_ENCODE_UPSCALE);
                break;
                
            case '5':
                printf("\n  [jpegIPW.jpg  -> decIPW_NormalDecode.dat]\n");    
                jpegCodecIPW(TEST_NORMAL_DECODE);
                break;  
        
            case '6':
                printf("\n  [jpegIPW.jpg  -> decIPW_DecodeDownscale.dat & Panel]\n"); 
                jpegCodecIPW(TEST_DECODE_DOWNSCALE);
                break;  
        
            case '7':
                printf("\n  [jpegOPW.jpg  -> DecOPW.dat]\n");  
                jpegCodec(TEST_DECODE_OUTPUTWAIT);
                break;  
        
            case '8':
                printf("\n  [jpegWIN.jpg  -> Panel]\n");   
                jpegWindowCodec();
                break;
                
            case '9':
                printf("\n  [jpegEnc.dat  -> NormalEncodeReserved.jpg]\n");
                jpegCodec(TEST_ENCODE_RESERVED);
                break; 
    
            case 'a':case 'A':
                jpeg_resize();
                break;
    
            case 'b':case 'B':
                g_qtflag = 0;           
                break;
        
            case 'c':case 'C':              
                printf("C.Active User-defined Q-Table\n");  
                g_qtflag = 1;
                break;

            case 's':case 'S':              
                slide_show(); 
                break;
        
            case 'x':
            case 'X':
                printf("Exit now.\n");
                goto end; 
        }
    }
end:
    close(fb_fd);
    close(fd);
    memcpy((void*)pVideoBuffer, (char*)backupVideoBuffer, uVideoSize);
    free(backupVideoBuffer);
    if(pJpegBuffer)
        munmap(pJpegBuffer, jpeg_buffer_size);

    return 0;
}
