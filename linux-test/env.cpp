#include <iostream>
#include <stdlib.h>
using namespace std;
int main(int argc, char *argv[])
{
    extern char **environ;
    //for(int i=0; environ[i]; i++) printf("%s\n", environ[i]);
    cout<<getenv("PATH")<<endl;
    cout<<getenv("USER")<<endl;
    return 0;
}