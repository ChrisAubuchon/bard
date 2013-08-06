#ifndef _B1CNV_DUN_H
#define _B1CNV_DUN_H

typedef struct {
	uint8_t		*name;
	uint8_t		numLevels;
	uint8_t		levels[5];
} b1levData_t;

extern b1levData_t	b1dungeons[];
extern uint8_t		b1dun_randomMask[];
extern uint8_t		b1dun_randomOffset[];
extern uint8_t		b1dun_poisonDamage[];
extern uint8_t		*b1dun_specialCode[];

#endif
