//
// Created by 16009 on 2020/10/22.
//
#ifndef INFOMANAGER_SYS_H
#define INFOMANAGER_SYS_H

#include "status.h"
#include "dataUtil.h"
#include "fileHandler.h"
#include "regex.h"
#include <time.h>
#include "query.h"
#include "analysis.h"

#define STATEMENT_MAX_SIZE 255
#define OPT_MAX_SIZE 24
#define STATEMENT_TYPE_NUM 6

#define NEST_LEVEL 5

typedef enum CommandType{
    IM_QUERY = 0, IM_SYS, IM_UPDATE, IM_DELETE, IM_INSERT, IM_ANA, IM_ILLEGAL,
}CommandType;


extern status (*save)(const char *, ptrData) ;
extern status (*open)(const char *, ptrData *, Type*);
extern clock_t start,end;

void init();
CommandType syntaxCheck(const char * in, char *msg);
status sysHandler(ptrData *pdate, const char *in);
status router(ptrData *ppdata, CommandType type, const char *in);
void errHandler(status s);
status script(ptrData *pdata, const char * filename);
void help();
#endif //INFOMANAGER_SYS_H
