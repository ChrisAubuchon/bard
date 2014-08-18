bin_PROGRAMS = b3cnv b3disasm

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
	city.c		\
	city_ops.c	\
	class.c		\
	class.h		\
	class_data.c	\
	data_ops.c	\
	dun.c		\
	dun.h		\
	dun_data.c	\
	dun_ops.c	\
	gfx.c		\
	gfx.h		\
	gfx_bigpic.c	\
	gfx_tile.c	\
	gfx_tile.h	\
	item.c		\
	item.h		\
	item_data.c	\
	mon.c		\
	mon.h		\
	mon_data.c	\
	picIndex.c	\
	race.c		\
	spell.c		\
	spell.h		\
	spell_data.c	\
	summon_data.c	\
	trap.h		\
	trap_data.c

b3cnv_LDADD = \
	$(top_builddir)/lib/libcnv/libcnv.a	\
	$(top_builddir)/lib/libbard/libbard.a	\
	$(top_builddir)/lib/libgnu/libgnu.a	\
	$(top_builddir)/lib/jansson/libjansson.a	\
	-lpng					\
	-lz					

b3disasm_SOURCES =	\
	b3disasm.c	\
	b3lib.h		\
	city.h		\
	city_ops.c	\
	class.h		\
	class_data.c	\
	data_ops.c	\
	dun.h		\
	dun_data.c	\
	dun_ops.c	\
	item.h		\
	item_data.c	\
	mon.h		\
	mon_data.c	\
	spell.h		\
	spell_data.c	\
	picIndex.c	

b3disasm_LDADD = \
	$(top_builddir)/lib/libbard/libbard.a		\
	$(top_builddir)/lib/libcnv/libcnv.a		\
	$(top_builddir)/lib/libgnu/libgnu.a	
