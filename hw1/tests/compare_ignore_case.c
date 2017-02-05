#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

Test(CompareIgnoreCase, case1){

	const char *str1 = "abcd", *str2 = "ABCD";
	cr_assert(streqIgnoreCase(str1, str2), "Two same strings did not show as equal. %s : %s", str1, str2);

}

Test(CompareIgnoreCase, case2){

	const char *str1 = "abcd", *str2 = "AbCd";
	cr_assert(streqIgnoreCase(str1, str2), "Two same strings did not show as equal. %s : %s", str1, str2);

}

Test(CompareIgnoreCase, case3){

	const char *str1 = "abcd", *str2 = "ABCE";
	cr_assert(!streqIgnoreCase(str1, str2), "Two diff strings did not show as unequal. %s : %s", str1, str2);

}