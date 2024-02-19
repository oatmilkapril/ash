#include <string.h>
#include "command.h"
#include "builtin.h"
#include "consts.h"

void cmd_init(command *cmd,
              char buffer[BUFFER_SIZE],
              void (*parse_argv_argc)(char buffer[BUFFER_SIZE], char *argv[MAX_ARGS], size_t *argc),
              bool (*parse_bg)(char *argv[MAX_ARGS], size_t *argc),
              cmd_type (*parse_type)(char **argv)) {
    parse_argv_argc(buffer, cmd->argv, &cmd->argc);
    if (cmd->argc == 0) {
        return;
    }
    cmd->is_bg = parse_bg(cmd->argv, &cmd->argc);
    cmd->type = parse_type(cmd->argv);
}

bool cmd_is_bg_job(const command *cmd) {
    return cmd->is_bg;
}

cmd_type cmd_get_type(const command *cmd) {
    return cmd->type;
}

char * const *cmd_get_argv(const command *cmd) {
    return cmd->argv;
}

size_t cmd_get_argc(const command *cmd) {
    return cmd->argc;
}
