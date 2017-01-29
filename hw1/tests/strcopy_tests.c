#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

Test(CopyStringTest_Suite, copy_string){

	char *original = "This is a test.";
	char dest[256];

	strcopy(original, dest);

	cr_assert(streq(dest, original), "A string was not copied.");

}