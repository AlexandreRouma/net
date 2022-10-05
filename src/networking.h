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

        /**
         * Close socket. The socket can no longer be used after this.
         */
        void close();

        /**
         * Send data on socket.
         * @param data Data to be sent.
         * @param len Number of bytes to be sent.
         * @return Number of bytes sent.
         */
        int send(const uint8_t* data, size_t len);

        /**
         * Send string on socket. Terminating null byte is not sent, include one in the string if you need it.
         * @param str String to be sent.
         * @return Number of bytes sent.
         */
        int sendstr(const std::string& str);

        /**
         * Receive data from socket.
         * @param data Buffer to read the data into.
         * @param maxLen Maximum number of bytes to read.
         * @param timeout Timeout in milliseconds. 0 means no timeout.
         * @return Number of bytes read
         */
        int recv(uint8_t* data, size_t maxLen, int timeout = 0);

    private:
        std::recursive_mutex mtx;
        struct sockaddr_in* raddr = NULL;
        SockHandle_t sock;
        bool open = true;

    };

    class Listener {
    public:
        Listener(SockHandle_t sock);
        ~Listener();

        /**
         * Stop listening. The listener can no longer be used after this.
         */
        void stop();

        /**
         * Accept connection.
         * @param timeout Timeout in milliseconds. 0 means no timeout.
         * @return Socket of the connection
         */
        std::shared_ptr<Socket> accept(int timeout);

    private:
        std::recursive_mutex mtx;
        SockHandle_t sock;
        bool open = true;

    };

    /**
     * Initialize networking library. No need to call manually, functions that need it call it.
     */
    void init();

    /**
     * Create TCP listener.
     * @param host Hostname or IP to listen on.
     * @param port Port to listen on.
     * @return Listener instance on success, null otherwise.
     */
    std::shared_ptr<Listener> listen(std::string host, int port);

    /**
     * Create TCP connection.
     * @param host Remote hostname or IP address.
     * @param port Remote port.
     * @return Socket instance on success, null otherwise.
     */
    std::shared_ptr<Socket> connect(std::string host, int port);

    /**
     * Create UDP socket.
     * @param rhost Remote hostname or IP address.
     * @param rport Remote port.
     * @param lhost Local hostname or IP used to bind the socket (optional, "0.0.0.0" for Any).
     * @param lpost Local port used to bind the socket (optional, 0 to allocate automatically).
     * @return Socket instance on success, null otherwise.
     */
    std::shared_ptr<Socket> openudp(std::string rhost, int rport, std::string lhost = "0.0.0.0", int lport = 0);

    /**
     * Get IP address in uint32 form associated with a hostname or parse IP address.
     * @param addr Pointer to uint32 that will store the address if successful
     * @return True on success, false otherwise.
     */    
    bool queryHost(uint32_t* addr, std::string host);
}