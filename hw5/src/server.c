#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include <string.h>

/**************************************/
typedef struct {
    char* name;
    int32_t id;
    double gpa;
}student_t;

#define NAME_LENGTH 100

student_t *gen_student(char *name, int32_t _id, double _gpa){

    student_t *ret = malloc(sizeof(student_t));
    ret->name = malloc(sizeof(char) * NAME_LENGTH);
    strcpy(ret->name, name);

    ret->id = _id;
    ret->gpa = _gpa;

    return ret;

}

void student_t_free_func(void *argptr){

    student_t *stu = (student_t*)argptr;

    if(stu == NULL)
        error("argptr (%p) was null!\n", SHORT_ADDR(argptr));
    else if(stu->name == NULL)
        warn("argptr's (%p) name was NULL!\n", SHORT_ADDR(argptr));
    else
        free(stu->name);

}

void print_stu(student_t *stu){

	printf("Name: %s\nID: %d\nGPA: %f\n", stu->name, stu->id, stu->gpa);

}
/**************************************/

int main(int argc, char *argv[]){




}
