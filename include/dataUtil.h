#include "status.h"
#include "linkList.h"
#include "stack.h"
#include "util.h"


#ifndef DATA_UTIL_H_INCLUDED
#define DATA_UTIL_H_INCLUDED

#define FIELD_MAX_NAME_SIZE 15//字段名称长度
#define FIELD_MAX_NUM 10    //字段个数
#define ROW_MAX_NUM 255     //记录数
#define CELL_MAX_LENGTH 255 //记录长度

#define NA "N/A"

/**
*支持的数据类型
*/
typedef enum{
    IM_STRING = 0,
    IM_INT,
    IM_FLOAT,
}Type;

/**
*数据集
*/
typedef struct Col {
    ptrLinkList data;
    Type type;  //记录列的数据类型
}Col, * ptrCol;

typedef struct Data {
    char field_names[FIELD_MAX_NUM][FIELD_MAX_NAME_SIZE];
    ptrCol *cols;
    Type * types;
    int colNum;
}* ptrData;
/**
 * 创建数据集
 * @param types 数据类型集
 * @param colNum 字段数
 * @return 新数据集指针
 */
ptrData createData(const Type types[], int colNum);
//以XXX方式获取数据
status getInt(ptrData pdata, int row, int col, int * ret);
status getString(ptrData pdata, int row, int col, char* ret);
status getFloat(ptrData pdata, int row, int col, double * ret);
/**
 * 获取字段数
 * @param pdata
 * @return
 */
int getColNum(ptrData pdata);
/**
 * 返回记录数
 * @param pdata
 * @return
 */
int count(ptrData pdata);
/**
 * 根据字段名检索列号
 * @param pdata
 * @param fieldname
 * @return
 */
int colIndex(ptrData pdata, const char *fieldname);
/**
 * 获取列的数据类型
 * @param pdata
 * @param col
 * @return
 */
Type getType(ptrData pdata, int col);
/**
 * 获取列号的字段名
 * @param pdata
 * @param col
 * @return
 */
const char *getFieldName(ptrData pdata, int col);
/**
 * 设置指定单元格的数据
 * @param pdata
 * @param item
 * @param row
 * @param col
 * @return
 */
status set(ptrData pdata, const char* item, int row, int col);
/**
 * 修改整行数据
 * @param pdate
 * @param K 键
 * @param V 值
 * @param num
 * @param rows
 * @return
 */
status updateRows(ptrData pdate,
                  const char K[][FIELD_MAX_NAME_SIZE],
                  const char V[][CELL_MAX_LENGTH],
                  int num,
                  const int* rows);
/**
 * 设置字段名称
 * @param pdata
 * @param name
 * @param i
 */
void setField(ptrData pdata, const char *name, int i);
/**
 * 通过字符串插入行
 * @param pdata
 * @param s
 * @param row
 * @return
 */
status insertRowByStr(ptrData pdata, const char *s, int row);
/**
 * 插入行
 * @param pdata
 * @param data
 * @param i
 * @return
 */
status insertRow(ptrData pdata, ptrLinkList data, int i);
status appendRowByStr(ptrData pdata, const char *s);
status appendRow(ptrData pdata, ptrLinkList data);
/**
 * 删除行
 * @param pdata
 * @param i
 * @return
 */
status delRow(ptrData pdata, int i);
/**
 * 字符串“,”分隔，转换为链表
 * @param s
 * @param del
 * @param colNum
 * @param pl
 * @return
 */
status toLinkListFromStr(const char *s, const char * del, int colNum,ptrLinkList pl);
/**
 * 释放数据集资源
 * @param pdata
 */
void freeData(ptrData pdata);
//数据展示
void printRow(ptrData pdata, int row);
void printRes(ptrData pdata, int *res);
void printFields(ptrData pdata);
void printData(ptrData pdata);




#endif // DATA_UTIL_H_INCLUDED
