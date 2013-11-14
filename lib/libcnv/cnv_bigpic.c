#include <cnvlib.h>
#include <cnv_bigpic.h>
#include <cnv_gfx.h>


/********************************/
/*				*/
/* Private prototypes		*/
/*				*/
/********************************/

static void		bigpic_free(const void *vbp);
static json_t		*bigpic_toJson(const void *vbp);
static btstring_t	*bigpic_toName(const void *vbp);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

/*
 * bigpic_free()
 */
static void bigpic_free(const void *vbp)
{
	bt_bigpic_t	*bp = (bt_bigpic_t *)vbp;

	if (bp == NULL)
		return;

	bts_free(bp->name);
	bts_free(bp->path);
	free(bp);
}

/*
 * bigpic_toJson()
 */
static json_t *bigpic_toJson(const void *vbp)
{
	bt_bigpic_t	*bp = (bt_bigpic_t *)vbp;
	json_t		*root;

	if (bp == NULL)
		return;

	root = json_object();

	JSON_BTSTRING(root, "path", bp->path);
	switch (bp->type) {
	case IMAGE_PNG:
		JSON_STRING(root, "type", "png");
		break;
	case IMAGE_BTA:
		JSON_STRING(root, "type", "bta");
		break;
	default:
		printf("Invalid bigpic type: %d\n", bp->type);
		exit(1);
	}

	JSON_BOOL(root, "isTimeAware", bp->isTimeAware);

	return root;
}

/*
 * bigpic_toName()
 */
static btstring_t *bigpic_toName(const void *vbp)
{
	bt_bigpic_t	*bp = (bt_bigpic_t *)vbp;

	if (bp == NULL)
		return;

	return bp->name;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

/*
 * bigpicList_new()
 */
cnvList_t *bigpicList_new(void)
{
	return cnvList_new(bigpic_free, bigpic_toJson, bigpic_toName);
}

/*
 * bigpicList_toJson()
 */
void bigpicList_toJson(cnvList_t *bpl, btstring_t *fname)
{
	json_t		*root;

	root = cnvList_toJsonObject(bpl);

	JSON_DUMP(root, fname);
}

/*
 * bigpic_new()
 */
bt_bigpic_t *bigpic_new(void)
{
	bt_bigpic_t 	*bp;

	bp = (bt_bigpic_t *)xzalloc(sizeof(bt_bigpic_t));

	return bp;
}
