//
// Created by 16009 on 2020/10/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "query.h"
#include <math.h>



status toRPN(const char *in, char *out) {
    char tmp[STATEMENT_MAX_SIZE];
    char *token;
    strcpy(tmp, in);
    out[0] = '\0';

    ptrStack ps = createStack();
    //去掉指令头
    strtok(tmp, " ");
    token = strtok(NULL, " ");
    //'打印全部'的情况
    if (strcmp(token, "*") == 0) {
        strcat(out, "*");
        return OK;
    }
    while (token) {
        if (strcmp(token, "=") == 0
            || strcmp(token, "!=") == 0
            || strcmp(token, ">") == 0
            || strcmp(token, "<") == 0
            || strcmp(token, ">=") == 0
            || strcmp(token, "<=") == 0
            || strcmp(token, "|") == 0
            || strcmp(token, "&") == 0) {
            if (isEmpty(ps)) {
                putInStack(ps, token);
            } else {
                if (getOrder(token) > getOrder(peek(ps))) {
                    putInStack(ps, token);
                } else {
                    while (!isEmpty(ps) && getOrder(token) <= getOrder(peek(ps))) {
                        char o[OPT_MAX_SIZE];
                        popFromStack(ps, o);
                        strcat(out, o);
                        strcat(out, " ");
                    }
                    putInStack(ps, token);
                }
            }
        } else {
            strcat(out, token);
            strcat(out, " ");
        }

        token = strtok(NULL, " ");
    }
    while (!isEmpty(ps)) {
        char t[OPT_MAX_SIZE];
        popFromStack(ps, t);
        strcat(out, t);
        strcat(out, " ");
    }
    free(ps);
    //printf("%s\n", out);
    return OK;
}

status queryForCondition(ptrData pdata,
                         const char *left,
                         const char *opt,
                         const char *right,
                         int *res) {
    int p = 0;
    int col = colIndex(pdata, left);
    if (col < 0) {
        return ERROR_NF;
    }
    if (strcmpi(right, NA) == 0) {
        if (strcmp(opt, "=") == 0) {
            for (int i = 0; i < count(pdata); i++) {
                char this[STATEMENT_MAX_SIZE];
                getString(pdata, i, col, this);
                if (strcmpi(this, right) == 0) res[p++] = i;
            }
        } else if (strcmp(opt, "!=") == 0) {
            for (int i = 0; i < count(pdata); i++) {
                char this[STATEMENT_MAX_SIZE];
                getString(pdata, i, col, this);
                if (strcmpi(this, right) != 0) res[p++] = i;
            }
        }
        res[p] = -1;
        return OK;
    }
    Type type = getType(pdata, col);
    switch (type) {
        case IM_INT: {
            int target;
            if (stoi(right, &target) == ERROR_TYPE) return ERROR_TYPE;
            for (int i = 0; i < count(pdata); i++) {
                int this;
                if (getInt(pdata, i, col, &this) == ERROR_TYPE) continue;
                if (strcmp(opt, "=") == 0) {
                    if (this == target) res[p++] = i;
                } else if (strcmp(opt, "!=") == 0) {
                    if (this != target) res[p++] = i;
                } else if (strcmp(opt, ">") == 0) {
                    if (this > target) res[p++] = i;
                } else if (strcmp(opt, "<") == 0) {
                    if (this < target) res[p++] = i;
                } else if (strcmp(opt, ">=") == 0) {
                    if (this >= target) res[p++] = i;
                } else if (strcmp(opt, "<=") == 0) {
                    if (this <= target) res[p++] = i;
                }
            }
            res[p] = -1;
            return OK;
        }
        case IM_FLOAT: {
            double target;
            if (stof(right, &target) == ERROR_TYPE) return ERROR_TYPE;
            for (int i = 0; i < count(pdata); i++) {
                double this;
                //printf("%s\n","searchforFloat");
                if (getFloat(pdata, i, col, &this) == ERROR_TYPE) continue;
                if (strcmp(opt, "=") == 0) {
                    if (fabs(this - target) < 0.000001) res[p++] = i;
                } else if (strcmp(opt, "!=") == 0) {
                    if (fabs(this - target) > 0.000001) res[p++] = i;
                } else if (strcmp(opt, ">") == 0) {
                    if (this - target > 0.000001) {
                        res[p++] = i;
                        //printf("%lf\n",this);
                    }
                } else if (strcmp(opt, "<") == 0) {
                    if (this - target < 0.000001) res[p++] = i;
                } else if (strcmp(opt, ">=") == 0) {
                    if (this - target > 0.000001 || fabs(this - target) < 0.000001) res[p++] = i;
                } else if (strcmp(opt, "<=") == 0) {
                    if (this - target < 0.000001 || fabs(this - target) < 0.000001) res[p++] = i;
                }
            }
            res[p] = -1;
            return OK;
        }
        case IM_STRING: {
            const char *target = right;
            for (int i = 0; i < count(pdata); i++) {
                char this[STATEMENT_MAX_SIZE];
                if (getString(pdata, i, col, this) == ERROR_TYPE) continue;
                if (strcmp(opt, "=") == 0) {
                    if (strcmp(this, target) == 0) res[p++] = i;
                } else if (strcmp(opt, "!=") == 0) {
                    if (strcmp(this, target) != 0) res[p++] = i;
                } else if (strcmp(opt, ">") == 0) {
                    if (strcmp(this, target) > 0) res[p++] = i;
                } else if (strcmp(opt, "<") == 0) {
                    if (strcmp(this, target) < 0) res[p++] = i;
                } else if (strcmp(opt, ">=") == 0) {
                    if (strcmp(this, target) >= 0) res[p++] = i;
                } else if (strcmp(opt, "<=") == 0) {
                    if (strcmp(this, target) <= 0) res[p++] = i;
                }
            }
            res[p] = -1;
            return OK;
        }
        default:
            break;
    }
}

