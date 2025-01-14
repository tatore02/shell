extern char *builtin_str[];
extern int (*builtin_func[]) (char**);

void lsh_loop();
char *lsh_read_line();
char **lsh_get_args(char*);
int lsh_launch(char **args);
int lsh_execute(char**);