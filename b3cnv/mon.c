#include <b3lib.h>
#include <cnv_mon.h>
#include <mon.h>
#include <fileio.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static monster_t	*convertMonster(b3mon_t *inMonster, uint32_t index);
static void		convertMonsterFile(cnvList_t *monsters, FILE *fp,
						uint32_t *base);
static void		unmaskName(uint8_t *src, uint8_t *dst);
static void		getMonBuf(uint8_t *buf, b3mon_t *m);
static btstring_t	*prepName(uint8_t *name, int pflag);
static void printMon(b3mon_t *m, uint32_t index);

/********************************/
/*				*/
/* Static data			*/
/*				*/
/********************************/

static uint8_t cnv_pronoun[] = {
	1, 2, 0, 3
};

static uint16_t diceMask[] = {
	2, 4, 8, 16, 32, 64, 128, 256
};

static int8_t monACPenList[] = {
	3, 2, 0, -2
};


/********************************/
/*				*/
/* Local functions		*/
/*				*/
/********************************/

static monster_t *convertMonster(b3mon_t *inMonster, uint32_t index)
{
	monster_t	*m;
	pic_t		p;

	m = monster_new();

	m->singular	= prepName(inMonster->name, 0);
	m->plural	= prepName(inMonster->name, 1);
	m->macro	= bts_strcpy(monMacro[index]);

	m->hpRndNdice	= NDICE(inMonster->hpDice);
	m->hpRndDie	= DIEVAL(inMonster->hpDice);
	m->hpBase	= inMonster->hpBase;

	m->willAdvance	= (inMonster->packedGender & 0xc0) != 0xc0;
	m->distance	= inMonster->distance & 0x0f;
	m->advSpeed	= inMonster->distance >> 4;

	m->pronoun	= cnv_pronoun[inMonster->packedGender >> 6];
	m->baseAC	= inMonster->packedGender & 0x3f;

	indexToPic(inMonster->picIndex, &p);
	m->picture	= getPicMacro(&p);
	m->reward	= inMonster->rewardHi << 16 | 
			  inMonster->rewardMid << 8 | inMonster->rewardLo;

	m->rndGroupSize = (inMonster->groupSize & 0x1f) ? 1 : 0;
	m->groupSize = (inMonster->groupSize & 0x1f) ? inMonster->groupSize : 1;
	m->numAttacks	= (inMonster->groupSize >> 5) + 1;

	m->noRandom	= IFBIT(inMonster->flags, 0x20, 1, 0);
	m->isIllusion	= IFBIT(inMonster->flags, 0x10, 1, 0);
	m->baseAC	+= monACPenList[inMonster->flags >> 6];

	m->breathSaveLo	= inMonster->breathSaveLo;
	m->breathSaveHi	= inMonster->breathSaveHi;
	m->spellSaveLo	= inMonster->spellSaveLo;
	m->spellSaveHi	= inMonster->spellSaveHi;

	m->toHitLo	= inMonster->toHitLo;
	m->toHitHi	= inMonster->toHitHi;
	m->priorityLo	= inMonster->priorityLo;
	m->priorityHi	= inMonster->priorityHi;

	m->strong.fire	= IFBIT(inMonster->strongElement, 0x80, 1, 0);
	m->strong.unk1	= IFBIT(inMonster->strongElement, 0x40, 1, 0);
	m->strong.holy	= IFBIT(inMonster->strongElement, 0x20, 1, 0);
	m->strong.ice	= IFBIT(inMonster->strongElement, 0x10, 1, 0);
	m->strong.lightning	= IFBIT(inMonster->strongElement, 0x08, 1, 0);
	m->strong.spell	= IFBIT(inMonster->strongElement, 0x04, 1, 0);
	m->strong.unk2	= IFBIT(inMonster->strongElement, 0x02, 1, 0);
	m->strong.unk3	= IFBIT(inMonster->strongElement, 0x01, 1, 0);

	m->weak.fire	= IFBIT(inMonster->weakElement, 0x80, 1, 0);
	m->weak.unk1	= IFBIT(inMonster->weakElement, 0x40, 1, 0);
	m->weak.holy	= IFBIT(inMonster->weakElement, 0x20, 1, 0);
	m->weak.ice	= IFBIT(inMonster->weakElement, 0x10, 1, 0);
	m->weak.lightning	= IFBIT(inMonster->weakElement, 0x08, 1, 0);
	m->weak.spell	= IFBIT(inMonster->weakElement, 0x04, 1, 0);
	m->weak.unk2	= IFBIT(inMonster->weakElement, 0x02, 1, 0);
	m->weak.unk3	= IFBIT(inMonster->weakElement, 0x01, 1, 0);

	m->repel.evil	= IFBIT(inMonster->repelFlags, 0x80, 1, 0);
	m->repel.demon	= IFBIT(inMonster->repelFlags, 0x40, 1, 0);
	m->repel.spellcaster	= IFBIT(inMonster->repelFlags, 0x20, 1, 0);
	m->repel.unk1	= IFBIT(inMonster->repelFlags, 0x10, 1, 0);
	m->repel.unk2	= IFBIT(inMonster->repelFlags, 0x08, 1, 0);
	m->repel.unk3	= IFBIT(inMonster->repelFlags, 0x04, 1, 0);
	m->repel.unk4	= IFBIT(inMonster->repelFlags, 0x02, 1, 0);
	m->repel.unk5	= IFBIT(inMonster->repelFlags, 0x01, 1, 0);

	return m;
}

