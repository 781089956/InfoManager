//
// Created by 16009 on 2020/10/18.
//

#ifndef INFOMANAGER_STACK_H
#define INFOMANAGER_STACK_H


#include "status.h"
/**
 * 简单的顺序表实现的栈(字符串)
 */
typedef struct Stack {
    char data[255][24];
    int top;
}Stack, * ptrStack;
/*创建栈*/
ptrStack createStack();
/*
 * 入栈
 */
status putInStack(ptrStack ps, const char* s);
/*
 * 出栈
 */
status popFromStack(ptrStack ps, char *ret);
/*
 * 空栈？
 */
int isEmpty(ptrStack ps);
/*
 * 查看栈顶元素
 */
const char * peek(ptrStack ps);

#endif //INFOMANAGER_STACK_H
