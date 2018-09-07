.SUFFIXES : .x .o .c .s

#ROOT = /usr/local/arm_linux_4.2
#INC = $(ROOT)/arm-linux/include
#LIB = $(ROOT)/lib/gcc/arm-linux/4.2.1
#LIB1 = $(ROOT)/arm-linux/lib
INC1 = ../../Include
INC2 = ../../Config
LIB2 = ../../Lib

CC = arm-linux-gcc -O2 -static -I$(INC1) -I$(INC2) -I.
INCS = -I$(INC1) -I$(INC2)
WEC_LDFLAGS = -L$(LIB2) 
STRIP = arm-linux-strip

LIU= main.c input-raw.c linear.c FramewinDLG.c fbutils.c font_8x8.c
LIUOBJ = main.o input-raw.o linear.o FramewinDLG.o fbutils.o font_8x8.o
TARGET = simpleDemo
SRCS =  $(INC2)/GUI_X.c $(INC2)/GUIConf.c $(INC2)/LCDConf.c $(INC2)/NVTFont16.c main.c input-raw.c linear.c FramewinDLG.c fbutils.c font_8x8.c N9H30TouchPanel.c
OBJS = GUI_X.o GUIConf.o LCDConf.o NVTFont16.o main.o input-raw.o linear.o FramewinDLG.o fbutils.o font_8x8.o N9H30TouchPanel.o
#LIBS = -lgcc
LIBS = $(OBJS)

	
all:  $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(LIBS) $(WEC_LDFLAGS) -lNUemWin -lm -L. -lts -ldl -static -pthread
	$(STRIP) $(TARGET)
	
GUI_X.o : $(INC2)/GUI_X.c
	$(CC) -c -o $@ $<

main.o : main.c
	$(CC) -c -o $@ $< 

input-raw.o : input-raw.c
	$(CC) -c -o $@ $< 

linear.o : linear.c
	$(CC) -c -o $@ $< 

FramewinDLG.o : FramewinDLG.c
	$(CC) -c -o $@ $<

fbutils.o : fbutils.c fbutils.h $(INC2)/LCDConf.h
	$(CC) -c -o $@ $<

font_8x8.o : font_8x8.c
	$(CC) -c -o $@ $<

GUIConf.o : $(INC2)/GUIConf.c
	$(CC) -c -o $@ $<

LCDConf.o : $(INC2)/LCDConf.c $(INC2)/LCDConf.h
	$(CC) -c -o $@ $<

NVTFont16.o : $(INC2)/NVTFont16.c
	$(CC) -c -o $@ $<

N9H30TouchPanel.o : N9H30TouchPanel.c
	$(CC) -c -o $@ $<
	
clean:
	rm -f *.o
	rm -f *.x
	rm -f *.flat
	rm -f *.map
	rm -f temp
	rm -f *.img
	rm -f $(TARGET)	
	rm -f *.gdb
	rm -f *.bak
	
