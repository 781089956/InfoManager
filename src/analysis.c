//
// Created by 16009 on 2020/10/30.
//

#include "analysis.h"
status analyse(ptrData pdata, const char * in) {
    if (pdata == NULL) return ERROR_NULL_POINTER;
    char tmp[STATEMENT_MAX_SIZE];
    char *fieldname;
    strcpy(tmp, in);

    strtok(tmp, " ");
    fieldname = strtok(NULL, " ");

    int col = colIndex(pdata, fieldname);
    if (col == -1) return ERROR_NF;

    if (getType(pdata, col) == IM_STRING) {
        puts("无法计算的数据类型");
        return OK;
    }

    double _mean = mean(pdata, col);
    double _median = median(pdata, col);
    printf("中位数=%.2lf\n平均数=%.2lf\n", _median, _mean);
    return OK;
}

double median(ptrData pdata, int col) {
    Type type = getType(pdata, col);
    if (type == IM_STRING) return -1;

    const char * fieldname = getFieldName(pdata,col);
    int *all;
    char stat[STATEMENT_MAX_SIZE] = "select ";
    strcat(stat, fieldname);
    strcat(stat, " != N/A orderi ");
    strcat(stat, fieldname);
    if(query(pdata, stat, &all) != OK) return -1;

    int num = 0;
    for (int i = 0; all[i] != -1; ++i) num++;


    double ret;
    if (num % 2) {
        getFloat(pdata, all[num / 2], col, &ret);
    } else {
        double r1, r2;
        getFloat(pdata, all[num / 2 - 1], col, &r1);
        getFloat(pdata, all[num / 2], col, &r2);
        ret = (r1 + r2) / 2.0;
    }

    return ret;
}

double mean(ptrData pdata, int col) {
    Type type = getType(pdata, col);
    if (type == IM_STRING) return -1;

    int *all;
    double addup = 0;
    char stat[STATEMENT_MAX_SIZE] = "select ";
    strcat(stat, getFieldName(pdata,col));
    strcat(stat, " != N/A");

    if(query(pdata, stat, &all) != OK) return -1;
    int num = 0;
    for (int i = 0; all[i] != -1; ++i) {
        double tmp;
        getFloat(pdata, all[i], col, &tmp);
        addup += tmp;
        num ++;
    }

    if (num == 0) return 0;
    return addup / num;
}