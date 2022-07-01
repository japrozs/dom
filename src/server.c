#include <main.h>

#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100
#define SERVER_NAME "dom@0x6a6170726f7a/0.0.1"

char *replace(const char *s, const char *old_str,
			  const char *new_str)
{
	char *result;
	int i, cnt = 0;
	int newWlen = strlen(new_str);
	int oldWlen = strlen(old_str);
	for (i = 0; s[i] != '\0'; i++)
	{
		if (strstr(&s[i], old_str) == &s[i])
		{
			cnt++;
			i += oldWlen - 1;
		}
	}

	result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

	i = 0;
	while (*s)
	{
		// compare the substring with the result
		if (strstr(s, old_str) == s)
		{
			strcpy(&result[i], new_str);
			i += newWlen;
			s += oldWlen;
		}
		else
			result[i++] = *s++;
	}

	result[i] = '\0';
	return result;
}

int check(int exp, const char *msg)
{
	if (exp == SOCKETERROR)
	{
		perror(msg);
		exit(1);
	}

	return exp;
}

void create_server(const int PORT, config_t *cfg)
{
	int server_socket, client_socket, addr_size;
	struct sockaddr_in server_addr, client_addr;

	check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
		  "Failed to create socket");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	check(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)),
		  "Bind failed!");

	check(listen(server_socket, SERVER_BACKLOG),
		  "Listen failed!");

	while (true)
	{
		printf("waiting for connections .... \n");
		// wait, and eventually accept an incoming connection
		check(client_socket = (accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size)),
			  "accept failed!");

		char buf[5000];
		recv(client_socket, buf, 5000, 0);

		char *route = strtok(buf, " ");
		route = strtok(NULL, " ");
		if (route[strlen(route) - 1] == '/' && strlen(route) != 1)
		{
			route[strlen(route) - 1] = '\0';
		}

		printf("route accessed :: %s\n", route);
		char res[1 << 17];
		FILE *pipe;
		char *cmd = malloc(sizeof(char) * 4096);
		for (int i = 0; i < cfg->num_routes; i++)
		{
			printf("path :: %s\n", cfg->handlers[i].path);
			printf("route :: %s\n", route);
			if (strcmp(cfg->handlers[i].path, route) == 0)
			{
				cmd = cfg->handlers[i].cmd;
				printf("orig cmd :: %s\n", cfg->handlers[i].cmd);
				printf("cmd :: %s\n", cmd);
			}
		}

		char *output;
		if (0 == (pipe = (FILE *)popen(cmd, "r")))
		{
			perror("popen() failed.");
			exit(EXIT_FAILURE);
		}

		fseek(pipe, 0, SEEK_END);
		int len = ftell(pipe);
		fseek(pipe, 0, SEEK_SET);

		output = malloc(sizeof(char) * (len + 1));
		fread(output, 1, len, pipe);
		output[len] = '\0';

		pclose(pipe);

		// trim starting whitespaces
		while (isspace(*cmd))
			*cmd++;

		output = replace(output, "\n", "\\n");
		sprintf(res,
				"HTTP/1.1 200 OK\n"
				"Server: %s\n"
				"Content-Type: application/json\n"
				"\n"
				"{\n"
				"\t\"command\" : \"%s\",\n"
				"\t\"output\" : \"%s\"\n"
				"}",
				SERVER_NAME, cmd, output);
		write(client_socket, res, strlen(res));
		close(client_socket);
	}
}
