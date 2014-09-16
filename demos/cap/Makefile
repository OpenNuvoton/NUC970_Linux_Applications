.SUFFIXES : .x .o .c .s

CC := arm-linux-gcc
STRIP := arm-linux-strip

TARGET = cap_demo
SRCS := cap_demo.c
LIBS= -lc -lgcc -lc -lpthread

all: 
	$(CC) -static $(SRCS) -o $(TARGET)
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
	rm -f *.bak
