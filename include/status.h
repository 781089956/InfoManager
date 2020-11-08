#ifndef STATUS_H_INCLUDED
#define STATUS_H_INCLUDED

#define OK 0x1u  //成功
#define ERROR_TYPE OK << 2u //类型失败
#define ERROR_SYNTAX OK << 3u //语法失败
#define ERROR_OUT OK << 4u //越界失败
#define ERROR_OPEN OK << 5u //文件打开失败
#define ERROR_NF OK << 6u //文件打开失败
#define ERROR_NULL_POINTER OK << 7u //空指针
#define ERROR_NEST OK << 8u //嵌套错误

typedef unsigned int status;

#endif // STATUS_H_INCLUDED
