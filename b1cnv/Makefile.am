bin_PROGRAMS = b1cnv

AM_CFLAGS = @CFLAGS@	\
	-I$(top_srcdir)/lib/libbard \
	-I$(top_srcdir)/lib/libgnu \
	-I$(top_srcdir)/lib/jansson \
	-I$(top_srcdir)/lib/libcnv

AM_CPPFLAGS = -DHAVE_CONFIG_H -DDATADIR=\"$(datadir)\" \
	-DBASEDIR=\"$(BARDONEDIR)\"

b1cnv_SOURCES = 	\
	b1cnv.c	\
	b1lib.h	\
	bi_mousegfx.c	\
	bitmap.c	\
	city.c		\
	class.c		\
	cnv.c	\
	dun.c		\
	dun_data.c	\
	dun.h		\
	common.c	\
	common.h	\
	gfx.c	\
	gfx.h	\
	gfx_bigpic.c	\
	gfx_bigpic.h	\
	gfx_city.c	\
	gfx_dun.c	\
	item.c	\
	item.h	\
	item_data.c	\
	mon.c	\
	mon.h	\
	mon_data.c	\
	picIndex.c	\
	race.c		\
	song_data.c	\
	spell.c	\
	spell.h	\
	spell_data.c	\
	summon_data.c	\
	trap.c	\
	trap.h	\
	trap_data.c

b1cnv_LDADD = \
	$(top_builddir)/lib/libcnv/libcnv.a	\
	$(top_builddir)/lib/libbard/libbard.a	\
	$(top_builddir)/lib/libgnu/libgnu.a	\
	$(top_builddir)/lib/jansson/libjansson.a	\
	-lpng					\
	-lz					


.PHONY: xml
xml: b1cnv.exe
	./b1cnv -g > b1_gfx.out
	./b1cnv -i > b1_item.out
	./b1cnv -d > b1_dun.out
