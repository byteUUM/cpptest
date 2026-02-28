#define _CRT_SECURE_NO_WARNINGS 1
#include"Queue.h"
int main()
{
	Queue ps;
	QueueInit(&ps);
	int n = 10;
	while (n--)
		QueuePush(&ps, n + 1);
	while (QueueEmpty(&ps))
	{
		printf("%d ",QueueFront(&ps));
		QueuePop(&ps);
	}
	QueueDestroy(&ps);
	return 0;
}
