SUBDIRS=res

noinst_PROGRAMS=graphics graphicsex

EXTRA_DIST=graphics.c

CUSTOMMG_SRC=../src/custom_minigui.c
graphics_SOURCES=graphics.c graphics_res_en.h graphics_res_cn.h \
				 graphics_res_tw.h $(CUSTOMMG_SRC)
graphicsex_SOURCES=graphicsex.c graphicsex_res_en.h \
				   graphicsex_res_cn.h graphicsex_res_tw.h \
				   $(CUSTOMMG_SRC)
