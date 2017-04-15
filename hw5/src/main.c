#define ABC
#ifdef ABC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "arraylist.h"

typedef struct{

	char *name;
	double gpa;

}Student;

Student *gen_stu(char *name, double _gpa){

	Student *stu = malloc(sizeof(Student));

	stu->name = malloc(sizeof(char)*50);
	strcpy(stu->name, name);

	stu->gpa = _gpa;

	return stu;

}

void disp_stu(Student *s){

	printf("Student name: %s\n", s->name);
	printf("Student gpa: %f\n", s->gpa);

}

int main(){

	arraylist_t *list = new_al(sizeof(Student));

	insert_al(list, gen_stu("Kuba", 2.0));
	insert_al(list, gen_stu("Maya", 3.0));
	insert_al(list, gen_stu("Amanda", 4.0));

	Student *s;

	s = (Student*)(get_index_al(list, 0));

	assert(s != NULL);

}

#endif
