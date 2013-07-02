#ifndef _MON_H
#define _MON_H

extern uint8_t *monStr[];
extern uint8_t *monLevel[];
extern uint8_t *monMacro[];

extern uint16_t diceMask[];
extern uint8_t monAttrib[];
extern uint8_t monMask[];
extern uint8_t monPicIndex[];
extern uint8_t monPri[];
extern uint8_t monReward[];
extern uint8_t monAttacks[];
extern uint8_t monPronoun[];
extern uint8_t monAttType[];
extern uint8_t monSpellTrans[];

typedef struct {
  uint8_t name[16];
  uint8_t att[4];
  uint8_t strength;
  uint8_t race;
  uint8_t ac;
  uint8_t hpdice;
  uint8_t hpbase;
} b1summon_t;

extern b1summon_t sumlist[];

#endif
