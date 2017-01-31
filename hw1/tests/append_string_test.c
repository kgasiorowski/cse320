#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

Test(AppendStringTest_Suite, case1)
{

	char s1[256] = "Hello";
	char* s2 = "Hello";

	appendString(s1, s2);

	cr_assert(streq(s1, "HelloHello"), "Append string did not work correctly, returned: %s", s1);


}


Test(AppendStringTest_Suite, case2)
{

	char s1[256] = "";
	char* s2 = "Hello";

	appendString(s1, s2);

	cr_assert(streq(s1, "Hello"), "Append string did not work correctly, returned: %s", s1);

}