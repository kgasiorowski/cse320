#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

Test(CapitalizeStringTest_Suite, capitalize_string){

	char *test = "boop";
	capitalizestring(test);
	cr_assert(streq(test, "Boop"), "capitalizestring did not capitalize string, returned %s, expected %s", test, "Boop");

}