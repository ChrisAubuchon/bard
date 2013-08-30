#include <b2lib.h>
#include <spell.h>
#include <bt.h>
#include <cnv_spell.h>

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static spellEffect_t *spf_batbonus(uint32_t index);
static spellEffect_t *spf_damagespell(uint32_t index);
static spellEffect_t *spf_disbelieve(uint32_t index);
static spellEffect_t *spf_generic(uint32_t index);
static spellEffect_t *spf_passiveeffect(uint32_t index);
static spellEffect_t *spf_rangespell(uint32_t index);
static spellEffect_t *spf_wordofhealing(uint32_t index);
static spellEffect_t *spf_summon(uint32_t index);
static spellEffect_t *spf_quickfix(uint32_t index);

/********************************/
/*				*/
/* Static Data			*/
/*				*/
/********************************/

static spfunc_t *spFuncs[] = {
  spf_passiveeffect, spf_damagespell, spf_batbonus, spf_generic,
  spf_batbonus, spf_passiveeffect, spf_batbonus, spf_wordofhealing,
  NULL, spf_passiveeffect, spf_damagespell, spf_summon,
  spf_damagespell, spf_generic, spf_batbonus, spf_disbelieve,
  spf_batbonus, spf_passiveeffect, spf_damagespell, spf_batbonus,
  spf_generic, spf_damagespell, spf_passiveeffect, spf_damagespell,
  spf_batbonus, spf_generic, spf_generic, spf_rangespell,
  spf_quickfix, spf_generic, spf_generic, spf_generic, 
  spf_rangespell, spf_generic, spf_generic
};

static uint8_t genSpell[] = {
  GENS_NONE, GENS_NONE, GENS_NONE, GENS_TRAPZAP,
  GENS_NONE, GENS_NONE, GENS_NONE, GENS_NONE,
  GENS_MAGESTAR, GENS_NONE, GENS_NONE, GENS_NONE,
  GENS_NONE, GENS_TELEPORT, GENS_NONE, GENS_NONE,
  GENS_NONE, GENS_NONE, GENS_NONE, GENS_NONE,
  GENS_SCRYSITE, GENS_NONE, GENS_NONE, GENS_NONE,
  GENS_NONE, GENS_PHASEDOOR, GENS_SPELLBIND, GENS_NONE,
  GENS_NONE, GENS_SAFETYSPELL, GENS_DREAMSPELL, GENS_MAGESTAR,
  GENS_NONE, GENS_BATCHSPELL, GENS_CAMARADERIE
};

static uint8_t spTypeStr[] = {
  S_PASSIVEEFFECT, S_DMGSPELL, S_BATBONUS, S_GENERIC,
  S_BATBONUS, S_PASSIVEEFFECT, S_BATBONUS, S_HEALSPELL,
  S_GENERIC, S_PASSIVEEFFECT, S_DMGSPELL, S_SUMMONSPELL,
  S_DMGSPELL, S_GENERIC, S_BATBONUS, S_DISBELIEVE,
  S_BATBONUS, S_PASSIVEEFFECT, S_DMGSPELL, S_BATBONUS,
  S_GENERIC, S_DMGSPELL, S_PASSIVEEFFECT, S_DMGSPELL,
  S_BATBONUS, S_GENERIC, S_GENERIC, S_DMGSPELL,
  S_HEALSPELL, S_GENERIC, S_GENERIC, S_GENERIC,
  S_DMGSPELL, S_GENERIC, S_GENERIC
};

static uint8_t *attEffList[] = {
  "ATTEFF_FRIES", "ATTEFF_FROZE", "ATTEFF_SHOCK", "ATTEFF_DRAIN",
  "ATTEFF_BURN", "ATTEFF_CHOKE", "ATTEFF_STEAM", "ATTEFF_BLAST",
  "ATTEFF_HIT"
};

/********************************/
/*				*/
/* Local functions		*/
/*				*/
/********************************/

/*
 * spf_batbonus()
 */
static spellEffect_t *spf_batbonus(uint32_t index)
{
  spellEffect_t *rsp;

  rsp = spellEffect_new(S_BATBONUS);

  switch (spellType[index]) {
    case sp_AntiMagic:
	BBOPTR(rsp, group) = 1;
	BBOPTR(rsp, antimagic) = 2;
	break;
    case sp_BattleSkill:
	BBOPTR(rsp, dmgBon) = 1;
	BBOPTR(rsp, dmgStk) = 1;
	BBOPTR(rsp, dmgAmt) = battleskill[spellDuration[index]];
        break;
    case sp_FreezeFoes:
	BBOPTR(rsp, group) = 1;

	BBOPTR(rsp, acPen) = 1;
	BBOPTR(rsp, acStk) = 1;
	BBOPTR(rsp, acAmt) = 1;
	break;
    case sp_PhaseBlur:
	BBOPTR(rsp, group) = 1;

	BBOPTR(rsp, acBon) = 1;
	BBOPTR(rsp, acStk) = 1;
	BBOPTR(rsp, acAmt) = phaseBlur[spellDuration[index]];
	break;
    case sp_VorpalPlating:
	BBOPTR(rsp, dmgBon) = 1;
	BBOPTR(rsp, dmgStk) = 1;
	BBOPTR(rsp, dmgRnd) = 1;
	BBOPTR(rsp, dmgAmt) = spellDuration[index];
	BBOPTR(rsp, dmgDice) = 8;
        break;
    case sp_WordOfFear:
	BBOPTR(rsp, group) = 1;

	BBOPTR(rsp, tohitPen) = 1;
	BBOPTR(rsp, tohitStk) = 1;
	BBOPTR(rsp, tohitAmt) = wordoffear[spellDuration[index]];

	BBOPTR(rsp, dmgPen) = 1;
	BBOPTR(rsp, dmgStk) = 1;
	BBOPTR(rsp, dmgAmt) = wordoffear[spellDuration[index]];

        break;
  }

  return rsp;
}

