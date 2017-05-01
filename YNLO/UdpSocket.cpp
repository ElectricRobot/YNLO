#include "UdpSocket.h"

namespace ynlo {

struct UdpSocket::Impl {
    int a;
};

UdpSocket::UdpSocket()
    : pImpl(std::make_unique<UdpSocket::Impl>())
{}

UdpSocket::~UdpSocket() = default;
UdpSocket::UdpSocket(UdpSocket&& rhs) = default;
UdpSocket& UdpSocket::operator=(UdpSocket&& rhs) = default;

UdpSocket::UdpSocket(const UdpSocket& rhs)
    : pImpl(std::make_unique<UdpSocket::Impl>(*rhs.pImpl))
{}

UdpSocket& UdpSocket::operator=(const UdpSocket& rhs) {
    *pImpl = *rhs.pImpl;
    return *this;
}

UdpServer::UdpServer() {

}

UdpClient::UdpClient() {

}

}
