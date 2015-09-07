#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/types.h>
int MotorolaOrder = 0;

#define FMT_STRING					0x0002
#define FMT_ULONG					0x0004

//--------------------------------------------------------------------------
// Describes tag values

#define TAG_INTEROP_INDEX          0x0001
#define TAG_INTEROP_VERSION        0x0002
#define TAG_IMAGE_WIDTH            0x0100
#define TAG_IMAGE_LENGTH           0x0101
#define TAG_BITS_PER_SAMPLE        0x0102
#define TAG_COMPRESSION            0x0103
#define TAG_PHOTOMETRIC_INTERP     0x0106
#define TAG_FILL_ORDER             0x010A
#define TAG_DOCUMENT_NAME          0x010D
#define TAG_IMAGE_DESCRIPTION      0x010E
#define TAG_MAKE                   0x010F
#define TAG_MODEL                  0x0110
#define TAG_SRIP_OFFSET            0x0111
#define TAG_ORIENTATION            0x0112
#define TAG_SAMPLES_PER_PIXEL      0x0115
#define TAG_ROWS_PER_STRIP         0x0116
#define TAG_STRIP_BYTE_COUNTS      0x0117
#define TAG_X_RESOLUTION           0x011A
#define TAG_Y_RESOLUTION           0x011B
#define TAG_PLANAR_CONFIGURATION   0x011C
#define TAG_RESOLUTION_UNIT        0x0128
#define TAG_TRANSFER_FUNCTION      0x012D
#define TAG_SOFTWARE               0x0131
#define TAG_DATETIME               0x0132
#define TAG_ARTIST                 0x013B
#define TAG_WHITE_POINT            0x013E
#define TAG_PRIMARY_CHROMATICITIES 0x013F
#define TAG_TRANSFER_RANGE         0x0156
#define TAG_JPEG_PROC              0x0200
#define TAG_THUMBNAIL_OFFSET       0x0201
#define TAG_THUMBNAIL_LENGTH       0x0202
#define TAG_Y_CB_CR_COEFFICIENTS   0x0211
#define TAG_Y_CB_CR_SUB_SAMPLING   0x0212
#define TAG_Y_CB_CR_POSITIONING    0x0213
#define TAG_REFERENCE_BLACK_WHITE  0x0214
#define TAG_RELATED_IMAGE_WIDTH    0x1001
#define TAG_RELATED_IMAGE_LENGTH   0x1002
#define TAG_CFA_REPEAT_PATTERN_DIM 0x828D
#define TAG_CFA_PATTERN1           0x828E
#define TAG_BATTERY_LEVEL          0x828F
#define TAG_COPYRIGHT              0x8298
#define TAG_EXPOSURETIME           0x829A
#define TAG_FNUMBER                0x829D
#define TAG_IPTC_NAA               0x83BB
#define TAG_EXIF_OFFSET            0x8769
#define TAG_INTER_COLOR_PROFILE    0x8773
#define TAG_EXPOSURE_PROGRAM       0x8822
#define TAG_SPECTRAL_SENSITIVITY   0x8824
#define TAG_GPSINFO                0x8825
#define TAG_ISO_EQUIVALENT         0x8827
#define TAG_OECF                   0x8828
#define TAG_EXIF_VERSION           0x9000
#define TAG_DATETIME_ORIGINAL      0x9003
#define TAG_DATETIME_DIGITIZED     0x9004
#define TAG_COMPONENTS_CONFIG      0x9101
#define TAG_CPRS_BITS_PER_PIXEL    0x9102
#define TAG_SHUTTERSPEED           0x9201
#define TAG_APERTURE               0x9202
#define TAG_BRIGHTNESS_VALUE       0x9203
#define TAG_EXPOSURE_BIAS          0x9204
#define TAG_MAXAPERTURE            0x9205
#define TAG_SUBJECT_DISTANCE       0x9206
#define TAG_METERING_MODE          0x9207
#define TAG_LIGHT_SOURCE           0x9208
#define TAG_FLASH                  0x9209
#define TAG_FOCALLENGTH            0x920A
#define TAG_SUBJECTAREA            0x9214
#define TAG_MAKER_NOTE             0x927C
#define TAG_USERCOMMENT            0x9286
#define TAG_SUBSEC_TIME            0x9290
#define TAG_SUBSEC_TIME_ORIG       0x9291
#define TAG_SUBSEC_TIME_DIG        0x9292

#define TAG_WINXP_TITLE            0x9c9b // Windows XP - not part of exif standard.
#define TAG_WINXP_COMMENT          0x9c9c // Windows XP - not part of exif standard.
#define TAG_WINXP_AUTHOR           0x9c9d // Windows XP - not part of exif standard.
#define TAG_WINXP_KEYWORDS         0x9c9e // Windows XP - not part of exif standard.
#define TAG_WINXP_SUBJECT          0x9c9f // Windows XP - not part of exif standard.

