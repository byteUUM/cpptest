#include "threadpool.hpp"
int main()
{
    ThreadPool pool(4);

    auto f1 = pool.submit([](int x) {
        return x * x;
    }, 5);

    std::cout << f1.get() << std::endl; // 25

    return 0;
}