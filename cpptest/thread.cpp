#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx;  // 全局互斥锁

// 普通函数，作为线程入口
void printMessage(const std::string& msg, int count) {
    for (int i = 0; i < count; ++i) {
        // 加锁保护输出，防止多个线程打印混乱
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "线程 " << std::this_thread::get_id()
                  << " : " << msg << " " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// 带引用参数的函数
void incrementCounter(int& counter) {
    for (int i = 0; i < 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            counter++;
            std::cout << "计数器: " << counter << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    std::cout << "主线程 ID: " << std::this_thread::get_id() << std::endl;

    // 1. 创建线程，传入普通函数和参数
    std::thread t1(printMessage, "Hello", 3);

    // 2. 使用 lambda 表达式创建线程
    std::thread t2([]() {
        for (int i = 0; i < 3; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Lambda 线程执行 " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
    });

    // 3. 传递引用参数，必须用 std::ref
    int counter = 0;
    std::thread t3(incrementCounter, std::ref(counter));

    // 4. 分离线程（detach）：让线程在后台独立运行
    std::thread t4([]() {
        for (int i = 0; i < 2; ++i) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "分离线程运行中 " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    });
    t4.detach();  // 分离后不再需要 join

    // 等待可 join 的线程结束
    t1.join();
    t2.join();
    t3.join();

    // 主线程稍等，让 detach 线程有机会执行完（否则主线程结束进程可能终止）
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "最终计数器值: " << counter << std::endl;
    std::cout << "主线程结束" << std::endl;

    return 0;
}
