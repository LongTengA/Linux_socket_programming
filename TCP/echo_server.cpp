//一个回声服务器
#include "../Socket_fun/net_fun.hpp"
#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void Server()
{
    net_addr::ip4 serAddr("0.0.0.0", 9000);
    net_addr::ip4 recvAddr;

    int sockfd = net_fun::socket();
    net_fun::bind(sockfd, serAddr.toSockaddr());
    net_fun::listen(sockfd);
label:
    fileDescriber connfd = accept(sockfd, recvAddr.toSockaddr(), recvAddr.getAddrLenPtr());
    recvAddr.show();
    char buffer[512] = { '\0' };
    int size = read(connfd, buffer, 512);
    if (size > 0)
        printf("Message:   %s\n", buffer);
    write(connfd, buffer, size);
    write(connfd, "\nBye!\n", 6);
    close(connfd);
    goto label;
    close(sockfd);
}

int main()
{
    Server();
}