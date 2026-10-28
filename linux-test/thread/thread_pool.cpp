#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <vector>

class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv_task, cv_space, cv_idle;
    size_t max_queue, active = 0;  // max_queue==0 表示无界
    bool stop = false;

    void worker() {
        for (;;) {
            std::function<void()> task;
            {
                std::unique_lock lk(mtx);
                cv_task.wait(lk, [this] { return stop || !tasks.empty(); });
                if (tasks.empty()) return;  // 此时必有 stop，且任务已清空
                task = std::move(tasks.front());
                tasks.pop();
                ++active;
            }
            cv_space.notify_one();
            try { task(); } catch (...) {}  // 防止异常杀死工作线程
            {
                std::lock_guard lk(mtx);
                if (--active == 0 && tasks.empty()) cv_idle.notify_all();
            }
        }
    }

public:
    explicit ThreadPool(size_t n = std::thread::hardware_concurrency(), size_t max_queue = 0)
        : max_queue(max_queue) {
        if (n == 0) n = 1;
        for (size_t i = 0; i < n; ++i) workers.emplace_back([this] { worker(); });
    }
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    template <class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using R = std::invoke_result_t<F, Args...>;
        // 用 lambda + tuple 完美转发，支持 move-only 参数
        auto task = std::make_shared<std::packaged_task<R()>>(
            [f = std::forward<F>(f), t = std::make_tuple(std::forward<Args>(args)...)]() mutable {
                return std::apply(std::move(f), std::move(t));
            });
        auto fut = task->get_future();
        {
            std::unique_lock lk(mtx);
            cv_space.wait(lk, [this] { return stop || max_queue == 0 || tasks.size() < max_queue; });
            if (stop) throw std::runtime_error("submit on stopped ThreadPool");
            tasks.emplace([task] { (*task)(); });
        }
        cv_task.notify_one();
        return fut;
    }

    // 等待所有已提交任务执行完毕（线程池仍可继续使用）
    void wait() {
        std::unique_lock lk(mtx);
        cv_idle.wait(lk, [this] { return tasks.empty() && active == 0; });
    }

    // 幂等；先跑完队列中剩余任务再退出
    void shutdown() {
        {
            std::lock_guard lk(mtx);
            stop = true;
        }
        cv_task.notify_all();
        cv_space.notify_all();
        for (auto& t : workers)
            if (t.joinable()) t.join();
    }

    ~ThreadPool() { shutdown(); }
};

int main()
{
    ThreadPool pool(4, 1024);                      // 4 线程，队列上限 1024
    auto f = pool.submit([](int a, int b) { return a + b; }, 1, 2);
    std::cout << f.get();                          // 3
    pool.wait();
    return 0;
}