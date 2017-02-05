#include "hw1.h"
#include "debug.h"

const char *vowels = "aeiouAEIOU";
const char *consonants = "bcdfghjklmnpqrstvwyxzBCDFGHJKLMNPQRSTVWXYZ";

const char *double_cons_lower = "squa";
const char *double_cons_upper = "Squa";

const char *double_vowel_lower = "squat";
const char *double_vowel_upper = "Squat";

//Does nothing. Used to avoid unused variable errors
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

				debug("Both chars consonants <%c> <%c>\n", c1, c2);

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
					debug("%s copied to buffer\n", buffer);

					char *temp = findStringInArray(c1, Tutnese);

					appendString(buffer, temp);

					debug("Final string: %s\n", buffer);

					fputs(buffer, out);

				}

			}else if(strcontains(vowels, c1)){

				debug("Both chars are vowels <%c> <%c>\n", c1, c2);

				//ftwo cases we gotta worry about
				if(isUpper(c1)){

					debug("First is upper: <%c>\n", c1);

					strcopy(double_vowel_upper, buffer);

				}else if(isLower(c1)){

					debug("First is lower: <%c>\n", c1);

					strcopy(double_vowel_lower, buffer);

				}

				debug("Copied into buffer: %s\n", buffer);

				appendChar(buffer, c2);

				debug("Final buffer: %s\n", buffer);

				fputs(buffer, out);

			}else{

				// Just print them out normally for now
				fputc(c1, out);
				fputc(c2, out);

			}

		}



	}

}


int tutneseDecrypt(FILE *in, FILE *out){

	char c = '\0';

	while((c = fgetc(in)) != EOF){

		debug("\n");
		debug("Scanned char: %c\n", c);

		clearbuffer();

		debug("Buffer cleared\n");

		if(!strcontains(consonants, c))

			//It's a symbol or vowel
			fputc(c, out);

		else{

			const char *syllable;

			appendChar(buffer, c);
			debug("Appended char %c to buffer: %s\n", c, buffer);

			if((syllable = findStringInArray(toLower(c), Tutnese)) == NULL)
				//No syllable can be found. Just print the char to out.
				fputc(c,out);

			debug("Found syllable: %s\n", syllable);

			int syllLength = strleng(syllable);

			debug("Length of syllable found: %d\n", syllLength);

			char temp;
			int i = 0;
			for(; i < syllLength-1; i++){

				if((temp = fgetc(in))== EOF)
					return 0;

				appendChar(buffer, temp);

			}

			debug("Scanned %d-1=%d chars into buffer: %s\n", syllLength, syllLength-1, buffer);

			if(streqIgnoreCase(buffer, syllable))
			{

				debug("Match found. Putting char %c into out file\n", c);
				fputc(c, out);


			}else if(cequals(*double_cons_lower, c)){

				debug("Match not found. Checking against double char strings...\n");
				debug("Scanning an additional %d chars...\n", strleng(double_cons_lower) - syllLength);

				//1 if it's upper. 0 if lower


				int i = 0;
				//Calculate how many additional characters to scan
				for(; i < strleng(double_cons_lower) - syllLength; i++){

					if((temp = fgetc(in))== EOF)
						return 0;

					appendChar(buffer, temp);

				}

				debug("Buffer: %s\n", buffer);

				if(streqIgnoreCase(buffer, double_cons_lower)){

					int consCase = isUpper(*buffer);

					debug("String match to double consonant: %s\n", buffer);
					debug("Clearing buffer...\n");
					clearbuffer();

					// Read next char.
					char ctemp = fgetc(in);

					// See if there's a syllable that matches it
					char *syllable;

					if((syllable = findStringInArray(toLower(ctemp), Tutnese)) == NULL){

						// If there isn't, just print the char.
						debug("Syllable for char %c not found. Printing...\n", ctemp);
						fputc(ctemp, out);

					}else{

						// if there is, read that many chars, and compare
						debug("Syllable found for char %c: %s\n", ctemp, syllable);

						appendChar(buffer, ctemp);

						debug("Scanning %d more chars\n", strleng(syllable)-1);
						debug("Buffer: %s\n", buffer);

						char temp;
						int i = 0;
						for(; i < strleng(syllable)-1; i++)
						{

							if((temp = fgetc(in)) == EOF)
								return 0;

							appendChar(buffer, temp);

							debug("Buffer: %s\n", buffer);

						}

						if(streqIgnoreCase(buffer, syllable)){

							debug("Syllable matched with buffer.\n");

							fputc(consCase?toUpper(ctemp):toLower(ctemp), out);
							fputc(*buffer, out);

							debug("Printed chars: %c %c\n", c, *buffer);

						}else{
							debug("Match not found. ERROR!\n");
							return 0;
						}

					}

				}else if(streqIgnoreCase(buffer, double_vowel_lower)){



				}else
					return 0;

			}else{

				//The syllable doesn't match what is found. Return fail
				debug("No match found. Returning failure\n");
				return 0;

			}

		}

	}

	return 1;

}