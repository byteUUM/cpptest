#define _CRT_SECURE_NO_WARNINGS 1
#include"snake.h"
#ifdef _WIN32
int clr[15] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
#else
char* clr[15] = { "\033[0;34m","\033[0;32m","\033[0;36m","\033[0;31m"
,"\033[0;35m","\033[0;33m","\033[0;37m","\033[1;30m","\033[1;34m","\033[1;32m"
,"\033[1;36m","\033[1;31m","\033[1;35m","\033[1;33m","\033[1;37m" };
#endif
//#define Key(x) (GetAsyncKeyState(x)&1)
//void SetPos(int x, int y)//设置光标的坐标
//{
//	HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);//GetStdHandle获取柄,HANDLE接收柄
//	COORD pos = { x,y };//坐标
//	SetConsoleCursorPosition(houtput, pos);
//}
mutex_t mutex;

// 跨平台设置光标位置
void SetPos(int x, int y)
{
#ifdef _WIN32
	// Windows 实现
	HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(houtput, pos);
#else
	printf("\033[%d;%dH", y + 1, x + 1);
	fflush(stdout);
#endif
}
void Prt()
{
	SetPos(X2 + 6, 30);
	COLOR(LIGHT_BLUE); printf("研发者：CSDN博主-敲上瘾"); COLOR(BRIGHT_WHITE);
}
int menu2()
{
	SetPos(34, 11);//与menu1()的不同点在于→的位置
	printf("%s", "->");
	SetPos(36, 10);
	printf("***** "); COLOR(BRIGHT_WHITE); printf("1.单人模式 "); COLOR(BRIGHT_WHITE); printf("******");
	SetPos(36, 11);
	printf("***** "); COLOR(LIGHT_RED); printf("2.双人博弈 "); COLOR(BRIGHT_WHITE); printf("******");
	SetPos(36, 13);
	printf("请选择>>");
	return 2;
}
int menu1()
{
	SetPos(34, 10);
	printf("%s", "->");
	SetPos(36, 10);
	printf("***** "); COLOR(LIGHT_RED); printf("1.单人模式 "); COLOR(BRIGHT_WHITE); printf("******");
	SetPos(36, 11);
	printf("***** "); COLOR(BRIGHT_WHITE); printf("2.双人博弈 "); COLOR(BRIGHT_WHITE); printf("******");
	SetPos(36, 13);
	printf("请选择>>");
	return 1;
}
int Menu()
{
	Prt();
	menu1();
	int x = 1;
	SetPos(36, 9);
	printf("****"); COLOR(LIGHT_BLUE); printf("$$>*********<$$"); COLOR(BRIGHT_WHITE); printf("****");
	SetPos(36, 12);
	printf("****"); COLOR(LIGHT_BLUE); printf("$$>*********<$$"); COLOR(BRIGHT_WHITE); printf("****");
	if (Key(VK_RETURN))
		SLEEP(200);
	while (1)
	{
		SLEEP(200);
		if (Key(VK_UP))
		{
			SetPos(34, 11);
			printf("  ");
			x = menu1();
		}
		if (Key(VK_DOWN))
		{
			SetPos(34, 10);
			printf("  ");
			x = menu2();
		}
		if (Key(VK_RETURN))
			return x;
	}
}
int WelcomeToGame()//欢迎界面打印
{
	SetPos(40, 10);
	printf("欢迎来到贪吃蛇游戏!");
	SetPos(80, 28);
	PAUSE();
	int x = Menu();
	CLEAR();
	if (x == 1)
	{
		SetPos(5, 10);
		printf("请用键盘按键↑← → ↓来控制蛇的移动方向，空格键暂停，Ese键退出游戏，");
		printf("蛇吃到食物会增加长度和分数，蛇撞到自身或墙壁游戏失败,");
		printf("Tab键加速，Q键减速");
	}
	else
	{
		SetPos(5, 10);
		printf("玩家一：请用键盘按键↑← → ↓来控制蓝蛇的移动方向，Alt右键加速, Ctrl右键减速");
		SetPos(5, 12);
		printf("玩家二：请用键盘按键 W A S D 来控制绿蛇的移动方向，Alt左键加速，C键减速");
		SetPos(16, 14);
		printf("空格键暂停游戏");
	}
	SetPos(80, 28);
	PAUSE();
	CLEAR();
	return x;
}
void CreateMap1()//单人模式地图绘制
{
	for (int i = 0; i < X; i += 2) printf("%s", WALL);
	
	SetPos(0, Y);
	for (int i = 0; i < X; i += 2) printf("%s", WALL);
	for (int i = 1; i <= Y; i++)
	{
		SetPos(0, i);
		printf("%s", WALL);
	}
	for (int i = 1; i < Y; i++)
	{
		SetPos(X - 2, i);
		printf("%s", WALL);
	}
	printf("  ");
	Prt();
}
void CreateMap2()//双人模式地图绘制
{
	for (int i = 0; i < X2; i += 2) printf("%s", WALL);
	SetPos(0, Y2);
	
	for (int i = 0; i < X2; i += 2) printf("%s", WALL);
	for (int i = 1; i <= Y2; i++)
	{
		SetPos(0, i);
		printf("%s", WALL);
	}
	for (int i = 1; i < Y2; i++)
	{
		SetPos(X2 - 2, i);
		printf("%s", WALL);
	}
	Prt();
}
void InitSnake1(pSnake ps)//玩家一蛇身的初始化
{
	ps->_pSnake = NULL;//重点!!
	pSnakeNode pnew = NULL;
	for (int i = 0; i < 5; i++)
	{
		pnew = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (!pnew)
		{
			printf("InitSnake1 fail");
			exit(-1);
		}
		pnew->x = pos_x + i * 2;
		pnew->y = pos_y;
		pnew->next = NULL;
		if ((ps->_pSnake) == NULL)
		{
			ps->_pSnake = pnew;
		}
		else
		{
			pnew->next = ps->_pSnake;
			ps->_pSnake = pnew;
		}
	}
	pnew = ps->_pSnake;
	while (pnew)
	{
		SetPos(pnew->x, pnew->y);
		printf("%s", BODY);
		pnew = pnew->next;
	}
	ps->_status = OK;
	ps->_food_weight = 10;
	ps->_score = 0;
	ps->_sleep_time = 200;
	ps->_dir = RIGHT;
}
void InitSnake2(pSnake ps)//蛇身的初始化
{
	ps->_pSnake = NULL;//重点!!
	pSnakeNode pnew = NULL;
	for (int i = 0; i < 5; i++)
	{
		pnew = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (!pnew)
		{
			printf("InitSnake2 fail");
			exit(-1);
		}
		pnew->x = 24 + i * 2;
		pnew->y = 10;
		pnew->next = NULL;
		if ((ps->_pSnake) == NULL)
		{
			ps->_pSnake = pnew;
		}
		else
		{
			pnew->next = ps->_pSnake;
			ps->_pSnake = pnew;
		}
	}
	pnew = ps->_pSnake;
	while (pnew)
	{
		SetPos(pnew->x, pnew->y);
		printf("%s", BODY);
		pnew = pnew->next;
	}
	ps->_status = OK;
	ps->_food_weight = 10;
	ps->_score = 0;
	ps->_sleep_time = 200;
	ps->_dir = RIGHT;
}
void PrintFood(pSnakeNode hfood)//打印食物
{
	pSnakeNode ps = hfood;
#ifdef WIN32
	int clr[15] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
#else
	char* clr[15] = { "\033[0;34m","\033[0;32m","\033[0;36m","\033[0;31m"
,"\033[0;35m","\033[0;33m","\033[0;37m","\033[1;30m","\033[1;34m","\033[1;32m"
,"\033[1;36m","\033[1;31m","\033[1;35m","\033[1;33m","\033[1;37m" };
#endif
	int i = 0;
	while (ps)
	{
		COLOR(clr[i++]);
		SetPos(ps->x, ps->y);
		printf("%s", FOOD);
		ps = ps->next;
		if (i == 14)
			i = 0;
	}
	COLOR(BRIGHT_WHITE);
}
void CreateFood2(pSnake ps1, pSnake ps2)//双人模式食物创造
{
	int x = 0, y = 0, n = 10;
	pSnakeNode cur1 = NULL, cur2 = NULL, Food1 = NULL, hFood1 = NULL;
	pSnakeNode Food2 = NULL, hFood2 = NULL;
	pSnakeNode hfood = NULL;
	while (n--)
	{
	again:
		cur1 = ps1->_pSnake;
		cur2 = ps2->_pSnake;
		do
		{
			x = rand() % (X2 - 6) + 2;
			y = rand() % (Y2 - 4) + 1;
		} while (x % 2 != 0);
		while (cur1)
		{
			if (x == cur1->x && y == cur1->y)
			{
				goto again;
			}
			cur1 = cur1->next;
		}
		while (cur2)
		{
			if (x == cur2->x && y == cur2->y)
			{
				goto again;
			}
			cur2 = cur2->next;
		}
		while (hfood)
		{
			if (x == hfood->x && y == hfood->y)
			{
				goto again;
			}
			hfood = hfood->next;
		}
		pSnakeNode pnew = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (!pnew)
		{
			printf("CreateFood2 fail");
			exit(-1);
		}
		pnew->x = x;
		pnew->y = y;
		pnew->next = NULL;
		if (!hFood1)
		{
			Food1 = pnew;
			hFood1 = Food1;
			hfood = Food1;
		}
		else
		{
			Food1->next = pnew;
			Food1 = Food1->next;
		}
	}
	ps1->_pFood = hFood1;
	ps2->_pFood = hFood1;
	PrintFood(hFood1);
}
void CreateFood1(pSnake ps)//单人模式食物创造
{
	int x, y;
	pSnakeNode cur = NULL;
again:
	cur = ps->_pSnake;
	do
	{
		x = rand() % 53 + 2;
		y = rand() % 25 + 1;
	} while (x % 2 != 0);
	while (cur)
	{
		if (x == cur->x && y == cur->y)
		{
			goto again;
		}
		cur = cur->next;
	}
	pSnakeNode Food = (pSnakeNode)malloc(sizeof(SnakeNode));
	if (!Food)
	{
		printf("CreateFood1 fail");
		exit(-1);
	}
	Food->x = x;
	Food->y = y;
	Food->next = NULL;
	ps->_pFood = Food;
	SetPos(x, y);
	printf("%s", FOOD);
}
int GameStart(pSnake ps1, pSnake ps2)//开始游戏
{
#ifdef WIN32
	//窗口
	system("mode con cols=100 lines=35 ");
	//system("title 贪吃蛇");
	//光标隐藏
	HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO Info;
	GetConsoleCursorInfo(houtput, &Info);
	Info.bVisible = false;
	SetConsoleCursorInfo(houtput, &Info);
#else
	// Linux 隐藏光标
	printf("\033[?25l");
	fflush(stdout);
#endif
	int x = WelcomeToGame();//欢迎界面
	if (x == 2)
	{
#ifdef WIN32
		system("mode con cols=150 lines=42 ");
		//system("COLOR 7F"); COLOR(BRIGHT_WHITE);
#endif
	}
	COLOR(RED);
	if (x == 1)
		CreateMap1();//地图绘制
	else
		CreateMap2();
	COLOR(BRIGHT_WHITE);
	if (x == 1)
	{
		InitSnake1(ps1);//蛇身的初始化
		CreateFood1(ps1);
	}
	else
	{
		InitSnake1(ps1);
		InitSnake2(ps2);
		CreateFood2(ps1, ps2);//食物坐标的设定
	}
	return x;
}
void PrintHelpInfo1()//单人模式打印玩法
{
	SetPos(60, 15);
	printf("请用键盘按键↑← → ↓来控制蛇的移动方向");
	SetPos(60, 16);
	printf("蛇吃到食物会增加长度和分数");
	SetPos(60, 17);
	printf("蛇撞到自身或墙壁游戏失败");
	SetPos(60, 18);
	printf("Tab键加速，Q键减速");
	SetPos(60, 19);
	printf("空格键表示暂停，Ese键退出游戏");
	SetPos(60, 1);
}
void PrintHelpInfo2()//双人模式打印玩法
{
	SetPos(X2 + 6, 15);
	printf("玩家1请用键盘按键↑← → ↓来控制蛇的移动方向");
	SetPos(X2 + 6, 16);
	printf("玩家2请用键盘按键 W A S D  来控制蛇的移动方向");
	SetPos(X2 + 6, 18);
	printf("蛇吃到食物会增加长度和分数");
	SetPos(X2 + 6, 19);
	printf("蛇撞到自己或另一玩家不会死亡,蛇撞墙壁游戏失败");
	SetPos(X2 + 6, 20);
	printf("Shift加速，Ctrl减速");
	SetPos(X2 + 6, 21);
	printf("空格键表示暂停，Ese键退出游戏");
	SetPos(X2 + 6, 1);
}
void SnakeMove1(pSnake ps)////////////单人模式
{
	pSnakeNode pnew = (pSnakeNode)malloc(sizeof(SnakeNode));
	if (!pnew)
	{
		printf("SnakeMove1 fail");
		exit(-1);
	}
	switch (ps->_dir)
	{
	case UP:
		pnew->y = ps->_pSnake->y - 1;
		pnew->x = ps->_pSnake->x;
		break;
	case DOWN:
		pnew->y = ps->_pSnake->y + 1;
		pnew->x = ps->_pSnake->x;
		break;
	case LEFT:
		pnew->x = ps->_pSnake->x - 2;
		pnew->y = ps->_pSnake->y;
		break;
	case RIGHT:
		pnew->x = ps->_pSnake->x + 2;
		pnew->y = ps->_pSnake->y;
		break;
	}
	pnew->next = ps->_pSnake;
	ps->_pSnake = pnew;
	if ((pnew->x == ps->_pFood->x) && (pnew->y == ps->_pFood->y))
	{
		while (pnew)
		{
			SetPos(pnew->x, pnew->y);
			COLOR(RED);
			printf("%s", BODY);
			pnew = pnew->next;
		}
		COLOR(BRIGHT_WHITE);
		ps->_score += ps->_food_weight;
		free(ps->_pFood);
		printf("\a");
		CreateFood1(ps);
	}
	else
	{
		while (pnew->next->next)
		{
			SetPos(pnew->x, pnew->y);
			COLOR(LIGHT_YELLOW);
			printf("%s", BODY);
			pnew = pnew->next;
		}
		COLOR(BRIGHT_WHITE);
		SetPos(pnew->next->x, pnew->next->y);
		printf("  ");
		free(pnew->next);
		pnew->next = NULL;
	}
	SLEEP(ps->_sleep_time);
}
void ChangeFood(pSnake ps1, pSnake ps2, pSnakeNode ps)//改变食物位置,ps为需要销毁或改变的食物
{
	int sum = 0;
	pSnakeNode pfd = ps1->_pFood;
	while (pfd)
	{
		sum++;
		pfd = pfd->next;
	}
	pfd = ps1->_pFood;
	if (sum > 10)
	{
		while (pfd && pfd->next != ps)
			pfd = pfd->next;
		assert(pfd && pfd->next);
		pfd->next = pfd->next->next;
		free(ps);
		return;
	}
	pSnakeNode cur1 = NULL, cur2 = NULL, hfood = ps2->_pFood;
	int x = 0, y = 0;
Again:
	cur1 = ps1->_pSnake;
	cur2 = ps2->_pSnake;
	do
	{
		x = rand() % (X2 - 6) + 2;
		y = rand() % (Y2 - 4) + 1;
	} while (x % 2 != 0);
	while (cur1)
	{
		if (x == cur1->x && y == cur1->y)
		{
			goto Again;
		}
		cur1 = cur1->next;
	}
	while (cur2)
	{
		if (x == cur2->x && y == cur2->y)
		{
			goto Again;
		}
		cur2 = cur2->next;
	}
	while (hfood)
	{
		if (x == hfood->x && y == hfood->y)
		{
			goto Again;
		}
		hfood = hfood->next;
	}
	ps->x = x, ps->y = y;
}
void Resurrect(pSnake pt, pSnake pn)//玩家复活
{
	assert(pt);
	pSnakeNode pff = pt->_pFood;
	while (pff->next)
	{
		pff = pff->next;
	}
	pff->next = pt->_pSnake->next;//蛇身变成食物
	pt->_pSnake = NULL;
	PrintFood(pt->_pFood);//打印食物
	int x = 0, y = 0;//复活坐标
reset:
	do
	{
		x = rand() % (X2 - 4) + 2;
		y = rand() % (Y2 - 1) + 1;
	} while (x % 2 != 0);//复活位置为偶数
	for (int i = 0; i < 5; i++)
	{
		y += i;
		pSnakeNode ph = pn->_pSnake;
		while (ph)//检查复活位置是否与对方玩家相撞
		{
			if (ph->x == x && ph->y == y)
				goto reset;
			ph = ph->next;
		}
		pSnakeNode pfd = pt->_pFood;
		while (pfd)//检查复活位置是否是食物
		{
			if (pfd->x == x && pfd->y == y)
				goto reset;
			pfd = pfd->next;
		}
		if ((y <= 0) || (y + 12 >= Y2)//检查复活位置是否是地图外
			|| (x <= 0) || (x + 12 >= X2 - 2))
			goto reset;
	}
	//检查复活位置合法后做成链表进行维护
	pSnakeNode pnew = NULL;
	for (int i = 0; i < 5; i++)
	{
		pnew = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (!pnew)
		{
			exit(-1);
		}
		pnew->x = x;
		pnew->y = y + i;
		pnew->next = NULL;
		if ((pt->_pSnake) == NULL)
		{
			pt->_pSnake = pnew;
		}
		else
		{
			pnew->next = pt->_pSnake;
			pt->_pSnake = pnew;
		}
	}
	pnew = pt->_pSnake;
	while (pnew)
	{
		SetPos(pnew->x, pnew->y);
		printf("%s", BODY);
		pnew = pnew->next;
	}
	pt->_status = OK;
	pt->_food_weight = 10;
	pt->_score = 0;
	pt->_sleep_time = 200;
	pt->_dir = RIGHT;
}
pSnakeNode OpFood(pSnakeNode ps, pSnakeNode hfood)//判断是否为食物
{
	pSnakeNode hf = hfood;
	while (hf)
	{
		if (ps->x == hf->x && ps->y == hf->y)
			return hf;
		hf = hf->next;
	}
	return NULL;
}
//玩家1///////////////////////
//DWORD WINAPI th1(LPVOID ps)
//{
#ifdef _WIN32
unsigned __stdcall th1(void* ps) {
#else
void* th1(void* ps) {
#endif
	pLSnake psk = (pLSnake)ps;
	pSnake pw1 = NULL, pw2 = NULL;
	pw1 = psk->p1;
	pw2 = psk->p2;
	int fup = 0;
	do
	{
		mutex_lock(&mutex);
		COLOR(BRIGHT_WHITE);
		SetPos(X2 + 6, 8);
		printf("玩家1 绿蛇");
		mutex_unlock(&mutex);

		mutex_lock(&mutex);
		SetPos(X2 + 6, 9);
		if (fup == 1)
		{
			printf("食物分数："); COLOR(RED); printf("%2d", pw1->_food_weight); COLOR(BRIGHT_WHITE);
		}
		else if (fup == -1)
		{
			printf("食物分数："); COLOR(LIGHT_GREEN); printf("%2d", pw1->_food_weight); COLOR(BRIGHT_WHITE);
		}
		else
		{
			printf("食物分数：%2d", pw1->_food_weight);
		}
		mutex_unlock(&mutex);

		mutex_lock(&mutex);
		COLOR(BRIGHT_WHITE);
		SetPos(X2 + 6, 10);
		printf("总得分:%d", pw1->_score);
		mutex_unlock(&mutex);
		if (Key(VK_UP) && pw1->_dir != DOWN)
			pw1->_dir = UP;
		else if (Key(VK_DOWN) && pw1->_dir != UP)
			pw1->_dir = DOWN;
		else if (Key(VK_LEFT) && pw1->_dir != RIGHT)
			pw1->_dir = LEFT;
		else if (Key(VK_RIGHT) && pw1->_dir != LEFT)
			pw1->_dir = RIGHT;
		if (Key(VK_SPACE))
		{
			mutex_lock(&mutex);
			SetPos(29, 13);
			printf("游戏已暂停，点击空格键继续");
			while (1)
			{
				SLEEP(200);
				if (Key(VK_SPACE))
					break;
			}
			int x = 26;
			SetPos(29, 13);
			while (x--)
				printf(" ");
			mutex_unlock(&mutex);
		}
		if (Key(VK_RSHIFT))
		{
			if (pw1->_sleep_time >= 80)
			{
				pw1->_sleep_time -= 30;
				pw1->_food_weight += 2;
				fup = 1;
			}
		}
		if (Key(0XA3))
		{
			if (pw1->_sleep_time <= 320)
			{
				pw1->_sleep_time += 30;
				pw1->_food_weight -= 2;
				fup = -1;
			}
		}
		///////////////
		pSnakeNode pnew = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (!pnew || !pw1->_pSnake)
		{
			printf("th1 fail");
			exit(-1);
		}
		switch (pw1->_dir)
		{
		case UP:
			pnew->y = pw1->_pSnake->y - 1;
			pnew->x = pw1->_pSnake->x;
			break;
		case DOWN:
			pnew->y = pw1->_pSnake->y + 1;
			pnew->x = pw1->_pSnake->x;
			break;
		case LEFT:
			pnew->x = pw1->_pSnake->x - 2;
			pnew->y = pw1->_pSnake->y;
			break;
		case RIGHT:
			pnew->x = pw1->_pSnake->x + 2;
			pnew->y = pw1->_pSnake->y;
			break;
		}
		pnew->next = pw1->_pSnake;
		assert(pw1->_pSnake);
		pw1->_pSnake = pnew;
		KillByselfp(pnew, pw2);
		KillByWallp(pw1);
		pSnakeNode k = OpFood(pnew, pw1->_pFood);
		int x1 = X2, y1 = Y2;
		if (k)
		{
			//打印加分
			x1 = pnew->x + 2;
			y1 = pnew->y + 2;
			mutex_lock(&mutex);
			SetPos(x1, y1);
			COLOR(RED);
			printf("+%d", pw1->_food_weight);
			mutex_unlock(&mutex);

			int i = 0;
			while (pnew)
			{
				mutex_lock(&mutex);
				SetPos(pnew->x, pnew->y);
				printf("%s", BODY);
				mutex_unlock(&mutex);
				COLOR(clr[i++]);
				pnew = pnew->next;
				if (i == 15) i = 0;
			}
			COLOR(BRIGHT_WHITE);
			printf("\a");
			mutex_lock(&mutex);
			ChangeFood(pw1, pw2, k);
			PrintFood(pw1->_pFood);
			mutex_unlock(&mutex);
			pw1->_score += pw1->_food_weight;
		}
		else
		{
			if (!pnew->next)
			{
				exit(-1);
			}
			while (pnew->next->next)
			{
				mutex_lock(&mutex);
				SetPos(pnew->x, pnew->y);
				COLOR(LIGHT_GREEN);
				if (pnew == pw1->_pSnake)
					printf("%s", BODY);
				else
					printf("%s", BODY);
				mutex_unlock(&mutex);
				pnew = pnew->next;
			}
			COLOR(BRIGHT_WHITE);

			mutex_lock(&mutex);
			SetPos(pnew->next->x, pnew->next->y);
			printf("  ");
			mutex_unlock(&mutex);
			free(pnew->next);
			pnew->next = NULL;
		}
		SLEEP(pw1->_sleep_time);

		mutex_lock(&mutex);
		SetPos(x1, y1);
		printf("   ");
		mutex_unlock(&mutex);
		if (pw2->_status == KILL_BY_SELF)
		{
			mutex_lock(&mutex);
			Resurrect(pw1, pw2);
			pw2->_status = OK;
			mutex_unlock(&mutex);
		}
		//CreateMap3();
	} while (pw1->_status != KILL_BY_WALL && pw2->_status != KILL_BY_WALL);
	return 0;
}
//玩家2///////////////////////
//DWORD WINAPI th2(LPVOID ps)
//{
#ifdef _WIN32
unsigned __stdcall th2(void* ps) {
#else
void* th2(void* ps) {
#endif
	pLSnake psk = (pLSnake)ps;
	pSnake pw1 = NULL, pw2 = NULL;
	pw1 = psk->p1;
	pw2 = psk->p2;
	int fp = 0;
	do {
		mutex_lock(&mutex);//上锁
		COLOR(BRIGHT_WHITE);//设置颜色
		SetPos(X2 + 6, 12);//坐标设置
		printf("玩家2 蓝蛇");//打印信息
		mutex_unlock(&mutex);//解锁

		mutex_lock(&mutex);
		SetPos(X2 + 6, 13);
		if (fp == 1)
		{
			printf("食物分数："); COLOR(RED); printf("%2d", pw2->_food_weight); COLOR(BRIGHT_WHITE);
		}
		else if (fp == -1)
		{
			printf("食物分数："); COLOR(LIGHT_GREEN); printf("%2d", pw2->_food_weight); COLOR(BRIGHT_WHITE);
		}
		else
		{
			printf("食物分数：%2d", pw2->_food_weight);
		}
		mutex_unlock(&mutex);

		mutex_lock(&mutex);
		COLOR(BRIGHT_WHITE);
		SetPos(X2 + 6, 14);
		printf("总得分:%2d", pw2->_score);
		mutex_unlock(&mutex);

		if (Key(0x57) && pw2->_dir != DOWN)
			pw2->_dir = UP;
		else if (Key(0x53) && pw2->_dir != UP)
			pw2->_dir = DOWN;
		else if (Key(0x41) && pw2->_dir != RIGHT)
			pw2->_dir = LEFT;
		else if (Key(0x44) && pw2->_dir != LEFT)
			pw2->_dir = RIGHT;
		if (Key(0xA4))
		{
			if (pw2->_sleep_time >= 80)
			{
				pw2->_sleep_time -= 30;
				pw2->_food_weight += 2;
				fp = 1;
			}
		}
		if (Key(0x43))
		{
			if (pw2->_sleep_time <= 320)
			{
				pw2->_sleep_time += 30;
				pw2->_food_weight -= 2;
				fp = -1;
			}
		}
		pSnakeNode pnew = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (!pnew || !pw2->_pSnake)
		{
			exit(-1);
		}
		switch (pw2->_dir)
		{
		case UP:
			pnew->y = pw2->_pSnake->y - 1;
			pnew->x = pw2->_pSnake->x;
			break;
		case DOWN:
			pnew->y = pw2->_pSnake->y + 1;
			pnew->x = pw2->_pSnake->x;
			break;
		case LEFT:
			pnew->x = pw2->_pSnake->x - 2;
			pnew->y = pw2->_pSnake->y;
			break;
		case RIGHT:
			pnew->x = pw2->_pSnake->x + 2;
			pnew->y = pw2->_pSnake->y;
			break;
		}
		pnew->next = pw2->_pSnake;
		pw2->_pSnake = pnew;

		KillByselfp(pnew, pw1);

		KillByWallp(pw2);

		pSnakeNode k = OpFood(pnew, pw2->_pFood);
		int x2 = X2, y2 = Y2;
		if (k)
		{
			x2 = pnew->x + 2;
			y2 = pnew->y + 2;
			mutex_lock(&mutex);
			SetPos(x2, y2);
			COLOR(RED);
			printf("+%d", pw2->_food_weight);
			mutex_unlock(&mutex);
			int i = 0;
			while (pnew)
			{
				mutex_lock(&mutex);
				SetPos(pnew->x, pnew->y);
				COLOR(clr[i++]);
				printf("%s", BODY);
				mutex_unlock(&mutex);
				pnew = pnew->next;
				if (i == 15) i = 0;
			}
			COLOR(BRIGHT_WHITE);
			printf("\a");
			mutex_lock(&mutex);
			ChangeFood(pw1, pw2, k);
			PrintFood(pw2->_pFood);
			mutex_unlock(&mutex);
			pw2->_score += pw2->_food_weight;
		}
		else
		{
			if (!pnew->next)
			{
				exit(-1);
			}
			while (pnew->next->next)
			{
				mutex_lock(&mutex);
				SetPos(pnew->x, pnew->y);
				COLOR(LIGHT_BLUE);
				if (pnew == pw2->_pSnake)
					printf("%s", BODY);
				else
					printf("%s", BODY);
				mutex_unlock(&mutex);
				pnew = pnew->next;
			}
			mutex_lock(&mutex);
			SetPos(pnew->next->x, pnew->next->y);
			printf("  ");
			mutex_unlock(&mutex);
			free(pnew->next);
			pnew->next = NULL;
		}
		SLEEP(pw2->_sleep_time);
		mutex_lock(&mutex);
		SetPos(x2, y2);
		printf("   ");
		mutex_unlock(&mutex);
		if (pw1->_status == KILL_BY_SELF)
		{
			mutex_lock(&mutex);
			Resurrect(pw2, pw1);
			mutex_unlock(&mutex);
			pw1->_status = OK;
		}
		//CreateMap3();
	} while (pw2->_status != KILL_BY_WALL && pw1->_status != KILL_BY_WALL);
	return 0;  // 可返回任意值
}
void KillByWall(pSnake ps)//判断是否撞墙
{
	if ((ps->_pSnake->y == 0) || (ps->_pSnake->y == Y)
		|| (ps->_pSnake->x == 0) || (ps->_pSnake->x == X - 2))
		ps->_status = KILL_BY_WALL;
}
void KillByWallp(pSnake ps)//判断是否撞墙
{
	if ((ps->_pSnake->y == 0) || (ps->_pSnake->y == Y2)
		|| (ps->_pSnake->x == 0) || (ps->_pSnake->x == X2 - 2))
		ps->_status = KILL_BY_WALL;
}
void KillByself(pSnake ps)//判断是否撞到自己
{
	pSnakeNode pr = ps->_pSnake;
	pSnakeNode pu = ps->_pSnake->next;
	while (pu)
	{
		if (pu->x == pr->x && pu->y == pr->y)
		{
			ps->_status = KILL_BY_SELF;
			return;
		}
		pu = pu->next;
	}
}
void KillByselfp(pSnakeNode pnew, pSnake ps)//判断是否撞到对方
{
	pSnakeNode pu = ps->_pSnake;
	while (pu)
	{
		if (pnew->x == pu->x && pnew->y == pu->y)
		{
			ps->_status = KILL_BY_SELF;
			return;
		}
		pu = pu->next;
	}
}
void GameRun1(pSnake ps)//游戏运行
{
	PrintHelpInfo1();
	int fuk = 0;
	do
	{
		SetPos(64, 12);
		printf("总得分:%d", ps->_score);
		SetPos(64, 13);
		if (fuk == 1)
		{
			printf("食物分数："); COLOR(RED); printf("%2d", ps->_food_weight); COLOR(BRIGHT_WHITE);
		}
		else if (fuk == -1)
		{
			printf("食物分数："); COLOR(LIGHT_GREEN); printf("%2d", ps->_food_weight); COLOR(BRIGHT_WHITE);
		}
		else
		{
			printf("食物分数：%2d", ps->_food_weight);
		}
		if (Key(VK_UP) && ps->_dir != DOWN)
			ps->_dir = UP;
		else if (Key(VK_DOWN) && ps->_dir != UP)
			ps->_dir = DOWN;
		else if (Key(VK_LEFT) && ps->_dir != RIGHT)
			ps->_dir = LEFT;
		else if (Key(VK_RIGHT) && ps->_dir != LEFT)
			ps->_dir = RIGHT;
		if (Key(VK_SPACE))
		{
			while (1)
			{
				SetPos(29, 13);
				printf("游戏已暂停，点击空格键继续");
				if (Key(VK_SPACE))
					break;
				SLEEP(300);
			}
			SetPos(29, 13);
			int x = 26;
			while (x--)
				printf(" ");
		}
		if (Key(VK_ESCAPE))
			ps->_status = END_NORMAL;
		if (Key(0x09))
		{
			if (ps->_sleep_time >= 80)//if (ps->_sleep_time >= 30)
			{
				ps->_sleep_time -= 30;
				ps->_food_weight += 2;
				fuk = 1;
			}
		}
		if (Key(0x51))
		{
			if (ps->_sleep_time <= 320)
			{
				ps->_sleep_time += 30;
				ps->_food_weight -= 2;
				fuk = -1;
			}
		}
		SnakeMove1(ps);
		KillByself(ps);
		KillByWall(ps);
	} while (ps->_status == OK);
}
//void GameRun2(pSnake pu1, pSnake pu2)
//{
//	pLSnake pm = (pLSnake)malloc(sizeof(LSnake));
//	pLSnake psk = pm;
//	if (!psk)
//	{
//		exit(-1);
//	}
//	psk->p1 = pu1;
//	psk->p2 = pu2;
//	HANDLE thp1 = NULL, thp2 = NULL;
//	// 初始化临界区
//	InitializeCriticalSection(&cs);
//
//	// 创建线程
//	thp1 = CreateThread(NULL, 0, th1, (LPVOID)psk, 0, NULL);//玩家1
//	thp2 = CreateThread(NULL, 0, th2, (LPVOID)psk, 0, NULL);//玩家2
//	if (!thp1 || !thp2)
//	{
//		exit(-1);
//	}
//
//	// 等待线程结束
//	WaitForSingleObject(thp1, INFINITE);
//	WaitForSingleObject(thp2, INFINITE);
//
//	// 销毁临界区
//	DeleteCriticalSection(&cs);
//
//	// 关闭线程句柄
//	CloseHandle(thp1);
//	CloseHandle(thp2);
//}
void GameRun2(pSnake pu1, pSnake pu2)
{
	pLSnake pm = (pLSnake)malloc(sizeof(LSnake));
	pLSnake psk = pm;
	if (!psk) exit(-1);
	psk->p1 = pu1;
	psk->p2 = pu2;
	thread_t t1, t2;
	// 初始化锁
	mutex_init(&mutex);
	// 创建线程
	thread_create(&t1, th1, (void*)psk);
	thread_create(&t2, th2, (void*)psk);

	// 等待线程
	thread_join(t1);
	thread_join(t2);

	// 销毁锁
	mutex_destroy(&mutex);

#ifdef _WIN32
	thread_close(t1);
	thread_close(t2);
#endif
}

void GameEnd(pSnake ps)//游戏结束善后处理
{
	SetPos(29, 13);
	if (ps->_status == KILL_BY_SELF)
	{
		COLOR(RED);
		printf("你撞到了自己，游戏结束");
	}
	if (ps->_status == KILL_BY_WALL)
	{
		COLOR(RED);
		printf("你撞到了墙，游戏结束");
	}
	free(ps->_pFood);
	while (ps->_pSnake)
	{
		pSnakeNode cur = ps->_pSnake->next;
		free(ps->_pSnake);
		ps->_pSnake = cur;
	}
}
void GameEndp(pSnake ps1, pSnake ps2)
{
	SetPos(29, 13);
	if (ps1->_status == KILL_BY_WALL)
	{
		COLOR(RED);
		printf("玩家1撞到了墙，玩家2获胜");
	}
	while (ps1->_pSnake)
	{
		pSnakeNode cur = ps1->_pSnake->next;
		free(ps1->_pSnake);
		ps1->_pSnake = cur;
	}
	SetPos(29, 14);
	if (ps2->_status == KILL_BY_WALL)
	{
		COLOR(RED);
		printf("玩家2撞到了墙，玩家1获胜");
	}
	SetPos(29, 15);
	if (ps2->_status == KILL_BY_SELF)
	{
		COLOR(RED);
		printf("玩家1死亡，玩家2获胜");
	}
	SetPos(29, 15);
	if (ps1->_status == KILL_BY_SELF)
	{
		COLOR(RED);
		printf("玩家2死亡，玩家1获胜");
	}
	free(ps2->_pFood);
	while (ps2->_pSnake)
	{
		pSnakeNode cur = ps2->_pSnake->next;
		free(ps2->_pSnake);
		ps2->_pSnake = cur;
	}
}