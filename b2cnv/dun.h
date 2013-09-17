#ifndef _B1CNV_DUN_H
#define _B1CNV_DUN_H

typedef struct {
	uint8_t		*name;
	uint8_t		numLevels;
	uint8_t		levels[5];
} b2levData_t;

extern b2levData_t	b2dungeons[];
extern uint8_t		*b2dun_specialCode[];

#endif
