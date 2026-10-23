// 单线程 Reactor (epoll, LT)：Reactor 负责 epoll 事件分发，Channel 绑定 fd 与回调
// 编译: g++ -std=c++17 -O2 reactor.cpp -o reactor
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Channel {
    int fd;
    uint32_t events = EPOLLIN;
    bool dead = false;
    std::function<void()> onRead, onWrite;
};

class Reactor {
    int ep_ = epoll_create1(0);
    std::unordered_map<int, std::unique_ptr<Channel>> chans_;
    std::vector<std::unique_ptr<Channel>> trash_;  // 延迟销毁，避免回调中自删
public:
    ~Reactor() { close(ep_); }

    Channel* add(int fd, uint32_t ev = EPOLLIN) {
        auto& c = chans_[fd] = std::make_unique<Channel>();
        c->fd = fd; c->events = ev;
        epoll_event e{ev, {.fd = fd}};
        epoll_ctl(ep_, EPOLL_CTL_ADD, fd, &e);
        return c.get();
    }
    void update(Channel* c) {
        epoll_event e{c->events, {.fd = c->fd}};
        epoll_ctl(ep_, EPOLL_CTL_MOD, c->fd, &e);
    }
    void remove(int fd) {
        auto it = chans_.find(fd);
        if (it == chans_.end()) return;
        it->second->dead = true;
        epoll_ctl(ep_, EPOLL_CTL_DEL, fd, nullptr);
        close(fd);
        trash_.push_back(std::move(it->second));
        chans_.erase(it);
    }
    void loop() {
        epoll_event evs[128];
        for (;;) {
            int n = epoll_wait(ep_, evs, 128, -1);
            for (int i = 0; i < n; ++i) {
                auto it = chans_.find(evs[i].data.fd);
                if (it == chans_.end()) continue;
                Channel* c = it->second.get();
                uint32_t e = evs[i].events;
                if ((e & (EPOLLIN | EPOLLHUP | EPOLLERR)) && c->onRead) c->onRead();
                if (!c->dead && (e & EPOLLOUT) && c->onWrite) c->onWrite();
            }
            trash_.clear();
        }
    }
};

int main() {
    Reactor r;
    int lfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    int on = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    sockaddr_in a{};
    a.sin_family = AF_INET; a.sin_port = htons(8888); a.sin_addr.s_addr = INADDR_ANY;
    if (bind(lfd, (sockaddr*)&a, sizeof a) < 0 || listen(lfd, 128) < 0) { perror("bind/listen"); return 1; }

    // Acceptor：可读 => 循环 accept 新连接并注册
    r.add(lfd)->onRead = [&] {
        int cfd;
        while ((cfd = accept4(lfd, nullptr, nullptr, SOCK_NONBLOCK)) >= 0) {
            Channel* c = r.add(cfd);
            auto out = std::make_shared<std::string>();  // 每连接的发送缓冲

            auto flush = [&r, c, cfd, out] {
                while (!out->empty()) {
                    ssize_t n = send(cfd, out->data(), out->size(), MSG_NOSIGNAL);
                    if (n > 0) out->erase(0, n);
                    else if (errno == EAGAIN) break;
                    else if (errno != EINTR) { r.remove(cfd); return; }
                }
                c->events = out->empty() ? EPOLLIN : (EPOLLIN | EPOLLOUT);  // 写不完才关注 EPOLLOUT
                r.update(c);
            };
            c->onWrite = flush;
            c->onRead = [&r, cfd, out, flush] {
                char buf[4096];
                ssize_t n = read(cfd, buf, sizeof buf);
                if (n > 0) { out->append(buf, n); flush(); }  // echo
                else if (n == 0 || (errno != EAGAIN && errno != EINTR)) r.remove(cfd);
            };
        }
    };
    printf("echo server on :8888\n");
    r.loop();
}