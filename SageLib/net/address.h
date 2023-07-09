#pragma once

#include <inttypes.h>

#include <string>

namespace sage {

class Address {
public:
    Address() = default;
    Address(std::string addr) { extractAddressInfo(addr); }
    Address(std::string ip, uint16_t port) : ip_(ip), port_(port) {}
    Address(const Address &addr) {
        ip_ = addr.ip_;
        port_ = addr.port_;
    }
    Address &operator=(const Address &addr) {
        ip_ = addr.ip_;
        port_ = addr.port_;
        return *this;
    }
    std::string getIp() { return ip_; }
    uint16_t getPort() { return port_; }
    void setIp(std::string ip) { ip_ = ip; }
    void setPort(uint16_t port) { port_ = port; }
    void setAddress(const std::string addr) { extractAddressInfo(addr); }

private:
    void extractAddressInfo(const std::string &addr) {
        if (!addr.empty()) {
            std::size_t midPos = addr.find(":");
            if (midPos != std::string::npos) {
                ip_ = addr.substr(0, midPos);
            } else {
                ip_ = addr.substr(0, addr.size());
                midPos = addr.size() - 1;
            }
            port_ = (addr.size() >= midPos + 2)
                        ? atoi(addr.substr(midPos + 1, addr.size() - 1).c_str())
                        : 0;
        }
    }

private:
    std::string ip_;
    uint16_t port_;
};

}  // namespace sage