int getOrder(const char *s) {
    int optNums = 8;
    char opts[][10] = {"=", "!=", ">", "<", ">=", "<=", "|", "&"};
    int orders[8] = {5, 5, 5, 5, 5, 5, 3, 4};
    for (int i = 0; i < optNums; ++i) {
        if (strcmp(opts[i], s) == 0) return orders[i];
    }
    return -1;
}

status query(ptrData pdata, const char *in, int **res) {
    if (pdata == NULL) return ERROR_NULL_POINTER;
    //30表示集合栈的数目
    static int ret[30][ROW_MAX_NUM];
    char rpn[STATEMENT_MAX_SIZE];
    char tmp[STATEMENT_MAX_SIZE];
    char *f;
    //结果集的排序方式
    int order = 0;
    int id_col;
    if ((f = strstr(in, "order")) != NULL) {
        char id_name[FIELD_MAX_NAME_SIZE];
        int length = (int) strlen(in) - (int) strlen(f) - 1;
        strncpy(tmp, in, length);
        tmp[length] = '\0';
        f[5] == 'i' ? (order = 1) : (order = -1);
        strcpy(id_name, f + 7);
        if ((id_col = colIndex(pdata, id_name)) == -1) return ERROR_NF;
    } else {
        strcpy(tmp, in);
    }
    //转换逆波兰
    toRPN(tmp, rpn);

    if (strcmp(rpn, "*") == 0) {
        int p = 0;
        for (int i = 0; i < count(pdata); i++) ret[0][p++] = i;
        ret[0][p] = -1;
        *res = sort(pdata, id_col, ret[0], order);
        return OK;
    }
    ptrStack ps = createStack();
    char *token = strtok(rpn, " ");
    int pset = 0;
    char obj1[128], obj2[128];
    while (token) {
        //比较运算符
        if (strcmp(token, "=") == 0
            || strcmp(token, "!=") == 0
            || strcmp(token, ">") == 0
            || strcmp(token, "<") == 0
            || strcmp(token, ">=") == 0
            || strcmp(token, "<=") == 0
                ) {
            popFromStack(ps, obj2);
            popFromStack(ps, obj1);
            status sta = queryForCondition(pdata, obj1, token, obj2, ret[pset++]);
            if (sta != OK) return sta;

        } //逻辑运算符
        else if (strcmp(token, "|") == 0) {
            pset -= 1;
            orSet(ret[pset - 1], ret[pset]);
        } else if (strcmp(token, "&") == 0) {
            pset -= 1;
            andSet(ret[pset - 1], ret[pset]);
        } //操作数
        else {
            putInStack(ps, token);
        }

        token = strtok(NULL, " ");
    }
    //排序
    *res = sort(pdata, id_col, ret[0], order);
    free(ps);
    return OK;
}

