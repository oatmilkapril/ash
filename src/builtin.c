#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "builtin.h"
#include "jobs.h"

static void handle_exit() {
    // todo: should optionally take arg n specifying exit code
    // todo: should use previous jobs exit code
    exit(EXIT_SUCCESS);
}

static void handle_jobs() {
    jobs_print();
}

static void handle_fg(command *cmd) {
    size_t argc = cmd_get_argc(cmd);
    char * const *argv = cmd_get_argv(cmd);

    if (argc < 2) {
        printf("fg <jid>\n");
        return;
    }

    char *end;
    errno = 0;
    size_t jid = strtol(argv[1], &end, 10);
    if (errno != 0 || (end == argv[1])) {
        printf("no such job\n");
        return;
    }

    job *j = jobs_get_from_jid(jid);
    if (j == NULL) {
        printf("no such job\n");
    }

    job_resume_in_fg(j);
}

static void handle_bg(command *cmd) {
    // todo: reduce duplication with fg command

    size_t argc = cmd_get_argc(cmd);
    char * const *argv = cmd_get_argv(cmd);

    if (argc < 2) {
        printf("bg <jid>\n");
        return;
    }

    char *end;
    errno = 0;
    size_t jid = strtol(argv[1], &end, 10);
    if (errno != 0 || (end == argv[1])) {
        printf("no such job\n");
        return;
    }

    job *j = jobs_get_from_jid(jid);
    if (j == NULL) {
        printf("no such job\n");
    }

    job_resume_in_bg(j);
}

// todo: this will need to be updated to take argv/argc
void builtin_handle(command *cmd) {
    switch (cmd_get_type(cmd)) {
        case BUILTIN_EXIT:
            handle_exit();
            break;
        case BUILTIN_JOBS:
            // todo
            handle_jobs();
            break;
        case BUILTIN_FG:
            handle_fg(cmd);
            break;
        case BUILTIN_BG:
            handle_bg(cmd);
            break;
        case BUILTIN_CD:
            // todo
            break;
        case BUILTIN_PROGRAM:
        default:
            break;
    }
}

