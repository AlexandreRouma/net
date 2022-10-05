#include "networking.h"
#include <string.h>

namespace net {
    Socket::Socket(SockHandle_t sock, struct sockaddr_in* raddr) {
        this->sock = sock;
        if (raddr) {
            this->raddr = (struct sockaddr_in*)malloc(sizeof(sockaddr_in));
            memcpy(this->raddr, raddr, sizeof(sockaddr_in));
        }
    }

    Socket::~Socket() {
        close();
        if (raddr) { free(raddr); }
    }

    void Socket::close() {
        std::lock_guard<std::recursive_mutex> lck(mtx);
        if (!open) { return; }
    }

    int Socket::send(const uint8_t* data, size_t len) {
        return sendto(sock, data, len, 0, (sockaddr*)raddr, sizeof(sockaddr_in));
    }

    int Socket::recv(uint8_t* data, size_t maxLen, int timeout) {
        // Create FD set
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sock, &set);

        // Define timeout
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = timeout * 1000;

        // Wait for data or error
        int err = select(sock+1, &set, NULL, &set, timeout ? &tv : NULL);
        if (err <= 0) { return err; }

        // Receive
        return ::recv(sock, data, maxLen, 0);
    }

    std::shared_ptr<Socket> connect(std::string host, int port) {
        // Get host address
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (!queryHost(addr.sin_addr.s_addr, host)) { return NULL; }
        
        // Create socket
        SockHandle_t s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        // Connect to server
        if (::connect(s, (sockaddr*)&addr, sizeof(addr))) {
            close(s);
            return NULL;
        }

        // Return socket class
        return std::make_shared<Socket>(s);
    }

    std::shared_ptr<Socket> udp(std::string lhost, int lport, std::string rhost, int rport) {
        // Get local address
        struct sockaddr_in laddr;
        laddr.sin_family = AF_INET;
        laddr.sin_port = htons(lport);
        if (!queryHost(laddr.sin_addr.s_addr, lhost)) { return NULL; }

        // Get remote address
        struct sockaddr_in raddr;
        raddr.sin_family = AF_INET;
        raddr.sin_port = htons(rport);
        if (!queryHost(raddr.sin_addr.s_addr, rhost)) { return NULL; }

        // Create socket
        SockHandle_t s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        // Bind socket to local port
        if (bind(s, (sockaddr*)&laddr, sizeof(laddr))) {
            close(s);
            return NULL;
        }
        
        // Return socket class
        return std::make_shared<Socket>(s, &raddr);
    }

    bool queryHost(uint32_t& addr, std::string host) {
        hostent* ent = gethostbyname(host.c_str());
        if (!ent || !ent->h_addr_list[0]) { return false; }
        addr = *(uint32_t*)ent->h_addr_list[0];
        return true;
    }
}