/*
 * spf_disbelieve(index)
 */
static spellEffect_t *spf_disbelieve(uint32_t index)
{
  spellEffect_t *rsp;

  rsp = spellEffect_new(S_DISBELIEVE);

  if (spellDuration[index] == 0xff) {
    DISPTR(rsp, disbelieve) = 1;
    DISPTR(rsp, allBattle) = 1;
    DISPTR(rsp, revealDoppl) = 1;
  } else {
    DISPTR(rsp, disbelieve) = 1;
  }

  return rsp;
}

/*
 * spf_damagespell()
 */
static spellEffect_t *spf_damagespell(uint32_t index)
{
  spellEffect_t *rsp;

  rsp = spellEffect_new(S_DMGSPELL);

  DMGPTR(rsp, dieval) = 4;
  DMGPTR(rsp, attype) = 10;	/* ATTEFF_NONE */

  switch (spellType[index]) {
    case sp_ArcFire:
      DMGPTR(rsp, levelMult) = 1;
      DMGPTR(rsp, ndice) = spellDuration[index];
      break;
    case sp_WarStrike:
      DMGPTR(rsp, group) = 1;
      DMGPTR(rsp, ndice) = spellDuration[index];
      break;
    case sp_SpectreTouch:
      DMGPTR(rsp, ndice) = spellDuration[index];
      break;
    case sp_MindBlade:
      DMGPTR(rsp, allFoes) = 1;
      DMGPTR(rsp, ndice) = spellDuration[index];
      break;
    case sp_StoneTouch:
      DMGPTR(rsp, spAttack) = spellDuration[index];
      DMGPTR(rsp, attype) = 0xff;
      break;
    case sp_HolyWater:
      if (index == 55) {
	DMGPTR(rsp, group) = 1;
	DMGPTR(rsp, rflags.spellcaster) = 1;
      } else {
	DMGPTR(rsp, rflags.evil) = 1;
      }
      DMGPTR(rsp, ndice) = spellDuration[index];
      break;
  }

  DMGPTR(rsp, outOfRange) = IFBIT(spellRange[index], 0x80, 1, 0);
  DMGPTR(rsp, range) = spellRange[index] & 0x7f;

  if (DMGPTR(rsp, attype) == 0xff) {
    DMGPTR(rsp, attype) = 10;
  } else {
    if (index >= 96)
      DMGPTR(rsp, attype) = 8;
    else 
      DMGPTR(rsp, attype) = spellAttr[index] >> 5;
  }

  return rsp;
}

/*
 * spf_generic()
 *
 */
static spellEffect_t *spf_generic(uint32_t index)
{
  spellEffect_t *rsp;

  rsp = spellEffect_new(S_GENERIC);

  GENPTR(rsp, type) = genSpell[spellType[index]];

  return rsp;
}

/*
 * spf_passiveeffect()
 */
static spellEffect_t *spf_passiveeffect(uint32_t index)
{
  spellEffect_t *rsp;

  rsp = spellEffect_new(S_PASSIVEEFFECT);

  switch (spellType[index]) {
    case sp_Lightspell:
	PASPTR(rsp, type) = PASS_LIGHT;
	PASPTR(rsp, duration) = lightDur[spellDuration[index]];
	PASPTR(rsp, lightDist) = lightDist[spellDuration[index]];
	PASPTR(rsp, secretDoor) = (lightSDFlag[spellDuration[index]] != 0);
	break;
    case sp_MagicCompass:
	PASPTR(rsp, type) = PASS_COMPASS;
	PASPTR(rsp, duration) = spellDuration[index];
	break;
    case sp_Levitation:
	PASPTR(rsp, type) = PASS_LEVITATE;
	PASPTR(rsp, duration) = levitDur[spellDuration[index]];
	break;
    case sp_GroupShield:
	PASPTR(rsp, type) = PASS_SHIELD;
	PASPTR(rsp, duration) = spellDuration[index];
	PASPTR(rsp, detectFlag) = 2;
	break;
    case sp_AreaEnchant:
	PASPTR(rsp, type) = PASS_DETECT;
	PASPTR(rsp, duration) = spellDuration[index];
	PASPTR(rsp, detectFlag) = spellAttr[index] >> 6;
	break;
  }

  return rsp;
}

