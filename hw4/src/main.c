#include "sfish.h"
#include "debug.h"

/*
 * As in previous hws the main function must be in its own file!
 */

int main(int argc, char const *argv[], char* envp[]){

    /* DO NOT MODIFY THIS. If you do you will get a ZERO. */
    rl_catch_signals = 0;
    /* This is disable readline's default signal handlers, since you are going to install your own.*/
    const char *cmd;
    while((cmd = readline("sfish> ")) != NULL) {

        //Make space for up to 20 command line arguments
        char **cmdtok = (char**)malloc(sizeof(char*)*20);

        if(cmdtok == NULL){

            error("%s", "Out of memory!\n");
            exit(EXIT_FAILURE);

        }

        int tokcounter = 0;
        const char *delim = " -";

        cmdtok[tokcounter] = strtok((char*)cmd, delim);

        while(cmdtok[tokcounter] != NULL){

            debug("Token %d: %s\n", tokcounter, cmdtok[tokcounter]);
            cmdtok[++tokcounter] = strtok(NULL, delim);

        }

        debug("%s\n",cmd);
        /* All your debug print statements should use the macros found in debu.h */
        /* Use the `make debug` target in the makefile to run with these enabled. */


        debug("Length of command entered: %ld\n", strlen(cmd));
        debug("Number of arguments passed: %d\n", tokcounter-1);
        /* You WILL lose points if your shell prints out garbage values. */

        execute_command(cmdtok);

    }

    printf("logout\n");

    /* Don't forget to free allocated memory, and close file descriptors. */
    free((void*)cmd);

    return EXIT_SUCCESS;
}
