#include "sfish.h"
#include "debug.h"

void execute_nopipe(char *path, char **args){

	debug("%s","Entered execute func\n");

	pid_t pid = fork();

	if(pid < 0){

		error("%s","Fork failed in execution of command\n");

	}else if(pid == 0){
		//Child
		debug("%s","About to execute!\n");
		execv(path, args); //Should never return

		error("Something went wrong with executing %s\n", path);
		exit(EXIT_FAILURE);

	}
	//Parent
	//waitpid(pid, NULL, 0);

}
