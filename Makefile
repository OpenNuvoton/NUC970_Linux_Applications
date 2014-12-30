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
endef

all:
	$(call make_subdir , all)
 
install :
	$(call make_subdir , install)
 
debug:
	$(call make_subdir , debug)
	
clean:
	$(call make_subdir , clean) 
