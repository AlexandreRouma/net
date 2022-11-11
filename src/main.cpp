#include <stdio.h>
#include "networking.h"
#include <string.h>
#include <vector>
#include <cctype>
#include <thread>

int main() {
    try {
        auto ifaces = net::listInterfaces();
        for (const auto& [name, i] : ifaces) {
            printf("%s:\n", name.c_str());
            printf("    Address:   %d.%d.%d.%d\n", (i.address >> 24) & 0xFF, (i.address >> 16) & 0xFF, (i.address >> 8) & 0xFF, (i.address >> 0) & 0xFF);
            printf("    Netmask:   %d.%d.%d.%d\n", (i.netmask >> 24) & 0xFF, (i.netmask >> 16) & 0xFF, (i.netmask >> 8) & 0xFF, (i.netmask >> 0) & 0xFF);
            printf("    Broadcast: %d.%d.%d.%d\n\n", (i.broadcast >> 24) & 0xFF, (i.broadcast >> 16) & 0xFF, (i.broadcast >> 8) & 0xFF, (i.broadcast >> 0) & 0xFF);
        }
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}