#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include "arraylist.h"
#include "debug.h"

// Test(al_suite, 3_removal, .timeout=2, .init=setup, .fini=teardown){



// }

/******************************************
 *                  ITEMS                 *
 ******************************************/
arraylist_t *global_list;

typedef struct {
    char* name;
    int32_t id;
    double gpa;
}student_t;

typedef struct{
    int i;
    float f;
    long double ld;
    char c1:4;
    char c2:4;
    short s;
    void *some_data;
}test_item_t;

/******************************************
 *              HELPER FUNCS              *
 ******************************************/
void test_item_t_free_func(void *argptr){
    test_item_t* ptr = (test_item_t*) argptr;
    if(!ptr)
        free(ptr->some_data);
    else
        cr_log_warn("Pointer was NULL\n");
}

void setup(void) {
    cr_log_warn("Setting up test");
    global_list = new_al(sizeof(test_item_t));
}

void teardown(void) {
    cr_log_warn("Tearing down");
    delete_al(global_list, test_item_t_free_func);
}

/******************************************
 *                  TESTS                 *
 ******************************************/
Test(al_suite, 0_creation, .timeout=2){
    arraylist_t *locallist = new_al(sizeof(test_item_t));

    cr_assert_not_null(locallist, "List returned was NULL");
}

Test(al_suite, 1_deletion, .timeout=2){
    arraylist_t *locallist = new_al(sizeof(test_item_t));

    cr_assert_not_null(locallist, "List returned was NULL");

    delete_al(locallist, test_item_t_free_func);

    cr_log_warn("Delete completed without crashing");
}

Test(al_suite, 2_insertion, .timeout=2){

    arraylist_t *list = new_al(sizeof(int));

    cr_assert_not_null(list, "List returned was NULL");
    cr_assert(list->length == 0, "Initialized list did not have length zero");

    int *test = calloc(1, sizeof(int));
    *test = 10;

    insert_al(list, test);

    cr_log_warn("Inserting completed without crashing");
    cr_assert(list->length == 1, "Insert didn't increment length");

}

Test(al_suite, 2_1_insertion, .timeout=2){

    arraylist_t *list = new_al(sizeof(int));
    int ret;

    cr_assert_not_null(list, "Returned list was NULL");
    cr_assert(list->length == 0, "Initialized list did not have length zero");

    int test[] = {1,2,3,4,5};

    //

    ret = insert_al(list, &test[0]);

    cr_assert(list->length == 1, "Unexpected length: %lu\n", list->length);
    cr_assert(list->capacity == 2, "Unexpected capacity: %lu\n", list->capacity);
    cr_assert(ret == 0, "Unexpected index returned: %d\n", ret);

    //

    ret = insert_al(list, &test[1]);

    cr_assert(list->length == 2, "Unexpected length: %lu\n", list->length);
    cr_assert(list->capacity == 2, "Unexpected capacity: %lu\n", list->capacity);
    cr_assert(ret == 1, "Unexpected index returned: %d\n", ret);

    //

    ret = insert_al(list, &test[2]);

    cr_assert(list->length == 3, "Unexpected length: %lu\n", list->length);
    cr_assert(list->capacity == 4, "Unexpected capacity: %lu\n", list->capacity);
    cr_assert(ret == 2, "Unexpected index returned: %d\n", ret);

    //

    ret = insert_al(list, &test[3]);

    cr_assert(list->length == 4, "Unexpected length: %lu\n", list->length);
    cr_assert(list->capacity == 4, "Unexpected capacity: %lu\n", list->capacity);
    cr_assert(ret == 3, "Unexpected index returned: %d\n", ret);

    //

    ret = insert_al(list, &test[4]);

    cr_assert(list->length == 5, "Unexpected length: %lu\n", list->length);
    cr_assert(list->capacity == 8, "Unexpected capacity: %lu\n", list->capacity);
    cr_assert(ret == 4, "Unexpected index returned: %d\n", ret);

}

Test(al_suite, 3_removal, .timeout=2){



}

Test(al_suite, 4_getdata, .timeout=2){

    arraylist_t *list = new_al(sizeof(int));

    int *temp1 = calloc(1, sizeof(int));
    int *temp2 = calloc(1, sizeof(int));
    *temp1 = 320;
    *temp2 = 321;

    insert_al(list, temp1);
    *temp1 = 420;
    insert_al(list, temp1);
    *temp1 = 500;
    insert_al(list, temp1);

    *temp1 = 320;
    int *ret = get_data_al(list, temp1);
    cr_assert(*ret == 320, "Unexpected return value: %d\n", *ret);

    *temp1 = 500;
    ret = get_data_al(list, temp1);
    cr_assert(*ret == 500, "Unexpected return value: %d\n", *ret);

    *temp1 = 420;
    ret = get_data_al(list, temp1);
    cr_assert(*ret == 420, "Unexpected return value: %d\n", *ret);


    ret = get_data_al(list, temp2);
    cr_assert(ret == NULL, "Unexpected return value: %d\n", *ret);


}
