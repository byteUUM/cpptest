#include"Queue.h"
void QueueInit(Queue* pQ)
{
	assert(pQ);
	pQ->pend = pQ->phead = NULL;
	pQ->size = 0;
}
void QueuePush(Queue* pQ, QueueData x)
{
	assert(pQ);
	QueueNode* pnew = (QueueNode*)malloc(sizeof(QueueNode));
	assert(pnew);
	pnew->val = x;
	pnew->next = NULL;
	if (pQ->size == 0)
	{
		pQ->pend = pQ->phead = pnew;
	}
	else
	{
		pQ->pend->next = pnew;
		pQ->pend = pnew;
	}
	pQ->size++;
}
QueueData QueueFront(Queue* pQ)
{
 
	assert(pQ);
	assert(pQ->size);
	return pQ->phead->val;
}
void QueuePop(Queue* pQ)
{
	assert(pQ);
	QueueData ret = pQ->phead->val;
	if (pQ->size == 1)
	{
		free(pQ->phead);
		pQ->pend = pQ->phead = NULL;
	}
	else
	{
		QueueNode* pn = pQ->phead;
		pQ->phead = pQ->phead->next;
		free(pn);
	}
	pQ->size--;
	return ret;
}
int QueueSize(Queue* pQ)
{
	assert(pQ);
	return pQ->size;
}
bool QueueEmpty(Queue* pQ)//判空
{
	assert(pQ);
	return pQ->size;
}
void QueueDestroy(Queue* pQ)
{
	assert(pQ);
	while (pQ->phead)
	{
		QueueNode* pnext = pQ->phead->next;
		free(pQ->phead);
		pQ->phead=pnext;
	}
	pQ->pend = pQ->phead = NULL;
	pQ->size = 0;
}
