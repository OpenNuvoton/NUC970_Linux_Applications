.SUFFIXES : .x .o .c .s

CC := arm-linux-gcc
STRIP := arm-linux-strip

TARGET = ebi_demo
SRCS := ebi.c

LIBS= -lc -lgcc -lc

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
