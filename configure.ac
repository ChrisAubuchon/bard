AC_INIT([bte],[1.00])
AM_INIT_AUTOMAKE
AC_CANONICAL_BUILD
AC_CONFIG_SRCDIR([bte/l_xml.c])

AC_GNU_SOURCE

AC_PROG_CC
gl_EARLY
gl_INIT
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_RANLIB
AC_CHECK_HEADERS(pthread.h)
AC_CHECK_FUNCS(strdup)
HW_FUNC_SNPRINTF
HW_FUNC_ASPRINTF

AC_SEARCH_LIBS([compress], [z])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([
 Makefile
 bte/Makefile
 b1cnv/Makefile
 lib/Makefile
 lib/lua/Makefile
 lib/libbard/Makefile
 lib/libcnv/Makefile
 lib/libgnu/Makefile
 lib/jansson/Makefile
])
AC_OUTPUT
