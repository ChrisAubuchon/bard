#include <b1lib.h>

char *program_name = "b1cnv";

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

int main(int argc, char **argv)
{
  extern int optind;
  extern char *optarg;
  int c;

  while ((c = getopt(argc,argv,"cdhgims")) != EOF) {
    switch (c) {
	case 'c':
		convertClasses();
		convertRaces();
		break;
	case 'd':
		convertLevels();
		convertDungeons();
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
	convertSongs();
	break;
    }
  }
}
