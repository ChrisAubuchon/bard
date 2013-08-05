bin_PROGRAMS = btcscnv

AM_CFLAGS = @CFLAGS@	\
	-I$(top_srcdir)/lib/libbard  \
	-I$(top_srcdir)/lib/libgnu

AM_CPPFLAGS = -DHAVE_CONFIG_H 

btcscnv_SOURCES = 	\
	btcscnv.c

btcscnv_LDADD = \
	$(top_builddir)/lib/libbard/libbard.a	\
	$(top_builddir)/lib/libgnu/libgnu.a	\
	-lpng					\
	-lz					

