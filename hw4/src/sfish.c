#include "sfish.h"

void execute_command(const char *command){

	//If it is a recognized built-in command
	if(is_builtin(command)){



	//If not...
	}else{




	}

}

//Return true if the command is a builtin, false otherwise
int is_builtin(const char *command){

	return strcmp(command, "help") || strcmp(command, "exit") || strcmp(command, "cd") || strcmp(command, "pwd");

}
