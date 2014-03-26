#include <cnvlib.h>
#include <cnv_action.h>
#include <stdarg.h>
#include <cnv_param.h>


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
				paramList_toJson(ba->pl));

	return root;
}























