#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "status.h"
#include "linkList.h"

Node *index(ptrLinkList pl, int i) {
    Node *pPre = pl->head;
    int p = 0;
    while (p < i) {
        pPre = pPre->next;
        p++;
    }

    return pPre->next;
}

ptrLinkList creatLinkList() {
    ptrLinkList ret = (ptrLinkList) malloc(sizeof(LinkList));
    //空头节点，便于后续操作
    Node *tmp = (Node *) malloc(sizeof(Node));
    tmp->data = NULL;
    tmp->next = NULL;
    //初始化
    ret->head = tmp;
    ret->length = 0;
    return ret;
}

status appendToLinkList(ptrLinkList pl, const void *data, int buffer_size) {
    Node *pNode = pl->head;
    //寻找尾部
    while (pNode->next) pNode = pNode->next;
    //新建节点
    Node *pNew = (Node *) malloc(sizeof(Node));
    pNew->next = NULL;
    //拷贝数据
    pNew->data = malloc(buffer_size);
    memcpy(pNew->data, data, buffer_size);
    //插入
    pNode->next = pNew;
    pl->length++;

    return OK;
}

status insertIntoLinkList(ptrLinkList pl, const void *data, int buffer_size, int i) {
    //可以在尾部插入
    if (i < 0 || i > pl->length) return ERROR_OUT;

    Node *pPre = pl->head;
    Node *pNext;
    int p = 0;
    while (p < i) {
        pPre = pPre->next;
        p++;
    }
    pNext = pPre->next;
    //新建节点
    Node *pNew = (Node *) malloc(sizeof(Node));
    pNew->next = NULL;
    //拷贝数据
    pNew->data = malloc(buffer_size);
    memcpy(pNew->data, data, buffer_size);
    //插入
    pPre->next = pNew;
    pNew->next = pNext;
    pl->length++;

    return OK;
}

status deleteFromLinkList(ptrLinkList pl, int i) {
    if (i < 0 || i > pl->length - 1) return ERROR_OUT;

    Node *pPre = pl->head;
    Node *pTarget;
    int p = 0;
    while (p < i) {
        pPre = pPre->next;
        p++;
    }
    //断链
    pTarget = pPre->next;
    pPre->next = pTarget->next;
    //释放
    free(pTarget);
    pl->length--;

    return OK;
}

status setDataInLinkList(ptrLinkList pl, const void *data, int buffer_size, int i) {
    if (i < 0 || i > pl->length - 1) return ERROR_OUT;


    Node *pTarget = index(pl, i);
    //free
    free(pTarget->data);
    pTarget->data = malloc(buffer_size);

    memcpy(pTarget->data, data, buffer_size);

    return OK;
}

const void *getDataInLinkList(ptrLinkList pl, int i) {
    if (i < 0 || i > pl->length - 1) return NULL;

    Node *pTarget = index(pl, i);

    return pTarget->data;
}

int getSizeOfLinkList(ptrLinkList pl) {
    return pl->length;
}
void freeLinkList(ptrLinkList pl) {
    Node * pn = pl->head;
    while (pn) {
        Node *tmp = pn;
        pn = pn->next;
        free(tmp->data);
        free(tmp);
    }
    free(pl);
}

void clearLinkList(ptrLinkList pl) {
    Node *p = pl->head;
    Node *tmp;
    p = p->next;
    while (p) {
        tmp = p;
        p = p->next;
        free(tmp->data);
        free(tmp);
        pl->length--;
    }
    pl->head->next = NULL;

}


/**
 * 测试用以int为例 打印
 * @param pl
 */
void printLinkList(ptrLinkList pl) {
    Node *pNode = pl->head->next;
    int count = 0;
    while (pNode) {
        count++;
        printf("%s\n", (char *) (pNode->data));
//        if (count > 5) {
//            count = 0;
//            printf("\n");
//        }
        pNode = pNode->next;
    }
    printf("Num: %d\n", getSizeOfLinkList(pl));
}

/**
 * 链表测试
 * @return
 */
void TestLinkList() {
    //数据初始化
    int t[20];
    ptrLinkList pl = creatLinkList();
    printf("===追加&遍历测试===\n");
    for (int i = 0; i < 20; i++) {
        t[i] = i;
        appendToLinkList(pl, t + i, sizeof(int));
    }
    printLinkList(pl);
    printf("===插入测试===\n");
    //0插入在0
    insertIntoLinkList(pl, t, sizeof(int), 0);
    insertIntoLinkList(pl, t, sizeof(int), 10);
    insertIntoLinkList(pl, t, sizeof(int), 19);
    printLinkList(pl);
    printf("===删除测试===\n");
    deleteFromLinkList(pl, 19);
    deleteFromLinkList(pl, 10);
    deleteFromLinkList(pl, 0);
    printLinkList(pl);
    printf("===Setter&Getter测试===\n");
    setDataInLinkList(pl, t + 2, sizeof(int), 0);
    int *res = (int *) getDataInLinkList(pl, 0);
    printf("0位置：%d\n", *res);
    printLinkList(pl);
}