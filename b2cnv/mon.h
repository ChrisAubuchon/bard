#ifndef _B2_MON_H
#define _B2_MON_H

typedef struct {
  uint8_t name[16];
  uint16_t hpRnd;
  uint16_t hpBase;
  uint8_t maxgrp;
  uint8_t advancespeed;
  uint8_t baseac2hit;
  uint8_t numattacks;
  uint8_t attTypes[4];
  uint8_t meleeatttype;
  uint8_t basemelee;
  uint8_t picindex;
  uint8_t flags;
} b2mon_t;

extern b2mon_t sumMons[];
extern uint8_t fireConv[];

#endif
