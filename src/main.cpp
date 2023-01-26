#include <stdio.h>
#include "net.h"
#include <string.h>
#include <vector>
#include <cctype>
#include <thread>
#include <string>
#include <fstream>
#include "proto/http.h"

int main() {
    try {
        // Create client
        auto sock = net::connect("localhost", 54664);
        net::http::Client client(sock);
        
        // Build request
        net::http::RequestHeader rqhdr(net::http::METHOD_GET, "/stream?format=float32", "localhost");

        // Send request
        client.sendRequestHeader(rqhdr);

        // Receive response
        net::http::ResponseHeader rshdr;
        client.recvResponseHeader(rshdr, 5000);

        // Dump response
        printf("Status Code: %d - '%s'\n", rshdr.getStatusCode(), rshdr.getStatusString().c_str());
        auto fields = rshdr.getFields();
        for (const auto& [key, value] : fields) {
            printf("%s: %s\n", key.c_str(), value.c_str());
        }

        if (fields["Transfer-Encoding"] != "chunked") {
            fprintf(stderr, "Encoding isn't chunked, not gonna work...\n");
            return -1;
        }

        const size_t dummyBufSize = 8192;
        uint8_t dummyBuf[dummyBufSize];

        while (sock->isOpen()) {
            // Get chunk header
            net::http::ChunkHeader chdr;
            client.recvChunkHeader(chdr, 5000);

            // If null length, finish
            size_t clen = chdr.getLength();
            if (!clen) {
                break;
            }

            // Read JSON
            std::string jsonData;
            int jlen = sock->recvline(jsonData, clen, 5000);
            if (jlen <= 0) {
                fprintf(stderr, "Something went wrong...\n");
                break;
            }

            // Read (and check for) record separator
            uint8_t rs;
            int rslen = sock->recv(&rs, 1, true, 5000);
            if (rslen != 1 || rs != 0x1E) {
                fprintf(stderr, "Something went wrong...\n");
                break;
            }

            // Read data
            for (int i = jlen + 1; i < clen;) {
                int toRead = std::min<int>(clen - i, dummyBufSize);
                sock->recv(dummyBuf, toRead, true);
                i += toRead;
            }
            printf("Received %llu bytes\n", clen);

            // Read trailing CRLF
            std::string dummy;
            sock->recvline(dummy, 2);
            if (dummy != "\r") {
                fprintf(stderr, "Something went wrong...\n");
                break;
            }
        }

        // Close client
        sock->close();
        
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}