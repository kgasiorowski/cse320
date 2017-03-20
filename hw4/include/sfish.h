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

#define print_help() do { 	\
printf("HELP MENU\n");		\
}while(0)

void execute_command(char **);
int is_builtin(const char *);
char *pwd();

#endif
