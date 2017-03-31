#ifndef SFISH_H
#define SFISH_H
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PATH_BUFFER_SIZE 512
#define NUM_ARGS 30

#define print_help() do {										\
const char* fmt = " %-10s%-40s\n";								\
printf("\nKuba Gasiorowski\nShell Implemented in CSE320\n\n");	\
printf(fmt,"Help","Displays this message");						\
printf(fmt, "cd", "Changes directory to the specified path");	\
printf(fmt, "pwd", "Prints the current working directory");		\
printf(fmt, "exit", "Exits the shell cleanly");					\
printf("\n");													\
}while(0)

extern char *last_directory;

typedef struct Pipe_bits{

	unsigned char numpipes : 2;
	unsigned char left_angle : 1;
	unsigned char right_angle : 1;

	unsigned char error : 1;

	char **prgm1_args;
	unsigned int prgm1_numargs;
	char **prgm2_args;
	unsigned int prgm2_numargs;
	char **prgm3_args;
	unsigned int prgm3_numargs;

	char *infile;
	char *outfile;

	unsigned int infile_index;
	unsigned int outfile_index;

	unsigned int pipe1_index;
	unsigned int pipe2_index;

	unsigned int left_angle_index;
	unsigned int right_angle_index;

} PipeData;

int execute_command(char **, int);
int is_builtin(const char *);
void execute_nopipe(char *path, char **args);
void pwd();
void cd(int numargs, char **cmdtok);
PipeData *pipe_parse_commands(char **argv, int argc);
char *searchPATH(char *cmd);
void dispStringArr(char **argv);

void finish();

void null_check(void*);

#endif
