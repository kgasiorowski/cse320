#include "hw2.h"
#include "misspelling.h"

/* Great filename. */

Dictionary dict;
Misspelled_word *m_list;

//WORKS
void processDictionary(FILE* f){
    
    debug("%s\n", "Entered processDictionary");
    
    dict.num_words = 0;
    
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

        currWord->num_misspellings = 0;
        currWord->misspelled_count = 0;

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

        //debug("Line scanned: <%s>\n", line);

        if((line[strlen(line)-2] != ' ' && line[strlen(line)-1] == '\n') || (line[strlen(line)-1] != ' ' && line[strlen(line)-1] != '\n'))
            strcat(line, " ");

        //debug("Line modified: <%s>\n", line);

        while(*character != 0)
        {

            debug("Current character: %c\n", *character);
            debug("Currently stored in word: %s\n", word);

            if(counter >= MAX_MISSPELLED_WORDS+1)
                break;
            
            //if the character is a space, add the word in word_list and make word NULL.
            if(*character == ' ')
            {

                //debug("%s", "Space encountered\n");
                //debug("%s", "Clearing word buffer\n");

                //Reset word buffer
                wdPtr = word;
                
                //Word was found, increment word counter
                counter++;

                if(firstWord)
                {

                    debug("%s", "First word encountered\n");

                    debug("Adding word: %s\n", wdPtr);
                    addWord(currWord, wdPtr);
                    debug("%s", "Added word\n");

                    dict.num_words++;

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
                    debug("Added misspelling: %s\n", wdPtr);

                }

                //Clear word buffer
                memset(word, 0, sizeof(char)*MAX_SIZE);

            }
            //if the character isn't a space or a new line, add the character to word.
            else if(*character != '\n')
                *(wdPtr++) = *character;
            
            character++;
        }
    }
}

//WORKS
void addWord(Dict_word* dWord, char* word){
    
    //setting up dWord fields
    dWord->misspelled_count = 0;
    dWord->num_misspellings = 0;
    
    //dWord->next = dict.word_list;
    
    Dict_word *temp = dict.word_list;

    if(temp == NULL)
        dict.word_list = dWord;
    else{

        //Get the last node in this list
        while(temp->next != NULL)
            temp = temp->next;

        temp->next = dWord;

    }

    strcpy(dWord->word, word);

}

//WORKS
void addMisspelledWord(Misspelled_word* misspelledWord, Dict_word* correctWord, char* word){
    
    //setting up misspelledWord fields
    strcpy(misspelledWord->word, word);

    misspelledWord->misspelled = 0;

    misspelledWord->correct_word = correctWord;

    //misspelledWord->next = m_list;
    Misspelled_word *temp = m_list;
    if(temp == NULL)
        m_list = misspelledWord;
    else{

        while(temp->next != NULL)
            temp = temp->next;

        temp -> next = misspelledWord;

    }

    (correctWord->misspelled)[correctWord->num_misspellings++] = misspelledWord;

}

//WORKS
void freeWords(Dict_word* currWord){
    if(currWord != NULL)
    {

        debug("Test%d\n", 0);
        freeWords(currWord->next);

        //free word
        debug("FREED %s\n", currWord->word);
        free(currWord);
    }
}

//WORKS
void printWords(Dict_word* currWord, FILE* f){

    while(currWord != NULL){

        int i;

        fprintf(f, "%s\n", currWord->word);
        fprintf(f, "\tNUMBER OF TIMES WORD IS MISSPELLED: %d\n", currWord->misspelled_count);
        fprintf(f, "\tNUMBER OF MISSPELLINGS: %d\n", currWord->num_misspellings);

        //debug("Num misspellings: %d\n", currWord->num_misspellings);

        for(i = 0; i<currWord->num_misspellings; i++)
        {
            
            fprintf(f, "\tMISSPELLED WORD #%d: %s\n", i,((currWord->misspelled)[i])->word);
            fprintf(f,"\t\tMISPELLED?: %d\n", ((currWord->misspelled)[i])->misspelled);
            fprintf(f, "\t\tACTUAL WORD: %s\n", ((currWord->misspelled)[i])->correct_word->word);

            if(((currWord->misspelled)[i])->next != NULL && ((currWord->misspelled)[i])->next->word != NULL)
            {
                

                fprintf(f, "\t\tNEXT MISPELLED WORD: %s\n", ((currWord->misspelled)[i])->next->word);
                
            }



        }

        if((currWord->next)->word != NULL)
        {

            fprintf(f, "\tNEXT WORD: %s\n", (currWord->next)->word);
        }
    
        currWord = currWord->next;

    }

}

//TODO!!!!!!!!!!!!!
void processWord(char* inputWord, int n){
    if(foundMisspelledMatch(inputWord))
        return;
    if(foundDictMatch(inputWord))
        return;
    else
    {

        Dict_word* newWord;
        //int counter = 0;

        if((newWord = (struct dict_word*) malloc(sizeof(struct dict_word))) == NULL)
        {
            printf("ERROR: OUT OF MEMORY.\n");
            return;
        }

        addWord(newWord, inputWord);

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
            // TODO ADD CALL FROM MISSPELLING.H

            char **misspellingsArray= gentypos(n, inputWord);

            //TODO - create new misspelled word object for each new misspelling
            //Pass it and the other stuff into addMisspelledWord

            // Generate a misspelling via randomness
            //printf("Enter misspelling: ");

            addMisspelledWord(newMWord, newWord, wdPtr);

            debug("%s", "Misspelling added\n");

            numMisspellings--;

        }
    }
}

//WORKS ?
bool foundMisspelledMatch(char* inputWord){
    Misspelled_word* cursor = m_list;
    while(cursor != NULL)
    {
        if(strcasecmp(inputWord, cursor->word) == 0)
        {
            strcpy(inputWord, cursor->correct_word->word); //What is this line for?
            cursor->misspelled = 1;
            cursor->correct_word->misspelled_count++;
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

//WORKS ?
bool foundDictMatch(char* inputWord){
    struct dict_word* cursor = dict.word_list;
    while(cursor != NULL)
    {
        if(strcasecmp(inputWord, cursor->word) == 0)
            return true;
        cursor = cursor->next;
    }
    return false;
}

//WORKS
int stringtoint(char *str){

    int rtn = 0;
    int status = sscanf(str, "%d", &rtn);

    if(status == 0)
        return -1;
    else
        return rtn;


}

//WORKS
void testPrintDictionaryWords(){

    debug("%s", "Entered test print dictionary words\n");

    Dict_word *cursor = dict.word_list;

    while(cursor != NULL)
    {

        fprintf(stderr, "Current dictionary word: %s\n", cursor->word);
        cursor = cursor->next;

    }

}

//WORKS
void testPrintMisspelledWords(){

    debug("%s", "Entered test print misspelled words\n");

    Misspelled_word *cursor = m_list;

    while(cursor != NULL){

        fprintf(stderr, "Current misspelled word: %s\n", cursor->word);
        cursor = cursor->next;

    }

}