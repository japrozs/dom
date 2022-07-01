#ifndef PARSE_H
#define PARSE_H

#include <main.h>

typedef struct
{
	char *path;
	char *cmd;
	bool dynamic_route;
} handler_t;

typedef struct
{
	handler_t *handlers;
	uint16_t num_routes;
} config_t;

config_t *parse_file(const char *path);

#endif // PARSE_H