#define TAG_FLASH_PIX_VERSION      0xA000
#define TAG_COLOR_SPACE            0xA001
#define TAG_PIXEL_X_DIMENSION      0xA002
#define TAG_PIXEL_Y_DIMENSION      0xA003
#define TAG_RELATED_AUDIO_FILE     0xA004
#define TAG_INTEROP_OFFSET         0xA005
#define TAG_FLASH_ENERGY           0xA20B
#define TAG_SPATIAL_FREQ_RESP      0xA20C
#define TAG_FOCAL_PLANE_XRES       0xA20E
#define TAG_FOCAL_PLANE_YRES       0xA20F
#define TAG_FOCAL_PLANE_UNITS      0xA210
#define TAG_SUBJECT_LOCATION       0xA214
#define TAG_EXPOSURE_INDEX         0xA215
#define TAG_SENSING_METHOD         0xA217
#define TAG_FILE_SOURCE            0xA300
#define TAG_SCENE_TYPE             0xA301
#define TAG_CFA_PATTERN            0xA302
#define TAG_CUSTOM_RENDERED        0xA401
#define TAG_EXPOSURE_MODE          0xA402
#define TAG_WHITEBALANCE           0xA403
#define TAG_DIGITALZOOMRATIO       0xA404
#define TAG_FOCALLENGTH_35MM       0xA405
#define TAG_SCENE_CAPTURE_TYPE     0xA406
#define TAG_GAIN_CONTROL           0xA407
#define TAG_CONTRAST               0xA408
#define TAG_SATURATION             0xA409
#define TAG_SHARPNESS              0xA40A
#define TAG_DISTANCE_RANGE         0xA40C
#define TAG_IMAGE_UNIQUE_ID        0xA420

//--------------------------------------------------------------------------
// Convert a 16 bit unsigned value to file's native byte order
//--------------------------------------------------------------------------
static void Put16u(void * Short, unsigned short PutValue)
{
    if (MotorolaOrder){
        ((unsigned char *)Short)[0] = (unsigned char)(PutValue>>8);
        ((unsigned char *)Short)[1] = (unsigned char)PutValue;
    }else{
        ((unsigned char *)Short)[0] = (unsigned char)PutValue;
        ((unsigned char *)Short)[1] = (unsigned char)(PutValue>>8);
    }
}

//--------------------------------------------------------------------------
// Convert a 16 bit unsigned value from file's native byte order
//--------------------------------------------------------------------------
int Get16u(void * Short)
{
    if (MotorolaOrder){
        return (((unsigned char *)Short)[0] << 8) | ((unsigned char *)Short)[1];
    }else{
        return (((unsigned char *)Short)[1] << 8) | ((unsigned char *)Short)[0];
    }
}

//--------------------------------------------------------------------------
// Convert a 32 bit signed value from file's native byte order
//--------------------------------------------------------------------------
int Get32s(void * Long)
{
    if (MotorolaOrder){
        return  ((( char *)Long)[0] << 24) | (((unsigned char *)Long)[1] << 16)
              | (((unsigned char *)Long)[2] << 8 ) | (((unsigned char *)Long)[3] << 0 );
    }else{
        return  ((( char *)Long)[3] << 24) | (((unsigned char *)Long)[2] << 16)
              | (((unsigned char *)Long)[1] << 8 ) | (((unsigned char *)Long)[0] << 0 );
    }
}

//--------------------------------------------------------------------------
// Convert a 32 bit unsigned value to file's native byte order
//--------------------------------------------------------------------------
void Put32u(void * Value, unsigned PutValue)
{
    if (MotorolaOrder){
        ((unsigned char *)Value)[0] = (unsigned char)(PutValue>>24);
        ((unsigned char *)Value)[1] = (unsigned char)(PutValue>>16);
        ((unsigned char *)Value)[2] = (unsigned char)(PutValue>>8);
        ((unsigned char *)Value)[3] = (unsigned char)PutValue;
    }else{
        ((unsigned char *)Value)[0] = (unsigned char)PutValue;
        ((unsigned char *)Value)[1] = (unsigned char)(PutValue>>8);
        ((unsigned char *)Value)[2] = (unsigned char)(PutValue>>16);
        ((unsigned char *)Value)[3] = (unsigned char)(PutValue>>24);
    }
}

