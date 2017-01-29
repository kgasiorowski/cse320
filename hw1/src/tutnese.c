#include "tutnese.h"
#include "const.h"
#include "helpers.h"

const char *vowels = "aeiouAEIOU";
const char *consonants = "bcdfghjklmnpqrstvwyxzBCDFGHJKLMNPQRSTVWXYZ";

const char *double_lower = "squa";
const char *double_upper = "Squa";

const char *double_vowel_lower = "squat";
const char *double_vowel_upper = "Squat";

//Vowels are written as-is
void tutneseEncrypt(FILE *in, FILE *out){

	char c1 = '\0', c2 = '\0';
	int finished = 0;

	while(!finished){

		c1 = fgetc(in);
		if(c1 == EOF)
			return;

		//Check the next char
		c2 = fgetc(in);
		if(c1 != c2)
			//If it's not the same, put it back
			ungetc(c2, in);
		else{
			//Double character


		}

		//Single character
		if(strcontains(consonants, c1)){
			//It's a consonant. Write some fun stuff to output
			char *word = findStringInArray(c1, Tutnese);

			if(isUpper(c1)){

				//First capitalize word, then put it into out
				capitalizestring(word);
				fputs(word, out);

			}else{

				//Put word up as-is
				fputs(word, out);

			}

		}else
			//It's a symbol or vowel
			fputc(c1, out);

	}

}


void tutneseDecrypt(FILE *in, FILE *out){

	char c = '\0';

	while((c = fgetc(in)) != EOF){



	}

}