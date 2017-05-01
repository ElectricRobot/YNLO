#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include <memory>

namespace ynlo {

class UdpSocket {
public:
    UdpSocket();
    UdpSocket(const UdpSocket& rhs);
    UdpSocket& operator=(const UdpSocket& rhs);

    UdpSocket(UdpSocket&& rhs);
    UdpSocket& operator=(UdpSocket&& rhs);
    ~UdpSocket();

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

class UdpServer
{
public:
    UdpServer();
};

class UdpClient
{
public:
    UdpClient();
};
}
#endif // UDPSOCKET_H
