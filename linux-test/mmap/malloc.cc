#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void* my_malloc(int size)
{
    void* ptr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if(ptr==MAP_FAILED)
    {
        std::cout<<"my_malloc fail"<<std::endl;
        return nullptr;
    }
    return ptr;
}

void my_free(void* ptr, int size)
{
    if(munmap(ptr, size)==-1) 
    {
        std::cout<<"my_free fail"<<std::endl;
    }
}
int main()
{
    int size = 1024;
    char* ptr = (char*)my_malloc(size);
    if(!ptr) return 1;
    for(char c = 'a'; c<='z'; c++)
    {
        ptr[c-'a'] = c;
        std::cout<<ptr[c-'a']<<" ";
    }
    std::cout<<std::endl;
    my_free(ptr, size);
    return 0;
}