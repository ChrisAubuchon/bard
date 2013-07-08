#include <cnvlib.h>
#include <cnv_function.h>


/********************************/
/*				*/
/* Private data			*/
/*				*/
/********************************/

static uint8_t *functionList[] = {
	"",
	"spells.attack(...)",
	"spells.disbelieve(...)",
	"spells.battleBonus(...)",
	"spells.heal(...)",
	"spells.summon(...)",
	"spells.trapZap(...)",
	"spells.mageStar(...)",
	"spells.teleport(...)",
	"spells.scrySite(...)",
	"spells.antiMagic(...)",
	"spells.phaseDoor(...)",
	"spells.spellBind(...)",
	"spells.safetySpell(...)",
	"spells.dreamSpell(...)",
	"spells.batchSpell(...)",
	"spells.camaraderie(...)",
	"spells.divine(...)",
	"spells.identify(...)",
	"spells.possess(...)",
	"spells.earthMaw(...)",
	"spells.chrono(...)",
	"spells.mapSpell(...)",
	"spells.reEnergize(...)",
	"spells.summonHelp(...)",
	"spells.tarjan(...)",
	"spells.doppel(...)",
	"spells.passive.light(...)",
	"spells.passive.compass(...)",
	"spells.passive.levitate(...)",
	"spells.passive.shield(...)",
	"spells.passive.detect(...)"
};

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

btFunction_t *btFunction_new(uint8_t type, ...)
{
	btFunction_t	*bf;
	va_list		args;

	bf = (btFunction_t *)xzalloc(sizeof(btFunction_t));
	bf->type = type;

	if (type == FUNC_STRING) {
		va_start(args, type);
		bf->string = va_arg(args, btstring_t *);
		va_end(args);
	}

	return bf;
}

void btFunction_free(const void *vbf)
{
	btFunction_t	*bf = (btFunction_t *)vbf;

	if (bf == NULL)
		return;

	bts_free(bf->string);
	free(bf);
}

json_t *btFunction_toJson(const void *vbf)
{
	btFunction_t	*bf = (btFunction_t *)vbf;
	json_t		*root;

	if (bf == NULL)
		return NULL;

	root = json_object();

	if (bf->type == FUNC_STRING) {
		JSON_BTSTRING(root, "inCode", bf->string);
	} else {
		JSON_STRING(root, "inFunction", functionList[bf->type]);
	}

	return root;
}






















