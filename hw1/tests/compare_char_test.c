#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"


Test(CompareCharTest_Suite, case1){

	const char c1 = 'a', c2 = 'a';
	cr_assert(cequals(c1, c2), "Two same chars did not show as equal. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case2){

	const char c1 = 'a', c2 = 'A';
	cr_assert(cequals(c1, c2), "Two same chars did not show as equal. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case3){

	const char c1 = 'A', c2 = 'a';
	cr_assert(cequals(c1, c2), "Two same chars did not show as equal. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case4){

	const char c1 = 'A', c2 = 'A';
	cr_assert(cequals(c1, c2), "Two same chars did not show as equal. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case5){

	const char c1 = 'b', c2 = 'c';
	cr_assert(!cequals(c1, c2), "Two diff chars did not show as unequal. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case6){

	const char c1 = 'b', c2 = 'C';
	cr_assert(!cequals(c1, c2), "Two diff chars did not show as unequal. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case7){

	const char c1 = 'B', c2 = 'c';
	cr_assert(!cequals(c1, c2), "Two diff chars did not show as unequal. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case8){

	const char c1 = 'B', c2 = 'C';
	cr_assert(!cequals(c1, c2), "Two diff chars did not show as unequal. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case9){

	const char c1 = '.', c2 = 'R';
	cr_assert(!cequals(c1, c2), "Symbol did not return false. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case10){

	const char c1 = 'R', c2 = '.';
	cr_assert(!cequals(c1, c2), "Symbol did not return false. %c : %c", c1, c2);

}

Test(CompareCharTest_Suite, case11){

	const char c1 = '.', c2 = '.';
	cr_assert(!cequals(c1, c2), "Symbola did not return false. %c : %c", c1, c2);

}