/*
 * spf_quickfix()
 */
static spellEffect_t *spf_quickfix(uint32_t index)
{
  spellEffect_t *rsp;

  rsp = spellEffect_new(S_HEALSPELL);

  HEAPTR(rsp, dice) = 8;

  return rsp;
}

/*
 * spf_rangespell()
 */
static spellEffect_t *spf_rangespell(uint32_t index)
{
  spellEffect_t *rsp;

  rsp = spellEffect_new(S_DMGSPELL);

  switch (spellType[index]) {
    case sp_FarFoe:
	DMGPTR(rsp, addFlag) = 1;
	DMGPTR(rsp, distance) = 4;
	break;
    case sp_MeleeMen:
	DMGPTR(rsp, distance) = 1;
	break;
  }

  return rsp;
}

/* 
 * spf_wordofhealing(index)
 * Parameters
 * groupHealFlag - If 1 heal the entire group
 * xd4 - The X in Xd4
 * fullHeal - Fully heal HP
 * dispossess - Flag to cure possession
 * resurrect - Flag to bring back to life
 * poison - Cure poision
 * paralysis - Cure paralysis
 * insanity - Cure insanity
 * old - Cure old status
 */
static spellEffect_t *spf_wordofhealing(uint32_t index)
{
  uint8_t mask;
  spellEffect_t *rsp;

  rsp = spellEffect_new(S_HEALSPELL);

  HEAPTR(rsp, rndHeal) = 1;
  if (spellDuration[index] & 0x80)
    HEAPTR(rsp, groupHeal) = 1;

  HEAPTR(rsp, dice) = spellDuration[index] & 0x0f;
  if (HEAPTR(rsp, dice) == 0x0f) {
    HEAPTR(rsp, dice) = 0;
    HEAPTR(rsp, fullHeal) = 1;
  }

  if (HEAPTR(rsp, dice) == 0)
    HEAPTR(rsp, rndHeal) = 0;

  mask = spellAttr[index] & 0xe0;

  if (mask == 0xc0)
    HEAPTR(rsp, dispossess) = 1;
  if (mask == 0xe0)
    HEAPTR(rsp, resurrect) = 1;
  if (mask == 0xa0) {
    HEAPTR(rsp, resurrect) = 1;
    HEAPTR(rsp, old) = 1;
    HEAPTR(rsp, poison) = 1;
    HEAPTR(rsp, paralysis) = 1;
    HEAPTR(rsp, insanity) = 1;
    HEAPTR(rsp, dispossess) = 1;
    HEAPTR(rsp, stoned) = 1;
  }
  if (mask == 0x80) {
    HEAPTR(rsp, poison) = 1;
    HEAPTR(rsp, paralysis) = 1;
    HEAPTR(rsp, insanity) = 1;
  }

  return rsp;
}

/*
 * spf_summon(index)
 * Parameters
 *   monIndex1
 *   monIndex2
 *   illusionFlag
 */
static spellEffect_t *spf_summon(uint32_t index)
{
  uint8_t *mon;
  spellEffect_t *rsp;

  rsp = spellEffect_new(S_SUMMONSPELL);

  if (spellDuration[index] & 0x80) 
    SUMPTR(rsp, illflag) = 1;

  SUMPTR(rsp, sum_zero) = getSummonMacro(spellDuration[index] & 0x7f);

  return rsp;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

spellEffect_t *getSummonEffect(uint32_t spell)
{
  return spf_summon(spell);
}

spellEffect_t *getSpellEffect(uint32_t spell)
{
  uint8_t sptype;

  sptype = spellType[spell];

  if (sptype == sp_None)
    return 0;

  return spFuncs[sptype](spell);
}

uint8_t *getSpellName(uint8_t spno)
{
  return spellFull[spno];
}

spellEffect_t *noSpellEffect(void)
{
  spellEffect_t *rsp;

  rsp = (spellEffect_t *)xmalloc(sizeof(spellEffect_t));
  rsp->type = S_NONE;
  rsp->effect = NULL;

  return rsp;
}

void convertSpells(void)
{
  int i;
  mageSpell_t *m;

  cnv_printSpellHeader();

  for (i = 0; i < 79; i++) {
    m = mageSpell_new(0);

    m->abbr = bts_strcpy(spellAbbr[i]);
    m->full = bts_strcpy(spellFull[i]);
    m->sppt = spptReq[i];

    m->combat = IFBIT(spellAttr[i], 8, 1, 0);
    m->noncombat = IFBIT(spellAttr[i], 0x10, 1, 0);
    m->targetted = ((spellAttr[i] & 7) < 4) ? 1 : 0;

    
    m->sp = getSpellEffect(i);

    printSpellXML(2, m);

  }

  cnv_printSpellFooter();
}
