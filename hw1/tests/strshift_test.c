#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

Test(StringRightShift_Test, case1)
{

	char s1[256] = "ABC";
	char *expected = "CAB";

	char *rtn = shiftStringRight(s1);

	cr_assert(streq(rtn, expected), "String rightshift didn't return what expected: <%s>, returned: <%s>" , expected, rtn);
	cr_assert(streq(rtn, s1), "String rightshift didn't modify memory as expected: <%s>, got: <%s>" , expected, s1);

}

Test(StringLeftShift_Test, left_shift1){

	char s1[256] = "ABCDE";
	char *expected = "BCDEA";

	char *rtn = shiftStringLeft(s1);

	cr_assert(streq(rtn, expected), "String leftshift didn't return what expected: <%s>, returned: <%s>" , expected, rtn);
	cr_assert(streq(rtn, s1), "String leftshift didn't modify memory as expected: <%s>, got: <%s>" , expected, s1);

}