int *sort(ptrData pdata, int id_col, int *res, int order) {
    if (order == 0) return res;
    Type type = getType(pdata, id_col);
    int (*cmp)(const char *, const char *);

    switch (type) {
        case IM_INT:
            cmp = cmpInt;
            break;
        case IM_FLOAT:
            cmp = cmpFloat;
            break;
        case IM_STRING:
            cmp = cmpStr;
            break;
        default:
            break;
    }
    //建立索引res ~ data
    char data[ROW_MAX_NUM][CELL_MAX_LENGTH];
    for (int i = 0; res[i] != -1; ++i) getString(pdata, res[i], id_col, data[res[i]]);
    //选择排序
    //printf("sorting\n");
    if (order == 1)
        for (int i = 0; res[i] != -1; i++) {
            int t = i;
            for (int j = i + 1; res[j] != -1; j++) {
                if (cmp(data[res[t]], data[res[j]]) > 0) t = j;
            }
            if (t != i) {
                int tmp = res[t];
                res[t] = res[i];
                res[i] = tmp;
                //printf("发生交换\n");
            }
        }
    else
        for (int i = 0; res[i] != -1; i++) {
            int t = i;
            for (int j = i + 1; res[j] != -1; j++) {
                if (cmp(data[res[t]], data[res[j]]) < 0) t = j;
            }
            if (t != i) {
                int tmp = res[t];
                res[t] = res[i];
                res[i] = tmp;
            }
        }
    return res;
}


status insert(ptrData pdata, const char *in) {
    if (pdata == NULL) return ERROR_NULL_POINTER;

    char tmp[STATEMENT_MAX_SIZE];
    strcpy(tmp, in);
    char *token;
    //去除指令头
    token = strtok(tmp, " ");
    token = strtok(NULL, " ");
    //没有指定行，追加模式
    if (strcmpi(token, "values") == 0) {
        token = strtok(NULL, " ");
        return appendRowByStr(pdata, token);
    } else {//插入模式
        int row;
        stoi(token, &row);
        token = strtok(NULL, " ");
        token = strtok(NULL, " ");
        return insertRowByStr(pdata, token, row);
    }
}

status delete(ptrData pdata, const char *in) {
    if (pdata == NULL) return ERROR_NULL_POINTER;

    int *res;
    status sta = query(pdata, in, &res);

    if (sta == OK) {
        int len = 0;
        while (res[len] != -1) len++;
        for (int i = len - 1; i >= 0; --i) {
            //printf("%d\n", res[i]);
            delRow(pdata, res[i]);
        }
    }
    return sta;
}

status update(ptrData pdata, const char *in) {
    if (pdata == NULL) return ERROR_NULL_POINTER;

    char buff[STATEMENT_MAX_SIZE];
    char K[getColNum(pdata)][FIELD_MAX_NAME_SIZE];
    char V[getColNum(pdata)][CELL_MAX_LENGTH];
    strcpy(buff, in);

    status ret;

    char *token;
    //去除指令头
    strtok(buff, " ");
    token = strtok(NULL, " ");
    int kv_length = 0;
    //键值指针
    int p = 0;
    //提取键值对
    while (token && strcmpi(token, "where") != 0) {
        if (token[0] == '=') {
            token = strtok(NULL, " ");
            continue;
        }
        if (p == 0) {
            strcpy(K[kv_length], token);
            p = 1;
        } else {
            strcpy(V[kv_length], token);
            p = 0;
            kv_length++;
        }
        //printf("%d\n", kv_length);
        token = strtok(NULL, " ");
    }
//    for(int i = 0; i < kv_length; i++) {
//        printf("%s\n", K[i]);
//    };
    //有条件
    if (token) {
        toLowerCase(buff, in);
        int *res;
        ret = query(pdata, strstr(buff, "where"), &res);
        if (ret != OK) return ret;

        ret = updateRows(pdata, K, V, kv_length, res);
    }//无条件
    else {
        int n = count(pdata);
        int res[n + 1];
        for (int i = 0; i < n + 1; ++i) {
            res[i] = i;
        }
        res[n] = -1;
        ret = updateRows(pdata, K, V, kv_length, res);
    }

    return ret;
}

