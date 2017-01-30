#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

char *alphabet = "ABCDEFG$";
int alphabetLength = 8;

Test(SubstitutionTest_Suite, get_char){

	char c = getChar(3, "ABCDEFG");

	cr_assert_eq(c, 'D', "getChar returned the wrong character. %c returned, expected %c", c, 'D');

}

Test(SubstitutionTest_Suite, get_char_too_small){

	char c = getChar(-1, "ABCDEFG");
	cr_assert_eq(c, '\0', "getChar didn't return 0 on bad index. %c returned", c);

}

Test(SubstitutionTest_Suite, get_char_out_of_bounds){

	char c = getChar(7, "ABCDEFG");
	cr_assert_eq(c, '\0', "getChar didn't return 0 on bad index. %c returned", c);

}

Test(SubstitutionTest_Suite, findIndex){

	int i = findIndex('C', "ABCDEFG");
	cr_assert_eq(i, 2, "findIndex didn't get right index: %d returned, expected %d", i, 2);

}

Test(SubstitutionTest_Suite, findIndex_not_found){

	int i = findIndex('a', "ABCDEFG");
	cr_assert_eq(i, -1, "findIndex didn't return -1 on no index: %d returned", i);

}

Test(SubstitutionTest_Suite, processSubChar){

	char c = 'A';
	int shiftAmt = 4;

	processSubChar(&c, alphabet, alphabetLength, shiftAmt);

	cr_assert_eq(c, 'E', "processSubChar returned %c: expected E", c);

}

Test(SubstitutionTest_Suite, processSubChar_lowercase){

	char c = 'a';
	int shiftAmt = 4;

	processSubChar(&c, alphabet, alphabetLength, shiftAmt);

	cr_assert_eq(c, 'E', "processSubChar returned %c: expected E", c);

}

Test(SubstitutionTest_Suite, processSubChar_symbol){

	char c = '$';
	int shiftAmt = 1;

	processSubChar(&c, alphabet, alphabetLength, shiftAmt);

	cr_assert_eq(c, 'A', "processSubChar returned %c: expected A", c);

}

Test(SubstitutionTest_Suite, processSubChar_largeN){

	char c = 'F';
	int shiftAmt = 23451;

	processSubChar(&c, alphabet, alphabetLength, shiftAmt);

	cr_assert_eq(c, 'A', "processSubChar returned %c: expected A", c);

}