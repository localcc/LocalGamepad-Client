//
// Created by local on 02.04.2020.
//

#ifndef LOCALGAMEPAD_CLIENT_UDP_CLIENT_H
#define LOCALGAMEPAD_CLIENT_UDP_CLIENT_H

#ifdef WIN32
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#endif

class udp_client {
public:
    udp_client(const char* hostname, int port);
    void connect();
    int write_data(void* buf, int count);
    int read_data(void* buf, int count);
    void disconnect();
    ~udp_client();

private:
    sockaddr_in srv;
    static int init_windows();
    int sock_fd;
};

#endif //LOCALGAMEPAD_CLIENT_UDP_CLIENT_H
