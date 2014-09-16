.SUFFIXES : .x .o .cpp .s

CC := arm-linux-gcc
STRIP := arm-linux-strip
LIBS = -lpthread -lc -lgcc

TARGETS=ex1 ex2 ex3 ex4 ex5 ex6 ex7
all: $(TARGETS)
	$(STRIP) ex1 ex2 ex3 ex4 ex5 ex6 ex7

ex1: ex1.o
	$(CC) -static ex1.o  -o ex1 $(LIBS)


ex2: ex2.o
	$(CC) -static ex2.o  -o ex2 $(LIBS)


ex3: ex3.o
	$(CC) -static ex3.o  -o ex3 $(LIBS)


ex4: ex4.o
	$(CC) -static ex4.o -o ex4 $(LIBS)


ex5: ex5.o
	$(CC) -static ex5.o  -o ex5 $(LIBS)


ex6: ex6.o
	$(CC) -static ex6.o  -o ex6 $(LIBS)


ex7: ex7.o
	$(CC) -static ex7.o  -o ex7 $(LIBS)


.c.o:
	$(CC) -c $<

clean:
	rm -f *.[oa] *~ core $(TARGETS) temp *.bak *.x *.gdb

