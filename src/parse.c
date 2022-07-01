#include <main.h>

config_t *parse_file(const char *path)
{
	FILE *fp = fopen(path, "r");
	int len = 0;
	char *buf;

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buf = malloc(sizeof(char) * (len + 1));
	fread(buf, 1, len, fp);
	buf[len] = '\0';
	fclose(fp);

	config_t *cfg = malloc(sizeof(config_t));
	cfg->handlers = malloc(sizeof(handler_t) * MAX_BUF);
	cfg->num_routes = 0;

	char *line = strtok(buf, "\n");
	while (line != NULL)
	{
		const int len = strlen(line);
		if (line[0] != '/' && line[0] != '"' && line[0] != '\'')
		{
			printf("each line should start with a // to denote a comment or ' or \" to denote a route\n");
			exit(EXIT_SUCCESS);
		}
		if (line[0] == '/')
		{
			line = strtok(NULL, "\n");
			continue;
		}
		else if (line[0] == '\'' || line[0] == '\"')
		{
			char type = line[0];
			int i = 1;
			int k = 0;
			char *path = malloc(sizeof(char) * MAX_BUF);
			while (line[i] != type && i < len)
			{
				path[k] = line[i];
				i++;
				k++;
			}

			while (line[i] != '>' && i < len)
				i++;
			i++;
			char *cmd = malloc(sizeof(char) * 1024);
			for (int d = 0; d < len; d++)
			{
				cmd[d] = line[i];
				i++;
			}

			cfg->handlers[cfg->num_routes].path = path;
			cfg->handlers[cfg->num_routes].cmd = cmd;
			cfg->handlers[cfg->num_routes].dynamic_route = false;
			cfg->num_routes += 1;
		}
		line = strtok(NULL, "\n");
	}

	return cfg;
}
