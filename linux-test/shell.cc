#include<iostream>    
#include<cstdio>    
#include<cstring>    
#include<string>    
#include<sys/types.h>    
#include<unistd.h>    
#include<vector>    
#include<cstdlib>    
#include<sys/wait.h>    
#include<algorithm>    
#include<sys/stat.h>    
#include<fcntl.h>    
using namespace std;    
#define COMMAND_SIZE 1024    
#define MAXARGV 128    
#define MAXENV 200    
#define ROOT_PROMPT "[%s@%s %s]# "    
#define OTHER_PROMPT "[%s@%s %s]$ "    
//重定向    
#define NONE_REDIR 0    
#define INPUT_REDIR 1    
#define OUTPUT_REDIR 2    
#define APPEND_REDIR 3    
int redir=NONE_REDIR;    
string filename;    
//命令行参数表    
char* g_argv[MAXARGV];    
int g_argc=0;    
//环境变量表    
char* g_env[MAXENV];    
int g_envc=0;    
//路径记录    
char cwd[1024];    
char envpwd[1024];    
char* GetPwd()    
{    
    char* const pwd=getcwd(cwd,sizeof(cwd));    
    if(pwd!=NULL)    
    {    
        snprintf(envpwd,sizeof(envpwd),"PWD=%s",pwd);    
        putenv(envpwd);    
    }    
    return pwd;    
}    
bool GetCommand(char out[],int size)    
{    
    char* c=fgets(out,size,stdin);    
    if(c==NULL) return false;    
    out[strlen(out)-1]=0;    
    if(strlen(out)==0) return false;    
    else return true;    
}    
string GetFileName(char arr[],int p)    
{    
    if(arr[p]==' ') p++;    
    return string(arr+p);    
}    
void PushEnv()    
{    
    for(int i=0;environ[i];i++)    
    {    
        g_env[i]=(char*)malloc(strlen(environ[i])+1);    
        strcpy(g_env[i],environ[i]);    
        g_envc++;    
    }    
    g_env[g_envc]=NULL;    
    for(int i=0;g_env[i];i++)    
        putenv(g_env[i]);  
    environ=g_env;    
}    
void Print()    
{    
    string s=GetPwd();    
    string tmp;    
    for(int i=s.size()-1;i>=0;i--)    
    {    
        if(s[i]=='/') break;    
        tmp.push_back(s[i]);    
    }    
    reverse(tmp.begin(),tmp.end());    
    if(strcmp(getenv("USER"),"root")==0)    
        printf(ROOT_PROMPT,getenv("USER"),getenv("HOSTNAME"),tmp.c_str());    
    else    
        printf(OTHER_PROMPT,getenv("USER"),getenv("HOSTNAME"),tmp.c_str());    
}    
bool AnalyseRedir(char arr[])    
{    
    redir=NONE_REDIR;    
    filename.clear();    
    int end=strlen(arr)-1;    
    while(end>=0)    
    {    
        if(arr[end]=='<')    
        {    
              redir=INPUT_REDIR;    
              arr[end++]=0;    
              filename = GetFileName(arr,end);    
              break;    
        }    
        else if(arr[end]=='>')    
        {    
            if(end-1>=0&&arr[end-1]=='>')    
            {    
                redir=APPEND_REDIR;    
                arr[end-1]=0;    
            }    
            else    
            {    
                redir=OUTPUT_REDIR;    
            }    
            filename = GetFileName(arr,end+1);    
            arr[end] = 0;    
            break;    
        }    
        else end--;     
    
    }    
    return true;    
}    
void AnalyseCommand(char* out)    
{    
#define SYMBOL " "    
    g_argc=0;    
    g_argv[g_argc++]=strtok(out,SYMBOL);    
    while((bool)(g_argv[g_argc++]=strtok(nullptr,SYMBOL)));    
    g_argv[g_argc]=0;    
}    
bool Cd()    
{    
    char* where;    
    if(g_argc==1)    
        where=getenv("HOME");    
    else    
        where=g_argv[1];    
    chdir(where);    
    return true;    
}    
bool Echo()    
{    
    //... ...
    return true;    
}    
void RunCmd()    
{    
    pid_t id=fork();    
    if(id==0)    
    {    
        //重定向文件打开+dup2    
        if(redir!=NONE_REDIR)    
        {    
            int fd=-1;    
            if(redir==INPUT_REDIR)    
            {    
                fd=open(filename.c_str(),O_RDONLY);    
                if(!(fd>=0)) exit(1);    
                dup2(fd,0);    
            }    
            else if(redir==OUTPUT_REDIR)    
            {    
                fd=open(filename.c_str(),O_CREAT | O_WRONLY | O_TRUNC,0666);    
                if(!(fd>=0)) exit(1);    
                dup2(fd,1);    
            }    
            else    
            {    
                fd=open(filename.c_str(),O_CREAT | O_WRONLY | O_APPEND,0666);    
                if(!(fd>=0)) exit(1);    
                dup2(fd,1);    
            }    
            close(fd);
        } 
        string tmp="cd";    
        if(g_argv[0]==tmp)    
        {    
            if(!Cd())  printf("-myshell: command not found\n");    
        }    
        else    
        {    
            execvp(g_argv[0],g_argv);    
            printf("-myshell: %s: command not found\n",g_argv[0]);    
            exit(1);    
        }    
    
    }    
    pid_t p=waitpid(id,NULL,0);    
    (void)p;    
}    
int main()    
{       
    //载入环境变量    
    PushEnv(); 
    while(1)    
    {    
        //命令行提示打印    
        Print();    
    
        //获取用户输入命令    
        char out[COMMAND_SIZE];    
        GetCommand(out,sizeof(out));    
    
        //重定向解析    
        AnalyseRedir(out);    
    
        //命令解析    
        AnalyseCommand(out);    
    
        //命令执行    
        RunCmd();    
    }    
    return 0;
} 
