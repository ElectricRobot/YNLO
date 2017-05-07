/*
** listener.c -- 一個 datagram sockets "server" 的 demo
*/
#include <iostream>
#include "UdpSocket.h"
#define MYPORT 4950 // 使用者所要連線的 port
#define MAXBUFLEN 100

int main(void)
{
    ynlo::UdpServer server;
    server.Init(MYPORT);
    char buf[MAXBUFLEN] = {0};
    std::string ip_loc;
    int numbytes;

    while((numbytes = server.RecvFrom(buf, MAXBUFLEN, &ip_loc)) != -1) {
        std::cout << "got packet from: " << ip_loc << std::endl;
        std::cout << "packet is " << numbytes << " bytes long" << std::endl;
        std::cout << "packet contains " << buf << std::endl << std::endl;
    }

    return 0;
}
