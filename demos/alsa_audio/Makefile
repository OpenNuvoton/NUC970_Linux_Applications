.SUFFIXES : .x .o .c .s

ROOT = /usr/local/arm_linux_4.3/usr
LIB = $(ROOT)/lib
LIB1 =$(ROOT)/lib/gcc/arm-linux-uclibcgnueabi/4.3.4
INC1:=$(ROOT)/lib/gcc/arm-linux-uclibcgnueabi/4.3.4/include
INC :=$(ROOT)/include

CC := $(ROOT)/bin/arm-linux-gcc
STRIP := $(ROOT)/bin/arm-linux-strip
WEC_LDFLAGS=-L$(LIB) -L$(LIB1)

TARGET = oss_demo
SRCS := demo.c

LIBS= -lc -lgcc -lc

all: 
	$(CC) -O2 -I$(INC) -I$(INCSYS) -static $(WEC_LDFLAGS) $(SRCS) -o $(TARGET) $(LIBS)
	$(STRIP) $(TARGET) 
	
clean:
	rm -f *.o 
	rm -f *.x 
	rm -f *.flat
	rm -f *.map
	rm -f temp
	rm -f *.img
	rm -f $(TARGET)	
	rm -f *.gdb
