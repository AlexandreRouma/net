#include <stdio.h>
#include "networking.h"
#include <string.h>
#include <vector>
#include <cctype>

std::string trim(const std::string& str) {
    int beg, end;
    for (beg = 0; beg < str.length(); beg++) {
        if (!std::isspace(str[beg])) { break; }
    }
    for (end = str.length() - 1; end > beg; end--) {
        if (!std::isspace(str[end])) { break; }
    }
    int rem = beg + ((str.length() - 1) - end);
    return str.substr(beg, str.length() - rem);
}

std::vector<std::string> split(const std::string& str) {
    
}

int main() {

    try {
        auto server = net::listen("0.0.0.0", 80);

        while (true) {
            auto client = server->accept();

            // Read first header
            std::string header;
            if (client->recvline(header) <= 0) { continue; }
            header = trim(header);

            // Decode first header


            // Read other headers
        }
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}