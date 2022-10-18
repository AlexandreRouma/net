#include <stdio.h>
#include "networking.h"
#include <string.h>
#include <vector>
#include <cctype>
#include <thread>

int main() {
    try {
        auto sock = net::openudp("192.168.0.255", 1024);

        uint8_t data[60];
        memset(data, 0, sizeof(data));

        *(uint16_t*)&data[0] = htons(0xEFFE);
        data[2] = 0x02;

        sock->send(data, sizeof(data));

        Sleep(1000);
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}