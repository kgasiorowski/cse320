#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

char *test[] = {
  "bub", "cash", "dud",
  "fud", "gug", "hash",
  "jay", "kuck", "lul", "mum", "nun",
  "pub", "quack", "rug", "sus", "tut",
  "vuv", "wack", "xux", "yuck", "zug", NULL
};

Test(FindStringTest_Suite, get_string_first){

	const char testChar = 'b';
	const char *testString = findStringInArray(testChar, test);

	cr_assert(streq(testString, "bub"), "FindStringInArray could not find the correct string, found: %s, expected %s", testString, "bub");

}

Test(FindStringTest_Suite, get_string_last){

	const char testChar = 'z';
	const char *testString = findStringInArray(testChar, test);

	cr_assert(streq(testString, "zug"), "FindStringInArray could not find the correct string, found: %s, expected %s", testString, "zug");

}

Test(FindStringTest_Suite, get_string_mid){

	const char testChar = 'l';
	const char *testString = findStringInArray(testChar, test);

	cr_assert(streq(testString, "lul"), "FindStringInArray could not find the correct string, found: %s, expected %s", testString, "lul");

}

Test(FindStringTest_Suite, get_string_null){

	const char testChar = 'o';
	const char *testString = findStringInArray(testChar, test);

	cr_assert(testString == NULL, "FindStringInArray did not return null on a vowel. Returned %s", testString);

}

Test(findStringTest_Suite, get_string_symbol){

	const char testChar = '&';
	const char *testString = findStringInArray(testChar, test);

	cr_assert(testString == NULL, "FindStringInArray did not return null on a vowel. Returned %s", testString);

}