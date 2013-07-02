#ifndef _SND_DATA_H
#define _SND_DATA_H

#include <stdint.h>

typedef struct {
  uint16_t freq;
  uint16_t ee2;
  uint16_t offset;
  uint8_t noteDur;
  uint8_t ee7;
  uint8_t ee8;
  int8_t ee9;
  uint8_t eea;
  uint8_t eeb;
  uint8_t eec;
  uint8_t eed;
  uint8_t eee;
  uint8_t eef;
  uint8_t ef0;
  uint8_t ef1;
  uint8_t ef2;
} song_t;

extern uint16_t baseFreq;
extern uint16_t freqMultipliers[];
extern uint8_t songData[];
extern uint16_t songIndex[];
extern uint16_t songLength[];
extern uint8_t *noteString[];

#endif
