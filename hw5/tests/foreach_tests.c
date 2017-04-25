#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include "arraylist.h"
#include "debug.h"
#include "foreach.h"

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

}

Test(foreach_suite, 00_init, .timeout=2, .init = init_stu, .fini = exit_stu){

	Student *rtn = foreach_init(list);

	cr_assert(rtn == NULL, "Unexpected return: %p\n", (void*)rtn);

}

Test(foreach_suite, 01_init, .timeout=2, .init = init_stu, .fini = exit_stu){

	Student *stu = gen_student("Kuba", 1, 1);
	insert_al(list, stu);

	Student *rtn = foreach_init(list);

	int temp = memcmp(rtn, list->base, list->item_size) == 0;

	cr_assert_not_null(rtn, "Unexpected return: %p\n", (void*)rtn);
	cr_assert(rtn != list->base, "Returned a pointer from the list: %p\n", (void*)rtn);
	cr_assert(temp, "Return did not return an identical copy: %d\n", temp);


}

Test(foreach_suite, 02_init, .timeout=2, .init = init_stu, .fini = exit_stu){

    Student *stu = gen_student("Kuba", 1, 1);
    insert_al(list, stu);
    stu = gen_student("Amanda", 2, 2);
    insert_al(list, stu);
    stu = gen_student("Maya", 3, 3);
    insert_al(list, stu);

    Student *rtn = foreach_init(list);

    int temp = memcmp(rtn, list->base, list->item_size) == 0;

    cr_assert_not_null(rtn, "Unexpected return: %p\n", (void*)rtn);
    cr_assert(rtn != list->base, "Returned a pointer from the list: %p\n", (void*)rtn);
    cr_assert(temp, "Return did not return an identical copy: %d\n", temp);


}

Test(foreach_suite, 10_foreach_index, .timeout=2, .init = init_stu, .fini = exit_stu){

    Student *stu;

    stu = gen_student("Kuba", 1, 1);
    insert_al(list, stu);
    stu = gen_student("Amanda", 2, 2);
    insert_al(list, stu);
    stu = gen_student("Maya", 3, 3);
    insert_al(list, stu);

    size_t i = 0;
    foreach(Student, stu, list){

        size_t index = foreach_index();

        cr_assert(index == i, "Unexpected index value: %lu", index);
        i++;

    }

}

Test(foreach_suite, 20_foreach_value, .timeout=2, .init = init_stu, .fini = exit_stu){

    Student *stu;

    stu = gen_student("Kuba", 1, 1);
    insert_al(list, stu);
    stu = gen_student("Amanda", 2, 2);
    insert_al(list, stu);
    stu = gen_student("Maya", 3, 3);
    insert_al(list, stu);

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
