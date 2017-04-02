#include "sfish.h"
#include "debug.h"


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

			pwd(pipedata);
			printf("\n");

		}else if(strcmp(cmdtok[0], "alarm") == 0){

			if(numargs > 1){

				unsigned int seconds = atoi(cmdtok[1]);
				my_alarm(seconds);

			}

		}else{

			printf("command not found: %s\n", cmdtok[0]);

		}

		return 1;

	//If not, search for and then execute it
	}else{

		//fprintf(stderr, "\n\nMEMES\n\n");

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

			//wait(NULL);

			free(path_to_exec);

		}else if(pipedata->numpipes == 0 && (pipedata->left_angle == 1 || pipedata->right_angle == 1)){
			//REDIRECTION!!!

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
				int out = open(pipedata->outfile, O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP | O_TRUNC);

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
				dispStringArr(pipedata->prgm1_args, pipedata->prgm1_numargs);

				execv(cmd, pipedata->prgm1_args); //Should never return

				error("%s","Error! Program could not execute\n");
				exit(EXIT_FAILURE);

			}

			fflush(stdout);
			fflush(stderr);
			//Parent
			//waitpid(pid, 0, 0);


		}else if(pipedata->numpipes == 1 && pipedata->left_angle == 0 && pipedata->right_angle == 0){
			//ONE PIPE!!!

			//int status;
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

			int fd[2];

			if(pipe(fd) == -1){

				error("%s","Error in pipe\n");
				fprintf(stderr, "Piping error\n");
				return 1;

			}

			pid_t pid1 = fork();

			if(pid1 < 0){

				fprintf(stderr, "Error forking\n");
				return 1;

			}else if(pid1 == 0){

				//Child
				if(dup2(fd[1], 1) == -1){

					fprintf(stderr, "Dup failed\n");
					return 1;

				}

				close(fd[0]);
				close(fd[1]);

				execv(cmd1, pipedata->prgm1_args);

				fprintf(stderr,"Error! Program could not execute\n");
				exit(EXIT_FAILURE);

			}
			//Parent

			pid_t pid2 = fork();

			if(pid2 < 0){

				error("%s","Error forking\n");
				return 1;

			}else if(pid2 == 0){

				//Second child
				if(dup2(fd[0], 0) == -1){

					fprintf(stderr, "Dup failed\n");
					return 1;

				}

				close(fd[0]);
				close(fd[1]);

				execv(cmd2, pipedata->prgm2_args);

				fprintf(stderr, "Error! Program could not execute\n");
				exit(EXIT_FAILURE);

			}

			fflush(stdout);
			fflush(stderr);

			//Parent
			//wait(NULL);

		}else if(pipedata->numpipes == 2 && pipedata->left_angle == 0 && pipedata->right_angle == 0){
			//TWO PIPES!

			char *cmd1 = searchPATH(*(pipedata->prgm1_args));
			char *cmd2 = searchPATH(*(pipedata->prgm2_args));
			char *cmd3 = searchPATH(*(pipedata->prgm3_args));

			if(cmd1 == NULL){

				fprintf(stderr, "Command not found: %s\n", *(pipedata->prgm1_args));
				return 1;

			}

			if(cmd2 == NULL){

				fprintf(stderr, "Command not found %s\n", *(pipedata->prgm2_args));
				return 1;

			}

			if(cmd3 == NULL){

				fprintf(stderr, "Command not found %s\n", *(pipedata->prgm3_args));
				return 1;

			}

			int fd1[2];
			int fd2[2];

			debug("%s","Piping first pipe...\n");
			if(pipe(fd1) == -1){

				error("%s","Error in pipe 1\n");
				fprintf(stderr, "Piping error 1\n");
				return 1;

			}
			debug("Pipe has file descriptors: %d %d\n", fd1[0], fd1[1]);

			debug("%s", "Piping second pipe...\n");
			if(pipe(fd2) == -1){

				error("%s","Error in pipe 2\n");
				fprintf(stderr, "Piping error 2\n");
				return 1;

			}
			debug("Pipe has file descriptors: %d %d\n", fd2[0], fd2[1]);

			//Fork first process
			pid_t pid;
			int status;
			(void)status;

			debug("%s\n","Forking first process...");
			pid = fork();

			if(pid < 0){

				fprintf(stderr, "Forking error\n");
				return 1;

			}else if(pid == 0){

				debug("%s\n","Child 1: dup2");
				if(dup2(fd1[1], 1) == -1){

					fprintf(stderr, "Dup 1 failed\n");
					return 1;

				}

				debug("%s","Child 1: closing pipes\n");
				close(fd1[0]);
				close(fd1[1]);

				debug("Child 1: executing %s\n", cmd1);
				execv(cmd1, pipedata->prgm1_args);

				fprintf(stderr, "Error! Program could not execute\n");
				exit(EXIT_FAILURE);

			}
			debug("1st pid spawned: %d\n", pid);
			wait(NULL);
			debug("%s","Parent: forking second child\n");

			//Fork second process
			pid = fork();

			if(pid < 0){

				fprintf(stderr, "Forking error\n");
				return 1;

			}else if(pid == 0){

				if(dup2(fd1[0], 0) == -1){

					fprintf(stderr, "Dup 2 failed\n");
					return 1;

				}

				if(dup2(fd2[1], 1) == -1){

					fprintf(stderr, "Dup 3 failed\n");
					return 1;

				}

				close(fd1[0]);
				close(fd1[1]);
				close(fd2[0]);
				close(fd2[1]);

				debug("2nd child: executing %s\n", cmd2);
				execv(cmd2, pipedata->prgm2_args);

				fprintf(stderr, "Error! Program could not execute\n");
				exit(EXIT_FAILURE);

			}else{

				close(fd1[0]);
				close(fd1[1]);
				//wait(NULL);

			}

			debug("2nd pid spawned: %d\n", pid);

			debug("%s","Parent: forking third child\n");
			//Fork third process
			pid = fork();

			if(pid < 0){

				fprintf(stderr, "Forking error\n");
				return 1;

			}else if(pid == 0){

				if(dup2(fd2[0], 0) == -1){

					fprintf(stderr, "Dup 4 failed\n");
					return 1;

				}

				close(fd2[0]);
				close(fd2[1]);

				execv(cmd3, pipedata->prgm3_args);

				fprintf(stderr, "Error! Program could not execute\n");
				exit(EXIT_FAILURE);

			}else{

				close(fd2[0]);
				close(fd2[1]);
				//wait(NULL);

			}
			debug("3rd pid spawned: %d\n", pid);

			fflush(stdout);
			fflush(stderr);

		}else{

			fprintf(stderr, "Error: Invalid commands\n");

		}

	return 1;

	}

}
