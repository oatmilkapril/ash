#ifndef APRILSHELL_COMMAND_H
#define APRILSHELL_COMMAND_H

#include <stdbool.h>
#include "consts.h"

typedef enum {
    BUILTIN_PROGRAM,
    BUILTIN_EXIT,
    BUILTIN_JOBS,
    BUILTIN_FG,
    BUILTIN_BG,
    BUILTIN_CD
} cmd_type;

// do not access struct elements outside command.c
// they're only here to allow declarations
typedef struct {
    char *argv[MAX_ARGS];
    size_t argc;
    bool is_bg;
    cmd_type type;
} command;

void cmd_init(command *cmd,
              char buffer[BUFFER_SIZE],
              void (*parse_argv_argc)(char buffer[BUFFER_SIZE], char *argv[MAX_ARGS], size_t *argc),
              bool (*parse_bg)(char *argv[MAX_ARGS], size_t *argc),
              cmd_type (*parse_type)(char **argv));

bool cmd_is_bg_job(const command *cmd);

cmd_type cmd_get_type(const command *cmd);

char * const *cmd_get_argv(const command *cmd);

size_t cmd_get_argc(const command *cmd);

#endif // APRILSHELL_COMMAND_H

