#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int g_unval;
int g_val = 100;
int main(int argc, char *argv[], char *env[])
{
    const char *str = "helloworld";//静态区（代码块）

    printf("code addr: %p\n", main);//栈区?
    printf("init global addr: %p\n", &g_val);//静态区（已初始化）
    printf("uninit global addr: %p\n", &g_unval);//静态区（未初始化区）
    static int test = 10;
    
    //堆区结构
    //这是指针（动态开辟的），所以指向的地址是堆区
    char *heap_mem = (char*)malloc(10);
    char *heap_mem1 = (char*)malloc(10);
    char *heap_mem2 = (char*)malloc(10);
    char *heap_mem3 = (char*)malloc(10);
    printf("heap addr: %p\n", heap_mem); 
    printf("heap addr: %p\n", heap_mem1); 
    printf("heap addr: %p\n", heap_mem2); 
    printf("heap addr: %p\n", heap_mem3);
    printf("test static addr: %p\n", &test); //静态区（已初始化）
    
    //栈区，指针变量！所存储在的区域是栈区
    printf("stack addr: %p\n", &heap_mem);
    printf("stack addr: %p\n", &heap_mem1);
    printf("stack addr: %p\n", &heap_mem2); 
    printf("stack addr: %p\n", &heap_mem3);
    
    printf("read only string addr: %p\n", str);//静态区（代码块）
    
    for(int i = 0 ;i < argc; i++) printf("argv[%d]: %p\n", i, argv[i]);//这是实参，在函数栈针里，属于栈区
    for(int i = 0; env[i]; i++) printf("env[%d]: %p\n", i, env[i]);//同样的是栈区
    return 0;
}