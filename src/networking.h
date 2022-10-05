#pragma once
#include <stdint.h>
#include <mutex>
#include <memory>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <poll.h>
#endif

namespace net {
#ifdef _WIN32
    typedef SOCKET SockHandle_t;
#else
    typedef int SockHandle_t;
#endif

    class Socket {
    public:
        Socket(SockHandle_t sock, struct sockaddr_in* raddr = NULL);
        ~Socket();

        void close();

        int send(const uint8_t* data, size_t len);
        int recv(uint8_t* data, size_t maxLen, int timeout = 0);

    private:
        std::recursive_mutex mtx;
        bool open = false;
        SockHandle_t sock;

        struct sockaddr_in* raddr;

    };

    class Listener {
    public:
        Listener(SockHandle_t sock);
        ~Listener();

        void close();

        std::shared_ptr<Socket> accept(int timeout);

    private:

    };

    std::shared_ptr<Socket> connect(std::string host, int port);
    std::shared_ptr<Socket> udp(std::string lhost, int lport, std::string rhost, int rport);

    bool queryHost(uint32_t& addr, std::string host);
}