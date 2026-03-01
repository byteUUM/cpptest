#include "process_pool.hpp"
int main()
{
    Task tk;
    tk.push(PrintLog);
    tk.push(Download);
    tk.push(Upload); 
    ProcessPool pp(10,tk);
    pp.CreateProcess();
    int k=10;
    while(k--) pp.Run();
    pp.Stop();
    return 0;
}