#define _CRT_SECURE_NO_WARNINGS 1
#include"Snake.h"
void test()
{
	system("color 7F"); color(112);
	Snake ps1, ps2;
	ps1._pFood = NULL, ps2._pFood = NULL;
	ps1._pSnake = NULL, ps2._pSnake = NULL;
	setlocale(LC_ALL, "");
	srand((unsigned int)time(NULL));
	int x = 0;
	do
	{
		x = GameStart(&ps1, &ps2);
		if (x == 1)
		{
			GameRun1(&ps1);
			GameEnd(&ps1);
		}
		else
		{
			GameRun2(&ps1, &ps2);
			GameEndp(&ps1, &ps2);
		}
		SetPos(27, 16);
		printf("M键重新开始游戏,Esc键退出");
		color(112);
		while (1)
		{
			if (Key(VK_ESCAPE))
				return;
			if (Key(0x4D))
				break;
			Sleep(23);
		}
	} while (1);
	if (x == 1)
		SetPos(0, Y + 1);
	else
		SetPos(0, Y2 + 1);
}
int main()
{


	test();
	color(127);
	return 0;
}