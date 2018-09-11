#ifndef SIGNAL_H
#define SIGNAL_H

#include "../../include/signal.h"

int __sigaction(int, const struct sigaction *, struct sigaction *);

void __block_all_sigs(void *);
void __block_app_sigs(void *);
void __restore_sigs(void *);

void __get_handler_set(sigset_t *);

#endif
