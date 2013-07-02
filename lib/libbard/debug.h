#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef DEBUG

#include <fpio.h>

#define debug(fmt, args...) printf("%d: "fmt, __LINE__, ##args)
#define dump_btstring(fname,bt,fexit) {\
	FILE *fp;							\
	fp = xfopen((fname), "wb");					\
	xfwrite((bt)->buf, sizeof(uint8_t), (bt)->size, fp);		\
	fclose(fp);							\
	if ((fexit)) {							\
		exit(0);						\
	}								\
}
	
#else

#define debug(fmt, args...)
#define dump_btstring(fname, bt, exit)

#endif



#endif
