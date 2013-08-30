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
 b2cnv/Makefile
 btcscnv/Makefile
 lib/Makefile
 lib/lua/Makefile
 lib/libbard/Makefile
 lib/libcnv/Makefile
 lib/libgnu/Makefile
 lib/jansson/Makefile
])

AC_ARG_WITH(dos-bard1,
	[  --with-dos-bard1=PATH   Specify path to DOS Bard1 installation ],
	[
		if test "x$withval" != "xno" ; then
			case "$withval" in
				./*|../*)	withval="`pwd`/$withval"
			esac
			if test -f "$withval/BARD.EXE" ; then
				BARDONEDIR=$withval
			else
				BARDONEDIR=
			fi
		fi
	]
)
AC_SUBST(BARDONEDIR)
AC_ARG_WITH(dos-bard2,
	[  --with-dos-bard2=PATH   Specify path to DOS Bard2 installation ],
	[
		if test "x$withval" != "xno" ; then
			case "$withval" in
				./*|../*)	withval="`pwd`/$withval"
			esac
			if test -f "$withval/DK.EXE" ; then
				BARDTWODIR=$withval
			else
				BARDTWODIR=
			fi
		fi
	]
)
AC_SUBST(BARDTWODIR)
AC_OUTPUT
