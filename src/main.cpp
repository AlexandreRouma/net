#include <stdio.h>
#include "networking.h"
#include <string.h>

int main() {
    // auto client = net::connect("example.com", 80);

    // client->sendstr("GET /index.html HTTP/1.1\n");
    // client->sendstr("Host: example.com\n");
    // client->sendstr("\n");

    // Sleep(5000);

    // printf("Success\n");

    try {
        auto client = net::openudp("8.8.8.8", 53);
        if (client->type() == net::SOCKET_TYPE_UDP) {
            printf("I'm a UDP socket!\n");
        }
        else {
            printf("I'm a TCP socket!\n");
        }
    }
    catch (std::exception e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    
    return 0;
}