#include <b3lib.h>

char *program_name = "b3cnv";

btstring_t	*inputDir	= NULL;
btstring_t	*outputDir	= NULL;

void usage(void);
void usage(void)
{
	fprintf(stderr, "b3cnv -d inputDir -o outputDir [-acghilms]\n");
	fprintf(stderr, "  -a\tConvert all\n");
	fprintf(stderr, "  -c\tConvert classes\n");
	fprintf(stderr, "  -g\tConvert graphics\n");
	fprintf(stderr, "  -h\tPrint usage\n");
	fprintf(stderr, "  -i\tConvert items\n");
	fprintf(stderr, "  -l\tConvert levels\n");
	fprintf(stderr, "  -m\tConvert monsters\n");
	fprintf(stderr, "  -s\tConvert spells\n");
}

int main(int argc, char *argv[])
{
  	extern int	optind;
	extern char	*optarg;
	int		c;

	uint8_t		_convertClasses		= 0;
	uint8_t		_convertLevels		= 0;
	uint8_t		_convertGraphics	= 0;
	uint8_t		_convertSpells		= 0;
	uint8_t		_convertMonsters	= 0;
	uint8_t		_convertItems		= 0;

	while ((c = getopt(argc, argv, "acd:ghilmo:s")) != EOF) {
		switch (c) {
		case 'a':
			_convertClasses		= 1;
			_convertLevels		= 1;
			_convertGraphics	= 1;
			_convertSpells		= 1;
			_convertMonsters	= 1;
			_convertItems		= 1;
			break;
		case 'c':
			_convertClasses = 1;
			break;
		case 'd':
			inputDir = bts_strcpy(optarg);
			break;
		case 'g':
			_convertGraphics = 1;
			break;
		case 'h':
			usage();
			return 0;
		case 'i':
			_convertItems = 1;
			break;
		case 'l':
			_convertLevels = 1;
			break;
		case 'm':
			_convertMonsters = 1;
			break;
		case 'o':
			outputDir = bts_strcpy(optarg);
			break;
		case 's':
			_convertSpells = 1;
			break;
		default:
			usage();
			return 0;
		}
	}

	if (inputDir == NULL) {
		fprintf(stderr, "Input directory not specified\n");
		usage();
		return 1;
	}

	if (outputDir == NULL) {
		fprintf(stderr, "Output directory not specified\n");
		usage();
		return 1;
	}

	xmkdir(bts_sprintf("%s/bt3", outputDir->buf));
	xmkdir(bts_sprintf("%s/bt3/images", outputDir->buf));
	xmkdir(bts_sprintf("%s/bt3/json", outputDir->buf));
	xmkdir(bts_sprintf("%s/bt3/fonts", outputDir->buf));

	if (_convertClasses) {
		convertClasses();
		convertRaces();
	}

	if (_convertLevels) {
		convertCities();
		convertDungeons();
	}

	if (_convertGraphics) {
		convertGFX();
	}

	if (_convertItems) {
		convertItems();
	}

	if (_convertMonsters) {
		convertMonsters();
	}

	if (_convertSpells) {
		convertSpells();
#if 0
		convertSongs();
#endif
	}

	bts_free(inputDir);
	bts_free(outputDir);

	return 0;
}
