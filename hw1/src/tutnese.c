#include "hw1.h"
#include "debug.h"

const char *vowels = "aeiouAEIOU";
const char *consonants = "bcdfghjklmnpqrstvwyxzBCDFGHJKLMNPQRSTVWXYZ";

const char *double_cons_lower = "squa";
const char *double_cons_upper = "Squa";

const char *double_vowel_lower = "squat";
const char *double_vowel_upper = "Squat";

//Does nothing. Used to avoid unused variables
void dummy(int i){
	return;
}

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
			//Double characters encountered
			debug("Double characters encountered: <%c><%c>\n", c1, c2);

			//Clear our buffer
			clearString(buffer);

			debug("Buffer cleared\n");

			if(strcontains(consonants, c1)){

				//four cases we need to worry about
				if(isUpper(c1) && isUpper(c2)){

					debug("Both chars are uppercase <%c>\n", c1);

					//Buffer is empty
					strcopy(double_cons_upper, buffer);
					debug("%s copied to buffer\n", double_cons_upper);

					char *temp = findStringInArray(toLower(c1), Tutnese);

					appendString(buffer, temp);
					debug("Appended %s to buffer to get: %s\n", temp, buffer);

					char c = *(buffer+4);
					dummy((int)c); //Remove "unused" error

					//Capitalize the second syllable
					*(buffer+4) -= ('a' - 'A');
					debug("Capitalized <%c> to <%c>\n", c, *(buffer+4));
					debug("Final string: %s\n", buffer);

					fputs(buffer, out);

				}else if(isUpper(c1) && isLower(c2)){

					debug("First char is uppercase <%c>, second lowercase <%c>\n", c1, c2);

					strcopy(double_cons_upper, buffer);
					debug("%s copied to buffer\n", double_cons_upper);

					char *temp = findStringInArray(toLower(c1), Tutnese);

					appendString(buffer, temp);

					debug("Final string: %s\n", buffer);

					fputs(buffer, out);

				}else if(isLower(c1) && isUpper(c2)){

					debug("First char is lowercase <%c>, second uppercase <%c>\n", c1, c2);

					//Buffer is empty
					strcopy(double_cons_lower, buffer);
					debug("%s copied to buffer\n", double_cons_upper);

					char *temp = findStringInArray(c1, Tutnese);

					appendString(buffer, temp);
					debug("Appended %s to buffer to get: %s\n", temp, buffer);

					char c = *(buffer+4);
					dummy((int)c); //Remove "unused" error

					//Capitalize the second syllable
					*(buffer+4) -= ('a' - 'A');
					debug("Capitalized <%c> to <%c>\n", c, *(buffer+4));
					debug("Final string: %s\n", buffer);

					fputs(buffer, out);

				}else if(isLower(c1) && isLower(c2)){

					debug("Both chars are lowercase <%c>\n", c1);

					strcopy(double_cons_lower, buffer);
					debug("%s copied to buffer\n", );

					char *temp = findStringInArray(c1, Tutnese);

					appendString(buffer, temp);

					debug("Final string: %s\n", buffer);

					fputs(buffer, out);

				}

			}else if(strcontains(vowels, c1)){

				//four cases we need to worry about
				if(isUpper(c1) && isUpper(c2)){

					//Print upper, with

				}else if(isUpper(c1) && isLower(c2)){



				}else if(isLower(c1) && isUpper(c2)){



				}else if(isLower(c1) && isLower(c2)){



				}

			}else{

				// Just print them out normally for now
				fputc(c1, out);
				fputc(c2, out);

			}

		}



	}

}


void tutneseDecrypt(FILE *in, FILE *out){

	char c = '\0';

	while((c = fgetc(in)) != EOF){



	}

}