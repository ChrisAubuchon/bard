#include <stdio.h>
#include <b1lib.h>

void cnv_printMonHeader(void)
{
#ifdef BT_IN_C
  printf("#include <bt.h>\n");
  printf("#include <bt/dmacros.h>\n");
  printf("#include <bt/mon.h>\n");
  printf("#include <b1pic.h>\n");
  printf("#include <b1spell.h>\n");

  printf("\n");

  printf("btmon_t b1_monList[] = {\n");
#endif

#ifdef BT_XML_DATA
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<b1_monsters>\n");
#endif
}

void cnv_printMonFooter(void)
{
#ifdef BT_IN_C
  printf("};\n");
#endif

#ifdef BT_XML_DATA
  printf("</b1_monsters>\n");
#endif
}

void cnv_printSpellHeader(void)
{
#ifdef BT_IN_C
  printf("#include <bt.h>\n");
  printf("#include <bt/dmacros.h>\n");
  printf("#include <bt/spell.h>\n");
  printf("#include <b1mon.h>\n");

  printf("\n");

  printf("mageSpell_t b1_spList[] = {\n");
#endif

#ifdef BT_XML_DATA
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<b1_spells>\n");
#endif
}

void cnv_printSpellFooter(void)
{
#ifdef BT_IN_C
  printf("};\n");
#endif

#ifdef BT_XML_DATA
  printf("</b1_spells>\n");
#endif
}

void cnv_printItemHeader(void)
{
#ifdef BT_IN_C
  printf("#include <bt.h>\n");
  printf("#include <bt/dmacros.h>\n");
  printf("#include <bt/item.h>\n");
  printf("#include <b1mon.h>\n");
  printf("#include <b1spell.h>\n");

  printf("\n");

  printf("btitem_t b1_itemList[] = {\n");
#endif

#ifdef BT_XML_DATA
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<b1_items>\n");
#endif
}

void cnv_printItemFooter(void)
{
#ifdef BT_IN_C
  printf("};\n");
#endif

#ifdef BT_XML_DATA
  printf("</b1_items>\n");
#endif
}

