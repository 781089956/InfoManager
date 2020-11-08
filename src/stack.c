//
// Created by 16009 on 2020/10/18.
//
#include "stack.h"
#include <stdlib.h>
#include <string.h>

ptrStack createStack() {
    ptrStack ps = (ptrStack)malloc(sizeof(Stack));
    ps->top = -1;

    return ps;
}
status putInStack(ptrStack ps, const char* s) {
    ps->top++;
    strcpy(ps->data[ps->top], s);
    return OK;
}
status popFromStack(ptrStack ps, char *ret) {
    if(isEmpty(ps)) return ERROR_OUT;

    strcpy(ret, ps->data[ps->top]);
    ps->top--;
    return OK;
}
int isEmpty(ptrStack ps) {
    if (ps->top == -1)
        return 1;
    else
        return 0;
}
const char * peek(ptrStack ps) {
    if (isEmpty(ps)) return NULL;

    return ps->data[ps->top];
}