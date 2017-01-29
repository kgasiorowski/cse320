#include "hw1.h"

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
		if(c1 == EOF){

			finished = 1;
			continue;

		}

		info("First char read: %c\n", c1);

		//Check the next char
		c2 = fgetc(in);

		info("Second char peeked: %c\n", c2);

		if(c1 != c2){
			//If it's not the same, put it back
			ungetc(c2, in);
			info("Put second char %c back\n", c2);
		}else{
			//Double character
			info("Double characters encountered: %c\n", c1);




















		}

		//Single character
		if(strcontains(consonants, c1)){
			//It's a consonant. Write some fun stuff to output
			info("c1 is a consonant: %c\n", c1);

			char *word = findStringInArray(toLower(c1), Tutnese);

			info("Scanned word %s from char %c\n", word, c1);

			if(isUpper(c1)){

				//First capitalize word, then put it into out
				word = capitalizestring(word);

				info("Capitalized word to %s to match char %c\n", word, c1);

				fputs(word, out);

			}else{

				info("Writing word as-is");
				//Put word up as-is
				fputs(word, out);

			}

		}else{
			//It's a symbol or vowel
			info("Character %c is a symbol or vowel. Printing to out", c1);

			fputc(c1, out);


		}

	}

}


void tutneseDecrypt(FILE *in, FILE *out){

	char c = '\0';

	while((c = fgetc(in)) != EOF){



	}

}