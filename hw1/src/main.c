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

    //char *programName = *argv;
    char *inputName = *(argv+3);
    char *outputName = *(argv+4);

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

    		//Encryption
	    	const int n = mode & NVAL;
	    	substitutionCipher(in, out, n);

            int i = 0;
            for(; i<n; i++)
                shiftStringLeft(Alphabet);

            info("Shifted alphabet: %s\n", Alphabet);
            info("shift amount: %d\n", n);

            if(streq(inputName, "-"))
                info("input file: %s\n", "STDIN");
            else
                info("input file: %s\n", inputName);

            if(streq(outputName, "-"))
                info("output file: %s\n", "STDOUT");
            else
                info("output file: %s\n", outputName);

            info("operation: encryption\n");

	    }else{

	    	//Decryption
	    	const int n = (mode & NVAL);
	    	substitutionCipher(in, out, -n);

            int i = 0;
            for(; i<n; i++)
                shiftStringRight(Alphabet);

            info("shift amount: %d\n", n);
            info("Shifted alphabet: %s\n", Alphabet);

            if(streq(inputName, "-"))
                info("input file: %s\n", "STDIN");
            else
                info("input file: %s\n", inputName);

            if(streq(outputName, "-"))
                info("output file: %s\n", "STDOUT");
            else
                info("output file: %s\n", outputName);

            info("operation: decryption\n");

	    }

    }else{

        if(!(mode & ENCR)){
            //Encryption
            tutneseEncrypt(in, out);

        }else{
            //Decryption
            if(!tutneseDecrypt(in, out))
                debug("Decryption failed.\n");

        }


    }

    fclose(in);
    fclose(out);

    return EXIT_SUCCESS;
}
