/*
 * l_sprintf.c
 *
 * This file contains a modified version of lua's string.format()
 * function. This function will replace numbers that are too large
 * for their format with a string filled with '*'
 */
#include <bte.h>


/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define L_ESC			'%'
#define uchar(c)		((unsigned char)(c))
#define LUA_INTFRMLEN		"l"
#define LUA_INTFRM_T		long
#define LUA_FLTFRMLEN		""
#define LUA_FLTFRM_T		double

/********************************/
/*				*/
/* Private function prototype	*/
/*				*/
/********************************/

/* Module functions */
static int l_sprintf(lua_State * L);
static void addquoted(lua_State * L, luaL_Buffer * b, int arg);
static const char *scanformat(lua_State * L, const char *strfrmt, char *form);
static void addlenmod(char *form, const char *lenmod);
static int l_sprintf(lua_State * L);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/



/* maximum size of each formatted item (> len(format('%99.99f', -1e308))) */
#define MAX_ITEM	512
/* valid flags in a format specification */
#define FLAGS	"-+ #0"
/*
** maximum size of each format specification (such as '%-099.99d')
** (+10 accounts for %99.99x plus margin of error)
*/
#define MAX_FORMAT	(sizeof(FLAGS) + sizeof(LUA_INTFRMLEN) + 10)


static void addquoted(lua_State * L, luaL_Buffer * b, int arg)
{
	size_t l;
	const char *s = luaL_checklstring(L, arg, &l);
	luaL_addchar(b, '"');
	while (l--) {
		if (*s == '"' || *s == '\\' || *s == '\n') {
			luaL_addchar(b, '\\');
			luaL_addchar(b, *s);
		} else if (*s == '\0' || iscntrl(uchar(*s))) {
			char buff[10];
			if (!isdigit(uchar(*(s + 1))))
				sprintf(buff, "\\%d", (int) uchar(*s));
			else
				sprintf(buff, "\\%03d", (int) uchar(*s));
			luaL_addstring(b, buff);
		} else
			luaL_addchar(b, *s);
		s++;
	}
	luaL_addchar(b, '"');
}

static const char *scanformat(lua_State *L, const char *strfrmt, char *form)
{
	const char *p = strfrmt;
	while (*p != '\0' && strchr(FLAGS, *p) != NULL)
		p++;		/* skip flags */
	if ((size_t) (p - strfrmt) >= sizeof(FLAGS) / sizeof(char))
		luaL_error(L, "invalid format (repeated flags)");
	if (isdigit(uchar(*p)))
		p++;		/* skip width */
	if (isdigit(uchar(*p)))
		p++;		/* (2 digits at most) */
	if (*p == '.') {
		p++;
		if (isdigit(uchar(*p)))
			p++;	/* skip precision */
		if (isdigit(uchar(*p)))
			p++;	/* (2 digits at most) */
	}
	if (isdigit(uchar(*p)))
		luaL_error(L, "invalid format (width or precision too long)");
	*(form++) = '%';
	memcpy(form, strfrmt, (p - strfrmt + 1) * sizeof(char));
	form += p - strfrmt + 1;
	*form = '\0';
	return p;
}

/*
 * getwidth()
 * Returns the width of the format. This is used by the 'd' and
 * 'i' options to determine the max number that can be printed.
 */
static int getwidth(const char *form)
{
	const char	*p;
	int		width = 0;

	p = form;

	/* Skip past '%' */
	p++;	
	while (*p != '\0' && strchr(FLAGS, *p) != NULL)
		p++;

	if (isdigit(uchar(*p))) 
		width = *p++ - '0';
	if (isdigit(uchar(*p))) {
		width = width * 10;
		width += (*p - '0');
	}

	return width;
	
}

/*
** add length modifier into formats
*/
static void addlenmod(char *form, const char *lenmod)
{
	size_t l = strlen(form);
	size_t lm = strlen(lenmod);
	char spec = form[l - 1];
	strcpy(form + l - 1, lenmod);
	form[l + lm - 1] = spec;
	form[l + lm] = '\0';
}

static int l_sprintf(lua_State *L)
{
	int top = lua_gettop(L);
	int arg = 1;
	size_t sfl;
	const char *strfrmt = luaL_checklstring(L, arg, &sfl);
	const char *strfrmt_end = strfrmt + sfl;
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	while (strfrmt < strfrmt_end) {
		if (*strfrmt != L_ESC)
			luaL_addchar(&b, *strfrmt++);
		else if (*++strfrmt == L_ESC)
			luaL_addchar(&b, *strfrmt++);	/* %% */
		else {		/* format item */

			/* to store the format (`%...') */
			char form[MAX_FORMAT];	

			/* to put formatted item */
			char *buff = luaL_prepbuffsize(&b, MAX_ITEM);

			int nb = 0;	/* number of bytes in added item */

			if (++arg > top)
				luaL_argerror(L, arg, "no value");

			strfrmt = scanformat(L, strfrmt, form);

			switch (*strfrmt++) {
			case 'c':{
				nb = sprintf(buff, form, luaL_checkint(L, arg));
				break;
			}
			case 'd':
			case 'i':{
				lua_Number n = luaL_checknumber(L, arg);
				LUA_INTFRM_T ni = (LUA_INTFRM_T) n;
				lua_Number diff = n - (lua_Number) ni;
				int width = getwidth(form);
				luaL_argcheck(L, 
					-1 < diff && diff < 1, arg, 
					"not a number in proper range"
					);

				addlenmod(form, LUA_INTFRMLEN);
				nb = sprintf(buff, form, ni);
				if ((width) && (nb > width)) {
					nb = width;

					while (width--) 
						buff[width] = '*';
					buff[nb] = '\0';
				}
		
				break;
			}
			case 'o':
			case 'u':
			case 'x':
			case 'X':{
				lua_Number n = luaL_checknumber(L, arg);
				unsigned LUA_INTFRM_T ni = (unsigned LUA_INTFRM_T) n;
				lua_Number diff = n - (lua_Number) ni;
				luaL_argcheck(L, -1 < diff && diff < 1, arg, "not a non-negative number in proper range");
				addlenmod(form, LUA_INTFRMLEN);
				nb = sprintf(buff, form, ni);
				break;
			}
			case 'e':
			case 'E':
			case 'f':
#if defined(LUA_USE_AFORMAT)
			case 'a':
			case 'A':
#endif
			case 'g':
			case 'G':{
				addlenmod(form, LUA_FLTFRMLEN);
				nb = sprintf(buff, form, (LUA_FLTFRM_T) luaL_checknumber(L, arg));
				break;
			}
			case 'q':{
				addquoted(L, &b, arg);
				break;
			}
			case 's':{
				size_t l;
				const char *s = luaL_tolstring(L, arg, &l);
				if (!strchr(form, '.') && l >= 100) {
					/* no precision and string is too long to be formatted;
					   keep original string */
					luaL_addvalue(&b);
					break;
				} else {
					nb = sprintf(buff, form, s);
					lua_pop(L, 1);	/* remove result from 'luaL_tolstring' */
					break;
				}
			}
			default:{
				/* also treat cases `pnLlh' */
				return luaL_error(L, "invalid option " LUA_QL("%%%c") " to " LUA_QL("format"), *(strfrmt - 1));
			}
			}
			luaL_addsize(&b, nb);
		}
	}
	luaL_pushresult(&b);
	return 1;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

void l_sprintf_open(lua_State * L)
{
	lua_pushcfunction(L, l_sprintf);
	lua_setglobal(L, "sprintf");
}
