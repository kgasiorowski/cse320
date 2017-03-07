#include <criterion/criterion.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sfmm.h"
#include "sfmm_utils.h"

Test(Ciel_test_suite, case1){

	cr_assert_eq(roundup(4.5), 5);


}

Test(Ciel_test_suite, case2){

	cr_assert_eq(roundup(4), 4);

}

Test(Ciel_test_suite, case3){

	cr_assert_eq(roundup(4.2), 5);

}

Test(Ciel_test_suite, case4){

	cr_assert_eq(roundup(.1), 1);

}

Test(Ciel_test_suite, case5){

	cr_assert_eq(roundup(0), 0);

}