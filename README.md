# Question 1

### oz
The unix shell

### Requirements and executing
The readline devel-package is required as the header 'readline'.
We are not including the steps in makefile as the evaluator would've installed it most probably.

If there is a need to install, use these commands:
* `sudo apt-get update -y`
* `sudo apt-get install -y libreadline-dev`

Now to compile, just use `make` and use the execuable `oz` to run the shell in prompt.

If the user wants the shell to read commands from external file, use `./a.out <filename>`

For convenience, put the commands in the file `commands` and run `./a.out commands`

### Language and Modules used
C++ 11

Headers:

* fstream - for file r/w
* stdlib.h, unistd.h - for some standard macros and functions
* sys/wait.h - for if need arises to use fork, parent needs to wait for child to finishs
* string.h - for string datatype
* signal.h - for handling some signals
* dirent.h - for directory entry struct needed for the dir command implementation
* errno.h - for standar error codes
* readline/readline.h - for getting input from realtime shell on prompt

