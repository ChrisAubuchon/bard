#include <stdio.h>
#include <stdint.h>
#include <snd_data.h>

#define NOTEDUR_OFFSET	6
#define OFFSET_OFFSET	4

/*#define DEBUG*/
#include <debug.h>

static song_t song;
static uint16_t baseIndex;

static uint16_t str_read16le(uint8_t *buf);
static void set_songdat(uint8_t index, uint16_t val);
static uint16_t get_songdat(uint8_t index);
static uint8_t *songdata_str(uint8_t index);


static uint16_t str_read16le(uint8_t *buf)
{
#if BYTE_ORDER == BIG_ENDIAN
  return (uint16_t)((uint8_t)buf[0] | ((uint8_t)buf[1] << 8));
#else
  return *((uint16_t *)buf);
#endif
} 

static void set_songdat(uint8_t index, uint16_t val)
{
  switch (index) {
    case 0:
	song.freq = val;
	break;
    case 2:
	song.ee2 = val;
	break;
    case 4:
	song.offset = val;
	break;
    case 6:
	song.noteDur = val & 0xff;
	break;
    case 7:
	song.ee7 = val & 0xff;
	break;
    case 8:
	song.ee8 = val & 0xff;
	break;
    case 9:
	song.ee9 = val & 0xff;
	break;
    case 10:
	song.eea = val & 0xff;
	break;
    case 11:
	song.eeb = val & 0xff;
	break;
    case 12:
	song.eec = val & 0xff;
	break;
    case 13:
	song.eed = val & 0xff;
	break;
    case 14:
	song.eee = val & 0xff;
	break;
    case 15:
	song.eef = val & 0xff;
	break;
    case 16:
	song.ef0 = val & 0xff;
	break;
    case 17:
	song.ef1 = val & 0xff;
	break;
    case 18:
	song.ef2 = val & 0xff;
	break;
  }
}

static uint16_t get_songdat(uint8_t index)
{
  switch (index) {
    case 0:
	return song.freq;
    case 2:
	return song.ee2;
    case 4:
	return song.offset;
    case 6:
	return song.noteDur;
    case 7:
	return song.ee7;
    case 8:
	return song.ee8;
    case 9:
	return song.ee9;
    case 10:
	return song.eea;
    case 11:
	return song.eeb;
    case 12:
	return song.eec;
    case 13:
	return song.eed;
    case 14:
	return song.eee;
    case 15:
	return song.eef;
    case 16:
	return song.ef0;
    case 17:
	return song.ef1;
    case 18:
	return song.ef2;
  }
}

static uint8_t *songdat_str(uint8_t index)
{
  switch (index) {
    case 0:
	return "song.freq";
    case 2:
	return "song.ee2";
    case 4:
	return "song.offset";
    case 6:
	return "song.noteDur";
    case 7:
	return "song.ee7";
    case 8:
	return "song.ee8";
    case 9:
	return "song.ee9";
    case 10:
	return "song.eea";
    case 11:
	return "song.eeb";
    case 12:
	return "song.eec";
    case 13:
	return "song.eed";
    case 14:
	return "song.eee";
    case 15:
	return "song.eef";
    case 16:
	return "song.ef0";
    case 17:
	return "song.ef1";
    case 18:
	return "song.ef2";
  }
}

static uint16_t notes[84];

static void configNotes(void);
static void configNotes(void)
{
  uint8_t shift = 0;
  uint8_t count = 0;
  uint8_t index = 0;
  uint16_t freq;
  int i;

  for (shift = 0; shift < 7; shift++) {
    freq = baseFreq;

    for (count = 0; count < 12; count++) {
      notes[index++] = (freq & 0x1) + (freq >> shift);
      freq = ((freqMultipliers[count] * baseFreq) & 0xffff0000) >> 16;
    }
  }
}

static uint8_t byte_32ee8 = 0x8;
static uint8_t byte_32eea = 0x0a;
static uint8_t byte_32ef1 = 0x11;
static uint8_t byte_32ef2 = 0x12;

/*
static void parseSong(uint16_t index);
static void parseSong(uint16_t index)
*/
static void getNextNote(void);
static void getNextNote(void)
{
  uint8_t ch, saved;
  uint16_t word;
  uint8_t stru_offset;

entry:
    debug("offset = %x\n", song.offset + 0x100);
    ch = songData[song.offset++];

    if (ch & 0x80) {
      ch &= 0x7f;

      if (song.ef2)
        debug("song.ef2 = %d\n", song.ef2);

      song.freq = ch + song.ef2;

      song.eea = 3;

      ch = songData[song.offset++];
      song.noteDur = ch & 0x3f;
      if (ch & 0xc0) {
	if (ch & 0x80)
	  ch = 0;
	else
	  ch = ch >> 1;
      }
      ch -= song.ef1;
      song.ee8 = ch;
      debug("song.ee8 = %d\n", song.ee8);
    } else if (ch & 0x40) {
      stru_offset = (ch & 0x3f) + 0x0c;
      saved = ch & 0x3f;
      word = str_read16le(&songData[song.offset]);
      song.offset += 2;
      debug("%s = %d\n", songdat_str(stru_offset), get_songdat(stru_offset));
      if (get_songdat(stru_offset) != 0) {
	set_songdat(stru_offset, get_songdat(stru_offset) - 1);
	if (get_songdat(stru_offset) == 0)
	  goto entry;
      }
      debug("Old offset = %x, New offset = %x\n", song.offset + 0x100, word);
      song.offset = word - 0x100;
      if (song.offset == baseIndex) {
	printf("Loop to beginning\n");
	return;
      }
      goto entry;
    } else {
      saved = ch;
      if (ch >= NOTEDUR_OFFSET) {
	ch = songData[song.offset++];
	set_songdat(saved, ch);
	debug("%s = %d\n", songdat_str(saved), ch);
	if ((saved == NOTEDUR_OFFSET) && (ch == 0)) {
	  return;
	}
      } else {
	word = str_read16le(&songData[song.offset]);
	song.offset += 2;
	debug("Saved offset = %x\n", saved);
	if (saved == OFFSET_OFFSET)
	  song.offset = word;
      }
      goto entry;
    }
}


int main(int argc, char *argv[])
{
  uint8_t tune;

  /* configNotes(); */

  song.freq = 0;
  song.ee2 = 0;
  song.offset = 0;
  song.noteDur = 1;
  song.ee7 = 0;
  song.ee8 = 0;
  song.ee9 = 0;
  song.eea = 0;

  if (argc != 2)
    tune = 0;
  else
    tune = atoi(argv[1]);

  song.offset = songIndex[tune];
  baseIndex = songIndex[tune];

  while (song.noteDur) {
    debug("song.freq += %d\n", song.ee2);
    song.ee9--;
    debug("song.ee9 = %d\n", song.ee9);
    if (song.ee9 <= 0) {
      if (song.ee8 != 0) {
	song.ee8--;
	if (song.ee8 == 0)
	  song.eea = 0;
      }
      song.noteDur--;
      debug("song.noteDur = %d\n", song.noteDur);
      if (song.noteDur == 0) {
	getNextNote();
	if (song.offset == baseIndex)
	  break;
        printf("Note: %s%d duration: %d\n", \
		noteString[(song.freq % 12)], 	\
		(song.freq / 12) + 1, 		\
		song.noteDur);
      }
      song.ee9 = song.ee7;
    }

  }
}
