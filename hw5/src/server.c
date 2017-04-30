#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUMTHREADS 5

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

static void *threadfunc1(void *arg){

    arraylist_t *list = arg;

    loop(5){

        printf("Starting remove\n");
        remove_index_al(list, 0);
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
            sleep(1);

        }
        printf("Finished foreach\n");

    }

    return NULL;

}

int main(int argc, char *argv[]){

	arraylist_t *list;

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

    info("%s","\nFINISHED INITIALIZATION\n\n");

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
