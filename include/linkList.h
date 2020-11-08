#include "status.h"

#ifndef LINKLIST_H_INCLUDED
#define LINKLIST_H_INCLUDED

typedef struct Node {
    void *data;
    struct Node *next;
}Node;

typedef struct{
    Node *head;
    int length;
}LinkList ,*ptrLinkList;



/**
 * 创建链表
 * @return
 */
ptrLinkList creatLinkList();
/**
 * 索引节点
 * @param pl
 * @param i
 * @return
 */
static Node * index(ptrLinkList pl, int i);
//新增数据
status appendToLinkList(ptrLinkList pl, const void *data, int buffer_size);
status insertIntoLinkList(ptrLinkList pl, const void *data, int buffer_size, int i);
/**
 * 删除节点
 * @param pl
 * @param i
 * @return
 */
status deleteFromLinkList(ptrLinkList pl, int i);
/**
 * 修改链表数据
 * @param pl
 * @param data
 * @param buffer_size
 * @param i
 * @return
 */
status setDataInLinkList(ptrLinkList pl, const void *data, int buffer_size, int i);
/**
 * 获取数据
 * @param pl
 * @param i
 * @return
 */
const void* getDataInLinkList(ptrLinkList pl, int i);
/**
 * 获取链表大小
 * @param pl
 * @return
 */
int getSizeOfLinkList(ptrLinkList pl);
/**
 * 清空链表
 * @param pl
 */
void clearLinkList(ptrLinkList pl);

void printLinkList(ptrLinkList pl);
/**
 * 释放链表
 * @param pl
 */
void freeLinkList(ptrLinkList pl);



void TestLinkList();

#endif // LINKLIST_H_INCLUDED
