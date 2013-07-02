#include <cnvlib.h>
#include <cnv_trap.h>
#include <cnv_int.h>

/********************************/
/*				*/
/* Local Function Prototypes	*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Static Variables		*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Local Functions		*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Constructor/Destructor	*/
/*				*/
/********************************/

trapList_t *trapList_new(uint8_t ntraps)
{
  trapList_t *rval;

  rval = (trapList_t *)xzalloc(sizeof(trapList_t));

  if (ntraps > 0) {
    rval->traps = (trap_t **)xzalloc(ntraps * sizeof(trap_t *));
    rval->ntraps = ntraps;
  } else {
    rval->traps = NULL;
    rval->ntraps = 0;
  }

  return rval;
}

trap_t *trap_new(void)
{
  trap_t *rval;

  rval = (trap_t *)xzalloc(sizeof(trap_t));

  memset(rval, 0, sizeof(trap_t));
}

void trapList_free(trapList_t *tl)
{
  uint32_t i;

  for (i = 0; i < tl->ntraps; i++) { 
    trap_free(tl->traps[i]);
  }

  free(tl->traps);
  free(tl);
}

void trap_free(trap_t *t)
{
  bts_free(t->macro);
  bts_free(t->string);
  free(t);
}

/********************************/
/*				*/
/* XML Output functions		*/
/*				*/
/********************************/

void outputTrapListXML(uint8_t indent, trapList_t *tl)
{
  uint32_t i;

  for (i = 0; i < tl->ntraps; i++) {
    trap_t *t = tl->traps[i];

    iprintf(0, "<%s>\n", t->macro->buf);
    iprintf(2, "<string>%s</string>\n", t->string->buf);
    iprintf(2, "<ndice>%d</ndice>\n", t->ndice);
    iprintf(2, "<dieval>%d</dieval>\n", t->dieval);
    iprintf(2, "<special_attack>%s</special_attack>\n", getSpecialAttack(t->spAttack));
    iprintf(2, "<save_lo>%d</save_lo>\n", t->sv_lo);
    iprintf(2, "<save_hi>%d</save_hi>\n", t->sv_hi);
    iprintf(2, "<save_percent>%d</save_percent>\n", t->sv_chance);
    iprintf(2, "<save_half>%d</save_half>\n", t->sv_half);
    iprintf(2, "<group_flag>%d</group_flag>\n", t->partyFlag);
    iprintf(0, "</%s>\n", t->macro->buf);
  }
}

