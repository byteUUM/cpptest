#define _CRT_SECURE_NO_WARNINGS 1
#include"snake.h"
void test()
{
	//system("COLOR 7F"); 
	CLEAR();
	COLOR(BRIGHT_WHITE);
	Snake ps1, ps2;
	ps1._pFood = NULL, ps2._pFood = NULL;
	ps1._pSnake = NULL, ps2._pSnake = NULL;
	//设置程序的本地化环境
	setlocale(LC_ALL, "");
	//随机数种子设置
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
		COLOR(BRIGHT_WHITE);
		while (1)
		{
			if (Key(VK_ESCAPE))
				return;
			if (Key(VK_M))
				break;
			SLEEP(23);
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
	COLOR(BRIGHT_WHITE);
	return 0;
}