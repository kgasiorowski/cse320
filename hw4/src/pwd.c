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

		int p[2];
		if(pipe(p) == -1){

			error("%s","Error in pipe\n");
			fprintf(stderr, "Piping error\n");
			return;

		}

		char *cmd2 = searchPATH(*(pipedata->prgm2_args));

		if(cmd2 == NULL){

				fprintf(stderr, "Command not found %s\n", *(pipedata->prgm2_args));
				return;

		}

		int status;
		(void)status;

		pid_t pid = fork();

		if(pid < 0){

			error("%s","Fork failed in PWD\n");

		}else if(pid == 0){

			if(dup2(p[1], 1) == -1){

				fprintf(stderr, "Dup failed 1\n");
				exit(EXIT_FAILURE);

			}

			//Child
			fprintf(stdout, "%s", getcwd(NULL, 0));
			exit(EXIT_SUCCESS);

		}

		pid = fork();

		if(pid < 0){

			error("%s","Fork failed in PWD\n");

		}else if(pid == 0){

			if(dup2(p[0], 0) == -1){

				fprintf(stderr, "Dup failed 2\n");
				exit(EXIT_FAILURE);

			}

			//Child
			execv(cmd2, pipedata->prgm2_args);

			fprintf(stderr,"Error! Program could not execute\n");
			exit(EXIT_FAILURE);

		}
		//Parent waits for child to finish
		//waitpid(pid, &status, 0);


	}else if(pipedata->numpipes == 2 && pipedata->left_angle == 0 && pipedata->right_angle == 0){



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
