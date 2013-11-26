bin_PROGRAMS = bte 

AM_CFLAGS = @CFLAGS@	\
	-I$(top_srcdir)/lib/libbard \
	-I$(top_srcdir)/lib/jansson \
	-I$(top_srcdir)/lib/libgnu \
	-I$(top_srcdir)/lib/lua

AM_CPPFLAGS = -DHAVE_CONFIG_H -DDATADIR=\"$(datadir)\"

bte_SOURCES = 	\
	bt_lua.c				\
	bt_lua.h				\
	btf.c					\
	sdl/l_anim.c				\
	sdl/l_font.c				\
	sdl/l_timer.c				\
	sdl/l_img.c				\
	sdl/l_rect.c				\
	sdl/l_renderer.c			\
	sdl/l_sdl.c				\
	sdl/l_surface.c				\
	sdl/l_textbox.c				\
	sdl/l_texture.c				\
	sdl/l_window.c				\
	sdl/sdl_util.c				\
	font.c \
	font.h \
	l_sdl.h \
	l_sprintf.c	\
	l_sys.c \
	l_utils.c \
	l_module.c				\
	l_module.h				\
	l_random.c \
	l_jansson.c \
	bte.c

bte_LDADD = \
	-lmingw32				\
	-lSDL2main				\
	-lSDL2					\
	-lSDL2_image				\
	-lz					\
	$(top_builddir)/lib/lua/liblua.a	\
	$(top_builddir)/lib/libbard/libbard.a	\
	$(top_builddir)/lib/libgnu/libgnu.a	\
	$(top_builddir)/lib/jansson/libjansson.a	\
	-lpthread

.PHONE: bini
bini: bte.exe
	cp bte.exe ../bin
