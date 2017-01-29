#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

Test(CapitalizeStringTest_Suite, capitalize_string){

	char *test = "boop";
	test = capitalizestring(test);
	cr_assert(streq(test, "Boop"), "capitalizestring did not capitalize string, returned %s, expected %s", test, "Boop");

}

Test(CapitalizeStringTest_Suite, cpitalize_string_null){

	char *test = NULL;
	test = capitalizestring(test);
	cr_assert_eq(test, NULL, "Empty string didn't return NULL. Returned: %s", test);

}

Test(CapitalizeStringTest_Suite, cpitalize_string_empty){

	char *test = "";
	test = capitalizestring(test);
	cr_assert_eq(test, "", "Empty string didn't return NULL. Returned: %s", test);

}

Test(CapitalizeStringTest_Suite, capitalize_string_capital){

	char *test = "Boop";
	test = capitalizestring(test);
	cr_assert(streq(test, "Boop"), "capitalizestring reacted strangely to already capital string: returned %s, expected %s", test, "Boop");

}