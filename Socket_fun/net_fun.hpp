
#ifndef __NET_FUN__
#define __NET_FUN__

#include <arpa/inet.h>
#include <cstring>
#include <stdio.h>
#include <sys/socket.h>
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
    const int addrlen { sizeof(sockaddr_in) };

public:
    ip4(const char* ip = "0.0.0.0", int port = 0);
    ip4(const ip4& ip);
    ip4(struct sockaddr_in* ip);
    sockaddr* toSockaddr();
    sockaddr_in* toSock_in();
    const int& getAddrlen();
    char* getIp();
    int getPort();
    void show();
    socklen_t* getAddrLenPtr();
    ~ip4();
};
inline ip4::~ip4()
{
    delete m_addrinfo;
}
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
    *m_addrinfo = *ip.m_addrinfo;
}
inline sockaddr* ip4::toSockaddr()
{
    return (sockaddr*)m_addrinfo;
};
inline sockaddr_in* ip4::toSock_in()
{
    return m_addrinfo;
};
inline const int& ip4::getAddrlen()
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
    printf("IP:\033[31m%s\033[39m Port:\033[31m%d\033[39m\n", this->getIp(), this->getPort());
}

inline socklen_t* ip4::getAddrLenPtr()
{
    return (socklen_t*)&addrlen;
}
}

class TCP_server {
public:
    net_addr::ip4 m_ser_Addr;
    net_addr::ip4 m_conn_Addr;
    int sockfd;

public:
    TCP_server(const char* ip, int port);
    TCP_server(net_addr::ip4& ip);
    void linten();
    void bind(net_addr::ip4& ip);
    fileDescriber accept(net_addr::ip4& addr);
    fileDescriber accept();
};

inline TCP_server::TCP_server(const char* ip, int port)
    : m_ser_Addr(ip, port)
{
    sockfd = net_fun::socket();
    net_fun::bind(sockfd, m_ser_Addr.toSockaddr());
};
inline TCP_server::TCP_server(net_addr::ip4& ip)
    : m_ser_Addr(ip)
{
    sockfd = net_fun::socket();
    net_fun::bind(sockfd, m_ser_Addr.toSockaddr());
};
inline void TCP_server::linten()
{
    net_fun::listen(sockfd);
};
inline void TCP_server::bind(net_addr::ip4& ip)
{
    net_fun::bind(sockfd, ip.toSockaddr());
};
inline fileDescriber TCP_server::accept(net_addr::ip4& addr)
{
    return ::accept(sockfd, addr.toSockaddr(), addr.getAddrLenPtr());
};
inline fileDescriber TCP_server::accept()
{
    return ::accept(sockfd, m_conn_Addr.toSockaddr(), m_conn_Addr.getAddrLenPtr());
};
#endif