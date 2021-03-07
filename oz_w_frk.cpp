#include <fstream>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h> //add in makefile
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include<thread>

#include "helpers.h"

#define endl '\n'
#define HISTFILE ".oz_history"

using namespace std;

// g++ oz.cpp -lreadline

void run_shell(char* line, char *envp[]);

int main(int argc, char* argv[], char *envp[]) {

	create_env();

	char *line;

	signal(SIGINT, SIG_IGN);

	// if argc == 1 run shell in realtime, else use argv arg as filename to run commands
	if(argc == 1){
		while(1){
			printf(GRN "≈> " RESET);
			line=readline("");

			run_shell(line, envp);
			free(line);
		}
	}
	else{
		FILE *commandsfile = fopen(argv[1], "rb");
		char chunk[200];
		while(fgets(chunk, sizeof(chunk), commandsfile)){
			int i=0;
			while((int)chunk[i]>=32 && (int)chunk[i]<=126) // valid character
				i++;
			chunk[i]='\0'; // terminating the chunk is necessary
			run_shell(chunk, envp);
		}
		fclose(commandsfile);
		exit(0);
	}
	return 0;
}

void run_shell(char* line, char *envp[]){
	if(strlen(line)==0) return;

	// writing to histfile
	FILE *histfile = fopen(HISTFILE, "ab");
	fprintf(histfile, "%s\n", line);
	fclose(histfile);

	char **args=parse_args(line);

	// handle cd
	if(strcmp(args[0],"cd")==0){ //done
		chdir(args[1]);
		return; // perform cd and rerun parent: no forking
	}
	else if(strcmp(args[0],"quit")==0){ //done
		// fclose(histfile);
		exit(0);
		return;
	}	

	pid_t child_pid;
	child_pid = fork();

	if(child_pid<0){
		perror("failed to create a fork.");
		exit(1);
	}

	// if the process is child execute it, else if parent: wait
	if (!child_pid){

		signal(SIGINT, SIG_DFL);

		if(strcmp(args[0],"clr")==0){ //done
			fputs("\033[2J\033[1;1H",stdout);
		}
		else if(strcmp(args[0],"pause")==0){ //done
			printf("Paused.....\n");  
			getchar(); 
		}
		else if(strcmp(args[0],"help")==0){ //done
			help();
		}
		else if(strcmp(args[0],"history")==0){ //done
			history((char*)HISTFILE);
		}
		else if(strcmp(args[0],"dir")==0){ // not working for absolute paths
			dir(args[1]);
		}
		else if(strcmp(args[0],"environ")==0){
			printf(GRN "%s", "environment variables for the current shell:\n" RESET);
			FILE *envfile = fopen("environment", "rb");
			char chunk[200];
			while(fgets(chunk, sizeof(chunk), envfile)){
				fputs(chunk, stdout);
			}
			cout<<endl;

			printf(GRN "%s", "environment variables for the bash(or your default) shell:\n" RESET);
			int i; 
			for (i = 0; envp[i] != NULL; i++) 
				printf("\n%s", envp[i]); 
			printf("%c", endl);


			return;
		}
		else if(strcmp(args[0],"echo")==0){ //done
			int i=1;
			while(args[i]!=NULL){
				printf("%s ", args[i++]);
			}
			printf("%c", endl);
			return;
		}
		
		else if(strcmp(args[0],"pwd")==0){ //done
			char cwd[200];
			getcwd(cwd, sizeof(cwd));
			printf("%s\n", cwd);
			return;
		}

		else{
			int exec_response = execvp(args[0], args);

			// execvp returns -1 on failure and nothing on success; API may return other negative vals
			if(exec_response < 0){
				string error_message = "could not execute: " + (string)args[0];
				perror(&error_message[0]);
			}
		}
		exit(0);
	}
	else{
		wait(NULL);
	}
	free(args);
}