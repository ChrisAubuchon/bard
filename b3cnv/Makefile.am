bin_PROGRAMS = b3cnv

AM_CFLAGS = @CFLAGS@	\
	-I$(top_srcdir)/lib/libbard \
	-I$(top_srcdir)/lib/libgnu \
	-I$(top_srcdir)/lib/jansson \
	-I$(top_srcdir)/lib/libcnv

AM_CPPFLAGS = -DHAVE_CONFIG_H 

b3cnv_SOURCES = 	\
	b3cnv.c		\
	b3lib.h		\
	bitmap.c	\
	gfx.c		\
	gfx.h		\
	gfx_bigpic.c	\
	gfx_tile.c	\
	gfx_tile.h	\
	picIndex.c	\
	spell.c		\
	spell.h		\
	spell_data.c

b3cnv_LDADD = \
	$(top_builddir)/lib/libcnv/libcnv.a	\
	$(top_builddir)/lib/libbard/libbard.a	\
	$(top_builddir)/lib/libgnu/libgnu.a	\
	$(top_builddir)/lib/jansson/libjansson.a	\
	-lpng					\
	-lz					

