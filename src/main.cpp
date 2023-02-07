#include <stdio.h>
#include "net.h"
#include <string.h>
#include <vector>
#include <cctype>
#include <thread>
#include <string>
#include <fstream>
#include "proto/rigctl.h"

int main() {
    try {
        auto client = net::rigctl::connect("172.19.13.134");

        printf("%d\n", client->getAntenna());
        int ret = client->setAntenna(1);
        printf("ret: %d\n", ret);
        printf("%d\n", client->getAntenna());

        client->close();
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}