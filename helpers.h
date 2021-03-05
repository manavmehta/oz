#include "helpers.cpp"

void help();
char **parse_args(char *line);
void history(FILE *histfile);
void sigint_handler(int signalnumber);
void dir(const char *dir,int op_a,int op_l);