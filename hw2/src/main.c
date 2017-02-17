#include "hw2.h"

int main(int argc, char *argv[]){    

    /*if((m_list = (struct misspelled_word*) malloc(sizeof(struct misspelled_word*))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }*/



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

                args.n = stringtoint(optarg);
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

    debug("%s", "Finished processing dictionary\n");
    debug("Number of words in dictionary: %d\n", dict.num_words);

    testPrintDictionaryWords();
    testPrintMisspelledWords();

    // return 0;

    fprintf(oFile, "\n--------INPUT FILE WORDS--------\n");

    while(!feof(iFile))
    {

        char word[MAX_SIZE];
        char* wdPtr = word;
        //char line[MAX_SIZE];
        char* character = line;

        //Initialize char arrays to zero
        memset(line, 0, sizeof(char)*MAX_SIZE);
        memset(word, 0, sizeof(char)*MAX_SIZE);

        fgets(line, MAX_SIZE+1, iFile);

        debug("Current content in line: <%s>\n", line);

        //if there isn't a space or newline at the end of the line, put one there
        if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n'))
            strcat(line, " ");

        debug("Current content in line: <%s>\n", line);

        //replaces spaces within a line with new lines
        while(*character != 0)
        {

            debug("Current char: %c\n", *character);
            debug("Current contents in word: %s\n", word);

            if(*character == ' ' || *character == '\n')
            {
                
                debug("%s", "Entered if\n");

                char* punct = wdPtr-1;
                printf("char:%c", *punct);
                
                while(!((*punct>='a' && *punct<='z') || (*punct>='A' && *punct<='Z')))
                {
                    punct--;
                }
                
                punct++;
                printf(" %d\n", (int)(strlen(wdPtr)-strlen(punct)));
                
                wdPtr = word;

                debug("wdPtr points at: %s\n", wdPtr);

                //Don't process the word for now
                //processWord(wdPtr, args.n);

                strcat(wdPtr, " ");
                fwrite(wdPtr, strlen(wdPtr)+1, 1, oFile);

                memset(word, 0, sizeof(char)*MAX_SIZE);

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

    fprintf(oFile, "\n--------DICTIONARY WORDS--------\n");
    //fwrite(line, strlen(line)+1, 1, oFile);

    debug("Current dictionary head: %s\n", dict.word_list->word);

    testPrintDictionaryWords();

    printWords(dict.word_list , oFile);

    //printf("\n--------FREED WORDS--------\n");
    freeWords(dict.word_list);
    //free dictionary
    //free(dict);
    //free m_list
    free(m_list);

    fclose(dFile);
    fclose(iFile);
    fclose(oFile);

    return EXIT_SUCCESS;
}
