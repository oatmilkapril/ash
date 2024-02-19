#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <april/sys/process.h>
#include <april/sys/signal.h>
#include "evaluate.h"
#include "consts.h"
#include "parse.h"
#include "command.h"
#include "jobs.h"
#include "signals.h"
#include "builtin.h"

void wait_on_fg_job(job *j) {
    sigset_t set;
    april_sigemptyset(&set);

    // wait for fg job to finish
    while (job_get_state(j) == JOB_STATE_FG)
    {
        april_sigsuspend(&set);
    }
}

void evaluate(char buffer[BUFFER_SIZE]) {
    command cmd;
    cmd_init(&cmd,
             buffer,
             parse_argv_argc,
             parse_bg,
             parse_builtin);

    if (cmd_get_argc(&cmd) == 0) {
        return;
    }

    // handle builtins immediately
    cmd_type type = cmd_get_type(&cmd);
    if (type != BUILTIN_PROGRAM) {
        builtin_handle(&cmd);
        return;
    }

    signals_block();

    // run command in child process
    pid_t pid;
    if ((pid = april_fork()) == 0) { // child process
        signals_uninstall_handlers();
        signals_unblock();
        setpgrp(); // shouldn't be in same process group as shell

        char * const *argv = cmd_get_argv(&cmd);

        // todo: add execvpe to libapril
        if (execvpe(argv[0], argv, environ) < 0) {
            printf("%s: Command not found.\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    job *j = jobs_add(pid, &cmd);

    // wait for command if not background
    if (!cmd_is_bg_job(&cmd)) {
        signals_unblock();
        wait_on_fg_job(j);
    } else {
        printf("[%zu], %d\n", job_get_jid(j), job_get_pid(j));
        signals_unblock();
    }
}
