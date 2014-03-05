#include <stdio.h>
#include <b3lib.h>

void cnv_printDunHeader(void)
{
#ifdef BT_XML_DATA
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<b3_dungeons>\n");
#endif
}

void cnv_printDunFooter(void)
{
#ifdef BT_XML_DATA
  printf("</b3_dungeons>\n");
#endif
}

void cnv_printItemHeader(void)
{
#ifdef BT_IN_C
  printf("#include <bt.h>\n");
  printf("#include <bt/dmacros.h>\n");
  printf("#include <bt/item.h>\n");
  printf("#include <b3spell.h>\n");

  printf("\n");

  printf("btitem_t b3_itemList[] = {\n");
#endif

#ifdef BT_XML_DATA
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<b3_items>\n");
#endif
}

void cnv_printItemFooter(void)
{
#ifdef BT_IN_C
  printf("};\n");
#endif

#ifdef BT_XML_DATA
  printf("</b3_items>\n");
#endif
}

void cnv_printMonHeader(void)
{
#ifdef BT_IN_C
  printf("#include <bt.h>\n");
  printf("#include <bt/dmacros.h>\n");
  printf("#include <bt/mon.h>\n");
  printf("#include <b3pic.h>\n");
  printf("#include <b3spell.h>\n");

  printf("\n");

  printf("btmon_t b3_monList[] = {\n");
#endif

#ifdef BT_XML_DATA
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<b3_monsters>\n");
#endif
}

void cnv_printMonFooter(void)
{
#ifdef BT_IN_C
  printf("};\n");
#endif

#ifdef BT_XML_DATA
  printf("</b3_monsters>\n");
#endif
}

void cnv_printSpellHeader(void)
{
#ifdef BT_IN_C
  printf("#include <bt.h>\n");
  printf("#include <bt/dmacros.h>\n");
  printf("#include <bt/spell.h>\n");
  printf("#include <b3mon.h>\n");

  printf("\n");

  printf("mageSpell_t b3_spList[] = {\n");
#endif

#ifdef BT_XML_DATA
  printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  printf("<b3_spells>\n");
#endif
}

void cnv_printSpellFooter(void)
{
#ifdef BT_IN_C
  printf("};\n");
#endif

#ifdef BT_XML_DATA
  printf("</b3_spells>\n");
#endif
}
