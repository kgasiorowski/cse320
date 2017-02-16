#include "hw2.h"

/* Great filename. */

void processDictionary(FILE* f){
    
    debug("%s\n", "Entered processDictionary");
    
    dict->num_words = 0;
    
    debug("%s\n", "Initialized number of words in dictionary to 0");

    while(!feof(f))
    {
        //initialize the current word.
        Dict_word* currWord;
        
        if((currWord = (Dict_word*) malloc(sizeof(Dict_word))) == NULL)
        {
            printf("OUT OF MEMORY.\n");
            return;
        }

        debug("%d\n", 1);

        currWord->num_misspellings = 0;
        currWord->misspelled_count = 0;

        debug("%d\n", 2);

        //variables
        char word[MAX_SIZE];
        char *wdPtr = word;
        memset(word, 0, sizeof(char)*MAX_SIZE); //Initialize to zero

        char line[MAX_SIZE];
        char *character = line;
        memset(word, 0, sizeof(char)*MAX_SIZE); //Initialize to zero

        //char word_list[MAX_MISSPELLED_WORDS+1][MAX_SIZE];

        int counter = 0;
        int firstWord = 1;

        fgets(line, MAX_SIZE+1, f);
        //if there isn't a space at the end of the line, put one there

        debug("Line scanned: <%s>\n", line);

        if((line[strlen(line)-2] != ' ' && line[strlen(line)-1] == '\n') || (line[strlen(line)-1] != ' ' && line[strlen(line)-1] != '\n'))
            strcat(line, " ");

        debug("Line modified: <%s>\n", line);

        while(*character != 0)
        {

            debug("Current character: %c\n", *character);
            debug("Currently stored in word: %s\n", word);

            if(counter >= MAX_MISSPELLED_WORDS+1)
                break;
            
            //if the character is a space, add the word in word_list and make word NULL.
            if(*character == ' ')
            {

                debug("%s", "Space encountered\n");
                debug("%s", "Clearing word buffer\n");

                //Clear and reset word buffer
                memset(word, 0, sizeof(char)*MAX_SIZE);
                wdPtr = word;
                
                //Word was found, increment word counter
                counter++;
                

                if(firstWord)
                {

                    debug("%s", "First word encountered\n");

                    debug("%s","Adding word...\n");
                    addWord(currWord, wdPtr);
                    debug("%s", "Added word\n");

                    dict->num_words++;

                    firstWord = 0;

                }
                else
                {
                    
                    Misspelled_word* currMisspelling;
                    
                    debug("%s", "Not first word. Adding misspelling...\n");

                    if((currMisspelling = malloc(sizeof(Misspelled_word))) == NULL)
                    {

                        printf("ERROR: OUT OF MEMORY.");
                        return;
                    
                    }

                    addMisspelledWord(currMisspelling, currWord, wdPtr);
                    debug("%s", "Added misspelling\n");

                }

            }
            //if the character isn't a space or a new line, add the character to word.
            else if(*character != '\n')
                *(wdPtr++) = *character;
            
            character++;
        }
    }
}

void addWord(Dict_word* dWord, char* word){
    
    //setting up dWord fields
    dWord->misspelled_count = 0;
    
    dWord->num_misspellings = 0;
    
    dWord->next = dict->word_list;
    
    strcpy(dWord->word, word);
    
    dict->word_list = dWord;
}

void addMisspelledWord(Misspelled_word* misspelledWord, Dict_word* correctWord, char* word){
    

                            //setting up misspelledWord fields
    strcpy(misspelledWord->word, word);

    misspelledWord->misspelled = 0;

    misspelledWord->correct_word = correctWord;

    misspelledWord->next = m_list;

    (correctWord->misspelled)[++correctWord->num_misspellings] = misspelledWord;

    m_list = misspelledWord;

}

void freeWords(Dict_word* currWord){
    if(currWord != NULL)
    {
        freeWords(currWord);

        //int i;
        //free word
        debug("FREED %s\n", currWord->word);
        free(currWord);
    }
}

