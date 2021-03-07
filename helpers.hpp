#include "helpers.cpp"

void help();
char **parse_args(char *line);
void history(FILE *histfile);
void dir(const char *dir,int op_a,int op_l);
void create_env();