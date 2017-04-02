#include "sfish.h"
#include "debug.h"

void pwd(PipeData *pipedata){

	debug("%s\n", "pwd was entered");

	if(pipedata->numpipes == 0 && pipedata->left_angle == 0 && pipedata->right_angle == 0){

		int status;
		(void)status;
		pid_t pid = fork();

		if(pid < 0){

			error("%s","Fork failed in PWD\n");

		}else if(pid == 0){

			//Child
			fprintf(stdout, "%s", getcwd(NULL, 0));
			exit(EXIT_SUCCESS);

		}
		//Parent waits for child to finish
		//waitpid(pid, &status, 0);

	}else if(pipedata->numpipes == 1 && pipedata->left_angle == 0 && pipedata->right_angle == 0){

		//int status;
			char *cmd1 = searchPATH(*(pipedata->prgm1_args));
			char *cmd2 = searchPATH(*(pipedata->prgm2_args));

			if(cmd1 == NULL){

				fprintf(stderr, "Command not found: %s\n", *(pipedata->prgm1_args));
				return;

			}

			if(cmd2 == NULL){

				fprintf(stderr, "Command not found %s\n", *(pipedata->prgm2_args));
				return;

			}

			int fd[2];

			if(pipe(fd) == -1){

				error("%s","Error in pipe\n");
				fprintf(stderr, "Piping error\n");
				return;

			}

			pid_t pid1 = fork();

			if(pid1 < 0){

				fprintf(stderr, "Error forking\n");
				return;

			}else if(pid1 == 0){

				//Child
				if(dup2(fd[1], 1) == -1){

					fprintf(stderr, "Dup failed\n");
					return;

				}

				close(fd[0]);
				close(fd[1]);

				fprintf(stdout, "%s", getcwd(NULL, 0));
				fflush(stdout);

				close(1);
				close(0);

				exit(EXIT_SUCCESS);

				// execv(cmd1, pipedata->prgm1_args);

				// fprintf(stderr,"Error! Program could not execute\n");
				// exit(EXIT_FAILURE);

			}
			//Parent

			pid_t pid2 = fork();

			if(pid2 < 0){

				error("%s","Error forking\n");
				return;

			}else if(pid2 == 0){

				//Second child
				if(dup2(fd[0], 0) == -1){

					fprintf(stderr, "Dup failed\n");
					return;

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
				return;

			}

			if(cmd2 == NULL){

				fprintf(stderr, "Command not found %s\n", *(pipedata->prgm2_args));
				return;

			}

			if(cmd3 == NULL){

				fprintf(stderr, "Command not found %s\n", *(pipedata->prgm3_args));
				return;

			}

			int fd1[2];
			int fd2[2];

			debug("%s","Piping first pipe...\n");
			if(pipe(fd1) == -1){

				error("%s","Error in pipe 1\n");
				fprintf(stderr, "Piping error 1\n");
				return;

			}
			debug("Pipe has file descriptors: %d %d\n", fd1[0], fd1[1]);

			debug("%s", "Piping second pipe...\n");
			if(pipe(fd2) == -1){

				error("%s","Error in pipe 2\n");
				fprintf(stderr, "Piping error 2\n");
				return;

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
				return;

			}else if(pid == 0){

				debug("%s\n","Child 1: dup2");
				if(dup2(fd1[1], 1) == -1){

					fprintf(stderr, "Dup 1 failed\n");
					return;

				}

				debug("%s","Child 1: closing pipes\n");
				close(fd1[0]);
				close(fd1[1]);

				debug("Child 1: executing %s\n", cmd1);

				fprintf(stdout, "%s", getcwd(NULL, 0));
				exit(EXIT_SUCCESS);
				// execv(cmd1, pipedata->prgm1_args);

				// fprintf(stderr, "Error! Program could not execute\n");
				// exit(EXIT_FAILURE);

			}
			debug("1st pid spawned: %d\n", pid);
			wait(NULL);
			debug("%s","Parent: forking second child\n");

			//Fork second process
			pid = fork();

			if(pid < 0){

				fprintf(stderr, "Forking error\n");
				return;

			}else if(pid == 0){

				if(dup2(fd1[0], 0) == -1){

					fprintf(stderr, "Dup 2 failed\n");
					return;

				}

				if(dup2(fd2[1], 1) == -1){

					fprintf(stderr, "Dup 3 failed\n");
					return;

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
				return;

			}else if(pid == 0){

				if(dup2(fd2[0], 0) == -1){

					fprintf(stderr, "Dup 4 failed\n");
					return;

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


	}else if(pipedata->numpipes == 0 && pipedata->left_angle == 0 && pipedata->right_angle == 1){

		int status;
		(void)status;
		pid_t pid = fork();

			if(pid < 0){

				error("%s","Fork failed in execution of command\n");

			}else if(pid == 0){
				//Child
				int out = open(pipedata->outfile, O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP | O_TRUNC);

				if(out != -1){

					dup2(out, 1);
					close(out);
					debug("%s","Output being redirected!\n");

				}

				debug("%s","PWD executing in child\n");;

				fprintf(stdout, "%s", getcwd(NULL, 0));
				exit(EXIT_SUCCESS);

			}

			//Parent waits for child to finish
			//waitpid(pid, &status, 0);

	}else{

		fprintf(stderr,"Invalid command");
		return;

	}

}
