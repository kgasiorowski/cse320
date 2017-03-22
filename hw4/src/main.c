#include "sfish.h"
#include "debug.h"

/*
 * As in previous hws the main function must be in its own file!
 */

int main(int argc, char const *argv[], char* envp[]){

    /* DO NOT MODIFY THIS. If you do you will get a ZERO. */
    rl_catch_signals = 0;
    /* This is disable readline's default signal handlers, since you are going to install your own.*/

    init();

    const char *cmd;

    char *pwd = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);
    getcwd(pwd, PATH_BUFFER_SIZE);

    char *command_prompt = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);

    strcpy(command_prompt, "<kgasiorowski> : <");
    strcat(command_prompt, pwd);
    strcat(command_prompt, "> $");

    while((cmd = readline(command_prompt)) != NULL) {

        //Make space for up to 20 command line arguments
        char **cmdtok = (char**)malloc(sizeof(char*)*20);

        if(cmdtok == NULL){

            error("%s", "Out of memory!\n");
            exit(EXIT_FAILURE);

        }

        int tokcounter = 0;
        const char *delim = " ";

        cmdtok[tokcounter] = strtok((char*)cmd, delim);

        while(cmdtok[tokcounter] != NULL){

            debug("Token %d: %s\n", tokcounter, cmdtok[tokcounter]);
            cmdtok[++tokcounter] = strtok(NULL, delim);

        }

        debug("%s\n",cmd);
        /* All your debug print statements should use the macros found in debu.h */
        /* Use the `make debug` target in the makefile to run with these enabled. */


        debug("Length of command entered: %ld\n", strlen(cmd));
        debug("Number of arguments passed: %d\n", tokcounter);
        /* You WILL lose points if your shell prints out garbage values. */

        if(!execute_command(cmdtok, tokcounter))
            break;

        getcwd(pwd, PATH_BUFFER_SIZE);

        strcpy(command_prompt, "<kgasiorowski> : <");
        strcat(command_prompt, pwd);
        strcat(command_prompt, "> $");

    }

    /* Don't forget to free allocated memory, and close file descriptors. */
    free((void*)cmd);
    free(pwd);
    free(command_prompt);

    return EXIT_SUCCESS;
}
