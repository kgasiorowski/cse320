#include "sfish.h"
#include "debug.h"

char *last_directory;

char *searchPATH(char *cmd){

	char *path = NULL;

	//Check if the path has the '/' character
	if(strchr(cmd, '/') != NULL){

		struct stat * const fs = (struct stat*)malloc(sizeof(struct stat));

		if(stat(cmd, fs) == -1){
		//If this is true, the function will return without executing
			free(fs);//We don't need this

			//Error checking
			if(errno == ENOENT){
				//File does not exist
				fprintf(stderr, "bash: %s: No such file or directory\n", cmd);
				return NULL;

			}else{
				//There was some other error
				debug("There was some other error at path: %s\n", cmd);
				return NULL;

			}

		}

		//Get some space and copy the command to execute
		path = (char*)malloc(sizeof(char)*strlen(cmd));
		strcpy(path, cmd);

		//Don't need this
		free(fs);

	}else{

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

			//debug("Path scanned: %s\n", *paths);
			*(++paths) = strtok(NULL, ":");

		}

		//Reset to the original pointer
		paths = PATHS_PTR;

		struct stat * const fs = (struct stat*)malloc(sizeof(struct stat));

		while(*paths != NULL){

			path = (char*)malloc(sizeof(char)* (strlen(*paths) + strlen(cmd) + 3) );

			strcpy(path, *paths);
			strcat(path, "/");
			strcat(path, cmd);
			strcat(path, "\0");

			//debug("Created path: %s. Checking...\n", path);

			if(stat(path, fs) == -1){

				if(errno == ENOENT){

					//File does not exist
					//debug("File not found at path: %s\n", path);
					free(path);
					path = NULL;
					paths++;
					continue;

				}else{

					//There was some other error
					fprintf(stderr, "There was some other error at path: %s\n", path);
					return 0;

				}

			}else{

				debug("Found program %s at path %s.\n", cmd, path);
				free(fs);
				free(PATHS_PTR);
				break;

			}

		}

	}

	debug("Returning: %s\n", path);
	return path;

}

void dispStringArr(char **argv, int argc){

	for(int i = 0; i < argc; i++)
		debug("%s\n", argv[i]);

}

