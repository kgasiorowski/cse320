#include "sfish.h"
#include "debug.h"

static char *last_directory;

int execute_command(char **cmdtok, int numargs){

	//Nothing was passed in
	if(*cmdtok == NULL || numargs == 0)
		return 1;

	//If it is a recognized built-in command
	if(is_builtin(cmdtok[0])){

		if(strcmp(cmdtok[0], "exit") == 0){

			finish();
    		printf("logout\n");
			return 0;

		}else if(strcmp(cmdtok[0], "help") == 0){

			print_help();

		}else if(strcmp(cmdtok[0], "cd") == 0){

			debug("%s","cd was entered\n");

			//Saving current directory location
			getcwd(last_directory, PATH_BUFFER_SIZE);

			if(numargs == 1)

				chdir(getenv("HOME"));

			else{

				if(strcmp(cmdtok[1], "..") == 0){
					//Move one up


				}else if(strcmp(cmdtok[1], "-") == 0){
					//Go to the last directory

					//Temporarily save the old path
					char *temp = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);
					strcpy(temp, last_directory);

					//Save the new old last directory path
					getcwd(last_directory, PATH_BUFFER_SIZE);

					//Change directory to the saved last directory
					chdir(temp);

					//Release the temporary space we needed
					free(temp);

				}else if(strcmp(cmdtok[1], ".") == 0){

					return 1;

				}else{



				}

			}



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

	return 1;

}

void pwd(){

	pid_t pid = fork();

	if(pid < 0){

		error("%s","Fork failed in PWD\n");

	}else if(pid == 0){

		//Child
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

//Called once to initialize things
void init(){

	last_directory = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);

}

//Call before exiting
void finish(){

	free(last_directory);

}
