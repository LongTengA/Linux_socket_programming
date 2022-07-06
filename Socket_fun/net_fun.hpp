#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <bits/types/FILE.h>
#include <cstring>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
//一些常用网络编程函数以及数据结构的封装
using fileDescriber = int;
namespace net_addr {
class ip4;
}

namespace net_fun {
//错误处理函数
static void err_hanlde(const char* message)
{
    perror(message);
    _exit(-1);
}
//取消套接字的time_wait状态
inline void setsockopt(fileDescriber fd)
{
    int option = true;
    int optlen = sizeof(option);

    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, optlen);
}

//opt:1为TCP(默认)----0为UDP
static fileDescriber socket(int opt = 1)
{
    fileDescriber fd = -1;
    if (opt)
        fd = ::socket(AF_INET, SOCK_STREAM, 0);
    else
        fd = ::socket(AF_INET, SOCK_DGRAM, 0);

    if (fd == -1) {
        err_hanlde("create socket");
    }
    setsockopt(fd);
    return fd;
}
static void bind(fileDescriber fd, sockaddr* ip)
{
    int errlog = ::bind(fd, ip, sizeof(sockaddr_in));
    if (errlog < 0)
        err_hanlde("bind");
}
static void listen(fileDescriber fd)
{
    int errlog = ::listen(fd, 1);
    if (errlog == -1)
        err_hanlde("listen");
}
} //namespace net_fun

namespace net_addr {
//一个ip4地址类，
class ip4 {
    struct sockaddr_in* m_addrinfo;
    static int addrlen;
    char buffer[16];

public:
    ip4(const char* ip = "0.0.0.0", int port = 0);
    ip4(const ip4& ip);
    ip4(struct sockaddr_in* ip);
    sockaddr* toSockaddr();
    sockaddr_in* toSock_in();
    int& getAddrlen();
    char* getIp();
    int getPort();
    void show();
    socklen_t* getAddrLenPtr();
};
int ip4::addrlen = sizeof(sockaddr_in);
inline ip4::ip4(struct sockaddr_in* ip)
{
    m_addrinfo = new sockaddr_in;
    if (!m_addrinfo)
        net_fun::err_hanlde("cpoy ip4addr");
    memcpy(m_addrinfo, ip, addrlen);
}

inline ip4::ip4(const char* ip, int port)
{
    m_addrinfo = new sockaddr_in;
    if (!m_addrinfo)
        net_fun::err_hanlde("create ip4addr");
    bzero(m_addrinfo, addrlen);
    m_addrinfo->sin_family = AF_INET;
    m_addrinfo->sin_addr.s_addr = inet_addr(ip);
    m_addrinfo->sin_port = htons(port);
}
inline ip4::ip4(const ip4& ip)
{
    m_addrinfo = new sockaddr_in;
    if (!m_addrinfo)
        net_fun::err_hanlde("cpoy ip");
    memcpy(m_addrinfo, &ip, addrlen);
}
inline sockaddr* ip4::toSockaddr()
{
    return (sockaddr*)m_addrinfo;
};
inline sockaddr_in* ip4::toSock_in()
{
    return m_addrinfo;
};
inline int& ip4::getAddrlen()
{
    return addrlen;
};
inline char* ip4::getIp()
{
    return inet_ntoa(m_addrinfo->sin_addr);
};
inline int ip4::getPort()
{
    return ntohs(m_addrinfo->sin_port);
};

inline void ip4::show()
{
    printf("IP:%s Port:%d\n", this->getIp(), this->getPort());
}

inline socklen_t* ip4::getAddrLenPtr()
{
    return (socklen_t*)&addrlen;
}
}