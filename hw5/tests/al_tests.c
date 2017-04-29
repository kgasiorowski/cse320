#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include "arraylist.h"
#include "debug.h"
#include <errno.h>
#include "const.h"
#include <pthread.h>


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

static student_t *gen_student(char *name, int32_t _id, double _gpa){

    student_t *ret = malloc(sizeof(student_t));
    ret->name = malloc(sizeof(char) * NAME_LENGTH);
    strcpy(ret->name, name);

    ret->id = _id;
    ret->gpa = _gpa;

    return ret;

}

static void student_t_free_func(void *argptr){

    student_t *stu = (student_t*)argptr;

    if(stu == NULL)
        error("argptr (%p) was null!\n", SHORT_ADDR(argptr));
    else if(stu->name == NULL)
        warn("argptr's (%p) name was NULL!\n", SHORT_ADDR(argptr));
    else
        free(stu->name);

}

static void print_stu(student_t *stu){

    printf("Name: %s\nID: %d\nGPA: %f\n", stu->name, stu->id, stu->gpa);

}
/**************************************/

void setup(void) {
    //printf("\n\nTest beginning: ");
}

void teardown(void) {
    info("%s","Tearing down");

}

/******************************************
 *                  TESTS                 *
 ******************************************/
Test(al_suite, 0_creation, .timeout=2, .init = setup){

    arraylist_t *locallist = new_al(sizeof(test_item_t));

    cr_assert_not_null(locallist, "List returned was NULL");
    cr_assert_not_null(locallist->base, "Arraylist base was NULL");

    cr_assert(locallist->length == 0, "Unexpected list length: %lu", locallist->length);
    cr_assert(locallist->item_size == sizeof(test_item_t), "Unexpected item size: %lu", locallist->item_size);
    cr_assert(locallist->capacity == INIT_SZ, "Unexpected capacity: %lu", locallist->capacity);

}

Test(al_suite, 1_deletion, .timeout=2, .init = setup){

    arraylist_t *list = new_al(sizeof(int));

    int a = 12345;
    insert_al(list, &a);

    delete_al(list, NULL);

}

Test(al_suite, 1_1_deletion, .timeout=2, .init = setup){

    arraylist_t *list = new_al(sizeof(student_t));
    student_t *stu;

    stu = gen_student("Kuba", 0, 0);
    insert_al(list, stu);

    stu = gen_student("Amanda", 0, 0);
    insert_al(list, stu);

    stu = gen_student("Maya", 0, 0);
    insert_al(list, stu);

    delete_al(list, student_t_free_func);


}

Test(al_suite, 2_insertion, .timeout=2, .init = setup){

    arraylist_t *list = new_al(sizeof(int));

    cr_assert_not_null(list, "List returned was NULL");
    cr_assert(list->length == 0, "Initialized list did not have length zero");

    int *test = calloc(1, sizeof(int));
    *test = 10;

    insert_al(list, test);

    cr_assert(list->length == 1, "Insert didn't increment length");

}

Test(al_suite, 2_1_insertion, .timeout=2, .init = setup){

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

    delete_al(list, NULL);

}

Test(al_suite, 3_removal, .timeout=2, .init = setup){

    arraylist_t *list = new_al(sizeof(student_t));
    student_t *stu;

    stu = gen_student("Kuba", 0, 0);
    insert_al(list, stu);

    stu = gen_student("Amanda", 1, 1);
    insert_al(list, stu);

    stu = gen_student("Maya", 2, 2);
    insert_al(list, stu);

    student_t *ret = remove_index_al(list, 1);

    cr_assert_not_null(ret, "Returned value from remove is null");

    cr_assert(strcmp(ret->name, "Amanda") == 0, "Unexpected name returned: %s", ret->name);
    cr_assert(ret->gpa == 1, "Unexpected gpa returned: %f", ret->gpa);
    cr_assert(ret->id == 1, "Unexpected id returned: %d", ret->id);

    ret = get_index_al(list, 1);

    cr_assert(strcmp(ret->name, "Maya") == 0, "Unexpected name returned: %s", ret->name);
    cr_assert(ret->gpa == 2, "Unexpected gpa returned: %f", ret->gpa);
    cr_assert(ret->id == 2, "Unexpected id returned: %d", ret->id);

    cr_assert(list->length == 2, "Unexpected list size: %lu", list->length);

    delete_al(list, student_t_free_func);


}

