#ifndef APRILSHELL_SIGNALS_H
#define APRILSHELL_SIGNALS_H

void signals_install_handlers();

void signals_uninstall_handlers();

void signals_block();

void signals_unblock();

void signals_resume(pid_t pid);

#endif // APRILSHELL_SIGNALS_H
