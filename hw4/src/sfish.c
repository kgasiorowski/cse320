#include "sfish.h"
#include "debug.h"
#include <stdio.h>
#include <fcntl.h>

int execute_command(char **cmdtok, int numargs){

	//Nothing was passed in
	if(*cmdtok == NULL || numargs == 0)
		return 1;

	PipeData *pipedata = pipe_parse_commands(cmdtok, numargs);

	if(pipedata->error == 1){

		fprintf(stderr, "Error: Invalid command sequence\n");
		return 1;

	}

	debug("Piping data: %d %d %d\n", pipedata->numpipes, pipedata->left_angle, pipedata->right_angle);

	//Cover 6 cases.
	//No pipes - execute normally
	//One pipe
	//Two pipes
	//Left angle
	//Right angle
	//Left and right angle

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

		//Logic to determine piping
		if(pipedata->numpipes == 0 && pipedata->left_angle == 0 && pipedata->right_angle == 0){

			char *path_to_exec = searchPATH(cmdtok[0]);

			//By now the execution path was found (or not).
			//Try to execute
			if(path_to_exec == NULL)
			{

				fprintf(stderr, "%s: command not found\n", cmdtok[0]);
				return 1;

			}else{

				debug("Forking and executing %s\n", path_to_exec);
				execute_nopipe(path_to_exec, cmdtok);

			}

			free(path_to_exec);

		}else if(pipedata->numpipes == 0 && (pipedata->left_angle == 1 || pipedata->right_angle == 1)){

			char *cmd = searchPATH(cmdtok[0]);

			if(cmd == NULL){

				fprintf(stderr, "Command not found: %s\n", cmd);
				return 1;

			}

			pid_t pid = fork();

			if(pid < 0){

				error("%s","Fork failed in execution of command\n");

			}else if(pid == 0){
				//Child
				int in = open(pipedata->infile, O_RDONLY);
				int out = open(pipedata->outfile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

				if(in != -1){

					dup2(in, 0);
					close(in);
					debug("%s","Input being redirected!\n");

				}

				if(out != -1){

					dup2(out, 1);
					close(out);
					debug("%s","Output being redirected!\n");

				}

				debug("%s","Child executing with arguments:\n");
				dispStringArr(pipedata->prgm1_args);

				execv(cmd, pipedata->prgm1_args); //Should never return

				error("%s","Error! Program could not execute\n");
				exit(EXIT_FAILURE);

			}else{
				//Parent
				wait(NULL);

			}


		}else if(pipedata->numpipes == 1 && pipedata->left_angle == 0 && pipedata->right_angle == 0){

			int fd[2];
			pipe(fd);

			char *cmd1 = searchPATH(*(pipedata->prgm1_args));
			char *cmd2 = searchPATH(*(pipedata->prgm2_args));

			if(cmd1 == NULL){

				fprintf(stderr, "Command not found: %s\n", *(pipedata->prgm1_args));
				return 1;

			}

			if(cmd2 == NULL){

				fprintf(stderr, "Command not found %s\n", *(pipedata->prgm2_args));
				return 1;

			}

			/*

			pid_t pid = fork();

			if(pid < 0){

				error("%s","Error forking\n");
				return 1;

			}else if(pid == 0){
				//Child
				exit(0);

			}else{
				//Parent
				wait(&pid);
				dup2(fd[0], 0);



			}
			*/

		}else if(pipedata->numpipes == 2 && pipedata->left_angle == 0 && pipedata->right_angle == 0){



		}

	return 1;

	}

}
