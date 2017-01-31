#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>
#include <stdio.h>
void strcopy(const char*, char*);
int streq(const char*,const char*);
int strleng(const char*);
int findIndex(const char, const char*);
char getChar(const int, const char*);
int isUpper(const char);
int isLower(const char);
char toUpper(const char);
char toLower(const char);
int strcontains(const char *, const char);
char *capitalizestring(char *);
char *findStringInArray(const char c, char**);
int cequals(const char c1, const char c2);
void appendString(char *, const char *);
void appendChar(char *, const char);
void clearString(char *);
char *shiftStringRight(char *);
char *shiftStringLeft(char *);
int stringToInt(char *);
void clearbuffer();
int compareStringToArray(const char *, const char * const*);

#endif