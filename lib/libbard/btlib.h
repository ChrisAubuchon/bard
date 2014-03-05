#ifndef _BTLIB_H
#define _BTLIB_H

#if HAVE_CONFIG_H
#include <config.h>
#endif

#if HAVE_STDARG_H

#  if !HAVE_ASPRINTF
int rpl_asprintf(char **, const char *, ...);
#  define asprintf rpl_asprintf
#  endif

#endif

#if HAVE_STDINT_H
#include <stdint.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

#include <stdio.h>

#define bt_error(format, ...) error_at_line(1, 1, __FILE__, __LINE__, format, __VA_ARGS__)

#include <bt_string.h>
#include <endian.h>
#include <fileio.h>
#include <fpio.h>
#include <xalloc.h>


#endif
