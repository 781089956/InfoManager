//
// Created by 16009 on 2020/10/22.
//

#include "sys.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
status (*save)(const char *, ptrData) ;
status (*open)(const char *, ptrData *, Type*);
clock_t start,end;
//存储编译后的表达式
static regex_t comp_reg[STATEMENT_TYPE_NUM];
static Type default_types[FIELD_MAX_NUM];

static void loadConfig() {
    for (int i = 0; i < FIELD_MAX_NUM; ++i) default_types[i] = IM_STRING;

    const char *line = linesReader("conf/default_type.txt");
    int i = 0;
    while (line != NULL) {
        if (strcmpi(line, "string") == 0)
            default_types[i++] = IM_STRING;
        else if (strcmpi(line, "int") == 0)
            default_types[i++] = IM_INT;
        else if (strcmpi(line, "float") == 0)
        {
            default_types[i++] = IM_FLOAT;
            //printf("%d FLOAT\n", i-1);
        }
        else {
            printf("无效的数据类型：%s\n", line);
        }
        line = linesReader(NULL);
    }
}
void init() {
    puts("初始化中...");
    char *select_pattern = "^select\\s(\\*|(([^[:space:],]+\\s(=|(!=)|<|>|(<=)|(>=))\\s[^[:space:],]+)(\\s[\\|&]\\s[^[:space:],]+\\s[=(!=)<>(<=)(>=)]\\s[^[:space:],]+)*))((\\sorderi\\s[^[:space:],]+)|(\\sorderr\\s[^[:space:],]+))?$";
    char *sys_pattern = "^((open\\s[^[:space:],]+)|exit|(save\\s[^[:space:],]+)|(help)|(script\\s[^[:space:],]+))$";
    char *update_pattern = "^update(\\s[^[:space:],]+\\s=\\s[^[:space:],]+)+(\\swhere\\s(\\*|([^[:space:],]+\\s(=|(!=)|<|>|(<=)|(>=))\\s[^[:space:],]+)(\\s[\\|&]\\s[^[:space:],]+\\s[=(!=)<>(<=)(>=)]\\s[^[:space:],]+)*))?$";
    char *delete_pattern = "^delete\\s(\\*|([^[:space:],]+\\s(=|(!=)|<|>|(<=)|(>=))\\s[^[:space:],]+)(\\s[\\|&]\\s[^[:space:],]+\\s[=(!=)<>(<=)(>=)]\\s[^[:space:],]+)*)$";
    char *insert_pattern = "^insert(\\s(0|[1-9][0-9]*))?\\svalues\\s[^[:space:],]+(,[^[:space:],]*)*$";
    char *analysis_pattern = "^analyse\\s[^[:space:],]+$";


    if (regcomp(comp_reg, select_pattern, REG_NEWLINE | REG_ICASE | REG_EXTENDED | REG_NOSUB))
        printf("select编译失败\n");
    if (regcomp(comp_reg + 1, sys_pattern, REG_NEWLINE | REG_ICASE | REG_EXTENDED | REG_NOSUB))
        printf("sys编译失败\n");
    if (regcomp(comp_reg + 2, update_pattern, REG_NEWLINE | REG_ICASE | REG_EXTENDED | REG_NOSUB))
        printf("update编译失败\n");
    if (regcomp(comp_reg + 3, delete_pattern, REG_NEWLINE | REG_ICASE | REG_EXTENDED | REG_NOSUB))
        printf("delete编译失败\n");
    if (regcomp(comp_reg + 4, insert_pattern, REG_NEWLINE | REG_ICASE | REG_EXTENDED | REG_NOSUB))
        printf("insert编译失败\n");
    if (regcomp(comp_reg + 5, analysis_pattern, REG_NEWLINE | REG_ICASE | REG_EXTENDED | REG_NOSUB))
        printf("analysis编译失败\n");
    //配置导入
    loadConfig();
    //接口绑定
    open = readFromCSV;
    save = saveAsCSV;
    puts("初始化完成");
    puts("==========================================");
}

