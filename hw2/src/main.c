#include "hw2.h"

int main(int argc, char *argv[]){

    //create dictionary
    if((dict = (struct dictionary*) malloc(sizeof(struct dictionary))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }

    if((m_list = (struct misspelled_word*) malloc(sizeof(struct misspelled_word*))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }
    m_list = NULL;

    Args args;
    // Set struct default values
    args.n = 0;
    strcpy(args.dictFile, DEFAULT_DICT_FILE);
    strcpy(args.input, "stdin");
    strcpy(args.output, "stdout");
    // Make a loop index

    char line[MAX_SIZE];
    //Declare Files
    FILE* dFile = fopen(DEFAULT_DICT_FILE, "r");
    FILE* iFile = DEFAULT_INPUT;
    FILE* oFile = DEFAULT_OUTPUT;

    char c = 0;

    while((c = getopt(argc, argv, "ho:i:d:A:")) != -1){

        switch(c){

            case 'h':
            
                USAGE(EXIT_SUCCESS);
            
            break;

            case 'i':
                
                strcpy(args.input, optarg);
                iFile = fopen(args.input, "r");

            break;

            case 'o':
                
                strcpy(args.output, optarg);
                oFile = fopen(args.output, "w");

            break;

            case 'd':
                
                strcpy(args.dictFile, optarg);
                dFile = fopen(args.dictFile, "r");

            break;

            case 'A':

                args.n = atoi(optarg);
                if(args.n < 0 || args.n > 5)
                    USAGE(EXIT_FAILURE);

            break;

            case '?':
            
                USAGE(EXIT_FAILURE);
            
            break;

        }


    }

    if(iFile == NULL)
    {

        debug("Unable to open: %s.\n", args.input);
        return EXIT_FAILURE;

    }
    else
    {

        debug("Opened input file: %s\n", args.input);

    }

    if(oFile == NULL){

        debug("Unable to open/create: %s.\n", args.output);

    }
    else
    {

        debug("Created/opened output file: %s\n", args.output);

    }

    if(dFile == NULL)
    {

        debug("Unable to open: %s.\n", args.dictFile);
        return EXIT_FAILURE;

    }
    else
    {

        debug("Opened dictionary file: %s\n", args.dictFile);
        debug("%s", "Processing dictionary file...\n");
        processDictionary(dFile);

    }

    return 0;

    strcpy(line,"\n--------INPUT FILE WORDS--------\n");
    fwrite(line, strlen(line)+1, 1, oFile);

    while(!feof(iFile))
    {

        char word[MAX_SIZE];
        char* wdPtr = word;
        //char line[MAX_SIZE];
        char* character = line;

        fgets(line, MAX_SIZE+1, iFile);

        //if there isn't a space or newline at the end of the line, put one there
        if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n'))
            strcat(line, " ");
        //replaces spaces within a line with new lines
        while(character != NULL)
        {

            if(*character == ' ' || *character == '\n')
            {
                /*char* punct = wdPtr-1;
                    printf("char:%c",punct);
                while(!((*punct>='a' && *punct<='z') || (*punct>='A' && *punct<='Z')))
                {
                    punct--;
                }
                punct++;
                printf("%d", strlen(wdPtr)-strlen(punct));
                */

                wdPtr = NULL;
                wdPtr = word;

                processWord(wdPtr, args.n);

                strcat(wdPtr, " ");
                fwrite(wdPtr, strlen(wdPtr)+1, 1, oFile);

            }
            else
            {

                *(wdPtr++) = *character;

            }

            character++;

        }

        if(iFile == stdin)
            break;
    }

    strcpy(line, "\n--------DICTIONARY WORDS--------\n");
    fwrite(line, strlen(line)+1, 1, oFile);
    printWords(dict->word_list , oFile);

    //printf("\n--------FREED WORDS--------\n");
    freeWords(dict->word_list);
    //free dictionary
    free(dict);
    //free m_list
    free(m_list);

    fclose(dFile);
    fclose(iFile);
    fclose(oFile);

    return EXIT_SUCCESS;
}
