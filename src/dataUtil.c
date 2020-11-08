#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dataUtil.h"

status getInt(ptrData pdata, int row, int col, int *ret) {
    const void *tmp = getDataInLinkList(pdata->cols[col]->data, row);
    if (!tmp) return ERROR_OUT;
    return (stoi((char *) tmp, ret));
}

status getString(ptrData pdata, int row, int col, char *ret) {
    char *tmp = (char *) getDataInLinkList(pdata->cols[col]->data, row);
    if (!tmp) return ERROR_OUT;
    //复制值
    strcpy(ret, tmp);
    return OK;
}


status getFloat(ptrData pdata, int row, int col, double *ret) {
    const void *tmp = getDataInLinkList(pdata->cols[col]->data, row);
    return stof((char *) tmp, ret);
}

status insertRow(ptrData pdata, ptrLinkList data, int row) {
    if (row < 0 || row > count(pdata) || getSizeOfLinkList(data) != getColNum(pdata))
        return ERROR_OUT;
    //类型检查
    for (int i = 0; i < getColNum(pdata); i++) {
        char *celldata = (char *) getDataInLinkList(data, i);
        if (strcmpi(celldata, "N/A") == 0) continue;
        switch (getType(pdata, i)) {
            case IM_INT: {
                int ret;
                if (stoi(celldata, &ret) == ERROR_TYPE)
                    return ERROR_TYPE;
                break;
            }
            case IM_FLOAT: {
                double ret;
                if (stof(celldata, &ret) == ERROR_TYPE)
                    return ERROR_TYPE;
                break;
            }
            case IM_STRING:
                break;
        }
    }
    for (int i = 0; i < getColNum(pdata); i++) {
        ptrCol pc = pdata->cols[i];
        const void *celldata = getDataInLinkList(data, i);
        if (insertIntoLinkList(pc->data,
                               celldata,
                               (int) strlen(celldata) + 1,
                               row) == ERROR_OUT) {
            return ERROR_OUT;
        }
    }

    return OK;
}


void setField(ptrData pdata, const char *name, int i) {
    strcpy(pdata->field_names[i], name);
}

status appendRow(ptrData pdata, ptrLinkList data) {
    status ret = insertRow(pdata, data, count(pdata));
    return ret;
}

status updateRows(ptrData pdate,
                  const char K[][FIELD_MAX_NAME_SIZE],
                  const char V[][CELL_MAX_LENGTH],
                  int num,
                  const int *rows) {
    status ret;
    //存储字段名对应的列号
    int cols[num];
    //列名检查
    for (int i = 0; i < num; i++) {
        cols[i] = colIndex(pdate, K[i]);
        if (cols[i] == -1) return ERROR_NF;
    }
    for (int i = 0; rows[i] != -1; ++i) {
        for (int j = 0; j < num; ++j) {
            ret = set(pdate, V[j], rows[i], cols[j]);
            if (ret != OK) return ret;
        }
    }

    return OK;
}

ptrData createData(const Type types[], int colNum) {
    ptrData pNew = (ptrData) malloc(sizeof(struct Data));
    //列数初始化
    pNew->colNum = colNum;
    //类型列表初始化
    pNew->types = (Type *) malloc(sizeof(Type) * colNum);
    for (int i = 0; i < colNum; i++) pNew->types[i] = types[i];
    //列初始化
    pNew->cols = (ptrCol *) malloc(sizeof(ptrCol) * colNum);
    for (int i = 0; i < colNum; i++) {
        ptrCol pc = (ptrCol) malloc(sizeof(Col));
        pc->data = creatLinkList();
        pc->type = types[i];
        pNew->cols[i] = pc;
    }

    return pNew;
}

int getColNum(ptrData pdata) {
    return pdata->colNum;
}


status set(ptrData pdata, const char *item, int row, int col) {
    if (row < 0 || row > count(pdata) - 1) return ERROR_OUT;
    if (col < 0 || col > getColNum(pdata) - 1) return ERROR_OUT;

    ptrCol pc = pdata->cols[col];
    Type type = pc->type;
    char buff[255];
    strcpy(buff, item);
    status ret;
    switch (type) {
        case IM_INT: {
            int res;
            ret = stoi(buff, &res);
            if (ret == ERROR_TYPE) return ERROR_TYPE;
            setDataInLinkList(pc->data, buff, (int) strlen(buff) + 1, row);
            break;
        }
        case IM_FLOAT: {
            double res;
            ret = stof(buff, &res);
            if (ret == ERROR_TYPE) return ERROR_TYPE;
            setDataInLinkList(pc->data, buff, (int) strlen(buff) + 1, row);
            break;
        }
        case IM_STRING: {
            ret = OK;
            setDataInLinkList(pc->data, buff, (int) strlen(buff) + 1, row);
            break;
        }
        default:
            break;
    }
    return ret;
}

