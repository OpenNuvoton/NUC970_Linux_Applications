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

TARGET = GUIDemo
SRCS =  $(INC2)/GUI_X.c $(INC2)/LCDConf.c $(INC2)/NVTFont16.c main.c input-raw.c linear.c fbutils.c font_8x8.c GUIDEMO.c GUIDEMO_AntialiasedText.c GUIDEMO_Automotive.c GUIDEMO_BarGraph.c GUIDEMO_Bitmap.c GUIDEMO_ColorBar.c GUIDEMO_Conf.c GUIDEMO_Cursor.c GUIDEMO_Fading.c GUIDEMO_Graph.c GUIDEMO_IconView.c GUIDEMO_ImageFlow.c GUIDEMO_Intro.c GUIDEMO_Listview.c GUIDEMO_RadialMenu.c GUIDEMO_Resource.c GUIDEMO_Skinning.c GUIDEMO_Speed.c GUIDEMO_Speedometer.c GUIDEMO_Start.c GUIDEMO_TransparentDialog.c GUIDEMO_Treeview.c GUIDEMO_VScreen.c GUIDEMO_WashingMachine.c GUIDEMO_ZoomAndRotate.c N9H30TouchPanel.c

OBJS = GUI_X.o GUIConf.o LCDConf.o NVTFont16.o main.o input-raw.o linear.o fbutils.o font_8x8.o GUIDEMO.o GUIDEMO_AntialiasedText.o GUIDEMO_Automotive.o GUIDEMO_BarGraph.o GUIDEMO_Bitmap.o GUIDEMO_ColorBar.o GUIDEMO_Conf.o GUIDEMO_Cursor.o GUIDEMO_Fading.o GUIDEMO_Graph.o GUIDEMO_IconView.o GUIDEMO_ImageFlow.o GUIDEMO_Intro.o GUIDEMO_Listview.o GUIDEMO_RadialMenu.o GUIDEMO_Resource.o GUIDEMO_Skinning.o GUIDEMO_Speed.o GUIDEMO_Speedometer.o GUIDEMO_Start.o GUIDEMO_TransparentDialog.o GUIDEMO_Treeview.o GUIDEMO_VScreen.o GUIDEMO_WashingMachine.o GUIDEMO_ZoomAndRotate.o N9H30TouchPanel.o

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

fbutils.o : fbutils.c fbutils.h $(INC2)/LCDConf.h
	$(CC) -c -o $@ $<

font_8x8.o : font_8x8.c
	$(CC) -c -o $@ $<

GUIConf.o : GUIConf.c
	$(CC) -c -o $@ $<

LCDConf.o : $(INC2)/LCDConf.c $(INC2)/LCDConf.h
	$(CC) -c -o $@ $<

NVTFont16.o : $(INC2)/NVTFont16.c
	$(CC) -c -o $@ $<

N9H30TouchPanel.o : N9H30TouchPanel.c
	$(CC) -c -o $@ $<

GUIDEMO.o : GUIDEMO.c	
	$(CC) -c -o $@ $<

GUIDEMO_AntialiasedText.o : GUIDEMO_AntialiasedText.c GUIDEMO.h
	$(CC) -c -o $@ $<

GUIDEMO_Automotive.o : GUIDEMO_Automotive.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_BarGraph.o : GUIDEMO_BarGraph.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Bitmap.o : GUIDEMO_Bitmap.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_ColorBar.o : GUIDEMO_ColorBar.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Conf.o : GUIDEMO_Conf.c GUIDEMO.h
	$(CC) -c -o $@ $<

GUIDEMO_Cursor.o : GUIDEMO_Cursor.c GUIDEMO.h
	$(CC) -c -o $@ $<

GUIDEMO_Fading.o : GUIDEMO_Fading.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Graph.o : GUIDEMO_Graph.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_IconView.o : GUIDEMO_IconView.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_ImageFlow.o : GUIDEMO_ImageFlow.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Intro.o : GUIDEMO_Intro.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Listview.o : GUIDEMO_Listview.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_RadialMenu.o : GUIDEMO_RadialMenu.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Resource.o : GUIDEMO_Resource.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Skinning.o : GUIDEMO_Skinning.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Speed.o : GUIDEMO_Speed.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Speedometer.o : GUIDEMO_Speedometer.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Start.o : GUIDEMO_Start.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_TransparentDialog.o : GUIDEMO_TransparentDialog.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_Treeview.o : GUIDEMO_Treeview.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_VScreen.o : GUIDEMO_VScreen.c GUIDEMO.h
	$(CC) -c -o $@ $<
	
GUIDEMO_WashingMachine.o : GUIDEMO_WashingMachine.c GUIDEMO.h
	$(CC) -c -o $@ $<

GUIDEMO_ZoomAndRotate.o : GUIDEMO_ZoomAndRotate.c GUIDEMO.h
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
	
