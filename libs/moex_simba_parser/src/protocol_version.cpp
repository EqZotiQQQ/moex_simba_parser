#include <stdexcept>
#include "protocol_version.h"

ProtocolVersion::ProtocolVersion(): value(0) {}

ProtocolVersion::ProtocolVersion(uint32_t version): value(version) {
    switch (version) {
        case 0x0800: {
            value = version;
            break;
        }
        default: {
            throw std::runtime_error("Protocol version not supported");
        }
    }
}

std::string ProtocolVersion::to_string() const {
    std::string ordering{std::to_string(value)};
    switch (value) {
        case 0x0800: {
            ordering += " => IPv4";
            break;
        }
        default: {
            throw std::runtime_error("Protocol version not supported");
        }
    }
    return ordering;
}

std::ostream& operator<<(std::ostream& os, const ProtocolVersion& version) {
    os << version.to_string();
    return os;
}