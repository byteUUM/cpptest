#include <iostream>
#include <mutex>

std::mutex m;

std::unique_lock<std::mutex> a;                          // 不关联任何 mutex
std::unique_lock<std::mutex> b(m);                       // 立即 lock(),阻塞
std::unique_lock<std::mutex> c(m, std::defer_lock);      // 关联但不加锁，延迟上锁
std::unique_lock<std::mutex> d(m, std::try_to_lock);     // try_lock,不阻塞
std::unique_lock<std::mutex> e(m, std::adopt_lock);      // 接管已加的锁


int main()
{
    return 0;
}