PipeData *pipe_parse_commands(char **argv, int argc){

	PipeData *pipe_bits = (PipeData*)calloc(1,sizeof(PipeData));

	pipe_bits->numpipes = 0;
	pipe_bits->left_angle = 0;
	pipe_bits->right_angle = 0;

	pipe_bits->error = 0;

	pipe_bits->prgm1_args = argv;
	pipe_bits->prgm1_numargs = argc;

	pipe_bits->prgm2_args = NULL;
	pipe_bits->prgm2_numargs = 0;

	pipe_bits->prgm3_args = NULL;
	pipe_bits->prgm3_numargs = 0;

	pipe_bits->infile = NULL;
	pipe_bits->outfile = NULL;

	pipe_bits->infile_index = -1;
	pipe_bits->outfile_index = -1;

	pipe_bits->pipe1_index = -1;
	pipe_bits->pipe2_index = -1;

	pipe_bits->left_angle_index = -1;
	pipe_bits->right_angle_index = -1;

	int i = 0;
	for(; i < argc; i++){

		if(strcmp(argv[i], "|") == 0){

			debug("Pipe detected at index: %d\n", i);

			if(i == 0 || i == argc-1){

				pipe_bits->error = 1;
				return pipe_bits;

			}

			pipe_bits->numpipes++;

			if(pipe_bits->numpipes == 1){

				debug("Index of pipe 1: %d\n", i);
				pipe_bits->pipe1_index = i;

			}else if(pipe_bits->numpipes == 2){

				debug("Index of pipe 2: %d\n", i);
				pipe_bits->pipe2_index = i;

			}


		}else if(strcmp(argv[i], ">") == 0){

			debug("Index of right angle: %d\n", i);

			if(i == 0 || i == argc-1)
			{

				pipe_bits->error = 1;
				return pipe_bits;

			}

			pipe_bits->right_angle++;
			pipe_bits->right_angle_index = i;

			pipe_bits->outfile_index = i+1;
			pipe_bits->outfile = argv[i+1];

		}else if(strcmp(argv[i], "<") == 0){

			debug("Index of left angle: %d\n", i);

			if(i == 0 || i == argc-1)
			{

				pipe_bits->error = 1;
				return pipe_bits;

			}

			pipe_bits->left_angle++;
			pipe_bits->left_angle_index = i;

			pipe_bits->infile_index = i+1;
			pipe_bits->infile = argv[i+1];

		}

	}

	//Figure out program 2 and 3 after the fact
	if(pipe_bits->numpipes >= 1){

		//There exists a pipe. We have to copy some args
		pipe_bits->prgm1_args = (char**)calloc(NUM_ARGS, sizeof(char*));
		pipe_bits->prgm2_args = (char**)calloc(NUM_ARGS, sizeof(char*));

		null_check(pipe_bits->prgm1_args);
		null_check(pipe_bits->prgm2_args);

		int i = 0;
		for(;i<pipe_bits->pipe1_index;i++){

			pipe_bits->prgm1_args[i] = argv[i];

		}
		pipe_bits->prgm1_numargs = pipe_bits->pipe1_index;

		//Null terminate
		pipe_bits->prgm1_args[pipe_bits->pipe1_index] = NULL;

		debug("Number of arguments in prgm 1: %d\n", pipe_bits->prgm1_numargs);
		dispStringArr(pipe_bits->prgm1_args, pipe_bits->prgm1_numargs);

		//Check if there's a second pipe
		if(pipe_bits->pipe2_index != -1){

			pipe_bits->prgm3_args = (char**)calloc(NUM_ARGS, sizeof(char*));

			//Subdivide the remaining args between the two pipes
			for(int i = pipe_bits->pipe1_index+1; i<pipe_bits->pipe2_index; i++){

				debug("String: %s, %d\n", argv[i], i-pipe_bits->pipe1_index-1);
				pipe_bits->prgm2_args[i-pipe_bits->pipe1_index-1] = argv[i];

			}
			pipe_bits->prgm2_numargs = pipe_bits->pipe2_index - pipe_bits->prgm1_numargs - 1;

			debug("Number of arguments in prgm 2: %d\n", pipe_bits->prgm2_numargs);
			dispStringArr(pipe_bits->prgm2_args, pipe_bits->prgm2_numargs);

			for(int i = pipe_bits->pipe2_index+1; i < argc; i++){

				debug("String: %s, %d\n", argv[i], i-pipe_bits->pipe2_index-1);
				pipe_bits->prgm3_args[i-pipe_bits->pipe2_index-1] = argv[i];

			}
			pipe_bits->prgm3_numargs = argc - pipe_bits->pipe2_index - 1;

			debug("Number of arguments in prgm 3: %d\n", pipe_bits->prgm3_numargs);
			dispStringArr(pipe_bits->prgm3_args, pipe_bits->prgm3_numargs);

		}else{

			//Parse the rest of the commands for program 2
			for(int i = pipe_bits->pipe1_index+1; i < argc; i++){

				debug("String: %s, %d\n", argv[i], i-pipe_bits->pipe1_index-1);
				pipe_bits->prgm2_args[i-pipe_bits->pipe1_index-1] = argv[i];

			}
			pipe_bits->prgm2_numargs = argc - pipe_bits->prgm1_numargs - 1;

			debug("Number of arguments in prgm 2: %d\n", pipe_bits->prgm2_numargs);
			dispStringArr(pipe_bits->prgm2_args, pipe_bits->prgm2_numargs);

		}

	}else{

		pipe_bits->prgm1_args = (char**)calloc(NUM_ARGS, sizeof(char*));

		int index_to_loop_until = 0;

		if(pipe_bits->right_angle_index == -1)
			index_to_loop_until = pipe_bits->left_angle_index;
		else if(pipe_bits->left_angle_index == -1)
			index_to_loop_until = pipe_bits->right_angle_index;
		else
			index_to_loop_until = pipe_bits->left_angle_index;

		for(int i = 0; i < index_to_loop_until; i++){

			pipe_bits->prgm1_args[i] = argv[i];

		}
		pipe_bits->prgm1_numargs = index_to_loop_until;

		debug("Number of arguments in prgm 2: %d\n", pipe_bits->prgm1_numargs);
		dispStringArr(pipe_bits->prgm1_args, pipe_bits->prgm1_numargs);

	}

	return pipe_bits;

}

//Return true if the command is a builtin, false otherwise
int is_builtin(const char *command){

	return !strcmp(command, "help") || !strcmp(command, "exit") || !strcmp(command, "cd") || !strcmp(command, "pwd") || !strcmp(command, "alarm");

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
