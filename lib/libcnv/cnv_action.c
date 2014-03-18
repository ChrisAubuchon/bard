#include <cnvlib.h>
#include <cnv_action.h>
#include <stdarg.h>


/********************************/
/*				*/
/* Internal Structures		*/
/*				*/
/********************************/

typedef struct {
	uint32_t	type;
	btstring_t	*name;

	btstring_t	*s;
	uint32_t	n;
	uint32_t	b;
} __param_t;

/********************************/
/*				*/
/* Internal Function Prototypes	*/
/*				*/
/********************************/

static void		param_free(const void *vp);
static btstring_t	*param_toName(const void *vp);
static json_t		*param_toJson(const void *vp);

/********************************/
/*				*/
/* Internal Functions		*/
/*				*/
/********************************/

/*
 * param_free()
 */
static void param_free(const void *vp)
{
	__param_t	*p = (__param_t *)vp;

	bts_free(p->name);
	bts_free(p->s);
	free(p);
}

/*
 * param_toName()
 */
static btstring_t *param_toName(const void *vp)
{
	__param_t	*p = (__param_t *)vp;

	return p->name;
}

/*
 * param_toJson()
 */
static json_t *param_toJson(const void *vp)
{
	__param_t	*p = (__param_t *)vp;

	if ((p == NULL) || (p->name == NULL))
		return NULL;

	switch (p->type) {
	case PARAM_STRING:
	case PARAM_BTSTRING:
		if (p->s == NULL);
			return NULL;

		return json_string(p->s->buf);
		break;
	case PARAM_NUMBER:
		return json_integer(p->n);
		break;
	case PARAM_BOOL:
		if (p->b)
			return json_true();
		else
			return json_false();
		break;
	}

	return NULL;
}

/********************************/
/*				*/
/* btAction_t functions		*/
/*				*/
/********************************/

/*
 * btAction_addParam()
 */
void btAction_addParam(btAction_t *ba, uint32_t type, uint8_t *name, ...)
{
	__param_t	*p;
	va_list		args;

	if (name == NULL)
		return;

	p = (__param_t *)xzalloc(sizeof(__param_t));

	p->name = bts_strcpy(name);
	p->type = type;

	va_start(args, name);

	switch (type) {
	case PARAM_STRING:
		p->s = bts_strcpy(va_arg(args, uint8_t *));
		break;
	case PARAM_BTSTRING:
		p->s = va_arg(args, btstring_t *);
		break;
	case PARAM_NUMBER:
		p->n = va_arg(args, uint32_t);
		break;
	case PARAM_BOOL:
		p->b = va_arg(args, uint32_t);
		break;
	default:
		free(p);
		va_end(args);
		return;
	}

	va_end(args);

	cnvList_add(ba->parameters, p);
}

/*
 * btAction_new()
 */
btAction_t *btAction_new(uint8_t ftype, uint8_t etype)
{
	btAction_t	*ba;

	ba = (btAction_t *)xzalloc(sizeof(btAction_t));
	if (ftype)
		ba->function	= btFunction_new(ftype);
	if (etype)
		ba->effect	= btEffect_new(etype);
	ba->parameters = cnvList_new(param_free, param_toJson, param_toName);

	return ba;
}


void btAction_free(const void *vba)
{
	btAction_t	*ba = (btAction_t *)vba;

	if (ba == NULL)
		return;

	btFunction_free(ba->function);
	btEffect_free(ba->effect);
	free(ba);
}

json_t *btAction_toJson(const void *vba)
{
	btAction_t	*ba = (btAction_t *)vba;
	json_t		*root;

	if (ba == NULL)
		return NULL;

	root = json_object();

	json_object_update(root, btFunction_toJson(ba->function));
	json_object_update(root, btEffect_toJson(ba->effect));
	json_object_set_new(root, "parameters", 
				cnvList_toJsonObject(ba->parameters));

	return root;
}























