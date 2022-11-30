#pragma once

#include "utils/log.h"
#include "net/address.h"

namespace sage {

enum class Domain;
enum class ConnType;

class Socket {
public:
    virtual bool init(const Domain, const ConnType) = 0;
    virtual bool bind(uint16_t, time_t, time_t) = 0;
    virtual bool send(void*, uint32_t, sage::Address*) = 0;
    int getSocket() { return socket_; }
    uint16_t getPort() { return port_; };
    
protected:
    int socket_ = 0;
    uint16_t port_ = 0;
};

}  // namespace sage