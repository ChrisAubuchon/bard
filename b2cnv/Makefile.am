bin_PROGRAMS = b2cnv

AM_CFLAGS = @CFLAGS@	\
	-I$(top_srcdir)/lib/libbard \
	-I$(top_srcdir)/lib/libgnu \
	-I$(top_srcdir)/lib/jansson \
	-I$(top_srcdir)/lib/libcnv

AM_CPPFLAGS = -DHAVE_CONFIG_H -DDATADIR=\"$(datadir)\" \
	-DBASEDIR=\"$(BARDTWODIR)\"

b2cnv_SOURCES = 	\
	b2cnv.c		\
	b2lib.h		\
	bitmap.c	\
	city.c		\
	class.c		\
	dun.c		\
	dun.h		\
	dun_data.c	\
	gfx.c		\
	gfx.h		\
	gfx_bigpic.c	\
	gfx_bigpic.h	\
	gfx_city.c	\
	gfx_dun.c	\
	gfx_wild.c	\
	item.c		\
	item.h		\
	item_data.c	\
	level.h		\
	level_data.c	\
	mon.c		\
	mon.h		\
	mon_data.c	\
	picIndex.c	\
	race.c		\
	spell.c		\
	spell.h		\
	spell_data.c	\
	trap.h		\
	trap_data.c	\
	summon_data.c	\
	wild.c

b2cnv_LDADD = \
	$(top_builddir)/lib/libcnv/libcnv.a	\
	$(top_builddir)/lib/libbard/libbard.a	\
	$(top_builddir)/lib/libgnu/libgnu.a	\
	$(top_builddir)/lib/jansson/libjansson.a	\
	-lpng					\
	-lz					

