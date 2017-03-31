#include "sfish.h"
#include "debug.h"

char *last_directory;

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