static btstring_t *prepName(uint8_t *name, int pflag)
{
	uint8_t length = 0;
	btstring_t *rval;

	if (*name == '\0')
		return;

	rval = bts_new(16);

	do {
		if (*name == '/') {
			name++;

			if (pflag) {
				while ((*name != '\\') && (*name != '\0'))
					name++;
			} else {
				while ((*name != '\\') && (*name != '\0'))
					rval->buf[length++] = *name++;
			}
		} else if (*name == '\\') {
			name++;

			if (pflag) {
				while ((*name != '\\') && (*name != '\0'))
					rval->buf[length++] = *name++;
			} else {
				while ((*name != '\\') && (*name != '\0'))
					name++;
			}
		} else {
			rval->buf[length++] = *name++;
		}
	} while (*name != '\0');
	rval->buf[length++] = '\0';

	bts_resize(rval, length);

	return rval;
}

static void unmaskName(uint8_t *src, uint8_t *dst)
{
	uint32_t	i;

	for (i = 0; i < 16; i++) {
		*dst = *src++ & 0x7f;
		if (*dst == 0x7f) {
			*dst = '\0';
			return;
		}
		dst++;
	}
}

static void getMonBuf(uint8_t *buf, b3mon_t *m)
{
	int i;

	unmaskName(buf, m->name);
	buf += 16;

	m->hpDice = *buf++;

	m->hpBase = str_read16le(buf);
	buf += 2;

	m->distance = *buf++;
	m->packedGender = *buf++;
	m->groupSize = *buf++;

	for (i = 0; i < 4; i++) {
		m->att[i].type = *buf++;
		m->att[i].damage = *buf++;
	}

	m->breathFlag = *buf++;
	m->breathRange = *buf++;
	m->picIndex = *buf++;
	m->rewardLo = *buf++;
	m->rewardMid = *buf++;
	m->rewardHi = *buf++;
	m->flags = *buf++;
	m->breathSaveLo = *buf++;
	m->breathSaveHi = *buf++;
	m->priorityLo = *buf++;
	m->priorityHi = *buf++;
	m->strongElement = *buf++;
	m->weakElement = *buf++;
	m->repelFlags = *buf++;
	m->toHitLo = *buf++;
	m->toHitHi = *buf++;
	m->spellSaveLo = *buf++;
	m->spellSaveHi = *buf++;
}

#if 0

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

btstring_t *getMonsterMacro(uint32_t index)
{
	return bts_strcpy(monMacro[index]);
}

#endif

btstring_t *getSummonMacro(uint32_t index)
{
	return bts_strcpy(sumMacro[index]);
}

#if 0

