//
// Created by 16009 on 2020/10/18.
//
#include "util.h"
#include "dataUtil.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

status stof(const char *s, double *ret) {
    char *end;
    *ret = strtod(s, &end);
    if (end[0] != '\0')
        return ERROR_TYPE;
    else
        return OK;
}
void toLowerCase(char* dest, const char*src) {
    strcpy(dest, src);
    int i = 0;
    while (dest[i] != '\0') {
        if (dest[i] > 'A' && dest[i] < 'Z') dest[i] += 32;
        i++;
    }
}
status stoi(const char *s, int *ret) {
    char *end;
    *ret = (int) strtol(s, &end, 10);
    if (end[0] != '\0')
        return ERROR_TYPE;
    else
        return OK;
}
//保证数组升序不等
status orSet(int *ret, int *b) {
    int a[255];
    int t;
    for(t = 0; ret[t] != -1;t++) {
        a[t] = ret[t];
    }
    a[t] = -1;
    int i = 0, j = 0;
    int p = 0;
    while (a[i] != -1 && b[j] != -1) {
        if (a[i] < b[j]) {
            ret[p] = a[i++];
        } else if (a[i] == b[j]) {
            ret[p] = a[i];
            i++;
            j++;
        } else {
            ret[p] = b[j++];
        }
        p++;
    }
    while (a[i] != -1) {
        ret[p++] = a[i++];
    }
    while (b[j] != -1) {
        ret[p++] = b[j++];
    }

    ret[p] = -1;
    return OK;
}

status andSet(int *ret, int *b) {
    int a[255];
    int t;
    for(t = 0; ret[t] != -1;t++) {
        a[t] = ret[t];
    }
    a[t] = -1;
    int p = 0;
    for (int i = 0; a[i] != -1; i++) {
        for (int j = 0; b[j] != -1; j++) {
            if(a[i] == b[j]) ret[p++] = a[i];
        }
    }
    ret[p] = -1;
    return OK;
}
int cmpStr(const char *str1,const char *str2) {
    return strcmp(str1, str2);
}
int cmpInt(const char *i1,const char *i2) {
    int a,b;
    stoi(i1, &a);
    stoi(i2, &b);
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}
int cmpFloat(const char *f1,const char *f2) {
    double d1;
    double d2;
    stof(f1, &d1);
    stof(f2, &d2);
    if (fabs(d1 - d2) < 0.0000001) return 0;
    if (d1 - d2 > 0.0000001) return 1;
    return -1;
}
