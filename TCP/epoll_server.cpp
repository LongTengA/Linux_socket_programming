#include "../Socket_fun/net_fun.hpp"
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#define MAX_CONN 100
std::map<int, net_addr::ip4> conn_info;
int status = 0;
int in = -1;
//一个小型聊天室
void server()
{
    TCP_server server("0.0.0.0", 9000);
    server.linten();

    //epoll 事件注册结构体
    struct epoll_event ctl_ev, recv_ev[MAX_CONN];
    ctl_ev.data.fd = server.sockfd;
    ctl_ev.events = EPOLLIN;

    int epollfd = epoll_create(10);
    //一次注册 一直使用
    epoll_ctl(epollfd, EPOLL_CTL_ADD, server.sockfd, &ctl_ev);
    ctl_ev.data.fd = 0;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &ctl_ev);

    while (1) {
        int ev_num = epoll_wait(epollfd, recv_ev, MAX_CONN, -1);
        for (int i = 0; i < ev_num; i++) {
            if (recv_ev[i].data.fd == server.sockfd) {
                int connfd = server.accept();
                printf("\r\033[31mNew connection:\033[39mClient \033[35m%d\033[39m ", connfd);
                server.m_conn_Addr.show();
                conn_info.insert(std::pair<int, net_addr::ip4>(connfd, server.m_conn_Addr));
                ctl_ev.data.fd = connfd;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ctl_ev);
                char hello[32];
                sprintf(hello, "Your name is %d", connfd);
                write(connfd, hello, strlen(hello));
            } else if (recv_ev[i].data.fd == 0) {
                char buffer[512] = { 0 };
                read(0, buffer, 512);
                if (!strcmp(buffer, "show\n")) {
                    for (auto& i : conn_info) {
                        printf("\r\033[34mClient:\033[31m%d\033[39m ", i.first);
                        i.second.show();
                    }
                }
            } else {
                char buffer[512] = { 0 };
                int size = read(recv_ev[i].data.fd, buffer, 512);
                if (size <= 0) {
                    close(recv_ev[i].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, recv_ev[i].data.fd, &recv_ev[i]);
                    printf("\r\033[33mClient %d outline!\033[39m\n", recv_ev[i].data.fd);
                } else {
                    for (auto& k : conn_info) {
                        if (k.first != recv_ev[i].data.fd)
                            write(k.first, buffer, size);
                    }
                    printf("\r\033[31mClient %d's message : \033[32m%s\033[39m\n", recv_ev[i].data.fd, buffer);
                }
            }
        }
    }
    close(server.sockfd);
}
int main()
{
    server();
}