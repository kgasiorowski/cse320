#include "helpers.h"
#include "const.h"
#include "hw1.h"
#include "debug.h"

//Calculates string length
int strleng(const char *str){

	register int count = 0; 	//Counter

	while(*str++)
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

	int index = 0;

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

//Copies a string into another.
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
char *findStringInArray(const char c, char **array){

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

	//debug("String copied into buffer: %s\n", buffer);

	char c = *buffer;
	//debug("Scanned first char: %c\n", c);

	if(isLower(c)){

		// debug("String is lowercase\n");
		// debug("first char: %c\n", *buffer);

		*buffer = c - ('a' - 'A');

		// debug("new first char: %c\n", *buffer);

	}else;
		// debug("String is not lowercase\n");



	return buffer;

}

//Compares two characters ignoring case
int cequals(const char c1, const char c2){

	return toLower(c1) == toLower(c2);

}

void appendString(char *dest, const char *orig){

	//Iterate through the original string first
	while(*dest++);

	//Go back to null terminator
	dest--;

	//Copy it at the end
	strcopy(orig, dest);

}

void appendChar(char *dest, const char c){

	while(*dest++);

	*(dest-1) = c;
	*dest = '\0';

}

//Clears a string
void clearString(char *str){

	*str = '\0';

}

char *shiftStringRight(char *str){

	//First, save intitial pointer
	char *saved_pointer = str;

	while(*++str); //Go to the end of the string

	str--; //Str points at last char now

	char endChar = *str; //Save last char

	do{

		//Swap chars
		*str = *(str-1);

	}while(--str != saved_pointer);

	*str = endChar;

	return str;

}

char *shiftStringLeft(char *str){

	char *saved_pointer = str;
	char saved_char = *str;

	do

		*str = *(str+1);

	while(*(++str+1));

	*str = saved_char;

	return saved_pointer;

}

int isNumeric(char c){

	return c <= '9' && c >='0';

}

int stringToInt(char *str){

	int total = 0;
	int counter = 0;
	int length = strleng(str);
	double power10 = 1;

	while(*(++str));
	str--;

	for(; counter < length; counter++){

		if(!isNumeric(*str))
			return 0;

		total += (*(str--) - '0') * power10;
		power10 *= 10;

	}

	return total;

}

void clearbuffer(){

	int i = 0;
	while(i < 10)
		*(buffer+(i++)) = '\0';

}

int compareStringToArray(const char* str, const char* const* arr){

	int counter = 0;

	while(*(arr+counter++)){

		if(streq(str, *arr))
			return counter;

	}

	return -1;

}

int streqIgnoreCase(const char *str1, const char *str2){

	while(*str1 && *str2)
		if(!cequals(*str1++, *str2++))
			return 0;

	return 1;

}

int strcapital(const char *str){

	return isUpper(*str);

}