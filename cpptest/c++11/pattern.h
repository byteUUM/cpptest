//  饿汉式
#include <iostream>
using namespace std;

class SingletonEager {
private:
    // 静态成员在程序启动时初始化
    static SingletonEager instance;

    SingletonEager() {
        cout << "Eager Singleton created" << endl;
    }

public:
    SingletonEager(const SingletonEager&) = delete;
    SingletonEager& operator=(const SingletonEager&) = delete;

    static SingletonEager& getInstance() {
        return instance;
    }

    void doSomething() {
        cout << "Doing something..." << endl;
    }
};

// 静态成员定义
SingletonEager SingletonEager::instance;

int main() {
    SingletonEager& s1 = SingletonEager::getInstance();
    s1.doSomething();
    return 0;
}


// 懒汉模式---非线程安全
class SingletonLazyUnsafe {
private:
    static SingletonLazyUnsafe* instance;

    SingletonLazyUnsafe() {
        cout << "Lazy Singleton created" << endl;
    }

public:
    SingletonLazyUnsafe(const SingletonLazyUnsafe&) = delete;
    SingletonLazyUnsafe& operator=(const SingletonLazyUnsafe&) = delete;

    static SingletonLazyUnsafe* getInstance() {
        if (instance == nullptr) {
            instance = new SingletonLazyUnsafe();
        }
        return instance;
    }
};

// 静态成员初始化为 nullptr
SingletonLazyUnsafe* SingletonLazyUnsafe::instance = nullptr;




// 线程安全-简单加锁-效率极低，每次访问都会加锁
#include <mutex>
#include <iostream>
using namespace std;

class SingletonLazySafe {
private:
    static SingletonLazySafe* instance;
    static std::mutex mtx;

    SingletonLazySafe() {
        cout << "Lazy Singleton (thread-safe) created" << endl;
    }

public:
    SingletonLazySafe(const SingletonLazySafe&) = delete;
    SingletonLazySafe& operator=(const SingletonLazySafe&) = delete;

    static SingletonLazySafe* getInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {  // 第一次检查（避免加锁开销）
            instance = new SingletonLazySafe();
        }
        return instance;
    }

    void doSomething() {
        cout << "Doing something..." << endl;
    }
};

// 静态成员定义
SingletonLazySafe* SingletonLazySafe::instance = nullptr;
std::mutex SingletonLazySafe::mtx;

int main() {
    SingletonLazySafe* s1 = SingletonLazySafe::getInstance();
    s1->doSomething();
    return 0;
}


// 线程安全-双重检查加锁-只有第一次调用并发会触发加锁，后续都不会触发-更高效
#include <mutex>
#include <iostream>
using namespace std;

class SingletonLazySafe {
private:
    static SingletonLazySafe* instance;
    static std::mutex mtx;

    SingletonLazySafe() {
        cout << "Lazy Singleton (thread-safe) created" << endl;
    }

public:
    SingletonLazySafe(const SingletonLazySafe&) = delete;
    SingletonLazySafe& operator=(const SingletonLazySafe&) = delete;

    static SingletonLazySafe* getInstance() {
        if (instance == nullptr) {  // 第一次检查（避免加锁开销）
            std::lock_guard<std::mutex> lock(mtx);
            if (instance == nullptr) {  // 第二次检查（确保只创建一次）
                instance = new SingletonLazySafe();
            }
        }
        return instance;
    }

    void doSomething() {
        cout << "Doing something..." << endl;
    }
};

// 静态成员定义
SingletonLazySafe* SingletonLazySafe::instance = nullptr;
std::mutex SingletonLazySafe::mtx;

int main() {
    SingletonLazySafe* s1 = SingletonLazySafe::getInstance();
    s1->doSomething();
    return 0;
}
