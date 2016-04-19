.SUFFIXES : .x .o .c .s

ROOT = /usr/local/arm_linux_4.3.2/usr
LIB = $(ROOT)/lib
LIB1 =$(ROOT)/lib/gcc/arm-linux-uclibcgnueabi/4.3.4
INC1:=$(ROOT)/lib/gcc/arm-linux-uclibcgnueabi/4.3.4/include
INC :=$(ROOT)/include


CC=arm-linux-gcc -O2 -I$(INC) -I$(INC1) -static
WEC_LDFLAGS=-L$(LIB) -L$(LIB1)
STRIP=arm-linux-strip

TARGET = keypad_demo
SRCS := keypad.c

LIBS= -lc -lgcc -lc -lm

all:
	$(CC) $(WEC_LDFLAGS) $(SRCS) -o $(TARGET) $(LIBS)
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
