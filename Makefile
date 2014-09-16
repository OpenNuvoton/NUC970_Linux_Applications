.SUFFIXES : .x .o .c .s

#SUBDIRS=`ls -d */ | grep -v 'bin' | grep -v 'lib' | grep -v 'include'`

SUBDIRS=wireless_tools.29 \
		benchmark/netperf-2.6.0 \
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
		demos/cap
		

define make_subdir
	@for subdir in $(SUBDIRS) ; do \
		( cd $$subdir && make $1) \
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
 