static void printMon(b3mon_t *mon, uint32_t index)
{
	uint8_t nbuf[32];
	monster_t *m;
	int i;

	m = monster_new();

	/* Print singular and plural names */
	m->singular = prepName(mon->name, 0);
	m->plural = prepName(mon->name, 1);
	m->macro = bts_strcpy(monMacro[index]);

	m->hpRndNdice = NDICE(mon->hpDice);
	m->hpRndDie = DIEVAL(mon->hpDice);
	m->hpBase = mon->hpBase;

	m->willAdvance = ((mon->packedGenAc.val & 0xc0) != 0xc0);
	m->distance = mon->distance & 0x0f;
	m->advSpeed = mon->distance >> 4;

	m->pronoun = cnv_pronoun[mon->packedGenAc.d.pronoun];
	m->baseAC = mon->packedGenAc.d.acBase;

	m->picture = bts_strcpy(getPicMacro(mon->picIndex));
	m->reward = mon->rewardHi << 16 | mon->rewardMid << 8 | mon->rewardLo;

	m->rndGroupSize = (mon->groupSize & 0x1f) ? 1 : 0;
	m->groupSize = (mon->groupSize & 0x1f) ? m->groupSize & 0x1f : 1;
	m->numAttacks = (mon->groupSize >> 5) + 1;

	m->noRandom = IFBIT(mon->flags, 0x20, 1, 0);
	m->isIllusion = IFBIT(mon->flags, 0x10, 1, 0);
	m->baseAC += monACPenList[mon->flags >> 6];

	m->breathSaveLo = mon->breathSaveLo;
	m->breathSaveHi = mon->breathSaveHi;
	m->spellSaveLo = mon->spellSaveLo;
	m->spellSaveHi = mon->spellSaveHi;

	m->toHitLo = mon->toHitLo;
	m->toHitHi = mon->toHitHi;
	m->priorityLo = mon->priorityLo;
	m->priorityHi = mon->priorityHi;

	m->strong.fire = IFBIT(mon->strongElement, 0x80, 1, 0);
	m->strong.unk1 = IFBIT(mon->strongElement, 0x40, 1, 0);
	m->strong.holy = IFBIT(mon->strongElement, 0x20, 1, 0);
	m->strong.ice = IFBIT(mon->strongElement, 0x10, 1, 0);
	m->strong.lightning = IFBIT(mon->strongElement, 0x08, 1, 0);
	m->strong.spell = IFBIT(mon->strongElement, 0x04, 1, 0);
	m->strong.unk2 = IFBIT(mon->strongElement, 0x02, 1, 0);
	m->strong.unk3 = IFBIT(mon->strongElement, 0x01, 1, 0);

	m->weak.fire = IFBIT(mon->weakElement, 0x80, 1, 0);
	m->weak.unk1 = IFBIT(mon->weakElement, 0x40, 1, 0);
	m->weak.holy = IFBIT(mon->weakElement, 0x20, 1, 0);
	m->weak.ice = IFBIT(mon->weakElement, 0x10, 1, 0);
	m->weak.lightning = IFBIT(mon->weakElement, 0x08, 1, 0);
	m->weak.spell = IFBIT(mon->weakElement, 0x04, 1, 0);
	m->weak.unk2 = IFBIT(mon->weakElement, 0x02, 1, 0);
	m->weak.unk3 = IFBIT(mon->weakElement, 0x01, 1, 0);

	m->repel.evil = IFBIT(mon->repelFlags, 0x80, 1, 0);
	m->repel.demon = IFBIT(mon->repelFlags, 0x40, 1, 0);
	m->repel.spellcaster = IFBIT(mon->repelFlags, 0x20, 1, 0);
	m->repel.unk1 = IFBIT(mon->repelFlags, 0x10, 1, 0);
	m->repel.unk2 = IFBIT(mon->repelFlags, 0x08, 1, 0);
	m->repel.unk3 = IFBIT(mon->repelFlags, 0x04, 1, 0);
	m->repel.unk4 = IFBIT(mon->repelFlags, 0x02, 1, 0);
	m->repel.unk5 = IFBIT(mon->repelFlags, 0x01, 1, 0);

	for (i = 0; i < 4; i++) {
		uint8_t type;
		uint8_t dmg;

		type = mon->att[i].type;
		dmg = mon->att[i].damage;

		if (type < 0x80) {
			m->att[i] = getSpellEffect(type);
			m->att[i]->type = ACT_SPELL;
		} else if ((type >= 0xf0) && (type < 0xfa)) {
			breathAtt_t b = { 0, 0, 0, 0, 0, 0, 1 };

			b.effect = type - 0xf0;
			b.damage = dmg;
			b.breathFlag = 0x10;

			m->att[i] = cnvBreathAttack(&b, 1);
			m->att[i]->type = ACT_MELEE;
			m->att[i]->subtype = mon->flags & 0x0f;
		} else if (type == 0xfa) {
			m->att[i] = genericSpellEffect(GENS_SUMMONHELP, 0);
			m->att[i]->type = ACT_SUMMONHELP;
		} else if (type == 0x80) {
			breathAtt_t b = { 0, 0, 0, 0, 0, 0x41, 1 };

			b.breathFlag = mon->breathFlag;
			b.damage = dmg;
			m->att[i] = cnvBreathAttack(&b, mon->breathRange);
			if (mon->breathFlag ^ 0x0a)
				m->att[i]->type = ACT_BREATH;
			else
				m->att[i]->type = ACT_FIRE;

		} else if (type == 0x83) {
			m->att[i] = genericSpellEffect(GENS_TARJAN, 0);
			m->att[i]->type = ACT_TARJAN;
		} else {
			debug("Type = %x\n", type);
			breathAtt_t b = { 0, 0, 0, 0, 0, 0, 1 };

			b.effect = 0;
			b.damage = dmg;
			b.breathFlag = 0x10;

			m->att[i] = cnvBreathAttack(&b, 1);
			m->att[i]->type = ACT_MELEE;
		}

		debug("m->att[i]->type = %d\n", m->att[i]->type);
	}

	printMonXML(2, m);

	monster_free(m);
}

