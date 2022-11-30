#pragma once 

#include "socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>

namespace sage {

enum class Domain {
    IP_LOCAL = AF_LOCAL,
    IP_V4 = AF_INET,
    IP_V6 =  AF_INET6,
};

enum class ConnType {
    UDP = SOCK_DGRAM,
    TCP = SOCK_STREAM,
};

class UnixSocket : public Socket {
public:
    UnixSocket();
    virtual ~UnixSocket();
    UnixSocket(const UnixSocket&) = default;
    UnixSocket& operator=(const UnixSocket&);
    bool init(const sage::Domain domain, const sage::ConnType conn) override;
    bool bind(uint16_t, time_t, time_t) override;
    bool send(void* data, uint32_t size, sage::Address* addr) override;
    bool enableBroadcast();
    int setOption(int sockfd, int level, int optname,  const void *optval, socklen_t optlen);
};

}