//
// Created by 16009 on 2020/10/18.
//
#include "status.h"
#ifndef INFOMANAGER_UTIL_H
#define INFOMANAGER_UTIL_H

status stoi(const char *s, int *ret);
status stof(const char *s, double *ret);
status orSet(int *a, int *b);
status andSet(int *a, int *b);
void toLowerCase(char* dest, const char*src);
int cmpStr(const char *str1,const char *str2);
int cmpInt(const char *i1,const char *i2);
int cmpFloat(const char *f1,const char *f2);
#endif //INFOMANAGER_UTIL_H
