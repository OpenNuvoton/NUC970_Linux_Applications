.SUFFIXES : .x .o .cpp .s

ROOT = /usr/local/arm_linux_4.3/usr
LIB = $(ROOT)/lib
LIB1 =$(ROOT)/lib/gcc/arm-linux-uclibcgnueabi/4.3.4
INC1:=$(ROOT)/lib/gcc/arm-linux-uclibcgnueabi/4.3.4/include
INC :=$(ROOT)/include

CC=arm-linux-gcc -O2 -I$(INC) -I$(INC1)
WEC_LDFLAGS=-L$(LIB) -L$(LIB1)
STRIP=arm-linux-strip
LIBS = -lpthread -lc -lgcc

TARGETS=ex1 ex2 ex3 ex4 ex5 ex6 ex7
all: $(TARGETS)
	$(STRIP) ex1 ex2 ex3 ex4 ex5 ex6 ex7

ex1: ex1.o
	$(CC) $(WEC_LDFLAGS) ex1.o  -o ex1 $(LIBS)


ex2: ex2.o
	$(CC) $(WEC_LDFLAGS) ex2.o  -o ex2 $(LIBS)


ex3: ex3.o
	$(CC) $(WEC_LDFLAGS) ex3.o  -o ex3 $(LIBS)


ex4: ex4.o
	$(CC) $(WEC_LDFLAGS) ex4.o -o ex4 $(LIBS)


ex5: ex5.o
	$(CC) $(WEC_LDFLAGS) ex5.o  -o ex5 $(LIBS)


ex6: ex6.o
	$(CC) $(WEC_LDFLAGS) ex6.o  -o ex6 $(LIBS)


ex7: ex7.o
	$(CC) $(WEC_LDFLAGS) ex7.o  -o ex7 $(LIBS)


.c.o:
	$(CC) -c $<

clean:
	rm -f *.[oa] *~ core $(TARGETS) temp *.bak *.x *.gdb

