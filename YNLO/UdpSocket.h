// reference : http://linux.m2osw.com/c-implementation-udp-clientserver
//             https://www.techpowerup.com/forums/threads/c-c-sockets-faq-and-how-to-win-linux.56901/
//             http://beej-zhtw.netdpi.net/05-system-call-or-bust/5-8-sendto
//             http://www.binarytides.com/programming-udp-sockets-c-linux/
//             http://www.binarytides.com/udp-socket-programming-in-winsock/
#ifndef UDPSOCKET_H
#define UDPSOCKET_H
// C and linux
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
// c++
#include <string>
#include "Except.h"

namespace ynlo {
namespace v1{
class UdpClient
{
public:
    UdpClient(const std::string& addr, int port);
    ~UdpClient();

    int GetSocket() const;
    int GetPort() const;
    std::string GetAddr() const;

    int Send(const char* msg, size_t size);
    int Recv(char* msg, size_t max_size);
    int TimedRecv(char* msg, size_t max_size, int max_wait_ms);

private:
    int socket_;
    int port_;
    std::string addr_;
    struct addrinfo* addrinfo_;
};

class UdpServer {
public:
    UdpServer(int port);
    ~UdpServer();

    int GetSocket() const;
    int GetPort() const;

    int Send(const char* msg, size_t size);
    int Recv(char* msg, size_t max_size);
    int TimedRecv(char* msg, size_t max_size, int max_wait_ms);
private:
    int socket_;
    int port_;
    struct addrinfo* addrinfo_;
};
}

inline namespace v2 {

// get sockaddr, IPv4 or IPv6:
void* GetInAddr(struct sockaddr* sa);

class UdpSocket {
public:
    UdpSocket();
    virtual ~UdpSocket();
    virtual void Close() = 0;
protected:
    int port_;
    int sockfd_;
    struct addrinfo* servinfo_;
};

class UdpServer:public UdpSocket {
public:
    UdpServer();
    ~UdpServer();

     bool Init(int port);
     void Close() override;
     int RecvFrom(char* buf, int max_size, std::string* their_ip = nullptr);
private:
     bool is_init_;
     socklen_t addr_len_;
     struct sockaddr_storage their_addr_;
};

class UdpClient:public UdpSocket {
public:
    UdpClient();
    ~UdpClient();

    bool Init(const std::string& addr, int port);
    void Close() override;
    int SendTo(char* buf, int size);
private:
    bool is_init_;
    std::string addr_;
    struct addrinfo* p_;
};
}
}
#endif // UDPSOCKET_H
