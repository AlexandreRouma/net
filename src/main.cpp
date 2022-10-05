#include <stdio.h>
#include "networking.h"
#include <string.h>

#pragma pack(push, 1)
struct DNSHeader {
    uint16_t id;
    uint16_t flags;
    uint16_t qdCount;
    uint16_t anCount;
    uint16_t nsCount;
    uint16_t arCount;
};
#pragma pack(pop)

int main() {
    auto client = net::connect("example.com", 80);

    client->sendstr("GET /index.html HTTP/1.1\n");
    client->sendstr("Host: example.com\n");
    client->sendstr("\n");

    Sleep(5000);

    printf("Success\n");
    return 0;
}