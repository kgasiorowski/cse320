#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

#define HELP 0x80
#define SUBS 0x40
#define TUTE 0x40
#define ENCR 0x20
#define DECR 0x20
#define NVAL 0x1F
//#define N320 0x08

Test(ValidArgs_Suite, help_menu){

    char *fake_args[] = {"program_name", "-h", NULL};
    FILE* in;
    FILE* out;

    unsigned char mode = validargs(2, fake_args, &in, &out);

    cr_assert_eq((mode & HELP), HELP, "Help menu bit wasn't set, got 0x%X ", mode);
}

Test(ValidArgs_Suite, subs_decr){
    char *fake_args[] = {"program_name", "-s", "-d", "-", "-", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(5, fake_args, &in, &out);

    cr_assert_eq((mode & SUBS), SUBS, "Substitution bit wasn't set, got 0x%X ", mode);
    cr_assert_eq((mode & DECR), DECR, "Decoding bit wasn't set, got 0x%X ", mode);
}

Test(ValidArgs_Suite, subs_decr_n){
    char *fake_args[] = {"program_name", "-s", "-d", "-", "-", "3452", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(6, fake_args, &in, &out);

    cr_assert_eq((mode & SUBS), SUBS, "Substitution bit wasn't set, got 0x%X ", mode);
    cr_assert_eq((mode & DECR), DECR, "Decoding bit wasn't set, got 0x%X ", mode);

    int n = 3452;
    unsigned int alphabetLength = strleng(Alphabet);


    cr_assert_eq((mode & NVAL), n % alphabetLength, "n value expected to be %d was %d, alphabet length:%d",
                 n % alphabetLength, (mode & NVAL), alphabetLength);
}

Test(ValidArgs_Suite, subs_encr){
    char *fake_args[] = {"program_name", "-s", "-e", "-", "-", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(5, fake_args, &in, &out);

    cr_assert_eq((mode & SUBS), SUBS, "Substitution bit wasn't set, got 0x%X ", mode);
    cr_assert_eq((mode & ENCR), 0, "Encoding bit wasn't zero, got 0x%X ", mode);
}

Test(ValidArgs_Suite, subs_encr_n){
    char *fake_args[] = {"program_name", "-s", "-e", "-", "-", "3", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(6, fake_args, &in, &out);

    cr_assert_eq((mode & SUBS), SUBS, "Substitution bit wasn't set, got 0x%X ", mode);
    cr_assert_eq((mode & ENCR), 0, "Encoding bit wasn't zero, got 0x%X ", mode);

    int n = 3;

    cr_assert_eq((mode & NVAL), n % 26, "n value expected to be %d was %d",
                 n % 26, (mode & NVAL));
}

Test(ValidArgs_Suite, tuts_encr){
    char *fake_args[] = {"program_name", "-t", "-e", "-", "-", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(5, fake_args, &in, &out);

    cr_assert_eq((mode & TUTE), 0, "Tutense bit wasn't zero, got 0x%X ", mode);
    cr_assert_eq((mode & ENCR), 0, "Encoding bit wasn't zero, got 0x%X ", mode);

    cr_assert_eq((mode & NVAL), 0, "n value expected to be 0 was %d",
                 (mode & NVAL));

}

Test(ValidArgs_Suite, tuts_decr){
    char *fake_args[] = {"program_name", "-t", "-d", "-", "-", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(5, fake_args, &in, &out);

    cr_assert_eq((mode & TUTE), 0, "Tutense bit wasn't zero, got 0x%X ", mode);
    cr_assert_eq((mode & ENCR), ENCR, "Decoding bit wasn't set, got 0x%X ", mode);

    cr_assert_eq((mode & NVAL), 0, "n value expected to be 0 was %d",
                 (mode & NVAL));
}

Test(ValidArgs_Suite, bad_args){
    char *fake_args[] = {"program_name", "-c", "-f", "foobar", "fizzbuzz", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(5, fake_args, &in, &out);

    cr_assert_eq(mode, 0, "Bad args got a non-zero mode, got 0x%X ", mode);
}

Test(ValidArgs_Suite, create_files){
    char *fake_args[] = {"program_name", "-s", "-e", "testfile", "foobar", NULL};
    FILE* in;
    FILE* out;

    // Create local test file
    FILE* testfile = fopen("testfile", "w");
    fclose(testfile);

    validargs(5, fake_args, &in, &out);

    cr_assert_not_null(in, "in file was NULL");

    fclose(in);

    cr_assert_not_null(out, "out file was NULL");

    fclose(out);
    remove("testfile");
    remove("foobar");
    cr_log_warn("Deleting created file.");
}

Test(ValidArgs_Suite, too_few_args){

    char *fake_args[] = {"program_name"};
    FILE *in, *out;

    unsigned char mode = validargs(1, fake_args, &in, &out);

    cr_assert_eq(mode, 0, "Program did not fail on too few arguments");

}

Test(ValidArgs_Suite, too_many_args){

    char *fake_args[] = {"program name", "a", "a", "a", "a", "a", "a"};
    FILE *in, *out;

    unsigned char mode = validargs(7, fake_args, &in, &out);

    cr_assert_eq(mode, 0, "Program did not fail on too many arguments");

}