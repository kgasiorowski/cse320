#include "hw1.h"





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

	//Compare each character
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