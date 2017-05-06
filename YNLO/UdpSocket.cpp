#include "UdpSocket.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <unistd.h>

namespace ynlo {
namespace v1{
UdpClient::UdpClient(const std::string& addr, int port)
    :port_(port), addr_(addr)
{
    // convert int port to string port
    std::string decimal_port;
    std::stringstream ss;
    ss << port;
    decimal_port = ss.str();
    ss.str("");
    ss.clear();

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r = getaddrinfo(addr_.c_str(), decimal_port.c_str(), &hints, &addrinfo_);

    if(r != 0 || addrinfo_ == NULL) {
        throw ynlo_runtime_error(("invalid address or port: \"" + addr_ + ":" + decimal_port + "\"").c_str());
    }
    socket_ = socket(addrinfo_->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(socket_ == -1) {
        freeaddrinfo(addrinfo_);
        throw ynlo_runtime_error(("could not create socket for: \"" + addr_ + ":" + decimal_port + "\"").c_str());
    }
}

UdpClient::~UdpClient() {
   freeaddrinfo(addrinfo_);
   close(socket_);
}

int UdpClient::GetSocket() const {
    return socket_;
}

int UdpClient::GetPort() const {
    return port_;
}

std::string UdpClient::GetAddr() const {
    return addr_;
}

int UdpClient::Send(const char* msg, size_t size) {
    return static_cast<int>(sendto(socket_, msg, size, 0, addrinfo_->ai_addr, addrinfo_->ai_addrlen));
}

int UdpClient::Recv(char* msg, size_t max_size) {
    return static_cast<int>(recv(socket_, msg, max_size, 0));
}

int UdpClient::TimedRecv(char* msg, size_t max_size, int max_wait_ms) {
    fd_set s;
    FD_ZERO(&s);
    FD_SET(socket_, &s);
    struct timeval timeout;
    timeout.tv_sec = max_wait_ms / 1000;
    timeout.tv_usec = (max_wait_ms % 1000) * 1000;

    int retval = select(socket_+1, &s, &s, &s, &timeout);
    if(retval   == -1) {
        // select() set errno accordingly
        return -1;
    }
    if(retval > 0) {
        // our socket has data
        return static_cast<int>(recv(socket_, msg, max_size, 0));
    }

    // our socket has no data
    errno = EAGAIN;
    return -1;
}

UdpServer::UdpServer(int port)
    :port_(port)
{
    // convert int port to string port
    std::string decimal_port;
    std::stringstream ss;
    ss << port;
    decimal_port = ss.str();
    ss.str("");
    ss.clear();

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE; // use default ip setting
    int r = getaddrinfo(NULL, decimal_port.c_str(), &hints, &addrinfo_);

    if(r != 0 || addrinfo_ == NULL) {
        throw ynlo_runtime_error(("invalid port for UDP socket: \""+ decimal_port + "\"").c_str());
    }

    socket_ = socket(addrinfo_->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(socket_ == -1) {
        freeaddrinfo(addrinfo_);
        throw ynlo_runtime_error(("could not create UDP socket for: \""+ decimal_port + "\"").c_str());
    }

    r = bind(socket_, addrinfo_->ai_addr, addrinfo_->ai_addrlen);
    if(r != 0) {
        freeaddrinfo(addrinfo_);
        close(socket_);
        throw ynlo_runtime_error(("could not bind UDP socket with: \""+ decimal_port + "\"").c_str());
    }
}

UdpServer::~UdpServer() {
    freeaddrinfo(addrinfo_);
    close(socket_);
}

int UdpServer::GetSocket() const {
    return socket_;
}

int UdpServer::GetPort() const {
    return port_;
}

int UdpServer::Send(const char* msg, size_t size) {
    return static_cast<int>(sendto(socket_, msg, size, 0, addrinfo_->ai_addr, addrinfo_->ai_addrlen));
}

int UdpServer::Recv(char* msg, size_t max_size) {
    return static_cast<int>(recv(socket_, msg, max_size, 0));
}

int UdpServer::TimedRecv(char* msg, size_t max_size, int max_wait_ms) {
    fd_set s;
    FD_ZERO(&s);
    FD_SET(socket_, &s);
    struct timeval timeout;
    timeout.tv_sec = max_wait_ms / 1000;
    timeout.tv_usec = (max_wait_ms % 1000) * 1000;

    int retval = select(socket_+1, &s, &s, &s, &timeout);
    if(retval   == -1) {
        // select() set errno accordingly
        return -1;
    }
    if(retval > 0) {
        // our socket has data
        return static_cast<int>(recv(socket_, msg, max_size, 0));
    }

    // our socket has no data
    errno = EAGAIN;
    return -1;
}
}

inline namespace v2 {
UdpSocket::UdpSocket() {}
UdpSocket::~UdpSocket() {}

UdpServer::UdpServer() :is_init_(false)
{
    addr_len_ = sizeof(their_addr_);
}
UdpServer::~UdpServer() {
    if(is_init_)
        close(sockfd_);
}

bool UdpServer::Init(int port) {
    if(is_init_ == true) {
        std::cerr << "Socket is binding now." << std::endl
                  << "If you want to reinit, please call Init(int) after Clear()" << std::endl;
        return true;
    }

    // convert int port to string
    port_ = port;
    std::stringstream ss;
    ss << port_;
    std::string string_port(ss.str());
    ss.str("");
    ss.clear();

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // if use AF_INET -> only IPv4 can connect
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // used default IP

    int rv;
    if((rv = getaddrinfo(NULL, string_port.c_str(), &hints, &servinfo_)) != 0)
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        return false;
    }

    // binding
    struct addrinfo* p = servinfo_;
    for(; p != NULL; p =p->ai_next) {
        // create socket
        if((sockfd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            std::cerr << "create socket fail, try next" << std::endl;
            continue;
        }

        if(bind(sockfd_, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd_);
            std::cerr << "bind error, try next" << std::endl;
            continue;
        }

        break;
    }

    if(p == NULL) {
        std::cerr << "failed to bind socket" << std::endl;
        return false;
    }
    is_init_ = true;
    freeaddrinfo(servinfo_);
    return true;
}

int UdpServer::RecvFrom(char* buf, int max_size, std::string* their_ip) {
    if(is_init_ == false) {
        std::cerr << "Initialization is incomplete." << std::endl;
        return -1;
    }

    int numbytes;
    if((numbytes = recvfrom(sockfd_, buf, max_size, 0, (struct sockaddr*)&their_addr_, &addr_len_)) == -1)
        return -1;

    if(their_ip != nullptr) {
        char s[INET6_ADDRSTRLEN];
        *their_ip = std::string(inet_ntop(their_addr_.ss_family, GetInAddr((struct sockaddr*)&their_addr_), s, sizeof(s)));
    }
    return numbytes;
}

void UdpServer::Close(){
    if(is_init_)
        close(sockfd_);
    is_init_ = false;
}

UdpClient::UdpClient() : is_init_(false){}
UdpClient::~UdpClient() {}

bool UdpClient::Init(const std::string& addr, int port) {
    if(is_init_ == true) {
        std::cerr << "Socket is created." << std::endl
                  << "If you want to reinit, please call Init after Clear()" << std::endl;
        return true;
    }
    addr_ = addr;
    port_ = port;
    std::string string_port;
    std::stringstream ss;
    ss << port;
    string_port = ss.str();
    ss.str("");
    ss.clear();

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    int rv;
}

void UdpClient::Close() {

}
}
}
