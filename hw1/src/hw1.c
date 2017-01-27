#include "hw1.h"

// For your helper functions (you may add additional files also)
// DO NOT define a main function here!

int strleng(char *str){

	register int ret = 0; 	//Counter
	char *temp = str; 		//Copy the pointer, just so stuff doesn't get messed up

	while(*temp++)
		ret++;

	return ret;

}

int streq(char *str1, char *str2){

	//Compare lengths
	if(strleng(str1) != strleng(str2))
		return 0;

	//Compare each character
	while(*str1 && *str2){

		if(*str1 != *str2)
			return 0;

		str1++;
		str2++;

	}

	return 1;

}

char validargs(int argc, char** argv, FILE** in, FILE** out) {
	char ret = 0;

	//const unsigned int alphabetLength = strleng(Alphabet);

	//Temporary until a fix is found
	const unsigned int alphabetLength = 26;

	const int default_n = 320;
	int n = 0;

	if(argc <= 1 || argc > 6)
		return 0;

	char *firstArg = 	argv[1];
	char *secondArg = 	argv[2];
	char *thirdArg = 	argv[3];
	char *fourthArg = 	argv[4];
	char *fifthArg = 	argv[5];

	//Parse first argument
	if(streq(firstArg, "-h"))
		return 0x80;
	else if(streq(firstArg, "-s")){
		// Set the second msb
		ret |= 0x40;

		//printf("test1\n");

		if(argc == 6){							//Was n passed in?

			//printf("test2\n");

			if(sscanf(fifthArg, "%d", &n) == 0){//Get the inputted number
				return 0;
			}									//Exit if no int can be scanned

		}else
			n = default_n;						// Or use the default if no number was passed in

		//printf("%d : 0x%x\n", n, ret);

		int mod = n%alphabetLength;

		//printf("%d mod %d = %d\n", n, alphabetLength, mod	);

		ret |= mod;				// Set the lower 5 bits to whatever this is

		//printf("0x%x\n", ret);

	}else if(streq(firstArg, "-t")){

		//Firstly, -t and n cannot be passed at the same time
		if(argc == 6)
			return 0;

		//Ensure that the second msb is not set (clear it)
		ret &= ~0x40;

	}else
		return 0;

	//Parse second argument
	if(streq(secondArg, "-d"))
		//Set third msb
		ret |= 0x20;
	else if(streq(secondArg, "-e"))
		//Clear third msb
		ret &= ~0x20;
	else
		//If it's anything else, return failure
		return 0;

	//Parse third argument (input file)
	if(streq(thirdArg, "-")){

		*in = stdin;

	}else
		if((*in = fopen(thirdArg, "r")) == NULL)
			return 0;

	//Parse fourth argument (output file)
	if(streq(fourthArg, "-")){

		*out = stdout;

	}else
		if((*out = fopen(fourthArg, "w")) == NULL)
			return 0;

	return ret;

}