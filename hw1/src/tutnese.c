#include "hw1.h"
#include "debug.h"

const char *vowels = "aeiouAEIOU";
const char *consonants = "bcdfghjklmnpqrstvwyxzBCDFGHJKLMNPQRSTVWXYZ";

const char *double_lower = "squa";
const char *double_upper = "Squa";

const char *double_vowel_lower = "squat";
const char *double_vowel_upper = "Squat";

//Vowels are written as-is
void tutneseEncrypt(FILE *in, FILE *out){

	char c1 = '\0', c2 = '\0';

	while(1){

		c1 = fgetc(in);

		if(c1 == EOF){
			debug("EOF read\n");
			break;
		}

		debug("First char read: <%c>\n", c1);

		//Check the next char
		c2 = fgetc(in);

		debug("Second char peeked: <%c>\n", c2);

		if(!cequals(c1, c2)){
			//If it's not the same, put it back
			ungetc(c2, in);
			debug("Put second char <%c> back\n", c2);

			//Single character
			if(strcontains(consonants, c1)){

				//It's a consonant. Write some fun stuff to output
				debug("c1 is a consonant: <%c>\n", c1);

				char *word = findStringInArray(toLower(c1), Tutnese);

				debug("Scanned word <%s> from char <%c>\n", word, c1);

				if(isUpper(c1)){

					//First capitalize word, then put it into out
					word = capitalizestring(word);
					debug("Capitalized word to <%s> to match char <%c>, written to file\n", word, c1);
					fputs(word, out);

				}else{

					//Put word up as-is
					debug("Writing word <%s> as-is\n", word);
					fputs(word, out);

				}

			}else{
				//It's a symbol or vowel
				debug("Character <%c> is a symbol or vowel. Printing to out\n", c1);
				fputc(c1, out);

			}

		}else{
			//Double character
			debug("\nDouble characters encountered: <%c> <%c>\n\n", c1, c2);

			fputc(c1, out);
			fputc(c2, out);

		}



	}

}


void tutneseDecrypt(FILE *in, FILE *out){

	char c = '\0';

	while((c = fgetc(in)) != EOF){



	}

}