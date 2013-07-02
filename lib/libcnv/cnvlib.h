#ifndef _CNVLIB_H
#define _CNVLIB_H

#if HAVE_CONFIG_H
#include <config.h>
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

#include <bt_string.h>
#include <endian.h>
#include <fileio.h>
#include <fpio.h>

#include <jansson.h>
/*#include <gl_xlist.h>*/


/********************************/
/*				*/
/* Some global typedefs		*/
/*				*/
/********************************/

typedef	void		(*btFunc_free)(const void *data);
typedef json_t *	(*btFunc_toJson)(const void *data);
typedef btstring_t *	(*btFunc_toName)(const void *data);

#include <cnv_list.h>

/********************************/
/*				*/
/* JSON helper macros		*/
/*				*/
/********************************/

#define JSON_FALSE(obj, name) json_object_set_new(obj, name, json_false())
#define JSON_TRUE(obj, name) json_object_set_new(obj, name, json_true())
#define JSON_BTSTRING(obj, name, _str) \
	json_object_set_new(obj, name, json_string(_str->buf))
#define JSON_STRING(obj, name, _str) \
	json_object_set_new(obj, name, json_string(_str))
#define JSON_NUMBER(obj, name, _num) \
	json_object_set_new(obj, name, json_integer(_num))
#define JSON_NUMBER_IF_NOT_ZERO(obj, name, _num) \
	do {								\
		if (_num != 0) {					\
			json_object_set_new(obj, name, json_integer(_num));\
		}							\
	} while(0)
#define	JSON_TRUE_IF(obj, name, _cond) \
	do {								\
		if (_cond) {						\
			json_object_set_new(obj, name, json_true());	\
		}							\
	} while(0)
#define JSON_BOOL(obj, name, _cond) \
	do {								\
		if (_cond)						\
			json_object_set_new(obj, name, json_true());	\
		else							\
			json_object_set_new(obj, name, json_false());	\
	} while(0)
#define JSON_NULL(obj, name) \
	json_object_set_new(obj, name, json_null())

#define JSON_DUMP(jsonNode, jsonBtstring)				\
	do {								\
		json_dump_file(jsonNode, jsonBtstring->buf,		\
				JSON_PRESERVE_ORDER | JSON_INDENT(2));	\
		json_decref(jsonNode);					\
		bts_free(fname);					\
	} while(0)
#define JSON_BTSTRING_IF(obj, name, flag) 				\
	do {								\
		if (flag != NULL) {					\
			JSON_STRING(obj, name, flag->buf);		\
		}							\
	} while(0)
#define JSON_STRING_IF(obj, name, flag) 				\
	do {								\
		if (flag != NULL) {					\
			JSON_STRING(obj, name, flag);			\
		}							\
	} while(0)

#define Bitfield(x,n)	unsigned x:n

#endif
