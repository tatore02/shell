#include "builtin.h"
#include <stdio.h>
#include <unistd.h>

char *builtin_str[] = {
    "cd",
    "help", 
    "exit"
};

int (*builtin_func[]) (char**)  = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit
};

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char*);
}

int lsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "%s\n", "lsh: expected argument to \"cd\"");
    }
    else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(char **args) {
    printf("%s\n", "Type program names and arguments, and hit enter.");
    printf("%s\n", "The following are built in:");
    for (int i = 0; i < lsh_num_builtins(); i++) {
        printf("%s\n", builtin_str[i]);
    }
    printf("%s\n", "Use the man command on the other programs");
    return 1;
}

int lsh_exit(char **args) {
    return 0;
}