#include <iostream>
#include <cstring>
using namespace std;
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        cout<<"input error"<<endl;
        exit(0);
    }
    FILE* fp = fopen(argv[1],"r");
    if(!fp)
    {
        cout<<argv[1]<<" open fail"<<endl;
        exit(0);
    }
    char bf;
    while(fread(&bf,1,sizeof(char),fp))
    {
        cout<<bf;
    }
    return 0;
}

// #include <stdio.h>
// #include <string.h>
// int main(int argc, char* argv[])
// {
//     if (argc != 2)
//     {
//         printf("argv error!\n");
//         return 1;
//     } 
//     FILE *fp = fopen(argv[1], "r");
//     if(!fp){
//         printf("fopen error!\n");
//         return 2;
//     } 
//     char buf[1024];
//     while(1){
//     //fread返回成功读取元素的个数
//         int s = fread(buf, 1, sizeof(buf), fp);
//         if(s > 0){
//             buf[s] = 0;
//             printf("%s", buf);
//         } 
//         if(feof(fp)){
//             break;
//         }
//     }
//     fclose(fp);
//     return 0;
// }