.SUFFIXES : .x .o .c .s

SUBDIRS=wireless_tools.29 \
		busybox-1.22.1 \
		demos/wwdt \
		demos/uart \
		demos/thread \
		demos/alsa_audio \
		demos/irda \
		demos/wdt \
		demos/crypto \
		demos/etimer \
		demos/gpio \
		demos/lcm \
		demos/cap \
		demos/keypad \
        demos/rtc
        
BENCHMARK_SUBDIRS=benchmark/netperf-2.6.0
TSLIB_DIRS=tslib-1.1
UUID_DIRS=libuuid-1.0.3
LZO_DIRS=lzo-2.09
MTD_DIRS=mtd-utils
YAFFS2_DIRS=yaffs2utils

SETUPPATH=$(PWD)/$(MTD_DIRS)/install

define make_subdir
	@for subdir in $(SUBDIRS) ; do \
		( cd $$subdir && make $1) \
	done;
	@for subdir in $(BENCHMARK_SUBDIRS) ; do \
		( cd $$subdir && ./configure --host=arm-linux && make $1) \
	done;

	@for subdir in $(TSLIB_DIRS) ; do \
		( cd $$subdir && ./configure --host=arm-linux --enable-static && make $1) \
	done;

	@for subdir in $(MTD_DIRS) ; do \
	( \
	cd $(LZO_DIRS) && ./configure --host=arm-linux --prefix=$(SETUPPATH) && make && make install && cd .. ;\
	cd $(UUID_DIRS)&& ./configure --host=arm-linux --prefix=$(SETUPPATH) && \
			make ACLOCAL=aclocal AUTOMAKE=automake && make install && cd .. ; \
	cd $(MTD_DIRS) && make LZOCPPFLAGS=-I$(SETUPPATH)/include LZOLDFLAGS=-L$(SETUPPATH)/lib  \
			CROSS=arm-linux-  WITHOUT_XATTR=1 DESTDIR=$(SETUPPATH) && make install DESTDIR=$(SETUPPATH) \
	) \
	done;

	@for subdir in $(YAFFS2_DIRS) ; do \
	( cd $$subdir && make CROSS=arm-linux- ) \
	done;
endef

all:
	$(call make_subdir , all)
 
install :
	$(call make_subdir , install)
 
debug:
	$(call make_subdir , debug)
	
clean:
	$(call make_subdir , clean) 
