#include <stdio.h>
#include <string.h>
#include <sys.h>
#include "fileHandler.h"

status readFromCSV(const char *file_name, ptrData *ret, Type* default_types) {
    FILE *f = fopen(file_name, "r");
    //分隔符
    const char del[2] = ",";
    if (f == NULL) return ERROR_OPEN;
    int colNum = 0;
    //读取列数字段
    int max_count = FIELD_MAX_NAME_SIZE * FIELD_MAX_NUM + FIELD_MAX_NUM;
    char buff[max_count];
    Type types[FIELD_MAX_NUM];
    fgets(buff, max_count, f);

    //去除尾回车
    char * find = strchr(buff, '\n');
    if (find) *find = '\0';

    ptrLinkList pl = creatLinkList();
    toLinkListFromStr(buff, del, FIELD_MAX_NUM, pl);
    //printLinkList(pl);
    for (int i = 0;;++i) {
        char * tmp = (char *)getDataInLinkList(pl, i);
        //printf("%s\n", tmp);
        if (strcmp(tmp, NA) != 0) {
            if (default_types == NULL) {
                while (1) {
                    char in_tip[255];
                    sprintf(in_tip,
                            "请输入字段{%s}数据类型(1-int,2-string,3-float)", tmp);
                    printf("%s\n", in_tip);
                    char c;
                    scanf("%c", &c);
                    getchar();
                    switch (c) {
                        case '1':
                            types[i] = IM_INT;
                            goto out;
                        case '2':
                            types[i] = IM_STRING;
                            goto out;
                        case '3':
                            types[i] = IM_FLOAT;
                            goto out;
                        default:
                            printf("输入无效\n");
                            break;
                    }
                }
            }
            out:
            colNum++;
        } else {
            break;
        }
    }
    ptrData pdata;
    //定义数据集
    if (default_types == NULL)
        pdata = createData(types, colNum);
    else
        pdata = createData(default_types, colNum);
    int i = 0;
    while (i < colNum) {
        setField(pdata, (char *)getDataInLinkList(pl, i), i);
        i++;
    }
    //读取记录
    while (!feof(f)) {
        fscanf(f, "%s\n", buff);
        appendRowByStr(pdata, buff);
    }
    fclose(f);
    *ret = pdata;
    freeLinkList(pl);
    return OK;
}

status saveAsCSV(const char *file_name, ptrData pdata) {
    if (pdata == NULL) return ERROR_NULL_POINTER;
    FILE *f = fopen(file_name, "w");
    if (f == NULL) return ERROR_OPEN;
    int col_num = getColNum(pdata);
    int row_num = count(pdata);

    for (int i = 0; i < col_num; i++) {
        fputs(getFieldName(pdata, i), f);
        if (i == col_num - 1)
            fputc('\n', f);
        else
            fputc(',', f);
    }
    for (int i = 0; i < row_num; ++i) {
        for (int j = 0; j < col_num; ++j) {
            char tmp[CELL_MAX_LENGTH];
            getString(pdata, i, j, tmp);
            fputs(tmp, f);
            if (j != col_num - 1) putc(',', f);
        }
        putc('\n', f);
    }
    fclose(f);
    return OK;
}

const char * linesReader(const char *file_name) {
    static FILE *f = NULL;
    static char buffer[STATEMENT_MAX_SIZE];
    if (file_name) {
        if(f) fclose(f);
        f = fopen(file_name,"r");
        if (!f) return NULL;
    }
    if (!f) return NULL;

    while (!feof(f)) {
        fgets(buffer, STATEMENT_MAX_SIZE, f);
        //去除尾回车
        char * find = strchr(buffer, '\n');
        if (find) *find = '\0';
        if (buffer[0] != '\n' && buffer[0] != '#' && buffer[0] != '\0') return buffer;
    }
    fclose(f);
    f = NULL;
    return NULL;
}
