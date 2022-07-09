//多进程并发回声服务器
#include "../Socket_fun/net_fun.hpp"
#include <csignal>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//防止僵尸进程
//#define DEBUGE
int count = 0;
void read_child_proc(int)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("remove proc id = %d\n", pid);
}

#ifdef DEBUGE
void printf_count(int)
{
    printf("\ncount = %d\n", count);
    _exit(1);
}
#endif

void Server()
{
    //防止产生僵尸进程
    struct sigaction act;
    act.sa_handler = read_child_proc;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, 0);

#ifdef DEBUGE
    //信号的处理会打断阻塞的IO
    act.sa_handler = printf_count;
    sigaction(SIGINT, &act, 0);
#endif

    pid_t pid = -1;

    TCP_server p("0.0.0.0", 9000);
    p.linten();

    while (1) {
        count++;
        int connfd = p.accept();
        if (connfd > 0) {
            puts("new connection from:");
            p.m_conn_Addr.show();
            pid = fork();
            if (pid == 0) {
                close(p.sockfd);
                char* buffer[512];
                int size = read(connfd, buffer, 512);
                write(connfd, buffer, size);
                shutdown(connfd, SHUT_WR);
                printf("send size = %d\n", size);
                close(connfd);
                _exit(1);
            } else
                close(connfd);
        }
    }
    close(p.sockfd);
}

int main()
{
    Server();
}
