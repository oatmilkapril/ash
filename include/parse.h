#ifndef APRILSHELL_PARSE_H
#define APRILSHELL_PARSE_H

#include "command.h"

void parse_argv_argc(char *buffer, char **argv, size_t *argc);

bool parse_bg(char **argv, size_t *argc);

cmd_type parse_builtin(char **argv);

#endif // APRILSHELL_PARSE_H