#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

Test(StringToInt_Suite, case1)
{

	char *str = "1234";
	int rtn = stringToInt(str);

	cr_assert_eq(rtn, 1234, "String to int did not convert correctly. Returned: %d, expected %d\n", rtn, 1234);



}

Test(StringToInt_Suite, case2)
{

	char *str = "123a";
	int rtn = stringToInt(str);

	cr_assert_eq(rtn, 0, "String to int did not convert correctly. Returned: %d, expected %d\n", rtn, 0);



}