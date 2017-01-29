#include "substitutionCipher.h"
#include "const.h"
#include "helpers.h"

//Main function for substitution encryption/decryption
void substitutionCipher(FILE *in, FILE *out, const int n){

	//Initialize our char to zero
	char c = '\0';

	const char* alphabetCopy = Alphabet;
	const int alphabetLength = strleng(alphabetCopy);

	//printf("\n\nFound alphabet %s : length %d\n\n", alphabetCopy, alphabetLength);

	while((c = fgetc(in)) != EOF)
	{

		//Convert lower case char to upper case
		processSubChar(&c, alphabetCopy, alphabetLength, n);
		fputc(c, out);

	}

}

//Processes a character for substitution cipher
void processSubChar(char *c, const char *alphabet, int alphabetLength, int shiftAmnt){

	int index = 0;

	//Change lowercase to uppercase only
	const int diff = 'a' - 'A';
	*c = (*c >= 'a' && *c <= 'z')?(*c - diff):(*c);

	index = findIndex(*c, alphabet);
	//printf("\nFound index for %c in %s: %d\n", *c, alphabet, index);

	if(index == -1)
		return;

	index += shiftAmnt;
	//printf("Shift amount %d added = %d\n", shiftAmnt, index);

	index %= alphabetLength;
	//printf("Shifted index %% length = %d\n\n", index);

	if(index<0)
		index+=alphabetLength;

	*c = getChar(index, alphabet);

}