#include "helpers.h"
#include "const.h"
#include "hw1.h"
#include "debug.h"

//Calculates string length
int strleng(const char *str){

	register int count = 0; 	//Counter
	const char *temp = str; 	//Copy the pointer, just so stuff doesn't get messed up

	while(*temp++)
		count++;

	return count;

}

//Returns if two strings are equivalent
int streq(const char *str1, const char *str2){

	//Compare lengths
	if(strleng(str1) != strleng(str2))
		return 0;

	//Compare each character while both are not null
	while(*str1 && *str2){

		//Compare and increment
		if(*str1++ != *str2++)
			return 0;

	}

	return 1;

}

//Find the index of a char in a string
int findIndex(const char c, const char* string)
{

	unsigned int index = 0;

	do{

		if(c == *string)
			return index;

		index++;

	}while(*string++);

	//Character not found
	return -1;

}

//Gets the char at a specific index of a string
char getChar(const int index, const char* string){

	int stringLength = strleng(string);

	if(index <= -1 || index >= stringLength)
		return '\0';

	return *(string += index);

}

//Copies a string into another
void strcopy(const char *orig, char *dest){

	while((*dest++ = *orig++));

}

//Returns true if the char is uppercase alphabetical
int isUpper(const char c){

	return c >= 'A' && c <= 'Z';

}

//Returns true if the char is lowercase alphabetical
int isLower(const char c){

	return c >= 'a' && c <= 'z';

}

//Finds and returns a string in an array of strings based on the first char
char *findStringInArray(const char c, char *array[]){

	char *string = NULL;

	while((string = *array++)){

		//printf("String scanned: %s\n", string);
		if(*string == c)
			return string;

	}

	return NULL;

}

//Returns true if str contains c
int strcontains(const char *str, const char c){

	return findIndex(c, str) != -1;

}

char toUpper(char c){

	return (c <= 'z' && c >= 'a')?(c - ('a' - 'A')):c;

}

char toLower(char c){

	return (c <= 'Z' && c >= 'A')?(c + ('a' - 'A')):c;

}

char *capitalizestring(char *str){

	if(str == NULL)
		return NULL;

	if(strleng(str) == 0)
		return str;

	strcopy(str, buffer);

	debug("String copied into buffer: %s\n", buffer);

	char c = *buffer;
	debug("Scanned first char: %c\n", c);

	if(isLower(c)){

		debug("String is lowercase\n");
		debug("first char: %c\n", *buffer);

		*buffer = c - ('a' - 'A');

		debug("new first char: %c\n", *buffer);

	}else
		debug("String is not lowercase\n");



	return buffer;

}

int cequals(const char c1, const char c2){

	const int diff = 'a' - 'A';

	if(isUpper(c1) && isUpper(c2))
		//Both are uppercase
		return c1 == c2;
	else if(isUpper(c1) && isLower(c2))
		//First is upper, second is lower
		return (c1+diff) == c2;
	else if(isLower(c1) && isUpper(c2))
		//First is lower, second is upper
		return (c1-diff) == c2;
	else if(isLower(c1) && isLower(c2))
		//Both lowercase
		return c1 == c2;
	else
		return 0;



}