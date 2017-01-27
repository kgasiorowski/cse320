#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

Test(StrEq_Suite, equal_strings)
{

	char* s1 = "Hello";
	char* s2 = "Hello";

	cr_assert_eq(streq(s1,s2), 1, "Two equal strings did not return 1 (true)");


}

Test(StrEQ_Suite, nonequal_strings_same_length)
{

	char* s1 = "Hello";
	char* s2 = "Nopee";

	cr_assert_eq(streq(s1,s2), 0, "Two nonequal strings did not return 0 (false)");

}

Test(StrEQ_Suite, nonequal_strings_diff_length){

	char* s1 = "Hello";
	char* s2 = "Hell";

	cr_assert_eq(streq(s1, s2), 0, "Two strings of different length did not return 0 (false)");

}