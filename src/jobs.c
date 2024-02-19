#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "jobs.h"
#include "command.h"
#include "signals.h"

static job jobs[MAX_JOBS];
static size_t next_job_id = 1;

static void copy_command_string(const command *cmd, char buf[BUFFER_SIZE]) {
    char *buf_pos = buf;
    size_t argc = cmd_get_argc(cmd);
    char * const *argv = cmd_get_argv(cmd);

    for (size_t i = 0; i < argc; i++) {
        const char *arg = argv[i];
        size_t arg_len = strlen(arg);
        for (size_t j = 0; j < arg_len; j++) {
            *buf_pos++ = *arg++;
        }
        *buf_pos++ = ' ';
    }
    *buf_pos++ = '\0';
}

static void clear_job(job *j) {
    j->pid = 0;
    j->jid = 0;
    j->state = JOB_STATE_INVALID;
}

void jobs_init() {
    for (size_t i = 0; i < MAX_JOBS; i++) {
        clear_job(&jobs[i]);
    }
}

job *jobs_add(pid_t pid, command *cmd) {
    // todo: ensure there aren't more than MAX_JOBS jobs
    size_t jid = next_job_id++; // todo: add more robust jid selection

    job *j = &jobs[jid];
    j->pid = pid;
    j->jid = jid;
    j->state = cmd_is_bg_job(cmd) ? JOB_STATE_BG : JOB_STATE_FG;
    copy_command_string(cmd, j->command);

    return j;
}

void jobs_remove(job *j) {
    clear_job(j);
}

void job_stop(job *j) {
    j->state = JOB_STATE_STOPPED;
}

void job_resume_in_fg(job *j) {
    // todo: assert there's no existing fg job
    if (j->state == JOB_STATE_STOPPED) {
        signals_resume(j->pid);
    }
    j->state = JOB_STATE_FG;
}

void job_resume_in_bg(job *j) {
    if (j->state == JOB_STATE_STOPPED) {
        signals_resume(j->pid);
    }
    j->state = JOB_STATE_BG;
}

job *jobs_get_from_jid(size_t jid) {
    if (jid >= MAX_JOBS) {
        return NULL;
    }

    job *j = &jobs[jid];
    if (j->state == JOB_STATE_INVALID) {
        return NULL;
    }

    return j;
}

job *jobs_get_from_pid(pid_t pid) {
    for (size_t i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].pid == pid) {
            return &jobs[i];
        }
    }
    return NULL;
}

void jobs_print() {
    for (size_t i = 0; i < next_job_id; i++) {
        if (jobs[i].state != JOB_STATE_INVALID) {
            printf("[%zu] %d    ", jobs[i].jid, jobs[i].pid);
            if (jobs[i].state == JOB_STATE_BG) {
                printf("Running        ");
            } else if (jobs[i].state == JOB_STATE_STOPPED) {
                printf("Stopped        ");
            } else {
                printf("ERROR          ");
            }
            printf("%s\n", jobs[i].command);
        }
    }
}

job *jobs_get_fg_job() {
    for (size_t i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].state == JOB_STATE_FG) {
            return &jobs[i];
        }
    }
    return NULL;
}

const char *job_get_command(job *j) {
    assert(j != NULL);
    return j->command;
}

pid_t job_get_pid(job *j) {
    assert(j != NULL);
    return j->pid;
}

size_t job_get_jid(job *j) {
    assert(j != NULL);
    return j->jid;
}

job_state job_get_state(job *j) {
    assert(j != NULL);
    return j->state;
}