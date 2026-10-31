// g++ -std=c++17 -O2 pool.cpp -o pool
#include <sys/socket.h>
#include <sys/wait.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

static bool io_all(int fd, void* buf, size_t n, bool wr) {
    char* p = (char*)buf;
    while (n) {
        ssize_t r = wr ? write(fd, p, n) : read(fd, p, n);
        if (r < 0 && errno == EINTR) continue;
        if (r <= 0) return false;
        p += r; n -= r;
    }
    return true;
}
// 协议: 4字节长度 + 内容
static bool send_msg(int fd, const std::string& s) {
    uint32_t len = s.size();
    return io_all(fd, &len, 4, true) && io_all(fd, (void*)s.data(), len, true);
}
static bool recv_msg(int fd, std::string& s) {
    uint32_t len;
    if (!io_all(fd, &len, 4, false)) return false;
    s.resize(len);
    return io_all(fd, s.data(), len, false);
}

class ProcessPool {
    struct Worker { pid_t pid; int fd; int pending = 0; };
    std::vector<Worker> ws_;
public:
    using Handler = std::function<std::string(const std::string&)>;

    ProcessPool(int n, Handler h) {
        signal(SIGPIPE, SIG_IGN);
        for (int i = 0; i < n; ++i) {
            int sv[2];
            socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
            pid_t pid = fork();
            if (pid == 0) {                       // 子进程: 循环收任务 -> 处理 -> 回结果
                close(sv[0]);
                for (auto& w : ws_) close(w.fd);  // 关掉继承来的兄弟进程 fd
                std::string req;
                while (recv_msg(sv[1], req)) send_msg(sv[1], h(req));
                _exit(0);                         // 父进程关闭 fd => EOF => 退出
            }
            close(sv[1]);
            ws_.push_back({pid, sv[0]});
        }
    }

    // 提交任务: 派给负载(未完成数)最小的 worker
    void submit(const std::string& task) {
        auto& w = *std::min_element(ws_.begin(), ws_.end(),
            [](auto& a, auto& b) { return a.pending < b.pending; });
        if (send_msg(w.fd, task)) ++w.pending;
    }

    // 阻塞等待任意一个结果; 无未完成任务返回 false
    bool wait_one(std::string& out) {
        std::vector<pollfd> pfds; std::vector<Worker*> idx;
        for (auto& w : ws_) if (w.pending) { pfds.push_back({w.fd, POLLIN, 0}); idx.push_back(&w); }
        if (pfds.empty()) return false;
        while (poll(pfds.data(), pfds.size(), -1) < 0 && errno == EINTR);
        for (size_t i = 0; i < pfds.size(); ++i)
            if (pfds[i].revents & (POLLIN | POLLHUP)) {
                if (!recv_msg(idx[i]->fd, out)) return false;
                --idx[i]->pending;
                return true;
            }
        return false;
    }

    ~ProcessPool() {
        for (auto& w : ws_) close(w.fd);          // 触发子进程 EOF 退出
        for (auto& w : ws_) waitpid(w.pid, nullptr, 0);  // 回收, 避免僵尸进程
    }
};

int main() {
    ProcessPool pool(4, [](const std::string& req) {
        return "[pid " + std::to_string(getpid()) + "] done: " + req;
    });
    for (int i = 0; i < 8; ++i) pool.submit("task-" + std::to_string(i));
    std::string res;
    while (pool.wait_one(res)) puts(res.c_str());
}
