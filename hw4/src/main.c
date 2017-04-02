#include "sfish.h"
#include "debug.h"

/*
 * As in previous hws the main function must be in its own file!
 */

int main(int argc, char const *argv[], char* envp[]){

    /* DO NOT MODIFY THIS. If you do you will get a ZERO. */
    rl_catch_signals = 0;
    /* This is disable readline's default signal handlers, since you are going to install your own.*/

    setup_signals();

    const char *cmd;

    char *command_prompt = (char*)malloc(sizeof(char)*PATH_BUFFER_SIZE);
    null_check(command_prompt);

    //Make space for up to 20 command line arguments
    char **cmdtok = (char**)malloc(sizeof(char*)*20);
    null_check(cmdtok);

    strcpy(command_prompt, "<kgasiorowski> : <");
    strcat(command_prompt, getcwd(NULL, 0));
    strcat(command_prompt, "> $ ");

    int finished = 0;
    while(!finished) {

        cmd = readline(command_prompt);

        int tokcounter = 0;
        const char *delim = " ";

        cmdtok[tokcounter] = strtok((char*)cmd, delim);

        while(cmdtok[tokcounter] != NULL){

            debug("Token %d: <%s> Length: %lu\n", tokcounter, cmdtok[tokcounter], strlen(cmdtok[tokcounter]));
            cmdtok[++tokcounter] = strtok(NULL, delim);

        }

        if(!execute_command(cmdtok, tokcounter))
            finished = 1;

        strcpy(command_prompt, "<kgasiorowski> : <");
        strcat(command_prompt, getcwd(NULL, 0));
        strcat(command_prompt, "> $ ");

        free((void*)cmd);

    }

    /* Don't forget to free allocated memory, and close file descriptors. */
    free(command_prompt);
    free(cmdtok);

    return EXIT_SUCCESS;
}
