#ifndef GSH_H
#define GSH_H

// Ignore SIGINT
static void handler_ctrlc(int sig);

// Generate prompt: User@Host:path$
static char *get_prompt();

#endif  // GSH_H