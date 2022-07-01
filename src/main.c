#include <main.h>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("%serror%s :: incorrect number of arguments\n", "\033[1;31m", "\033[0m");
		printf("usage -> dom <filename>\n");
		exit(EXIT_SUCCESS);
	}
	config_t *parsed_cfg = parse_file(argv[1]);
	create_server(3000, parsed_cfg);

	return EXIT_SUCCESS;
}
