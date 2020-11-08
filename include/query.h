//
// Created by 16009 on 2020/10/18.
//

#ifndef INFOMANAGER_QUERY_H
#define INFOMANAGER_QUERY_H

#include "status.h"
#include "dataUtil.h"
#include "util.h"
#include "sys.h"

/**
 * 查询
 * @param pdata 数据集
 * @param in 标准查询语句
 * @param res 结果集
 * @return 查询状态
 */
status query(ptrData pdata, const char * in, int **res);
/**
 * 插入
 * @param pdata 数据集
 * @param in 标准插入语句
 * @return 插入状态
 */
status insert(ptrData pdata, const char * in);
/**
 * 删除
 * @param pdata 数据集
 * @param in 标准删除语句
 * @return 删除状态
 */
status delete(ptrData pdata, const char *in);
/**
 * 更新
 * @param pdata 数据集
 * @param in 标准更新语句
 * @return 更新状态
 */
status update(ptrData pdata, const char *in);
/**
 * 逆波兰表达式转换
 * @param in 标准条件表达式
 * @param out 转换后的逆波兰表达式
 * @return 转换状态
 */
static status toRPN(const char *in, char *out);
/**
 * 结果集排序
 * @param pdata 数据集
 * @param id_col 排序依据列
 * @param res 结果集
 * @param order 排序方式（顺序/逆序）
 * @return 返回排序后的结果集（res==return）
 */
static int * sort(ptrData pdata, int id_col, int* res, int order);
/**
 * 获取运算符的优先级
 * @param s 运算符
 * @return 优先级
 */
static int getOrder(const char * s);
/**
 * 单条件查询
 * @param pdata 数据集
 * @param left 左操作数
 * @param opt 操作符
 * @param right 右操作符
 * @param res 结果集
 * @return 查询状态
 */
static status queryForCondition(ptrData pdata,
                         const char *left,
                         const char *opt,
                         const char * right,
                         int *res);



#endif //INFOMANAGER_QUERY_H
