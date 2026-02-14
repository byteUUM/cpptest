#include <vector>
#include <functional>
#include <cstdio>
using namespace std;
using func = function<void()>;
class Tasks
{
public:
    Tasks()
    {
        srand((int unsigned)time(NULL));
    }
    void Insert(void (*fun)())
    {
        _tasks.push_back(fun);
    }
    void work(int code)
    {
        _tasks[code]();
    }
    int get_code()
    {
        return rand()%_tasks.size();
    }
private:
    vector<func> _tasks;
};