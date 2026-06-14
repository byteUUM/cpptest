// select_server.cpp
// g++ -std=c++17 -o select_server select_server.cpp
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <array>

static constexpr int PORT    = 8080;
static constexpr int BACKLOG = 10;

int main() {
    // 1. 创建监听 socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(PORT);
    bind(listen_fd, (sockaddr*)&addr, sizeof(addr));
    listen(listen_fd, BACKLOG);
    std::cout << "select server listening on :" << PORT << '\n';

    // 2. 用数组手动维护所有 fd（select 无法自动记住上次的集合）
    std::array<int, FD_SETSIZE> fds;
    fds.fill(-1);
    fds[0] = listen_fd;
    int max_fd = listen_fd;

    while (true) {
        // 3. 每次循环都要重建 fd_set —— select 会修改它
        fd_set read_set;
        FD_ZERO(&read_set);
        for (int fd : fds) {
            if (fd != -1) {
                FD_SET(fd, &read_set);
                if (fd > max_fd) max_fd = fd;
            }
        }

        // 4. 阻塞等待（timeout=nullptr 表示永久阻塞）
        int ready = select(max_fd + 1, &read_set, nullptr, nullptr, nullptr);
        if (ready < 0) { perror("select"); break; }

        // 5. 返回后必须遍历全部 fd，逐一检查 FD_ISSET
        for (int& fd : fds) {
            if (fd == -1 || !FD_ISSET(fd, &read_set)) continue;

            if (fd == listen_fd) {
                // 新连接
                int conn = accept(listen_fd, nullptr, nullptr);
                // 找空槽存入
                for (int& slot : fds) {
                    if (slot == -1) { slot = conn; break; }
                }
                std::cout << "new connection: fd=" << conn << '\n';
            } else {
                // 已连接客户端可读
                char buf[1024];
                ssize_t n = recv(fd, buf, sizeof(buf) - 1, 0);
                if (n <= 0) {
                    std::cout << "client fd=" << fd << " disconnected\n";
                    close(fd);
                    fd = -1;   // 释放槽位
                } else {
                    buf[n] = '\0';
                    std::cout << "fd=" << fd << " says: " << buf;
                    send(fd, buf, n, 0);  // echo back
                }
            }
        }
    }
    close(listen_fd);
}
