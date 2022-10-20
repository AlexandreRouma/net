# Basic Usage

For detailed information, check the header. Everything is documented in javadoc format for easy autocompletion.

## Adding to a project

Simply copy the `networking.h` and `networking.cpp` files from the `lib` directory to a suitable place in your project.

## Creating a TCP server

Here is a simple "echo" server example

```c++
#include <stdio.h>
#include "networking.h"

int main() {
    try {
        // Listen on any IP on port 1234.
        auto listener = net::listen("0.0.0.0", 1234);

        while (true) {
            // Wait for a client
            net::Address clientAddress;
            auto client = client->accept(&clientAddress);

            // Show info about the client in the console
            char addrStr[128];
            sprintf(addrStr, "%s:%d", clientAddress.getIPStr().c_str(), clientAddress.getPort());
            printf("Connection from %s\n", addrStr);

            while (true) {
                // Read line or end connection on error/disconnect
                std::string line;
                if (client->recvline(line) <= 0) {
                    break;
                }

                // print line to console
                printf("[%s] '%s'\n", line.c_str());

                // Send back the line
                client->sendstr(line + '\n');
            }

            // End connection just in case.
            client->close();
        }
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}
```

Here to, you can alternatively give `net::listen` a `net::Address` instead of a host/port pair.

## Connecting to a TCP server

To connect to a server, use the `net::connect()` function. On error, it throws an exception.

```c++
#include <stdio.h>
#include "networking.h"

int main() {
    try {
        // Connect to the server
        auto client = net::connect("yourserver.com", 1234);

        // Send a few characters
        client->sendstr("Hello World!\n");

        // Close the connection
        client->close();
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}
```

You can alternatively give `net::connect` a `net::Address` instead of a host/port pair.

## Create a UDP socket

This example sends a broadcast message, waits for a response and creates a new socket from it.

```c++
#include <stdio.h>
#include "networking.h"

int main() {
    try {
        // Create a UDP socket that sends on the broadcast address on port 1234.
        auto sock = net::openudp("192.168.0.255", 1234);

        while (true) {
            // Broadcast a message
            sock->sendstr("Hello! Is anyone here?");

            // Handle all responses
            while (true) {
                // Wait for a response or timeout after 1 seconds (1000 milliseconds) if nobody responds.
                char msg[1024];
                net::Address remoteAddr;
                int len = sock->recv((uint8_t*)msg, sizeof(msg)-1, false, 1000, &remoteAddr);
                
                // If we timed out, break out of the loop and send back the message again.
                if (len <= 0) {
                    break;
                }

                // Add a null termination so we can print properly.
                msg[len] = 0;

                // Show the response.
                printf("[%s:%d] '%s'\n", clientAddress.getIPStr().c_str(), clientAddress.getPort(), msg);
            }
        }
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}
```

Again, you can alternatively give `net::listen` a `net::Address` instead of a host/port pair.

This next example acts as the listener for the example before.

```c++
#include <stdio.h>
#include "networking.h"

int main() {
    try {
        // Create a UDP socket with no particular destination since we'll specify a destination for each response. Listen on any IP and port 1234.
        auto sock = net::openudp(Address(), "0.0.0.0", 1234);

        while (true) {
            // wait for a message
            char msg[1024];
            net::Address remoteAddr;
            int len = sock->recv((uint8_t*)msg, sizeof(msg)-1, false, net::NO_TIMEOUT, &remoteAddr);

            // Send back directly to the server
            sock->sendstr("I'm here!", &remoteAddr);
        }
    }
    catch (std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
    }
    return 0;
}
```

# Important notes

This project is still in development.