#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include "arraylist.h"
#include "debug.h"
#include "foreach.h"
#include <unistd.h>

/**************************************/
typedef struct {
    char* name;
    int32_t id;
    double gpa;
}student_t;

typedef student_t Student;

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

static arraylist_t *list;

//Init functions
void init_stu(){

	list = new_al(sizeof(Student));
    Student *stu = gen_student("Kuba", 1, 1);
    insert_al(list, stu);
    free(stu);

    stu = gen_student("Amanda", 2, 2);
    insert_al(list, stu);
    free(stu);

    stu = gen_student("Maya", 3, 3);
    insert_al(list, stu);
    free(stu);

    stu = gen_student("John", 4, 4);
    insert_al(list, stu);
    free(stu);

    stu = gen_student("Ronak", 5, 5);
    insert_al(list, stu);
    free(stu);

}

void init_int(){

	list = new_al(sizeof(int));

}

//Teardown functions
void exit_int(){

	delete_al(list, NULL);

}

void exit_stu(){

	delete_al(list, student_t_free_func);
    free(list);

}

Test(foreach_suite, 00_init, .timeout=2, .init = init_stu, .fini = exit_stu){

    arraylist_t *mylist = new_al(sizeof(Student));

	Student *rtn = foreach_init(mylist);

	cr_assert(rtn == NULL, "Unexpected return: %p\n", (void*)rtn);

}

Test(foreach_suite, 01_init, .timeout=2, .init = init_stu, .fini = exit_stu){

	Student *rtn = foreach_init(list);

	int temp = memcmp(rtn, list->base, list->item_size) == 0;

	cr_assert_not_null(rtn, "Unexpected return: %p\n", (void*)rtn);
	cr_assert(rtn != list->base, "Returned a pointer from the list: %p\n", (void*)rtn);
	cr_assert(temp, "Return did not return an identical copy: %d\n", temp);


}

Test(foreach_suite, 02_init, .timeout=2, .init = init_stu, .fini = exit_stu){

    Student *rtn = foreach_init(list);

    int temp = memcmp(rtn, list->base, list->item_size) == 0;

    cr_assert_not_null(rtn, "Unexpected return: %p\n", (void*)rtn);
    cr_assert(rtn != list->base, "Returned a pointer from the list: %p\n", (void*)rtn);
    cr_assert(temp, "Return did not return an identical copy: %d\n", temp);

}

Test(foreach_suite, 10_foreach_index, .timeout=2, .init = init_stu, .fini = exit_stu){

    size_t i = 0;
    foreach(Student, stu, list){

        size_t index = foreach_index();

        cr_assert(index == i, "Unexpected index value: %lu", index);
        i++;

    }

}

Test(foreach_suite, 20_foreach_value, .timeout=2, .init = init_stu, .fini = exit_stu){

    foreach(Student, stu, list){

        char *name = stu->name;

        if(foreach_index() == 0)
            cr_assert(strcmp(name, "Kuba") == 0);
        else if(foreach_index() == 1)
            cr_assert(strcmp(name, "Amanda") == 0);
        else if(foreach_index() == 2)
            cr_assert(strcmp(name, "Maya") == 0);

    }

}

Test(foreach_suite, 30_foreach_data, .timeout=2){

    arraylist_t *mylist = new_al(sizeof(Student));
    Student *stu = gen_student("Kuba", 1, 1);
    insert_al(mylist, stu);
    free(stu);

    stu = gen_student("Amanda", 2, 2);
    insert_al(mylist, stu);
    free(stu);

    stu = gen_student("Maya", 3, 3);
    insert_al(mylist, stu);
    free(stu);

    int semval;

    foreach_init(mylist);

    sem_getvalue(&mylist->foreach_lock, &semval);

    cr_assert(semval == 0, "Semval had unexpected value: %d\n", semval);
    cr_assert(foreach_data->list == mylist, "Unexpected list address: %p, should be %p\n", (void*)(foreach_data->list), (void*)list);
    cr_assert(foreach_data->current_index == 0, "Unexpected index: %lu\n", foreach_data->current_index);

    cr_assert(foreach_break_f(), "Break did not return true\n");

}

static void *threadfunc1(void *arg){

    arraylist_t *list = arg;

    loop(5){

        printf("Starting remove\n");
        remove_index_al(list, __i__);
        printf("Finished remove\n");

    }

    return NULL;

}

static void *threadfunc2(void *arg){

    arraylist_t *list = arg;

    loop(1){

        printf("Starting foreach\n");
        foreach(Student, stu, list){

            printf("Student name: %s\n", stu->name);
            sleep(10);

        }
        printf("Finished foreach\n");

    }

    return NULL;

}

#define NUMTHREADS 5

Test(foreach_suite, 40_thread_safety, .timeout=2, .init = init_stu, .fini = exit_stu){

    pthread_t removethread;
    pthread_t foreach_threads[NUMTHREADS];

    loop(NUMTHREADS){

        pthread_create(&foreach_threads[__i__], NULL, threadfunc2, list);

    }
    pthread_create(&removethread, NULL, threadfunc1, list);

    loop(NUMTHREADS){

        pthread_join(foreach_threads[__i__], NULL);

    }
    pthread_join(removethread, NULL);

}
