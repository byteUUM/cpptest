#pragma once
#ifdef _WIN32
#include<windows.h>
#include <process.h>
#endif
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<locale.h>
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

#define WALL "o"//地图边界
#define BODY "@"//蛇身
#define FOOD "o"//食物



// keyboard.h












#ifdef _WIN32
#define VK_M 0x4D 
#define Key(x) (GetAsyncKeyState(x) & 1)
#else
    // Linux X11 实现
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <fcntl.h>

static struct termios old_term, new_term;
// 初始化非阻塞键盘输入
static inline void init_keyboard() {
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}
// 恢复键盘设置
static inline void restore_keyboard() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}
// 检测按键
static inline bool check_key(int key_char) {
    int ch = getchar();
    if (ch != EOF) {
        ungetc(ch, stdin); // 放回字符
        return ch == key_char;
    }
    return false;
}
// 定义键码（使用ASCII码）
#define VK_SPACE  ' '
#define VK_ESCAPE 27     // ESC键ASCII码
#define VK_RETURN '\n'   // 回车键
#define VK_TAB    '\t'   // Tab键
#define VK_Q      'q'
#define VK_W      'w'
#define VK_A      'a'
#define VK_S      's'
#define VK_D      'd'
#define VK_M      'm'
#define VK_C      'c'

// 方向键（termios特殊键码）
// 需要额外处理，termios中方向键通常是转义序列
#define VK_UP     1000   // 自定义编码，比如1000-1003
#define VK_DOWN   1001
#define VK_LEFT   1002
#define VK_RIGHT  1003

// Shift/Ctrl/Alt等修饰键（在纯文本模式很难检测）
// 可以重新映射到其他键或暂时不用
#define VK_LSHIFT  'Q'   // 映射到大写Q
#define VK_RSHIFT  'W'   // 映射到大写W
#define VK_LCONTROL 'C' & 0x1F  // Ctrl+C
#define VK_RCONTROL 'X' & 0x1F  // Ctrl+X
#define VK_LALT     'A' & 0x1F  // Ctrl+A (Alt模拟)
#define VK_RALT     'S' & 0x1F  // Ctrl+S
#define Key(x) check_key(x)
#endif

















//颜色等适配
#ifdef _WIN32
#define SLEEP(x) Sleep(x)//睡眠操作
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15
#define COLOR(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x)
#define CLEAR() system("cls")//清屏操作
#define PAUSE() system("pause")
#else
#define SLEEP(x) usleep(x*1000)
#define BLACK       "\033[0;30m"
#define BLUE        "\033[0;34m"
#define GREEN       "\033[0;32m"
#define CYAN        "\033[0;36m"
#define RED         "\033[0;31m"
#define MAGENTA     "\033[0;35m"
#define YELLOW      "\033[0;33m"
#define WHITE       "\033[0;37m"
#define GRAY        "\033[1;30m"
#define LIGHT_BLUE  "\033[1;34m"
#define LIGHT_GREEN "\033[1;32m"
#define LIGHT_CYAN  "\033[1;36m"
#define LIGHT_RED   "\033[1;31m"
#define LIGHT_MAGENTA "\033[1;35m"
#define LIGHT_YELLOW "\033[1;33m"
#define BRIGHT_WHITE "\033[1;37m"
#define COLOR(x) printf(x)
#define CLEAR() system("clear")
#define PAUSE() getchar()
#endif


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

//void COLOR(int x);//颜色设置

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

#ifdef _WIN32
unsigned __stdcall th1(void* ps);
unsigned __stdcall th2(void* ps);
#else
void* th1(void* ps);
void* th2(void* ps);
#endif


//DWORD WINAPI th1(LPVOID ps);//玩家1的线程
//DWORD WINAPI th2(LPVOID ps);//玩家2的线程

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




#ifdef _WIN32
// 线程句柄
typedef HANDLE thread_t;
// 线程函数
typedef unsigned(__stdcall* thread_func_t)(void*);
// 互斥锁
typedef CRITICAL_SECTION mutex_t;
#else
typedef pthread_t thread_t;
typedef void* (*thread_func_t)(void*);
typedef pthread_mutex_t mutex_t;
#endif

// 创建线程
static inline int thread_create(thread_t* thread, thread_func_t func, void* arg) {
#ifdef _WIN32
    uintptr_t h = _beginthreadex(NULL, 0, func, arg, 0, NULL);
    if (!h) return -1;
    *thread = (HANDLE)h;
    return 0;
#else
    return pthread_create(thread, NULL, func, arg);
#endif
}


// 等待线程
static inline int thread_join(thread_t thread) {
#ifdef _WIN32
    return WaitForSingleObject(thread, INFINITE) != WAIT_OBJECT_0 ? -1 : 0;
#else
    return pthread_join(thread, NULL);
#endif
}

// 初始化锁
static inline int mutex_init(mutex_t* mutex) {
#ifdef _WIN32
    InitializeCriticalSection(mutex);
    return 0;
#else
    return pthread_mutex_init(mutex, NULL);
#endif
}

// 销毁锁
static inline void mutex_destroy(mutex_t* mutex) {
#ifdef _WIN32
    DeleteCriticalSection(mutex);
#else
    pthread_mutex_destroy(mutex);
#endif
}

// 加锁
static inline void mutex_lock(mutex_t* mutex) {
#ifdef _WIN32
    EnterCriticalSection(mutex);
#else
    pthread_mutex_lock(mutex);
#endif
}

// 解锁
static inline void mutex_unlock(mutex_t* mutex) {
#ifdef _WIN32
    LeaveCriticalSection(mutex);
#else
    pthread_mutex_unlock(mutex);
#endif
}

// 关闭线程句柄(Windows)
#ifdef _WIN32
static inline void thread_close(thread_t thread) {
    CloseHandle(thread);
}
#endif

