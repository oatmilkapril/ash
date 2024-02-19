#ifndef APRILSHELL_JOBS_H
#define APRILSHELL_JOBS_H

#include <unistd.h>
#include <stdbool.h>
#include "consts.h"
#include "command.h"

typedef enum {
    JOB_STATE_BG,
    JOB_STATE_FG,
    JOB_STATE_STOPPED,
    JOB_STATE_INVALID,
} job_state;

typedef struct {
    char command[BUFFER_SIZE];
    pid_t pid;
    size_t jid;
    job_state state;
} job;

void jobs_init();

job *jobs_add(pid_t pid, command *cmd);

void jobs_remove(job *j);

void job_stop(job *j);

void job_resume_in_fg(job *j);

void job_resume_in_bg(job *j);

job *jobs_get_from_jid(size_t jid);

job *jobs_get_from_pid(pid_t pid);

void jobs_print();

job *jobs_get_fg_job();

const char *job_get_command(job *j);

pid_t job_get_pid(job *j);

size_t job_get_jid(job *j);

job_state job_get_state(job *j);

#endif // APRILSHELL_JOBS_H
