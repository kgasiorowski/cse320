#include "hw1.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;

    /* Note: create a variable to assign the result of validargs */
    int mode = validargs(argc, argv, &in, &out);

    if(mode & 0x80) //Help bit was set
    	USAGE(EXIT_SUCCESS);

    if(mode == 0)
    	USAGE(EXIT_FAILURE);

    return EXIT_SUCCESS;
}
