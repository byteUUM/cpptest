#include<stdio.h>
#if 0
int main()
{
    int* p;
    printf("%ld",sizeof p);
    return 0;
}
int main()
{
    char *c[] = {"ENTER","NEW","POINT","FIRST"};
    char**cp[] = {c+3,c+2,c+1,c};
    char***cpp = cp;
    printf("%s\n", **++cpp);
    printf("%s\n", *--*++cpp+3);
    printf("%s\n", *cpp[-2]+3);
    printf("%s\n", cpp[-1][-1]+1);
    return 0;
}
//判断大小端
int main()
{
    int a = 1;
    if(*(char*)&a==1) printf("小端\n");
    else printf("大端\n");
    return 0;
}
#endif
int main()
{
int n = 9;
float *pFloat = (float *)&n;
printf("n的值为：%d\n",n);
printf("*pFloat的值为：%f\n",*pFloat);
*pFloat = 9.0;
printf("num的值为：%d\n",n);
printf("*pFloat的值为：%f\n",*pFloat);
return 0;
}