.SUFFIXES : .x .o .c .s

CC := arm-linux-gcc
STRIP := arm-linux-strip

TARGET = dma_proxy_test
SRCS := dma_proxy_test.c
LIBS = -lpthread -lc -lgcc

all:
	$(CC) -static $(SRCS) -o $(TARGET) $(LIBS)
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
