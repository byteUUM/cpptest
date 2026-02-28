#pragma once
#include<stdio.h>
#include<stdbool.h>
#include<assert.h>
#include<stdlib.h>
#define QueueData int
 
typedef struct QueueNode
{
	QueueData val;
	struct QueueNode* next;
}QueueNode;
typedef struct
{
	QueueNode* phead;
	QueueNode* pend;
	int size;
}Queue;//储存的是链表的头节点和链表的尾节点以及队列的元素个数，更容易维护队列。
void QueueInit(Queue* pQ);//初始化
void QueuePush(Queue* pQ, QueueData x);//队尾入队
void QueuePop(Queue* pQ);//出队
QueueData QueueFront(Queue* pQ);//取队头元素
int QueueSize(Queue* pQ);//获取队列元素个数
bool QueueEmpty(Queue* pQ);//判空
void QueueDestroy(Queue* pQ);//销毁队列
