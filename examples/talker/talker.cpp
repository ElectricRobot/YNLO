/*
** talker.c -- 一個 datagram "client" 的 demo
*/
#include <cstdio>
#include <iostream>
#include <string>
#include "UdpSocket.h"

#define SERVERPORT 4950 // 使用者所要連線的 port

int main(int argc, char *argv[])
{
    ynlo::UdpClient clinet;
    clinet.Init(argv[1], SERVERPORT);

    std::string line;
    int numbytes;

    while(std::getline(std::cin, line)) {
        numbytes = clinet.SendTo(&line[0], line.size()+1);
        std::cout << "sent " << numbytes << " bytes to " << argv[1] << std::endl;
    }
    return 0;
}
