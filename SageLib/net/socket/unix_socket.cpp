#include "unix_socket.h"

sage::UnixSocket::UnixSocket() {}

sage::UnixSocket::~UnixSocket() {}

sage::UnixSocket& sage::UnixSocket::operator=(const sage::UnixSocket&) {
    return *this;
}

bool sage::UnixSocket::init(const sage::Domain domain,
                            const sage::ConnType conn) {
    if ((socket_ = socket((int)domain, (int)conn, 0)) == -1) {
        Log::critical("[Socket] Failed to create socket");
        return false;
    }
    return true;
}

bool sage::UnixSocket::enableBroadcast() {
    if (getSocket() != -1) {
        int isBroadcast = 1;
        if (setsockopt(getSocket(), SOL_SOCKET, SO_BROADCAST, &isBroadcast,
                       sizeof(isBroadcast)) == -1) {
            Log::critical("[Socket] Failed to set broad cast option.");
            return false;
        }
    }
    return true;
}

bool sage::UnixSocket::bind(uint16_t port, time_t time_recv, time_t time_send) {
    if (getSocket() != -1) {
        struct timeval recv_timeo;
        recv_timeo.tv_sec = time_recv;
        struct timeval send_timeo;
        send_timeo.tv_sec = time_send;
        if (setsockopt(getSocket(), SOL_SOCKET, SO_RCVTIMEO, &recv_timeo,
                       sizeof(recv_timeo)) == -1) {
            Log::critical("[Socket] Failed to set socket recieve timeout");
            return false;
        }
        if (setsockopt(getSocket(), SOL_SOCKET, SO_SNDTIMEO, &send_timeo,
                       sizeof(send_timeo)) == -1) {
            Log::critical("[Socket] Failed to set socket recieve timeout");
            return false;
        }
        sockaddr_in address;
        address.sin_family = AF_INET, address.sin_port = htons(port),
        address.sin_addr = in_addr{INADDR_ANY};
        if (::bind(getSocket(), (struct sockaddr*)&address, sizeof(address)) ==
            -1) {
            Log::critical("[Socket] Failed to bind socket");
            return false;
        }
        return true;
    }
    return false;
}

bool sage::UnixSocket::send(void* data, uint32_t size,
                            sage::Address* addrReceiver) {
    if (getSocket() != -1) {
        if (addrReceiver) {
            sockaddr_in address;
            address.sin_family = AF_INET,
            address.sin_port = htons(addrReceiver->getPort());
            if (!inet_aton(addrReceiver->getIp().c_str(), &address.sin_addr)) {
                Log::critical("[Socket] Can't convert IP",
                              addrReceiver->getIp(), "to binary mode.");
                return false;
            }
            if (sendto(getSocket(), data, size, 0, (struct sockaddr*)&address,
                       sizeof(address)) == -1) {
                return false;
            }
            return true;
        }
    }
    return false;
}

bool sage::UnixSocket::receive(void* data, uint32_t size,
                               sage::Address* addrSender) {
    if (getSocket() != -1) {
        if (addrSender) {
            struct sockaddr_storage address;
            socklen_t address_len = sizeof(address);
            if (recvfrom(getSocket(), data, size, 0, (struct sockaddr*)&address,
                         &address_len) > 0) {
                char host[NI_MAXHOST];
                char port[NI_MAXSERV];
                if (!getnameinfo((struct sockaddr*)&address, address_len, host,
                                 sizeof(host), port, sizeof(port),
                                 NI_NUMERICHOST | NI_NUMERICSERV)) {
                    addrSender->setIp(std::string(host));
                    addrSender->setPort(std::stoi(port));
                    return true;
                } else {
                    Log::critical("[Socket] Error obtain sender address");
                }
            }
        }
    }
    return false;
}

bool sage::UnixSocket::close() {
    if (::close(getSocket() == -1)) {
        return false;
    }
    return true;
}
