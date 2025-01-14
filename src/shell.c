#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "shell.h"
#include "builtin.h"

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

void lsh_loop() {
    char *line;
    char **args;
    int status;

    do {
        printf("%s", "> ");
        line = lsh_read_line();
        args = lsh_get_args(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

char *lsh_read_line() {
    int bufsize = LSH_RL_BUFSIZE;
    char *buffer = malloc(sizeof(char) * bufsize);
    int index = 0;
    int c;  // use int and not a char because EOF is an integer

    if (buffer == 0) {
        fprintf(stderr, "%s\n", "lsh: Allocation error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();
        if (c == EOF || c =='\n') {
            buffer[index] = '\0';
            return buffer;
        }
        else {
            buffer[index] = c;
        }
        index++;

        if (index >= bufsize) {
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (buffer == 0) {
                fprintf(stderr, "%s\n", "lsh: allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **lsh_get_args(char* line) {
    int bufsize = LSH_TOK_BUFSIZE;
    int index = 0;
    char **tokens = malloc(sizeof(char*) * bufsize);
    char *token;

    if (tokens == 0) {
        fprintf(stderr, "%s\n", "lsh: allocation error");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[index] = token;
        index++;

        if (index >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * bufsize);
            if (tokens == 0) {
                fprintf(stderr, "%s\n", "lsh: allocation error");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, LSH_TOK_DELIM);
    }

    /*
    REIMPLEMENTATION WITH strsep FUNCTION
    char *p_line = line;
    while ((token = strsep(&p_line, LSH_TOK_DELIM))) {
        tokens[index] = token;
        index++;

        if (index >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * bufsize);
            if (tokens == 0) {
                fprintf(stderr, "%s\n", "lsh: allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }
    */

    tokens[index] = NULL;
    return tokens;
}

/*
Possible improvement:
1. return WEXITSTATUS(status); to get the return code of child process
2. add a timeout to waitpid() to avoid an infite wait
*/
int lsh_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("lsh");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0) {
        perror("lsh");
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

int lsh_execute(char **args) {
    if (args[0] == NULL) {
        // Empty command
        return 1;
    }

    for (int i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }
    
    return lsh_launch(args);
}