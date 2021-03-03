#include <fstream>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include "helpers.h"

#define endl '\n'
#define HISTFILE ".oz_history"

using namespace std;

// g++ oz.cpp -lreadline

int main() {
	char **args;
	char *line;
	pid_t child_pid;

	FILE *histfile = fopen(HISTFILE, "ab");

	// TODO: Handle signals

	while(1){
		printf(GRN "≈> " RESET);
		line=readline("");

		fprintf(histfile, "%s\n", line);

		args=parse_args(line);

		// handle cd
		if(strcmp(args[0],"cd")==0){ //done
			chdir(args[1]);
			continue; // perform cd and rerun parent: no forking
		}
		else if(strcmp(args[0],"quit")==0){ //done
			fclose(histfile);
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
			
			if(strcmp(args[0],"clr")==0){ //done
				cout << "\033[2J\033[1;1H";
			}
			else if(strcmp(args[0],"pause")==0){ //done
				system("read -p 'Paused.....'");
				// system("read -p 'Paused.....' var");
			}
			else if(strcmp(args[0],"help")==0){ //done
				help();
			}
			else if(strcmp(args[0],"history")==0){ //done
				history((char*)HISTFILE);
			}
			else if(strcmp(args[0],"dir")==0){ // not working for absolute paths
				execvp((char*)("ls"), args);
			}
			else if(strcmp(args[0],"environ")==0){
				continue;
			}
			else if(strcmp(args[0],"echo")==0){ //done
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