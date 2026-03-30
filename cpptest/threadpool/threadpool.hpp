#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

class ThreadPool
{
public:
    // 构造：创建线程池
    ThreadPool(size_t n = std::thread::hardware_concurrency())
        : stop(false)
    {
        for (size_t i = 0; i < n; ++i)
        {
            workers.emplace_back([this]() {
                while (true)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        cv.wait(lock, [this]() {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty())
                            return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task(); // 执行任务
                }
            });
        }
    }

    // 提交任务
    template<class F, class... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(mtx);

            if (stop)
                throw std::runtime_error("ThreadPool has stopped");

            tasks.emplace([task]() { (*task)(); });
        }

        cv.notify_one();
        return res;
    }

    // 析构：关闭线程池
    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
        }

        cv.notify_all();

        for (std::thread& worker : workers)
        {
            if (worker.joinable())
                worker.join();
        }
    }

private:
    std::vector<std::thread> workers;              // 线程池
    std::queue<std::function<void()>> tasks;       // 任务队列

    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
};