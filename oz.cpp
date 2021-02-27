#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
// #include "helpers.h"

#define endl '\n'
#define GRN  "\x1B[32m"
#define WHT   "\x1B[37m"
#define BLU  "\x1B[34m"
#define RESET "\x1B[0m"
#define HISTFILE "~/.oz_history"

using namespace std;

// g++ oz.cpp -lreadline

// parse the input line into and return *argv[] of the same
char **parse_args(char *line){
	char **args = new char*;
	char *sep=" ", *token;
	int index=0;

	token = strtok(line, sep);
	while(token){
		args[index++]=token;
		token = strtok(NULL, sep);
		// cout<<args[index-1]<<" ";
	}
	args[index]=NULL;
	return args;
}

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

int main() {
	char **args;
	char *line;
	pid_t child_pid;
	while(1){
		printf(GRN "≈> " RESET);
		line=readline("");
		args=parse_args(line);

		// handle cd
		if(strcmp(args[0],"cd")==0){
			chdir(args[1]);
			continue; // perform cd and rerun parent: no forking
		}
		else if(strcmp(args[0],"quit")==0){
			exit(0);
			continue;
		}
		

		child_pid = fork();

		if(child_pid<0){
			perror("failed to create a fork.");
			exit(1);
		}

		// if the process is child execute it, else if parent: wait
		if (!child_pid){
			
			if(strcmp(args[0],"clr")==0){
				cout << "\033[2J\033[1;1H";
				continue; // perform cd and rerun parent: no forking
			}
			else if(strcmp(args[0],"pause")==0){
				system("read -p 'Paused.....'");
				// system("read -p 'Paused.....' var");
				continue;
			}
			else if(strcmp(args[0],"help")==0){
				help();
				continue;
			}else if(strcmp(args[0],"history")==0){
				continue;
			}
			else if(strcmp(args[0],"dir")==0){ // not working for absolute paths
				execvp((char*)("ls"), args);
			}
			else if(strcmp(args[0],"environ")==0){
				continue;
			}
			else if(strcmp(args[0],"echo")==0){
				int i=1;
				while(args[i]!=NULL){
					cout<<args[i++]<<" ";
				}
				cout<<endl;
				continue;
			}
			else{
				int exec_response = execvp(args[0], args);

				// execvp returns -1 on failure and nothing on success; API may return other negative vals
				if(exec_response < 0){
					string error_message = "could not execute: " + (string)args[0];
					perror(&error_message[0]);
				}
			}
		}
		else{
			wait(NULL);
			// waitpid(child_pid, &stat_loc, WUNTRACED);
		}
		free(line);
		free(args);
	}
	return 0;
}


// handle invalid commands as it is blocking other commands for the same numebr of times like quit command.