//
// Created by local on 02.04.2020.
//
#include <udp_client/udp_client.h>
#include <cstdio>



udp_client::udp_client(const char *hostname, int port): srv{NULL} {
    int res = 0;
#ifdef WIN32
    res = init_windows();
    if(res != 0) {
        printf("WSAStartup() failed!\n");
        return;
    }
#endif
    this->sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(this->sock_fd < 0) {
        printf("socket() failed: %d\n", this->sock_fd);
        return;
    }
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    inet_pton(AF_INET, hostname, &srv.sin_addr);
}

void udp_client::connect() {
    // send magic
    char authentication_bytes[] = {6, 6, 4, 2, 9};
    this->write_data(authentication_bytes, 5);
}

int udp_client::write_data(void *buf, int count) {
    return sendto(this->sock_fd, static_cast<char*>(buf), count, 0, reinterpret_cast<sockaddr*>(&this->srv), sizeof(this->srv));
}

int udp_client::read_data(void *buf, int count) {
    sockaddr_in from{};
    unsigned int size = sizeof(from);
    int ret = recvfrom(this->sock_fd, static_cast<char*>(buf), count, 0, reinterpret_cast<sockaddr*>(&from), &size);
    return ret;
}

#ifdef WIN32
int udp_client::init_windows() {
    WSAData wsaData{};
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData); // NOLINT(hicpp-signed-bitwise)
    if(res < 0) {
        return res;
    }
    return 0;
}
#endif

void udp_client::disconnect() {
    this->write_data(new char[4]{9, 2, 4, 1}, 4);
}

udp_client::~udp_client() {

}

