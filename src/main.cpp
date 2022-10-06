#include <stdio.h>
#include "networking.h"
#include <string.h>

int main() {
    try {
        auto client = net::connect("localhost", 1234);
        
        uint8_t dummy[128];
        int ret = client->recv(dummy, 128, false, net::TIMEOUT_NONBLOCK);

        printf("%d, %s\n", ret, client->isOpen() ? "Y" : "N");
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}