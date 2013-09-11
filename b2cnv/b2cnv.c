#include <b2lib.h>

char *program_name = "b2cnv";

void usage(void);
void usage(void)
{
  printf("b1cnv [-Mhims]\n");
  printf("  -M\tMonster header\n");
  printf("  -h\tHelp message\n");
  printf("  -i\tItem data\n");
  printf("  -m\tMonster data\n");
  printf("  -s\tSpell data\n");
}

int main(int argc, char *argv[])
{
  extern int optind;
  extern char *optarg;
  int c;

  while ((c = getopt(argc,argv,"dghimst")) != EOF) {
    switch (c) {
	case 'd':
		convertCities();
		convertWild();
		break;
      case 'g':
	convertGFX();
	break;
      case 'h':
	usage();
	break;
      case 'i':
	convertItems();
	break;
      case 'm':
	convertMonsters();
	break;
      case 's':
	convertSpells();
	break;
#if 0
      case 't':
	convertTraps();
	break;
#endif
    }
  }
}
