#include <string.h>
#include <assert.h>
#include "parse.h"

void parse_argv_argc(char buffer[BUFFER_SIZE], char *argv[MAX_ARGS], size_t *argc) {
    char *delim;
    char *cursor = buffer;

    // replace trailing \n with space
    buffer[strlen(buffer)-1] = ' ';

    // ignore leading spaces
    while (*cursor && (*cursor == ' ')) {
        cursor++;
    }

    // build argv
    size_t arg_count = 0;
    while ((delim = strchr(cursor, ' '))) {
        argv[arg_count++] = cursor;
        *delim = '\0';
        cursor = delim + 1;

        // ignore trailing spaces
        while (*cursor && (*cursor == ' ')) {
            cursor++;
        }
    }
    argv[arg_count] = NULL;
    *argc = arg_count;
}

bool parse_bg(char **argv, size_t *argc) {
    // todo: add debug-only assertions in libapril
    assert(*argc > 0);
    bool is_bg = *argv[*argc-1] == '&';

    if (is_bg) {
        argv[*argc-1] = NULL;
        *argc = *argc - 1;
    }

    return is_bg;
}

cmd_type parse_builtin(char **argv) {
    if (strcmp(argv[0], "exit") == 0) {
        return BUILTIN_EXIT;
    } else if (strcmp(argv[0], "jobs") == 0) {
        return BUILTIN_JOBS;
    } else if (strcmp(argv[0], "fg") == 0) {
        return BUILTIN_FG;
    } else if (strcmp(argv[0], "bg") == 0) {
        return BUILTIN_BG;
    }
    return BUILTIN_PROGRAM;
}
