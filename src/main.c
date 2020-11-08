#include <stdio.h>
#include "sys.h"


int main() {
    init();
    ptrData pdata = NULL;
    char msg[255];
    char in[STATEMENT_MAX_SIZE];
    puts("键入help获取帮助");
    while (1) {
        printf("im>>>");
        gets(in);
        //滴答
        start = clock();
        //语法检查
        CommandType type = syntaxCheck(in, msg);
        status ret = router(&pdata, type, in);
        //滴答
        end = clock();
        errHandler(ret);
    }
}





