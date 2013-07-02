bin_PROGRAMS = bte

AM_CFLAGS = @CFLAGS@	\
	-I$(top_srcdir)/lib/libbard \
	-I$(top_srcdir)/lib/jansson \
	-I$(top_srcdir)/lib/libgnu \
	-I$(top_srcdir)/lib/lua

AM_CPPFLAGS = -DHAVE_CONFIG_H -DDATADIR=\"$(datadir)\"

bte_SOURCES = 	\
	bt_lua.c \
	bt_lua.h \
	btf.c \
	l_sdl.c \
	sdl/l_surface.c \
	sdl/l_anim.c \
	sdl/l_img.c \
	sdl/l_rect.c \
	sdl/l_color.c \
	sdl/l_font.c \
	sdl/l_textbox.c \
	sdl/l_timer.c \
	font.c \
	font.h \
	sdl.c \
	sdl.h \
	l_sdl.h \
	l_sys.c \
	l_int.h \
	l_utils.c \
	l_module.c \
	l_random.c \
	l_jansson.c \
	bte.c

bte_LDADD = \
	-lmingw32				\
	-lSDLmain				\
	-lSDL					\
	-lSDL_image				\
	-lSDL_TTF				\
	-lz					\
	$(top_builddir)/lib/lua/liblua.a	\
	$(top_builddir)/lib/libbard/libbard.a	\
	$(top_builddir)/lib/libgnu/libgnu.a	\
	$(top_builddir)/lib/jansson/libjansson.a	\
	-lpthread

.PHONE: bini
bini: bte.exe
	cp bte.exe ../bin
