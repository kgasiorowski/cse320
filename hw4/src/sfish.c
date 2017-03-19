#include "sfish.h"

void execute_command(const char *command){

	//If it is a recognized built-in command
	if(is_builtin(command)){

		if(strcmp(command, "exit") == 0){

			exit(EXIT_SUCCESS);

		}else if(strcmp(command, "help") == 0){

			print_help();

		}else if(strcmp(command, "cd") == 0){



		}else if(strcmp(command, "pwd") == 0){

			pwd();

		}else{

			printf("command not found: %s", command);

		}


	//If not...
	}else{




	}

}

char *pwd(){

	pid_t pid = fork();

	if(pid == 0){
		//Child
		size_t buffersize = 100;
		char *buffer = (char*)malloc(sizeof(char)*buffersize);

		if(buffer == NULL){

			fprintf(stderr, "Not enough memory!\n");

		}

		getcwd(buffer, buffersize);
		printf("%s\n", buffer);

	}else{
		//Parent
		return NULL;

	}

	return NULL;

}

//Return true if the command is a builtin, false otherwise
int is_builtin(const char *command){

	return strcmp(command, "help") || strcmp(command, "exit") || strcmp(command, "cd") || strcmp(command, "pwd");

}
