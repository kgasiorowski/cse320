#include "sfish.h"
#include "debug.h"

static char *last_directory;

int execute_command(char **cmdtok, int numargs){

	//Nothing was passed in
	if(*cmdtok == NULL || numargs == 0)
		return 1;

	//If it is a recognized built-in command
	if(is_builtin(cmdtok[0])){

		debug("%s","Builtin detected\n");

		if(strcmp(cmdtok[0], "exit") == 0){

			debug("%s","Entered exit\n");

			finish();

			debug("%s", "Called cleanup\n");

    		printf("logout\n");
			return 0;

		}else if(strcmp(cmdtok[0], "help") == 0){

			print_help();

		}else if(strcmp(cmdtok[0], "cd") == 0){

			cd(numargs, cmdtok);

		}else if(strcmp(cmdtok[0], "pwd") == 0){

			pwd();
			printf("\n");

		}else{

			printf("command not found: %s\n", cmdtok[0]);

		}

		return 1;

	//If not, search for and then execute it
	}else{

		//Check if the path has the '/' character
		if(strchr(cmdtok[0], '/') != NULL){

			struct stat * const fs = (struct stat*)malloc(sizeof(struct stat));

			if(stat(cmdtok[0], fs) == -1){

				if(errno == ENOENT){
					//File does not exist
					debug("File not found at path: %s\n", cmdtok[0]);

				}else{
					//There was some other error
					debug("There was some other error at path: %s\n", cmdtok[0]);
					return 0;

				}

				return 1;

			}else{

				execute(cmdtok[0], cmdtok);

			}

			free(fs);
			return 1;

		}

		//If it isn't - gotta check all the paths
		char * const PATH = getenv("PATH");
		char *pathcpy = (char*)malloc(sizeof(char) * (strlen(PATH)+1));

		strcpy(pathcpy, PATH);

		debug("Path variable: %s\n", pathcpy);

		//Space for 100 paths
		char ** const PATHS_PTR = (char**)calloc(100, sizeof(char*));
		char **paths = PATHS_PTR;

		*paths = strtok(pathcpy, ":");

		while(*paths != NULL){

			debug("Path scanned: %s\n", *paths);
			*(++paths) = strtok(NULL, ":");

		}

		//Reset to the original pointer
		paths = PATHS_PTR;

		struct stat * const fs = (struct stat*)malloc(sizeof(struct stat));
		char *path;

		while(*paths != NULL){

			path = (char*)malloc(sizeof(char)* (strlen(*paths) + strlen(cmdtok[0]) + 3) );

			strcpy(path, *paths);
			strcat(path, "/");
			strcat(path, cmdtok[0]);
			strcat(path, "\0");

			debug("Created path: %s. Checking...\n", path);

			if(stat(path, fs) == -1){

				if(errno == ENOENT){

					//File does not exist
					debug("File not found at path: %s\n", path);
					free(path);
					paths++;
					continue;

				}else{

					//There was some other error
					debug("There was some other error at path: %s\n", path);
					return 0;

				}

			}else{

				debug("Found program %s at path %s. Forking and executing\n", cmdtok[0], path);

				execute(path, cmdtok);

				debug("%s","First free\n");
				free(fs);

				debug("%s", "Second free\n");
				free(path);

				debug("%s", "Third free\n");
				free(PATHS_PTR);

				debug("%s", "Done freeing\n");
				return 1;

			}

		}

	}

	return 1;

}

void execute(char *path, char **args){

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

	}else{
		//Parent
		wait(NULL);

	}

}

void pwd(){

	debug("%s\n", "pwd was entered");

	pid_t pid = fork();

	if(pid < 0){

		error("%s","Fork failed in PWD\n");

	}else if(pid == 0){

		//Child
		fprintf(stdout, "%s", getcwd(NULL, 0));
		exit(EXIT_SUCCESS);

	}else{
		//Parent
		wait(NULL);

	}

}

//Return true if the command is a builtin, false otherwise
int is_builtin(const char *command){

	return !strcmp(command, "help") || !strcmp(command, "exit") || !strcmp(command, "cd") || !strcmp(command, "pwd");

}

void null_check(void *ptr){

	if(ptr == NULL){

        error("%s", "Out of memory!\n");
        exit(EXIT_FAILURE);

    }

}

void finish(){

	free(last_directory);

}

void cd(int numargs, char **cmdtok){

	debug("%s","cd was entered\n");

	if(numargs == 1){

		//Save current directory location
		last_directory = getcwd(NULL,0);

		chdir(getenv("HOME"));

	}else{

		if(strcmp(cmdtok[1], "..") == 0){

			//Save current directory location
			last_directory = getcwd(NULL,0);

			//Move one up
			const char *delims = "/";

			//Tokenize string with '/'s
			char *path = getcwd(NULL,0);

			//Directory list
			char ** const DIRECTORY_LIST = malloc(sizeof(char*)*100);
			char **cursor = DIRECTORY_LIST;

			int counter = 0;
			*cursor = strtok(path, delims);

			if(*cursor == NULL){
				debug("%s","Can't up one. Exiting\n");
				return;
			}

			while(*cursor != NULL){

				counter++;
				*(++cursor) = strtok(NULL, delims);
				debug("Scanned directory in path: %s, counter: %d\n", *cursor, counter);

			}

			//Rebuild the string except without the last token
			char * const new_path = (char*)calloc(PATH_BUFFER_SIZE, sizeof(char));

			*new_path = '/';

			cursor = (char**)DIRECTORY_LIST;

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
			free(DIRECTORY_LIST);

		}else if(strcmp(cmdtok[1], "-") == 0){
			//Go to the last directory

			debug("%s","Going to last directory...\n");

			//Temporarily save the old path
			char * const temp = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);
			strcpy(temp, last_directory);

			debug("Last directory: %s\n", last_directory);

			//Save the new old last directory path
			last_directory = getcwd(NULL, 0);

			debug("New old directory: %s\n", last_directory);

			//Change directory to the saved last directory
			if(chdir(temp) == -1)
			{

				printf("Directory %s not found.", temp);

			}

			//Release the temporary space we needed
			free(temp);

		}else if(strcmp(cmdtok[1], ".") == 0){

			//Save current directory location, but don't do
			//anything else
			last_directory = getcwd(NULL, 0);

		}else if(strcmp(cmdtok[1], "/") == 0){

			//Save current directory location
			last_directory = getcwd(NULL, 0);

			chdir("/");

		}else{

			//Save current directory location
			last_directory = getcwd(NULL, 0);

			//Try relative path
			// char * const buf = getcwd(NULL,0);

			char *buf = (char*)calloc(256, sizeof(char));
			getcwd(buf, 256);

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

	const char *temp;

	//Reset the PWD environment variable
	if(setenv("PWD", temp = getcwd(0,0), 1) == -1){

		error("Could not set pwd env to %s\n", getcwd(0,0));

	}

	free((void*)temp);

}