status delRow(ptrData pdata, int row) {
    if (row < 0 || row > count(pdata) - 1) return ERROR_OUT;

    char *tmp = (char *) getDataInLinkList(pdata->cols[1]->data, row);
    //printf("%d%s\n",row, tmp);

    for (int j = 0; j < getColNum(pdata); j++) {
        ptrCol pc = pdata->cols[j];

        deleteFromLinkList(pc->data, row);
    }

    return OK;
}

status insertRowByStr(ptrData pdata, const char *s, int row) {
    ptrLinkList tmp = creatLinkList();
    toLinkListFromStr(s, ",", getColNum(pdata), tmp);
    status ret = insertRow(pdata, tmp, row);
    freeLinkList(tmp);
    return ret;
}

status appendRowByStr(ptrData pdata, const char *s) {
    return insertRowByStr(pdata, s, count(pdata));
}

status toLinkListFromStr(const char *s, const char *del, int colNum,
                         ptrLinkList pl) {
    char buff[strlen(s) + 1];
    strcpy(buff, s);
    clearLinkList(pl);

    //当前存储列数
    int i = 0;
    //当前字符串指针
    int pb = 0;
    //存储缓存
    char tmp[255];
    tmp[0] = '\0';
    //读取行中列数据
    while (buff[pb] != '\0') {
        if (buff[pb] == ',') {
            if (tmp[0] == '\0') {
                appendToLinkList(pl, NA, (int) strlen(NA) + 1);
            } else {
                appendToLinkList(pl, tmp, (int) strlen(tmp) + 1);
                tmp[0] = '\0';
            }
            i++;
        } else {
            int len = (int) strlen(tmp);
            tmp[len] = buff[pb];
            tmp[len + 1] = '\0';
        }
        pb++;
    }
    if (tmp[0] == '\0')
        appendToLinkList(pl, NA, (int) strlen(NA) + 1);
    else
        appendToLinkList(pl, tmp, (int) strlen(tmp) + 1);
    i++;
    if (i < colNum) {
        for (; i < colNum; i++) appendToLinkList(pl, NA, (int) strlen(NA) + 1);
    }

    return OK;
}

int count(ptrData pdata) {
    return getSizeOfLinkList(pdata->cols[0]->data);
}

void printFields(ptrData pdata) {
    printf("%-5s", "");
    for (int i = 0; i < getColNum(pdata); i++) {
        char temp[20];
        //printf("%-10s", pdata->field_names[i]);
        sprintf(temp, "%%-%ds", FIELD_MAX_NAME_SIZE);
        printf(temp, getFieldName(pdata, i));
    }
}

void freeData(ptrData pdata) {
    if (pdata == NULL) return;
    for (int i = 0; i < getColNum(pdata); ++i) {
        ptrCol pc = pdata->cols[i];
        freeLinkList(pc->data);
        free(pc);
    }
    free(pdata->types);
    free(pdata);
}

void printRow(ptrData pdata, int row) {
    printf("%-5d", row);
    for (int i = 0; i < getColNum(pdata); ++i) {
        ptrLinkList pl = pdata->cols[i]->data;
        char temp[20];
        sprintf(temp, "%%-%ds", FIELD_MAX_NAME_SIZE);
        printf(temp, (char *) getDataInLinkList(pl, row));
    }
}

void printType(ptrData pdata) {
    int colnum = getColNum(pdata);
    printf("dataTypes|");
    for (int i = 0; i < colnum; ++i) {
        switch (getType(pdata, i)) {
            case IM_STRING:
                printf("%d:string ", i);
                break;
            case IM_INT:
                printf("%d:int ", i);
                break;
            case IM_FLOAT:
                printf("%d:float ", i);
                break;
        }
    }
    printf("\n");
}

void printRes(ptrData pdata, int *res) {
    int i = 0;
    if (res[0] == -1) {
        printf("不存在满足条件的记录！\n");
        return;
    }

    printFields(pdata);
    printf("\n");
    while (res[i] != -1) {
        //printf("i:%d\n",i);
        printRow(pdata, res[i++]);
        printf("\n");
    }
    printf("count:%d\n", i);
    printType(pdata);
}


Type getType(ptrData pdata, int col) {
    return pdata->types[col];
}

const char *getFieldName(ptrData pdata, int col) {
    if (col < -1 || col > getColNum(pdata) - 1) return "N/A";

    return pdata->field_names[col];
}

int colIndex(ptrData pdata, const char *fieldname) {
    for (int i = 0; i < getColNum(pdata); ++i) {
        if (strcmp(getFieldName(pdata, i), fieldname) == 0) return i;
    }
    return -1;
}

void printData(ptrData pdata) {
    //打印字段
    printFields(pdata);
    printf("\n");
    //打印记录
    //printf("行数: %d\n", count(pdata));
    for (int i = 0; i < count(pdata); ++i) {
        //printf("第%d行: ", i);
        printRow(pdata, i);
        printf("\n");
    }
}