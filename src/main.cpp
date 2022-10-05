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
static_assert(sizeof(DNSHeader) == 12);
#pragma pack(pop)

int main() {
    auto client = net::connect("139.165.99.199", 53);
    //auto client = net::udp("0.0.0.0", 4532, "1.1.1.1", 53);
    if (!client) {
        fprintf(stderr, "Connection failed\n");
        return -1;
    }

    char qname[] = "\003www\007ryzerth\003com";

    uint8_t data[sizeof(DNSHeader) + sizeof(qname) + 4];
    memset(data, 0, sizeof(data));
    DNSHeader* hdr = (DNSHeader*)&data[0];
    hdr->id = htons(0x4269);
    hdr->flags = htons(0b0000000100000000);
    hdr->qdCount = htons(1);

    memcpy(&data[sizeof(DNSHeader)], qname, sizeof(qname));
    
    *((uint16_t*)&data[sizeof(DNSHeader) + sizeof(qname)]) = htons(5);
    *((uint16_t*)&data[sizeof(DNSHeader) + sizeof(qname) + 2]) = htons(1);


    client->send(data, sizeof(data));

    sleep(1);

    printf("Success\n");
    return 0;
}