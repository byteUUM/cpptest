#pragma
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<locale.h>
#include<windows.h>
#include<stdbool.h>
#include<time.h>
//蛇的初始坐标
#define pos_x 24
#define pos_y 5

//单人模式 地图坐标
#define X 58
#define Y 26

//双人模式 地图坐标
#define X2 98
#define Y2 38

#define WALL L'□'//地图边界
#define BODY L'●'//蛇身
#define FOOD L'★'//食物
#define Key(x) (GetAsyncKeyState(x)&1)//键盘敲击信息读取

CRITICAL_SECTION cs;

enum DIRECTION//蛇的方向
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
};
enum GAME_STATUS
{
	OK,//正常
	KILL_BY_WALL,//撞墙
	KILL_BY_SELF,//撞到自己
	END_NORMAL//正常退出
};
typedef struct SnakeNode
{
	//坐标
	int x;
	int y;
	struct SnakeNode* next;
}SnakeNode, * pSnakeNode;
typedef struct Snake
{
	pSnakeNode _pSnake;//蛇的坐标
	pSnakeNode _pFood;//食物的坐标
	enum DIRECTION _dir;//蛇的移动方向
	enum GAME_STATUS _status;//蛇的状态
	int _food_weight;//一个食物的分数
	int _score;//分数
	int _sleep_time;//蛇的速度
}Snake, * pSnake;
typedef struct LSnake
{
	pSnake p1;
	pSnake p2;
	int x;
}LSnake, * pLSnake;

int Menu();//菜单打印
void Prt();

void color(int x);//颜色设置

void SetPos(int x, int y);//光标位置设定

int GameStart(pSnake ps1, pSnake ps2); //游戏开始
int WelcomeToGame();//欢迎界面

void PrintHelpInfo1();//单人模式 玩法说明
void PrintHelpInfo2();//双人模式 玩法说明

void CreateMap1();//单人模式 地图绘制
void CreateMap2();//双人模式 地图绘制

void InitSnake1(pSnake ps);//玩家1蛇身的初始化
void InitSnake2(pSnake ps);//玩家2蛇身的初始化

void CreateFood1(pSnake ps);//单人模式 食物坐标的设定
void CreateFood2(pSnake ps1, pSnake ps2);//双人模式 食物坐标的设定
void PrintFood(pSnakeNode hfood);//打印食物

DWORD WINAPI th1(LPVOID ps);//玩家1的线程
DWORD WINAPI th2(LPVOID ps);//玩家2的线程

void GameRun1(pSnake ps);//单人模式 游戏运行主逻辑
void GameRun2(pSnake ps1, pSnake ps2);//双人模式 游戏运行主逻辑

void KillByWallp(pSnake ps1);
void KillByWall(pSnake ps);//判断是否撞墙
void KillByself(pSnake ps);//判断是否撞到自己
void KillByselfp(pSnakeNode pnew, pSnake ps);//是否撞到对方玩家身体

pSnakeNode OpFood(pSnakeNode ps, pSnakeNode hfood);//判断是否吃到食物

void SnakeMove1(pSnake ps);//单人模式 蛇的移动

void Resurrect(pSnake pt, pSnake pn);//玩家复活

void GameEnd(pSnake ps);//单人模式 游戏结束善后
void GameEndp(pSnake ps1, pSnake ps2);//双人模式 游戏结束善后
