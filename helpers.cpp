#include<iostream>
#include<stdio.h>
#include <unistd.h>
#define GRN  "\x1B[32m"
#define WHT   "\x1B[37m"
#define BLU  "\x1B[34m"
#define YEL  "\x1B[33m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define RESET "\x1B[0m"

#define endl '\n'

using namespace std;

void help(){
	printf(BLU "\nHello. I am OZ, the shell. I recongnise the following commands.\n\n" RESET);
	printf(BLU "clr" RESET " - Clear the screen.\n");
	printf(BLU "pause" RESET " - Pause operations of the shell until ‘Enter’ is pressed.\n");
	printf(BLU "help" RESET " - Display User Manual.\n");
	printf(BLU "quit" RESET " - Quit the shell.\n");
	printf(BLU "history" RESET " - Display the list of previously executed commands.\n");
	printf(BLU "cd" RESET " - Display the list of previously executed commands.\n");
	printf(BLU "dir" RESET " - List the contents of the specified directory.\n");
	printf(BLU "echo" RESET " - Echo the comment entered after the command.\n");
	printf("\nI can also perform basic Linux/Unix system commands like pwd, ls, etc.\n\n");
}

// parse the input line into and return *argv[] of the same
char **parse_args(char *line){
	char **args = new char*;
	char *sep=(char*)" ", *token;
	int index=0;

	token = strtok(line, sep);
	while(token){
		args[index++]=token;
		token = strtok(NULL, sep);
	}
	args[index]=NULL;
	return args;
}

void history(char *HISTFILE){
    FILE* histfile = fopen(HISTFILE, "rb");
    char chunk[200];
    while(fgets(chunk, sizeof(chunk), histfile)){
		fputs(chunk, stdout);
	}
	cout<<endl;
}

void sigint_handler(int signalnumber) {
    printf("Caught SIGINT\n");
}

void dir(const char *dir_arg){
	struct dirent *curr_dir;
	DIR *dh = opendir(dir_arg);

	// Bonus: handle dir <no args> ie present dir

	if(!dh){
		if(errno == ENOENT)
			perror("Invalid directory");
		else
			perror("Unable to read the directory. Please try with sudo");
		exit(EXIT_FAILURE); // exit code: 8
	}

	// Directory is valid and readable -> print contents
	// Print dir contents till unreadable entry encountered
	while ((curr_dir = readdir(dh))){
		if(curr_dir->d_name[0]=='.')
			if(!curr_dir->d_name[1] || curr_dir->d_name[1]=='.')
				continue;
			else
				printf(YEL "%s     ", curr_dir->d_name);
		else
			printf(BLU "%s     ", curr_dir->d_name);
	}
	printf("\n");
}

void create_env(){
	FILE *envfilewrite = fopen("environment", "wb");
	char cwd[200];
	getcwd(cwd, sizeof(cwd));
	fprintf(envfilewrite, "shell=%s\n", cwd);
	fclose(envfilewrite);
}