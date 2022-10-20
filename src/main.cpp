#include <stdio.h>
#include "networking.h"
#include <string.h>
#include <vector>
#include <cctype>
#include <thread>

int main() {
    try {
        auto listener = net::listen("0.0.0.0", 1234);

        auto client = listener->accept(NULL, net::NONBLOCKING);

        printf("Client null: %s, Is listening: %s\n", !client ? "Y":"N", listener->listening() ? "Y":"N");

        if (client) {
            client->sendstr("Hello!\n");
            client->close();
        }
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}