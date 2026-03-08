#include<stdio.h>
#include"Heap.h"
void CreateNDate()
{
	//造数据，写入文件中
	int n = 10000;
	srand((unsigned int)time(NULL));
	const char* file = "data.txt";
	FILE* fin = fopen(file, "w");
	if (fin == NULL)
	{
		perror("fopen error");
		return;
	}
	for (int i = 0; i < n; ++i)
	{
		int x = rand() % 1000000;
		fprintf(fin, "%d\n", x);
	}
	fclose(fin);
}
void PrintTopK(int k)
{
	int* arr = (int*)malloc(sizeof(int) * k);
	assert(arr);
	FILE* fop = fopen("data.txt", "r");
	if (!fop)
	{
		perror("fopen error");
		return;
	}
	for (int i = 0; i < k; i++)//先取出k个建大堆
		fscanf(fop, "%d", &arr[i]);
	for (int i = (k - 1 - 1) / 2; i >= 0; i--)
		AdjustDOWN(arr, k, i);
	int x = 0;
	while (fscanf(fop, "%d", &x) != EOF)
	{
		if (arr[0] > x)
		{
			arr[0] = x;
			AdjustDOWN(arr, k, 0);
		}
	}
	for (int i = 0; i < k; i++)//输出堆中元素
		printf("%d ", arr[i]);
}
int main()
{
	CreateNDate();
	int k = 0;
	scanf("%d", &k);
	PrintTopK(k);
	return 0;
}
