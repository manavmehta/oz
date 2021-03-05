#include "helpers.cpp"

void help();
char **parse_args(char *line);
void history(FILE *histfile);
void sigint_handler(int signalnumber);