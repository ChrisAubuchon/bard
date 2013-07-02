#ifndef _LIBCNV_CNV_INT_H
#define _LIBCNV_CNV_INT_H

#include <cnvlib.h>
#include <cnv_common.h>
#include <gl_list.h>

#define iprintf(extind,fmt,args...) printf("%*s"fmt, (indent + (extind)), " ", ##args)

#if 0
typedef	json_t *(*toJsonFunc_t)(const void *);
typedef	btstring_t *(*toJsonNameFunc_t)(const void *);

json_t	*gl_list_to_json_array(toJsonFunc_t toJson, gl_list_t list);
json_t	*gl_list_to_json_object(toJsonFunc_t toJson, 
			toJsonNameFunc_t toJsonName, gl_list_t list);
#endif

#endif
