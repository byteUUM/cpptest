// #include <stdio.h>
// int main()
// {
//     int a = 10;
//     printf("%zd\n", sizeof(a));
//     printf("%zd\n", sizeof a);//a是变量的名字，可以省略掉sizeof后边的()
//     printf("%zd\n", sizeof(int));
//     printf("%zd\n", sizeof(3 + 3.5));
//     return 0;
// }
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    char arr1[] = "welcome to bit...";
    char arr2[] = "#################";
    int left = 0;
    int right = strlen(arr1)-1;
    printf("%s\n", arr2);
    while(left<=right)
    {
        arr2[left] = arr1[left];
        arr2[right] = arr1[right];
        left++;
        right--;
        printf("%s\n", arr2);
        sleep(1);
        system("clear");
    }
    return 0;
}