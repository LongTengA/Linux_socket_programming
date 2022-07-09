//多进程并发回声服务器
#include "../Socket_fun/net_fun.hpp"
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

void Server()
{
    TCP_server p("0.0.0.0", 9000);
    p.linten();
    while (1) {
        int connfd = p.accept();
        if (connfd) {
            puts("new connection from:");
            p.m_conn_Addr.show();
        }
        pid_t pid = fork();
        if (pid == 0) {
            close(p.sockfd);
            char* buffer[512];
            int size = read(connfd, buffer, 512);
            write(connfd, buffer, size);
            printf("send size = %d\n", size);
            close(connfd);
            _exit(1);
        } else {
            close(connfd);
        }
    }
    close(p.sockfd);
}

int main()
{
    Server();
}