Test(al_suite, 3_1_removal, .timeout=2, .init = setup){

    arraylist_t *list = new_al(sizeof(student_t));
    student_t *stu, *save;

    stu = gen_student("Kuba", 0, 0);
    insert_al(list, stu);

    stu = gen_student("Amanda", 1, 1);
    save = stu;
    insert_al(list, stu);

    stu = gen_student("Maya", 2, 2);
    insert_al(list, stu);

    remove_data_al(list, save);

    stu = get_index_al(list, 1);

    cr_assert(strcmp(stu->name, "Maya") == 0, "Unexpected name retrieved: %s", stu->name);
    cr_assert(stu->gpa == 2, "Unexpected gpa: %f", stu->gpa);
    cr_assert(stu->id == 2, "Unepexted id: %d", stu->id);

    cr_assert(list->length == 2, "Unexpected list length: %lu", list->length);

    delete_al(list, student_t_free_func);

}

Test(al_suite, 4_getdata_prim, .timeout=2, .init = setup){

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

    delete_al(list, NULL);

    free(ret);
    free(temp1);
    free(temp2);

}

Test(al_suite, 4_1_getdata, .timeout=2, .init = setup){

    arraylist_t *list = new_al(sizeof(student_t));
    student_t *stu, *save;

    stu = gen_student("stu1", 0, 0);
    insert_al(list, stu);

    stu = gen_student("stu2", 1, 1);
    save = stu;
    insert_al(list, stu);

    stu = gen_student("stu3", 2, 2);
    insert_al(list, stu);

    size_t index = get_data_al(list, save);

    cr_assert(index == 1, "Unexpected index returned: %lu\n", index);

}

Test(al_suite, 5_shrink, .timeout=2, .init = setup){

    arraylist_t *list = new_al(sizeof(student_t));
    student_t *stu;

    for(int i = 0; i < 6; i++){

        stu = gen_student("Dummy", i/2, i);
        insert_al(list, stu);

    }

    cr_assert(list->capacity == INIT_SZ*2, "Unexpected capacity, should have grown: %lu\n", list->capacity);

    remove_index_al(list, 0);
    remove_index_al(list, 0);
    remove_index_al(list, 0);

    cr_assert(list->capacity == INIT_SZ, "Unexpected capacity, should have shrunk: %lu\n", list->capacity);

    delete_al(list, student_t_free_func);

}

#define NUM_THREADS 100
#define NUM_OPS 10

void *threadfunc1(void *arg){

    arraylist_t *list = (arraylist_t*)arg;

    int *myint = get_index_al(list, 0);
    cr_assert(myint != NULL && *myint == 10);

    return NULL;

}

void *threadfunc2(void *arg){

    arraylist_t *list = (arraylist_t*)arg;
    (void)list;

    // if(list->length == 0){

    //     //Add an element
    //     printf("No elements found, inserting one\n");
    //     int *temp = malloc(sizeof(int));
    //     insert_al(list, temp);
    //     free(temp);

    // }else{

    //     //Remove an element
    //     printf("An element exists, removing first one\n");
    //     int *temp = remove_index_al(list, 0);
    //     cr_assert(temp != NULL && *temp == 100);
    //     free(temp);

    // }

    return NULL;

}

Test(al_suite, 6_0_threadsafe, .timeout=2, .init = setup){

    arraylist_t *list = new_al(sizeof(int));

    int *myint = malloc(sizeof(int));
    *myint = 10;

    insert_al(list, myint);

    pthread_t threads[NUM_THREADS];

    int i;
    for(i = 0; i < NUM_THREADS; i++)
        if(pthread_create(&threads[i], NULL, threadfunc2, (void*)list))
            cr_assert(false, "Could not create thread %d\n", i);

    for(i = 0; i < NUM_THREADS; i++)
        if(pthread_join(threads[i], NULL))
            cr_assert(false, "Couldn't join thread %d\n", i);

}