CommandType syntaxCheck(const char *in, char *msg) {
    char buff[STATEMENT_MAX_SIZE];
    strcpy(buff, in);

    for (int i = 0; i < STATEMENT_TYPE_NUM; i++) {
        int ret = regexec(comp_reg + i, buff, 0, NULL, 0);
        if (ret == 0) {
            //printf("合法！\n");
            return i;
        }
    }
    strcpy(msg, "非法的表达式");
    return IM_ILLEGAL;
}

status sysHandler(ptrData *ppdata, const char *in) {
    status ret = OK;
    char buff[STATEMENT_MAX_SIZE];
    strcpy(buff, in);
    char *token;
    token = strtok(buff, " ");
    if(strcmp(token, "open") == 0) {
        token = strtok(NULL, " ");
        freeData(*ppdata);
        *ppdata = NULL;
        ret = open(token, ppdata, default_types);
        //printf("%d", getType(*ppdata, 0) == IM_STRING);
    } else if (strcmp(token, "save") == 0) {
        token = strtok(NULL, " ");
        ret = save(token, *ppdata);
    } else if (strcmp(token, "exit") == 0) {
        freeData(*ppdata);
        exit(0);
    } else if (strcmp(token, "help") == 0) {
        help();
        ret = OK;
    }else if (strcmp(token, "script") == 0) {
        static int nest_level = 0;
        token = strtok(NULL, " ");
        if (nest_level < NEST_LEVEL) {
            nest_level++;
            ret = script(ppdata, token);
            nest_level--;
        } else {
            nest_level = 0;
            ret = ERROR_NEST;
        }
    }
    return ret;
}
status script(ptrData *ppdata, const char * filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        return ERROR_OPEN;
    };
    char buffer[STATEMENT_MAX_SIZE];
    status ret = OK;
    while (!feof(f)) {
        fgets(buffer, STATEMENT_MAX_SIZE, f);
        char * find = strchr(buffer, '\n');
        if (find) *find = '\0';
        //puts(buffer);
        if (buffer[0] != '\0' && buffer[0] != '#') {
            char msg[255];
            CommandType type = syntaxCheck(buffer, msg);
            ret = router(ppdata, type, buffer);
            if (ret != OK) break;
        }
    }
    return ret;
}

void errHandler(status s) {
    switch (s) {
        case OK:{
            char out[24];
            sprintf(out, "OK[%.3lfs]",(double)(end-start)/CLK_TCK);
            puts(out);
            break;
        }
        case ERROR_OUT:
            printf("索引越界\n");
            break;
        case ERROR_TYPE:
            printf("类型不匹配\n");
            break;
        case ERROR_NF:
            printf("字段未找到\n");
            break;
        case ERROR_OPEN:
            printf("文件打开失败\n");
            break;
        case ERROR_SYNTAX:
            printf("语法错误\n");
            break;
        case ERROR_NULL_POINTER:
            printf("空指针异常\n");
            break;
        case ERROR_NEST:
            printf("script嵌套错误\n");
            break;
        default:
            printf("未知错误\n");
    }
}
status router(ptrData* ppdata, CommandType type,const char *in) {
    status ret;
    switch (type) {
        case IM_QUERY: {
            int *res;
            ret = query(*ppdata, in, &res);
            if (ret == OK) printRes(*ppdata, res);
            break;
        }
        case IM_SYS:
            ret = sysHandler(ppdata, in);
            break;
        case IM_UPDATE:
            ret = update(*ppdata, in);
            break;
        case IM_DELETE:
            ret = delete(*ppdata, in);
            break;
        case IM_INSERT:
            ret = insert(*ppdata, in);
            break;
        case IM_ILLEGAL:
            ret = ERROR_SYNTAX;
            break;
        case IM_ANA:
            ret = analyse(*ppdata, in);
            break;
        default:
            ret = OK;
            break;
    }
    return ret;
}
void help() {
    const char *line = linesReader("docs/help.txt");
    if (line == NULL) {
        puts("未找到帮助文档");
        return;
    }
    while (line != NULL) {
        puts(line);
        line = linesReader(NULL);
    }
}