// epoll_server.cpp
// g++ -std=c++17 -o epoll_server epoll_server.cpp
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>

static constexpr int PORT       = 8080;
static constexpr int BACKLOG    = 128;
static constexpr int MAX_EVENTS = 64;

// 设置非阻塞（ET 模式必须配合非阻塞 fd）
static void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// 向 epoll 实例注册/修改/删除 fd
static void epoll_ctl_add(int epfd, int fd, uint32_t events) {
    epoll_event ev{};
    ev.events  = events;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int main() {
    // 1. 创建监听 socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    set_nonblocking(listen_fd);

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(PORT);
    bind(listen_fd, (sockaddr*)&addr, sizeof(addr));
    listen(listen_fd, BACKLOG);

    // 2. 创建 epoll 实例（内核维护红黑树 + 就绪链表）
    int epfd = epoll_create1(EPOLL_CLOEXEC);
    // 3. 注册监听 fd，使用 ET 边沿触发（更高效，需配合非阻塞循环 read）
    epoll_ctl_add(epfd, listen_fd, EPOLLIN | EPOLLET);
    std::cout << "epoll server listening on :" << PORT << '\n';

    std::vector<epoll_event> events(MAX_EVENTS);

    while (true) {
        // 4. 阻塞等待，直接返回"就绪事件数组"（不需要遍历全部 fd）
        int n = epoll_wait(epfd, events.data(), MAX_EVENTS, -1);
        if (n < 0) { perror("epoll_wait"); break; }

        for (int i = 0; i < n; ++i) {
            int fd            = events[i].data.fd;
            uint32_t ev_flags = events[i].events;

            if (fd == listen_fd) {
                // 5a. ET 下需要循环 accept 直到 EAGAIN
                while (true) {
                    int conn = accept4(listen_fd, nullptr, nullptr, SOCK_NONBLOCK | SOCK_CLOEXEC);
                    if (conn < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        perror("accept4"); break;
                    }
                    epoll_ctl_add(epfd, conn, EPOLLIN | EPOLLET);
                    std::cout << "new connection: fd=" << conn << '\n';
                }
            } else if (ev_flags & (EPOLLERR | EPOLLHUP)) {
                // 5b. 错误/挂断
                std::cout << "fd=" << fd << " error/hup\n";
                epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                close(fd);
            } else if (ev_flags & EPOLLIN) {
                // 5c. ET 下必须循环读直到 EAGAIN，否则数据会丢失
                char buf[4096];
                while (true) {
                    ssize_t nr = recv(fd, buf, sizeof(buf), 0);
                    if (nr < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break; // 读完了
                        perror("recv");
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                        close(fd);
                        break;
                    } else if (nr == 0) {
                        std::cout << "client fd=" << fd << " disconnected\n";
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                        close(fd);
                        break;
                    } else {
                        buf[nr] = '\0';
                        std::cout << "fd=" << fd << " says: " << buf;
                        send(fd, buf, nr, 0); // echo back
                    }
                }
            }
        }
    }
    close(epfd);
    close(listen_fd);
}
