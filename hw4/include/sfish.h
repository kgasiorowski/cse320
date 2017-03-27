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

#define print_help() do {										\
const char* fmt = " %-10s%-40s\n";								\
printf("\nKuba Gasiorowski\nShell Implemented in CSE320\n\n");	\
printf(fmt,"Help","Displays this message");						\
printf(fmt, "cd", "Changes directory to the specified path");	\
printf(fmt, "pwd", "Prints the current working directory");		\
printf(fmt, "exit", "Exits the shell cleanly");					\
printf("\n");													\
}while(0)

int execute_command(char **, int);
int is_builtin(const char *);
void pwd();
void cd(int numargs, char **cmdtok);

void finish();

void null_check(void*);

#endif
