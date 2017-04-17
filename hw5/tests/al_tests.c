#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include "arraylist.h"
#include "debug.h"
#include <errno.h>
#include "const.h"

// Test(al_suite, 3_removal, .timeout=2, .init=setup, .fini=teardown){



// }

/******************************************
 *                  ITEMS                 *
 ******************************************/
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
        warn("%s","Item was NULL\n");

}

/**************************************/
typedef struct {
    char* name;
    int32_t id;
    double gpa;
}student_t;

#define NAME_LENGTH 100

student_t *gen_student(char *name){

    student_t *ret = malloc(sizeof(student_t));
    ret->name = malloc(sizeof(char) * NAME_LENGTH);
    strcpy(ret->name, name);

    return ret;

}

void student_t_free_func(void *argptr){

    student_t *stu = (student_t*)argptr;

    if(stu == NULL)
        error("%s","argptr was null!\n");
    else if(stu->name == NULL)
        warn("%s\n","argptr's name was NULL!\n");
    else
        free(stu->name);

}
/**************************************/

void setup(void) {
    printf("\n\nTest beginning: ");
}

void teardown(void) {
    info("%s","Tearing down");

}

/******************************************
 *                  TESTS                 *
 ******************************************/
Test(al_suite, 0_creation, .timeout=2, .init = setup){

    printf("Creation test 1.0\n");

    arraylist_t *locallist = new_al(sizeof(test_item_t));

    cr_assert_not_null(locallist, "List returned was NULL");
    cr_assert_not_null(locallist->base, "Arraylist base was NULL");

    cr_assert(locallist->length == 0, "Unexpected list length: %lu", locallist->length);
    cr_assert(locallist->item_size == sizeof(test_item_t), "Unexpected item size: %lu", locallist->item_size);
    cr_assert(locallist->capacity == INIT_SZ, "Unexpected capacity: %lu", locallist->capacity);

}

Test(al_suite, 1_deletion, .timeout=2, .init = setup){

    printf("Deletion test 1.0\n");

    arraylist_t *list = new_al(sizeof(int));

    int a = 12345;
    insert_al(list, &a);

    delete_al(list, NULL);

}

Test(al_suite, 1_1_deletion, .timeout=2, .init = setup){

    printf("Deletion test 1.1\n");

    arraylist_t *list = new_al(sizeof(student_t));
    student_t *stu;

    stu = gen_student("Kuba");
    insert_al(list, stu);

    stu = gen_student("Amanda");
    insert_al(list, stu);

    stu = gen_student("Maya");
    insert_al(list, stu);

    delete_al(list, student_t_free_func);


}

Test(al_suite, 2_insertion, .timeout=2, .init = setup){

    printf("Insertion test 2.0\n");

    arraylist_t *list = new_al(sizeof(int));

    cr_assert_not_null(list, "List returned was NULL");
    cr_assert(list->length == 0, "Initialized list did not have length zero");

    int *test = calloc(1, sizeof(int));
    *test = 10;

    insert_al(list, test);

    info("%s\n","Inserting completed without crashing");
    cr_assert(list->length == 1, "Insert didn't increment length");

}

Test(al_suite, 2_1_insertion, .timeout=2, .init = setup){

    printf("Insertion test 2.1\n");

    arraylist_t *list = new_al(sizeof(int));
    int ret;

    cr_assert_not_null(list, "Returned list was NULL");
    cr_assert(list->length == 0, "Initialized list did not have length zero");

    int test[] = {1,2,3,4,5};

    //

    ret = insert_al(list, &test[0]);

    cr_assert(list->length == 1, "Unexpected length: %lu\n", list->length);
    cr_assert(list->capacity == 4, "Unexpected capacity: %lu\n", list->capacity);
    cr_assert(ret == 0, "Unexpected index returned: %d\n", ret);

    //

    ret = insert_al(list, &test[1]);

    cr_assert(list->length == 2, "Unexpected length: %lu\n", list->length);
    cr_assert(list->capacity == 4, "Unexpected capacity: %lu\n", list->capacity);
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

    //delete_al(list, NULL);

}

Test(al_suite, 3_removal, .timeout=2, .init = setup){

    printf("Removal test 3.0\n");

}

Test(al_suite, 4_getdata_prim, .timeout=2, .init = setup){

    printf("Get_data test 4.0\n");

    arraylist_t *list = new_al(sizeof(int));

    int *temp1 = calloc(1, sizeof(int));
    int *temp2 = calloc(1, sizeof(int));
    int *ret = 0;
    size_t index = 0;

    *temp1 = 320;
    insert_al(list, temp1);
    *temp1 = 420;
    insert_al(list, temp1);
    *temp1 = 500;
    insert_al(list, temp1);

    *temp1 = 320;
    index = get_data_al(list, temp1);
    cr_assert(index == 0, "Unexpected return value: %lu\n", index);

    *temp1 = 420;
    index = get_data_al(list, temp1);
    cr_assert(index == 1, "Unexpected return value: %lu\n", index);

    *temp1 = 500;
    index = get_data_al(list, temp1);
    cr_assert(index == 2, "Unexpected return value: %lu\n", index);

    index = get_data_al(list, NULL);
    cr_assert(index == 0, "Unexpected return value: %lu\n", index);

    index = get_data_al(NULL, NULL);
    cr_assert(index == UINT_MAX, "Unexpected return value: %lu\n", index);

    ret = get_index_al(list, 0);
    cr_assert(*ret == 320, "Unexpected return value: %d\n", *ret);

    ret = get_index_al(list, 1);
    cr_assert(*ret == 420, "Unexpected return value: %d\n", *ret);

    ret = get_index_al(list, 2);
    cr_assert(*ret == 500, "Unexpected return value: %d\n", *ret);

    ret = get_index_al(list, -1);
    cr_assert(ret == NULL && errno == EINVAL, "Unexpected return value, should return NULL");

    ret = get_index_al(list, 3);
    cr_assert(ret == NULL && errno == EINVAL, "Unexpected return value, should return NULL");

    ret = get_index_al(list, 1000);
    cr_assert(ret == NULL && errno == EINVAL, "Unexpected return value, should return NULL");

    //delete_al(list, NULL);

    free(ret);
    free(temp1);
    free(temp2);

}
