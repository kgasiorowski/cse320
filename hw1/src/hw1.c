#include "hw1.h"
#define DEFAULT_N 320

// For your helper functions (you may add additional files also)
// DO NOT define a main function here!

//Validates arguments
char validargs(int argc, char** argv, FILE** in, FILE** out) {
	char ret = 0;

	const unsigned int alphabetLength = strleng(Alphabet);
	int n = 0;

	//printf("Alphabet length scanned: %d\n", alphabetLength);

	if(argc <= 1 || argc > 6)
		return 0;

	char * const* arguments = argv;

	//Skip program executable name
	char *firstArg = 	*(arguments+1);
	char *secondArg = 	*(arguments+2);
	char *thirdArg = 	*(arguments+3);
	char *fourthArg = 	*(arguments+4);
	char *fifthArg = 	*(arguments+5);

	//First check if the first arg is h.
	if(streq(firstArg, "-h"))
		return 0x80;
	else if(argc != 5 && argc != 6)
		return 0;

	//Parse first argument
	if(streq(firstArg, "-s")){
		// Set the second msb
		ret |= 0x40;

		//Was n passed in?
		if(argc == 6){

			//Get the inputted number
			//if(sscanf(fifthArg, "%d", &n) == 0){
			if((n = stringToInt(fifthArg)) == 0){
				//Exit if no int can be scanned
				return 0;
			}

			if(n < 0)
				return 0;

		}else{

			// Or use the default if no number was passed in
			n = DEFAULT_N;

		}

		int mod = n%alphabetLength;

		// Set the lower 5 bits to whatever this is
		ret |= mod;

	}else if(streq(firstArg, "-t")){

		//Firstly, -t and n cannot be passed at the same time
		if(argc == 6)
			return 0;

		//Ensure that the second msb is not set (clear it)
		ret &= ~0x40;
		ret |= DEFAULT_N%alphabetLength;

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

	}else if((*in = fopen(thirdArg, "r")) == NULL)
		return 0;

	//Parse fourth argument (output file)
	if(streq(fourthArg, "-"))
		*out = stdout;
	else if((*out = fopen(fourthArg, "w")) == NULL)
		return 0;


	return ret;

}