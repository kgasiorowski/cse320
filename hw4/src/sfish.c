#include "sfish.h"
#include "debug.h"

void execute_command(char **cmdtok){

	//Nothing was passed in
	if(*cmdtok == NULL)
		return;

	//If it is a recognized built-in command
	if(is_builtin(cmdtok[0])){

		if(strcmp(cmdtok[0], "exit") == 0){

			exit(EXIT_SUCCESS);

		}else if(strcmp(cmdtok[0], "help") == 0){

			print_help();

		}else if(strcmp(cmdtok[0], "cd") == 0){

			debug("%s","cd was entered\n");

		}else if(strcmp(cmdtok[0], "pwd") == 0){

			debug("%s", "pwd was entered");
			pwd();
			printf("\n");

		}else{

			printf("command not found: %s\n", cmdtok[0]);

		}


	//If not...
	}else{




	}

}

int cd(const char* path){

	return 0;

}

void pwd(){

	pid_t pid = fork();

	if(pid < 0){

		error("%s","Fork failed in PWD\n");

	}else if(pid == 0){

		//Child
		fflush(stdin);
		const int num_chars = 1024;
		char *buf = (char*)malloc(sizeof(char)*num_chars);
		printf("%s",getcwd(buf, num_chars));
		free(buf);
		exit(EXIT_SUCCESS);

	}else{

		//Parent
		wait(NULL);

	}

}

//Return true if the command is a builtin, false otherwise
int is_builtin(const char *command){

	return strcmp(command, "help") || strcmp(command, "exit") || strcmp(command, "cd") || strcmp(command, "pwd");

}
