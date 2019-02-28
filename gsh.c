#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "gsh.h"
#include "gsh_gc.h"
#include "gsh_parser.h"

int sig_int;

static char *line = (char *)NULL;
static char prompt[1024];

static void handler_ctrlc(int sig) {
    sig_int = 1;
    printf("\n");
    // Cleanup
    _gc(line);
    rl_free_line_state();
    rl_cleanup_after_signal();
}

/*
\e   - ESC
[01  - Bold
[32m - Green
[34m - Blue
[m   - Restore to default (black)
*/
static char *get_prompt() {
    char hostname[255];
    gethostname(hostname, 255);
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);
    char home[PATH_MAX];
    strcpy(home, getenv("HOME"));
    // Shorten user home with '~'
    if (strstr(cwd, home) - cwd == 0)
        sprintf(prompt, "\e[01;32m%s@%s:\e[34m~%s\e[m$ ", getenv("USER"), hostname, cwd + strlen(home));
    else
        sprintf(prompt, "\e[01;32m%s@%s:\e[34m%s\e[m$ ", getenv("USER"), hostname, cwd);
    return prompt;
}

int main(int argc, char const *argv[], char **envp)
{
    char *path = NULL;              // Execution path
    char **args = NULL;             // Arguments
    signal(SIGINT, handler_ctrlc);  // Set singal handler
    setenv("SHELL", "gsh", 1);      // Initialize ENV

    // Main loop
    while (1) {
        sig_int = 0;
        // Read input
        line = readline(get_prompt());
        if (sig_int)
            continue;
        // Input finished (^D), exit
        if (line == NULL)
            break;
        // Filter valid command
        char *real_line = trim(line);
        if (!*real_line)
            continue;
        add_history(line);  // Use up/down arrow to view history


        int argl;           // Argument array length
        int ret = 0;        // Whether `execvp()` succeed
        if (parse_args(real_line, &path, &argl, &args)) {
            fprintf(stderr, "Syntax error\n");
            continue;
        }
        _gc(line);  // Parse finished, no longer needed

        // Built-in commands, `cd` has been implemented so far
        if (!strcmp(path, "cd")) {
            // Come to HOME directory if no arguments
            if (!args[1]) {
                chdir(getenv("HOME"));
                setenv("PWD", getenv("HOME"), 1);
                continue;
            }
            if (chdir(args[1]))
                fprintf(stderr, "cd: no such file or directory: %s\n", args[1]);
            else {
                char cwd[PATH_MAX];
                getcwd(cwd, PATH_MAX);
                setenv("PWD", cwd, 1);
            }
            clean_char_arr(argl, &args);
            free(path);
            continue;
        }

        // Fork and execute a command
        int pid = fork();
        if (pid == 0) {
            // Run program in the child process
            ret = execvp(path, args);
            if (ret)
                // Failed
                fprintf(stderr, "gsh: command not found: %s\n", args[0]);
            exit(0);
        } else {
            // Wait for child process to finish 
            // and goto next loop
            wait(&ret);
            clean_char_arr(argl, &args);
            free(path);
        }
    }
    _gc(line);
    return 0;
}
