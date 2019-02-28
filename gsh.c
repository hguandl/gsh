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

char *path = NULL;
char **args = NULL;
int sig_int;

static char *line = (char *)NULL;
static char prompt[1024];

static void handler_default(int sig) {
    sig_int = 1;
    printf("\n");
    _gc(line);
    rl_free_line_state();
    rl_cleanup_after_signal();
}

static char *trim(char *text) {
    int len = strlen(text);
    int start, end;
    for (start = 0; start < len; ++start)
        if (text[start] != ' ' && text[start] != '\n')
            break;
    for (end = len - 1; end >= 0; --end)
        if (text[end] != ' ' && text[end] != '\n')
            break;

    char *new_text = text + start;
    *(text + end + 1) = '\0';

    return new_text;
}

int parse_args(char *line, int *argl) {
    char *s = line;
    char *p = line;
    while (*p != ' ' && *p != '\0') {
        ++p;
    }
    path = (char *)malloc((p - s + 1) * sizeof(char));
    strncpy(path, s, p - s);
    path[p - s] = '\0';

    int buf_size = 8;
    *argl = 0;
    args = (char **)malloc((buf_size) * sizeof(char *));
    p = line;
    while (*p != '\0') {
        char *arg_s;
        char *arg_t;
        while (*p == ' ' && *p != '\0')
            ++p;
        arg_s = p;
        if (*p == '"') {
            ++arg_s;
            ++p;
            while (*p != '"') {
                if (*p == '\0')
                    return -1;
                ++p;
            }
            arg_t = p;
            ++p;
        } else if (*p == '\'') {
            ++arg_s;
            ++p;
            while (*p != '\'') {
                if (*p == '\0')
                    return -1;
                ++p;
            }
            arg_t = p;
            ++p;
        } else {
            while (*p != ' ' && *p != '\0') 
                ++p;
        }
        arg_t = p;
        ++(*argl);
        char *arg = (char *)malloc((arg_t - arg_s + 1) * sizeof(char));
        strncpy(arg, arg_s, arg_t - arg_s);
        arg[arg_t - arg_s] = '\0';
        args[*argl - 1] = arg;
        if (*argl >= buf_size) {
            buf_size <<= 1;
            args = (char **)realloc(args, (buf_size) * sizeof(char *));
        }
    }
    args[*argl] = NULL;
    return 0;
}

static char *get_prompt() {
    char hostname[255];
    gethostname(hostname, 255);
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);
    char home[PATH_MAX];
    strcpy(home, getenv("HOME"));
    if (strstr(cwd, home) - cwd == 0)
        sprintf(prompt, "\e[01;32m%s@%s:\e[34m~%s\e[m$ ", getenv("USER"), hostname, cwd + strlen(home));
    else
        sprintf(prompt, "\e[01;32m%s@%s:\e[34m%s\e[m$ ", getenv("USER"), hostname, cwd);
    return prompt;
}

int main(int argc, char const *argv[], char **envp)
{
    signal(SIGINT, handler_default);
    setenv("SHELL", "gsh", 1);
    while (1) {
        sig_int = 0;
        line = readline(get_prompt());
        if (sig_int)
            continue;
        if (line == NULL)
            break;
        if (!*line)
            continue;
        int argl, ret = 0;
        if (parse_args(trim(line), &argl)) {
            fprintf(stderr, "Syntax error\n");
            for (int i = 0; i < argl; ++i)
                free(args[i]);
            free(args);
            continue;
        }
        add_history(line);
        if (!strcmp(path, "cd")) {
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
            continue;
        }
        _gc(line);
        int pid = fork();
        if (pid == 0) {
            ret = execvp(path, args);
            if (ret)
                fprintf(stderr, "gsh: command not found: %s\n", args[0]);
            exit(0);
        } else {
            wait(&ret);
            for (int i = 0; i < argl; ++i)
                free(args[i]);
            free(args);
            free(path);
        }
    }
    _gc(line);
    return 0;
}
