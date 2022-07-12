#include "../Socket_fun/net_fun.hpp"
#include <bits/types/struct_sched_param.h>
#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <string>
#include <sys/select.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <utility>
// 实现一个多个用户同时链接服务端,服务端可以选择并发送消息的I/O复用服务器
// 选择编号与IP地址对应,输入Q关闭对应链接

//存储已连接的用户信息
std::string commandbuffer;
int status = 0;
int in = -1;

std::map<int, net_addr::ip4> conn_info;
void new_connection(TCP_server& server, fd_set& fd_set, int& maxfd)
{
    int connfd = server.accept();
    printf("\r\033[31mNew connection:\033[39mClient \033[35m%d\033[39m ", connfd);
    server.m_conn_Addr.show();
    fflush(stdout);
    conn_info.insert(std::pair<int, net_addr::ip4>(connfd, server.m_conn_Addr));
    FD_SET(connfd, &fd_set);
    std::string Hello("Your client number is ");
    Hello += std::to_string(connfd);
    write(connfd, Hello.c_str(), Hello.size());
    if (maxfd < connfd)
        maxfd = connfd;
}

void new_stdin(TCP_server& server, fd_set& fd_set, const int& maxfd)
{
    char buffer[512];
    fgets(buffer, 512, stdin);
    std::string command(buffer);
    if (status == 1) {
        write(in, buffer, 512);
        status = 0;
        return;
    }
    if (command == "show\n") {
        if (conn_info.empty()) {
            printf("\rNo connection!\n");
        } else {
            for (auto& i : conn_info) {
                std::cout << "\r\033[34mClient:\033[31m" << i.first << "\033[39m ";
                i.second.show();
            }
        }
    } else {

        if (command.substr(0, 7) == "sendto " && command.size() > 7) {
            in = atoi(&command[7]);
            if (in < 0 || in >= maxfd) {

                printf("%s", buffer);
                printf("%d\n", in);
                printf("\rNo connection!\n");
            } else {
                printf("message:");
                fflush(stdout);
                status = 1;
            }
        } else {
            printf("error command!\n");
        }
    }
}
void new_recv(TCP_server& server, fd_set& fd_set, int& fd, int& maxfd)
{
    //零拷贝 in_fd 不能为socketfd
    //int size = sendfile(1, fd, 0, 512);
    char buffer[512] = { '\0' };
    int size = read(fd, buffer, 512);
    if (size == 0) {
        close(fd);
        FD_CLR(fd, &fd_set);
        conn_info.erase(fd);
        printf("\r\033[33mClient %d outline!\033[39m\n", fd);
        if (maxfd == fd)
            maxfd--;
    } else {
        printf("\r\033[31mClient %d's message : \033[32m%s\033[39m\n", fd, buffer);
    }
}

void Server()
{
    TCP_server p("0.0.0.0", 9000);
    p.linten();
    //定义轮询文件描述符集合,
    fd_set read_set;
    //清空轮询描述符集合
    FD_ZERO(&read_set);
    //socket
    //标准输入
    FD_SET(0, &read_set);
    FD_SET(p.sockfd, &read_set);
    //超时时间
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    fileDescriber maxfd = p.sockfd;
    while (1) {
        fd_set do_set = read_set;
        select(maxfd + 1, &do_set, NULL, NULL, NULL);
        for (int i = 0; i <= maxfd; i++) {
            //FD_ISSET查询对应文件描述符是否有事件发生，有返回1否则为0
            if (FD_ISSET(i, &do_set)) {
                if (i == p.sockfd) {
                    //printf("socket new\n");
                    new_connection(p, read_set, maxfd);
                } else if (i == 0) {
                    //printf("stdin\n");
                    new_stdin(p, read_set, maxfd);
                } else {
                    //printf("socket recv\n");
                    new_recv(p, read_set, i, maxfd);
                }
            }
        }
    }
};
int main()
{
    Server();
    return 1;
};