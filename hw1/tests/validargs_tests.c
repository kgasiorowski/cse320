#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

#define HELP 0x80
#define SUBS 0x40
#define TUTE 0x40
#define ENCR 0x20
#define DECR 0x20
#define NVAL 0x31
#define N320 0x08

Test(ValidArgs_Suite, help_menu){

    char *fake_args[] = {"program_name", "-h", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(2, fake_args, &in, &out);

    cr_assert_eq((mode & HELP), 1, "Help menu bit wasn't set, got 0x%X ", mode);
}

Test(ValidArgs_Suite, subs_decr){
    char *fake_args[] = {"program_name", "-s", "-d", "-", "-", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(5, fake_args, &in, &out);

    cr_assert_eq((mode & SUBS), 1, "Substitution bit wasn't set, got 0x%X ", mode);
    cr_assert_eq((mode & DECR), 1, "Decoding bit wasn't set, got 0x%X ", mode);
}

Test(ValidArgs_Suite, subs_decr_n){
    char *fake_args[] = {"program_name", "-s", "-d", "-", "-", "3452", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(6, fake_args, &in, &out);

    cr_assert_eq((mode & SUBS), 1, "Substitution bit wasn't set, got 0x%X ", mode);
    cr_assert_eq((mode & DECR), 1, "Decoding bit wasn't set, got 0x%X ", mode);

    int n = 3452;

    cr_assert_eq((mode & NVAL), n % 26, "n value expected to be %d was %d",
                 n % 26, (mode & NVAL));
}

Test(ValidArgs_Suite, subs_encr){
    char *fake_args[] = {"program_name", "-s", "-e", "-", "-", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(5, fake_args, &in, &out);

    cr_assert_eq((mode & SUBS), 1, "Substitution bit wasn't set, got 0x%X ", mode);
    cr_assert_eq((mode & ENCR), 0, "Encoding bit wasn't zero, got 0x%X ", mode);
}

Test(ValidArgs_Suite, subs_encr_n){
    char *fake_args[] = {"program_name", "-s", "-e", "-", "-", "3", NULL};
    FILE* in;
    FILE* out;

    char mode = validargs(6, fake_args, &in, &out);

    cr_assert_eq((mode & SUBS), 1, "Substitution bit wasn't set, got 0x%X ", mode);
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
    cr_assert_eq((mode & ENCR), 0, "Decoding bit wasn't set, got 0x%X ", mode);

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