#endif

static void convertMonsterFile(cnvList_t *monsters, FILE *fp, uint32_t *base)
{
	uint32_t	start, end;
	uint32_t	count;
	uint32_t	i;

	uint8_t		mbuf[0x30];
	b3mon_t		m;

	start = fp_read16le(fp);
	end = xfseek(fp, 0, SEEK_END);

	count = (end - start) / 0x30;

	xfseek(fp, start, SEEK_SET);

	for (i = 0; i < count; i++) {
		xfread(mbuf, 0x30, 1, fp);
		getMonBuf(mbuf, &m);
		cnvList_add(monsters, convertMonster(&m, *base));
		(*base)++;
	}
}

void convertMonsters(void)
{
	FILE		*fp;
	cnvList_t	*monsters;
	uint32_t	start, end;
	uint32_t	count		= 0;

	monsters = monList_new();

	fp = xfopen(mkBardThreePath("MONSTERL"), "rb");
	convertMonsterFile(monsters, fp, &count);
	fclose(fp);

	fp = xfopen(mkBardThreePath("MONSTERH"), "rb");
	convertMonsterFile(monsters, fp, &count);
	fclose(fp);

	monList_to_json(monsters, mkJsonPath("monsters.json"));
	cnvList_free(monsters);
#if 0
	int fd;
	int i, nmon;
	uint32_t count = 0;

	uint16_t start;
	uint16_t end;

	uint8_t mbuf[0x30];
	b3mon_t m;

	cnv_printMonHeader();

	fd = xopen("monsterl", O_RDONLY);

	start = file_read16le(fd);
	end = xlseek(fd, 0, SEEK_END);

	nmon = (end - start) / 0x30;

	xlseek(fd, start, SEEK_SET);

	for (i = 0; i < nmon; i++) {
		xread(fd, mbuf, 0x30);
		getMonBuf(mbuf, &m);

		printMon(&m, count);

		count++;
	}

	close(fd);

	fd = xopen("monsterh", O_RDONLY);

	start = file_read16le(fd);
	end = xlseek(fd, 0, SEEK_END);

	nmon = (end - start) / 0x30;

	xlseek(fd, start, SEEK_SET);

	for (i = 0; i < nmon; i++) {
		xread(fd, mbuf, 0x30);
		getMonBuf(mbuf, &m);

		printMon(&m, count);

		count++;
	}

	close(fd);

	for (i = 0; i < 22; i++) {
		printMon(&sumMons[i], count);

		count++;
	}

	cnv_printMonFooter();
#endif
}
