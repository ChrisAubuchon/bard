#include <cnvlib.h>
#include <cnv_param.h>

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
	paramList_t	*pl;
} param_t;


/********************************/
/*				*/
/* Internal prototypes		*/
/*				*/
/********************************/

static void		param_free(const void *vp);
static btstring_t	*param_toName(const void *vp);
static json_t		*param_toJson(const void *vp);


/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

/*
 * param_free()
 */
static void param_free(const void *vp)
{
	param_t		*p = (param_t *)vp;

	if (p == NULL)
		return;

	switch (p->type) {
	case PARAM_STRING:
	case PARAM_BTSTRING:
		bts_free(p->s);
		break;
	case PARAM_LIST:
		cnvList_free(p->pl);
		break;
	}

	bts_free(p->name);
	free(p);
}

/*
 * param_toName()
 */
static btstring_t *param_toName(const void *vp)
{
	param_t		*p = (param_t *)vp;

	return p->name;
}
	
/*
 * param_toJson()
 */
static json_t *param_toJson(const void *vp)
{
	param_t		*p = (param_t *)vp;

	if ((p == NULL) || (p->name == NULL))
		return NULL;

	switch (p->type) {
	case PARAM_STRING:
	case PARAM_BTSTRING:
		if (p->s == NULL)
			return NULL;

		return json_string(p->s->buf);
	case PARAM_NUMBER:
		if (p->n)
			return json_integer(p->n);

		break;
	case PARAM_BOOL:
		if (p->b)
			return json_true();

		break;
	case PARAM_LIST:
		if (p->pl == NULL)
			return NULL;

		return paramList_toJson(p->pl);
	case PARAM_ARRAY:
		if (p->pl == NULL)
			return NULL;

		return paramList_toJsonArray(p->pl);
	}

	return NULL;
}


/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

/*
 * paramList_new()
 */
paramList_t *paramList_new(void)
{
	return cnvList_new(param_free, param_toJson, param_toName);
}

/*
 * paramList_free()
 */
void paramList_free(const void *vpl)
{
	paramList_t	*pl = (paramList_t *)vpl;

	cnvList_free(pl);
}

/*
 * paramList_toJson()
 */
json_t *paramList_toJson(paramList_t *pl)
{
	return cnvList_toJsonObject(pl);
}

/*
 * paramList_toJsonArray()
 */
json_t *paramList_toJsonArray(paramList_t *pl)
{
	return cnvList_toJsonArray(pl);
}

/*
 * param_add()
 */
void param_add(paramList_t *pl, uint32_t type, uint8_t *name, ...)
{
	param_t		*p;
	va_list		args;

	if (name == NULL)
		return;

	p = (param_t *)xzalloc(sizeof(param_t));

	p->name = bts_strcpy(name);
	p->type = type;

	va_start(args, name);

	switch (type) {
	case PARAM_STRING:
	{
		uint8_t		*s;

		s = va_arg(args, uint8_t *);
		if (s == NULL) {
			param_free(p);
			return ;
		}

		p->s = bts_strcpy(s);
		break;
	}
	case PARAM_BTSTRING:
	{
		btstring_t	*s;

		s = va_arg(args, btstring_t *);
		if (s == NULL) {
			param_free(p);
			return ;
		}

		p->s = s;

		break;
	}
	case PARAM_NUMBER:
		p->n = va_arg(args, uint32_t);
		break;
	case PARAM_BOOL:
		p->b = va_arg(args, uint32_t);
		break;
	case PARAM_LIST:
	case PARAM_ARRAY:
	{
		paramList_t	*pl;

		pl = va_arg(args, paramList_t *);
		if (pl == NULL) {
			param_free(p);
			return;
		}

		p->pl = pl;
		break;
	}
	default:
		param_free(p);
		va_end(args);
		return;
	}

	va_end(args);

	cnvList_add(pl, p);
}
