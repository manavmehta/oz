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
#define RESET "\x1B[0m"

using namespace std;

// g++ oz.cpp -lreadline

// 
char **parse_args(char *line){
	char **args = new char*;
	char *sep=" ", *token;
	int index=0;

	token = strtok(line, sep);
	while(token){
		args[index++]=token;
		token = strtok(NULL, sep);
	}
	args[index]=NULL;
	return args;
}

int main() {
	char **args;
	char *line;
	pid_t child_pid;
	while(1){
		printf(GRN "≈> " RESET);
		line=readline("");
		args=parse_args(line);
		// int i=0;
		// while(args[i]){
		//     cout<<args[i++]<<endl;
		// }

		child_pid = fork();

		if(child_pid<0){
			perror("failed to create a fork.");
			exit(1);
		}

		// if the process is child execute it, else if parent: wait
		if (!child_pid){
			int exec_response = execvp(args[0], args);

			// execvp returns -1 on failure and nothing on success; API may return other negative vals
			if(exec_response < 0){
				string error_message = "could not execute: " + (string)args[0];
				perror(&error_message[0]);
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