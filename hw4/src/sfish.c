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

			if(numargs == 1){

				//Save current directory location
				getcwd(last_directory, PATH_BUFFER_SIZE);

				chdir(getenv("HOME"));

			}else{

				if(strcmp(cmdtok[1], "..") == 0){

					//Save current directory location
					getcwd(last_directory, PATH_BUFFER_SIZE);

					//Move one up

					const char *delims = "/";

					//Tokenize string with '/'s
					char *path = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);
					getcwd(path, PATH_BUFFER_SIZE);

					//Directory list
					char **directory_list = (char**)malloc(sizeof(char*)*100);
					char **cursor = directory_list;

					int counter = 0;
					*cursor = strtok(path, delims);

					if(*cursor == NULL){
						debug("%s","Can't up one. Exiting\n");
						return 1;
					}

					while(*cursor != NULL){

						counter++;
						*(++cursor) = strtok(NULL, delims);
						debug("Scanned directory in path: %s, counter: %d\n", *cursor, counter);

					}

					//Rebuild the string except without the last token
					char *new_path = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);

					strcpy(new_path, "/");

					cursor = directory_list;

					int i = 0;
					for(;i<counter-1;i++){

						debug("Cursor value: %s\n", *cursor);

						strcat(new_path, *(cursor++));
						strcat(new_path, "/");
						debug("New path generated: %s\n", new_path);

					}

					debug("Final new path: %s\n", new_path);
					chdir(new_path);

					free(path);
					free(new_path);
					free(directory_list);

				}else if(strcmp(cmdtok[1], "-") == 0){
					//Go to the last directory

					debug("%s","Going to last directory...\n");

					//Temporarily save the old path
					char *temp = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);
					strcpy(temp, last_directory);

					debug("Last directory: %s\n", last_directory);

					//Save the new old last directory path
					getcwd(last_directory, PATH_BUFFER_SIZE);

					debug("New old directory: %s\n", last_directory);

					//Change directory to the saved last directory
					if(chdir(temp) == -1)
					{

						printf("Directory %s not found.", temp);

					}

					//Release the temporary space we needed
					free(temp);

				}else if(strcmp(cmdtok[1], ".") == 0){

					//Save current directory location
					getcwd(last_directory, PATH_BUFFER_SIZE);

					return 1;

				}else if(strcmp(cmdtok[1], "/") == 0){

					//Save current directory location
					getcwd(last_directory, PATH_BUFFER_SIZE);

					chdir("/");

				}else{

					//Save current directory location
					getcwd(last_directory, PATH_BUFFER_SIZE);

					//Try relative path

					char *buf = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);

					getcwd(buf, PATH_BUFFER_SIZE);

					debug("Current path: %s\n", buf);

					strcat(buf, "\\/");
					strcat(buf, cmdtok[1]);

					debug("Path to switch to: %s\n", buf);

					if(chdir(buf) == -1 && errno == ENOENT){

						//The path wasn't found in the relative directory.
						//Try absolute
						strcpy(buf, cmdtok[1]);

						debug("%s","Relative path no good.\n");
						debug("Trying absolute path: %s\n", buf);

						if(chdir(buf) == -1 && errno == ENOENT)
							printf("cd: %s: No such file or directory\n", cmdtok[1]);

					}else{

						debug("%s\n", "Changed directory");

					}

					free(buf);

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
