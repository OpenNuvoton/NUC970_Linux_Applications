.SUFFIXES : .x .o .c .s

CC=arm-linux-gcc
LINK=$(CC)
STRIP=arm-linux-strip
CFLAGS=-MD -O2
LDFLAGS=-L../../Lib -lNUemWin -lm -L./tslib -lts -ldl -pthread -static

APPDIR=.
CONFHEADER=../../Config
GUIHEADER=../../Include
CONFDIR=$(CONFHEADER)
TSLIBDIR=tslib
TSLIBHEADER=$(TSLIBDIR)

BINARY=SimpleDemo

APPSOURCES := $(shell find $(APPDIR) -name '*.c')
APPOBJECTS := $(addprefix ./,$(APPSOURCES:%.c=%.o))
APPDFILES := $(addprefix ./,$(APPSOURCES:%.c=%.d))

CONFSOURCES := $(shell find $(CONFDIR) -name '*.c')
CONFOBJECTS := $(addprefix ./,$(CONFSOURCES:%.c=%.o))
CONFDFILES := $(addprefix ./,$(CONFSOURCES:%.c=%.d))

RM=rm -rf

all: $(BINARY)

$(APPDIR)/%.o: $(APPDIR)/%.c
	$(CC) $(CFLAGS) -I. -I$(CONFHEADER) -I$(GUIHEADER) -I$(TSLIBHEADER) -c $< -o $@

$(CONFDIR)/%.o: $(CONFDIR)/%.c
	$(CC) $(CFLAGS) -I. -I$(CONFHEADER) -I$(GUIHEADER) -c $< -o $@

$(BINARY): $(APPOBJECTS) $(CONFOBJECTS)
	$(LINK) $(APPOBJECTS) $(CONFOBJECTS) $(LDFLAGS) -o $(BINARY)
	$(STRIP) $(BINARY)

clean:
	$(RM) $(BINARY) $(APPOBJECTS) $(APPDFILES) $(CONFOBJECTS) $(CONFDFILES)