void printWords(Dict_word* currWord, FILE* f){
    if(currWord != NULL)
    {
        printWords(currWord->next, f);

        char line[MAX_SIZE];
        int i;

        sprintf(line, "%s\n", currWord->word);
        fwrite(line, strlen(line)+1, 1, f);

        sprintf(line, "\tNUMBER OF TIMES WORD IS MISSPELLED: %d\n", currWord->misspelled_count); // puts string into buffer
        fwrite(line, strlen(line)+1, 1, f);

        sprintf(line, "\tNUMBER OF MISSPELLINGS: %d\n", currWord->num_misspellings);
        fwrite(line, strlen(line)+1, 1, f);

        for(i = 0; i<currWord->num_misspellings; i++)
        {
            sprintf(line, "\tMISPELLED WORD #%d: %s\n", i,((currWord->misspelled)[i])->word);
            fwrite(line, strlen(line)+1, 1, f);

            sprintf(line,"\t\tMISPELLED?: %d\n", ((currWord->misspelled)[i])->misspelled);
            fwrite(line, strlen(line)+1, 1, f);

            sprintf(line, "\t\tACTUAL WORD: %s\n", ((currWord->misspelled)[i])->correct_word->word);
            fwrite(line, strlen(line)+1, 1, f);

            if(((currWord->misspelled)[i])->next->word != NULL)
            {
                sprintf(line, "\t\tNEXT MISPELLED WORD: %s\n", ((currWord->misspelled)[i])->next->word);
                fwrite(line, strlen(line)+1, 1, f);
            }
        }

        if((currWord->next)->word != NULL)
        {
            sprintf(line,"\tNEXT WORD: %s\n", (currWord->next)->word);
            fwrite(line, strlen(line)+1, 1, f);
        }
    }
}

void processWord(char* inputWord, int n){
    if(foundMisspelledMatch(inputWord))
        return;
    if(foundDictMatch(inputWord))
        return;
    else
    {
        char ch;
        char conf;

        do
        {
            printf("\"%s\" was not found in the dictionary. Do you want to add it (Y/N)? ", inputWord);
            scanf("%c", &conf);
            while ((ch = getchar()) != '\n' && ch != EOF);
        }while(conf!='Y' && conf!='N');

        if(conf == 'Y')
        {
            struct dict_word* newWord;
            //int counter = 0;

            if((newWord = (struct dict_word*) malloc(sizeof(struct dict_word))) == NULL)
            {
                printf("ERROR: OUT OF MEMORY.\n");
                return;
            }

            addWord(newWord, inputWord);
            dict->word_list = newWord;
            printf("Added \"%s\" to Dictionary. Add misspellings (Y/N)? ", inputWord);

            // do
            // {
            //     scanf("%c", &conf);
            //     while ((ch = getchar()) != '\n' && ch != EOF);
            // }while(conf!='Y' && conf!='N');

            // if(conf=='Y')
            // {
                
                // do
                // {
                //     printf("How many misspellings (1-5)?");
                //     scanf("%d", &numMisspellings);
                //     while ((ch = getchar()) != '\n' && ch != EOF);

                // }while(numMisspellings<1 || numMisspellings>5);

            int numMisspellings=n;

            while(numMisspellings > 0)
            {

                char word[WORDLENGTH];
                char* wdPtr = word;
                struct misspelled_word* newMWord;

                if((newMWord = (struct misspelled_word*) malloc(sizeof(struct misspelled_word))) == NULL)
                {
                    printf("ERROR: OUT OF MEMORY.");
                    return;
                }

                // TODO remove user interactivity
                // Generate a misspelling via randomness
                printf("Enter misspelling: ");
                scanf("%s", word);

                addMisspelledWord(newMWord, newWord, wdPtr);

                printf("Misspelling added\n");

                while ((ch = getchar()) != '\n' && ch != EOF);

                numMisspellings--;

            }
            // }
        }
    }
}

bool foundMisspelledMatch(char* inputWord){
    Misspelled_word* listPtr = m_list;
    while(listPtr != NULL)
    {
        if(strcasecmp(inputWord, listPtr->word) == 0)
        {
            strcpy(inputWord, listPtr->correct_word->word);
            listPtr->misspelled = 1;
            listPtr->correct_word->misspelled_count++;
            return true;
        }
        listPtr = listPtr->next;
    }
    return false;
}

bool foundDictMatch(char* inputWord){
    struct dict_word* listPtr = dict->word_list;
    while(listPtr != NULL)
    {
        if(strcasecmp(inputWord, listPtr->word) == 0)
            return true;
        listPtr = listPtr->next;
    }
    return false;
}
