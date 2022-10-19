#include <stdio.h>
#include "networking.h"
#include <string.h>
#include <vector>
#include <cctype>
#include <thread>

int main() {
    try {
        auto sock = net::openudp("192.168.0.255", 1024);

        uint8_t pkt[64];
        memset(pkt, 0, sizeof(pkt));
        *(uint16_t*)&pkt[0] = htons(0xEFFE);
        pkt[2] = 2;
        sock->send(pkt, sizeof(pkt));

        uint8_t rpkt[1024];
        net::Address raddr;
        sock->recv(rpkt, sizeof(rpkt), false, net::NO_TIMEOUT, &raddr);

        printf("Receive response from %s:%d\n", raddr.getIPStr().c_str(), raddr.getPort());
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}