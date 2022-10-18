#include <stdio.h>
#include "networking.h"
#include <string.h>
#include <vector>
#include <cctype>
#include <thread>

int main() {
    try {
        auto server = net::listen("0.0.0.0", 1024);
        net::Address clientAddr;
        auto client = server->accept(&clientAddr);

        printf("Client: %s:%d\n", clientAddr.getIPString().c_str(), clientAddr.getPort());

        client->close();
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}