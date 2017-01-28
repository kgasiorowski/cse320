#include "hw1.h"

#define HELP 0x80
#define SUBS 0x40
#define TUTE 0x40
#define ENCR 0x20
#define DECR 0x20
#define NVAL 0x1F


int main(int argc, char **argv) {

    FILE* in;
    FILE* out;

    /* Note: create a variable to assign the result of validargs */
    int mode = validargs(argc, argv, &in, &out);

    if(mode & HELP) //Help bit was set
    	USAGE(EXIT_SUCCESS);

    //Mode is zero
    if(!mode)
    	USAGE(EXIT_FAILURE);

    //Determine if it's -s or -t
    if(mode & SUBS)
    {

    	//Encryption or decryption?
    	if(!(mode & ENCR)){

	    	const int n = mode & NVAL;
	    	substitutionCipher(in, out, n);

	    }else{

	    	const int n = -(mode & NVAL);
	    	substitutionCipher(in, out, n);

	    }

    }else{




    }

    fclose(in);
    fclose(out);

    return EXIT_SUCCESS;
}