//--------------------------------------------------------------------------
// Convert a 32 bit unsigned value from file's native byte order
//--------------------------------------------------------------------------
unsigned Get32u(void * Long)
{
    return (unsigned)Get32s(Long) & 0xffffffff;
}
//--------------------------------------------------------------------------
// Create minimal exif header - just date and thumbnail pointers,
// so that date and thumbnail may be filled later.
//--------------------------------------------------------------------------
void create_simple_EXIF(char *Buffer, int thumbnail_offset,int thumbnail_size)
{
    unsigned short NumEntries;
    int DataWriteIndex;
    int DateIndex;
    int DirIndex;
    int DirContinuation;
    time_t cur_time;
    char timeString[20];
    struct tm *ltime;
    time(&cur_time);
    ltime = localtime(&cur_time);
    sprintf(timeString, "%d:%02d:%02d %02d:%02d:%02d\n",
        1900 + ltime->tm_year,
	ltime->tm_mon + 1,
	ltime->tm_mday,
	ltime->tm_hour,
	ltime->tm_min,
	ltime->tm_sec
    );
    MotorolaOrder = 0;

    memcpy(Buffer, "Exif\0\0II",8);
    Put16u(Buffer+8, 0x2a);

    DataWriteIndex = 14;
    Put32u(Buffer+10, DataWriteIndex-6); // first IFD offset.  Means start 14 bytes in.

    {
        DirIndex = DataWriteIndex;
        NumEntries = 2;
        DataWriteIndex += 2 + NumEntries*12 + 4;

        Put16u(Buffer+DirIndex, NumEntries); // Number of entries
        DirIndex += 2;
  
        // Enitries go here...
        {
            // Date/time entry
            Put16u(Buffer+DirIndex, TAG_DATETIME);         // Tag
            Put16u(Buffer+DirIndex + 2, FMT_STRING);       // Format
            Put32u(Buffer+DirIndex + 4, 20);               // Components
            Put32u(Buffer+DirIndex + 8, DataWriteIndex-6); // Pointer or value.
            DirIndex += 12;

            DateIndex = DataWriteIndex;
            // If we had a pre-existing exif header, use time from that.
            memcpy(Buffer+DataWriteIndex, timeString, 19);
            Buffer[DataWriteIndex+19] = '\0';

            DataWriteIndex += 20;
        
            // Link to exif dir entry
            Put16u(Buffer+DirIndex, TAG_EXIF_OFFSET);      // Tag
            Put16u(Buffer+DirIndex + 2, FMT_ULONG);        // Format
            Put32u(Buffer+DirIndex + 4, 1);                // Components
            Put32u(Buffer+DirIndex + 8, DataWriteIndex-6); // Pointer or value.
            DirIndex += 12;
        }

        // End of directory - contains optional link to continued directory.
        DirContinuation = DirIndex;
    }

    {
        DirIndex = DataWriteIndex;
        NumEntries = 1;
        DataWriteIndex += 2 + NumEntries*12 + 4;

        Put16u(Buffer+DirIndex, NumEntries); // Number of entries
        DirIndex += 2;

        // Original date/time entry
        Put16u(Buffer+DirIndex, TAG_DATETIME_ORIGINAL);         // Tag
        Put16u(Buffer+DirIndex + 2, FMT_STRING);       // Format
        Put32u(Buffer+DirIndex + 4, 20);               // Components
        Put32u(Buffer+DirIndex + 8, DataWriteIndex-6); // Pointer or value.
        DirIndex += 12;

        memcpy(Buffer+DataWriteIndex, Buffer+DateIndex, 20);
        DataWriteIndex += 20;
        
        // End of directory - contains optional link to continued directory.
        Put32u(Buffer+DirIndex, 0);
    }

    {
        //Continuation which links to this directory;
        Put32u(Buffer+DirContinuation, DataWriteIndex-6);
        DirIndex = DataWriteIndex;
        NumEntries = 2;
        DataWriteIndex += 2 + NumEntries*12 + 4;

        Put16u(Buffer+DirIndex, NumEntries); // Number of entries
        DirIndex += 2;
        {
            // Link to exif dir entry
            Put16u(Buffer+DirIndex, TAG_THUMBNAIL_OFFSET);         // Tag
            Put16u(Buffer+DirIndex + 2, FMT_ULONG);       // Format
            Put32u(Buffer+DirIndex + 4, 1);               // Components
            Put32u(Buffer+DirIndex + 8, thumbnail_offset); // Pointer or value.
           // printf("TAG_THUMBNAIL_OFFSET 0x%X\n", thumbnail_offset);
            DirIndex += 12;

        }

        {
            // Link to exif dir entry
            Put16u(Buffer+DirIndex, TAG_THUMBNAIL_LENGTH);         // Tag
            Put16u(Buffer+DirIndex + 2, FMT_ULONG);       // Format
            Put32u(Buffer+DirIndex + 4, 1);               // Components
            Put32u(Buffer+DirIndex + 8,  thumbnail_size); // Pointer or value.
            //printf("TAG_THUMBNAIL_LENGTH 0x%X\n", thumbnail_size);
            DirIndex += 12;
        }

        // End of directory - contains optional link to continued directory.
        Put32u(Buffer+DirIndex, 0);
    }
}
