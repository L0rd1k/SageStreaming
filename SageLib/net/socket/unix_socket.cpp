#include "unix_socket.h"

sage::UnixSocket::UnixSocket() {}

sage::UnixSocket::~UnixSocket() {}

sage::UnixSocket& sage::UnixSocket::operator=(const sage::UnixSocket& socket) {
    return *this;
}

bool sage::UnixSocket::init(const sage::Domain domain,
                            const sage::ConnType conn) {
    if ((socket_ = socket((int)sage::Domain::IP_V4, (int)sage::ConnType::UDP,
                          0)) == -1) {
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
        struct timeval recv_timeo = {time_recv};
        struct timeval send_timeo = {time_send};
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
        sockaddr_in address = {
            sin_family : AF_INET,
            sin_port : htons(port),
            sin_addr : in_addr{INADDR_ANY}
        };
        if(::bind(getSocket(), (struct sockaddr*)&address, sizeof(address)) == -1) {
            Log::critical("[Socket] Failed to bind socket");
            return false;
        }
        return true;
    }
    return false;
}

bool sage::UnixSocket::send(void* data, uint32_t size, sage::Address* addr) {

}
