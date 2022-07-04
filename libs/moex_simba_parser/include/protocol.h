#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

enum class ProtocolValue {
    UDP = 17,
};

struct Protocol {
    ProtocolValue value {};

    Protocol() {};

    explicit Protocol(uint16_t proto) {
        switch (proto) {
            case 17: {
                value = ProtocolValue::UDP;
                break;
            }
            default: {
                throw std::runtime_error("Unsupported protocol");
            }
        }
    }

    std::string to_string() const {
        std::string result = std::to_string(static_cast<uint32_t>(value));
        switch (value) {
            case ProtocolValue::UDP: {
                result += " => UDP";
                break;
            }
            default: {
                throw std::runtime_error("Unsupported protocol");
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Protocol& protocol) {
        os << protocol.to_string();
        return os;
    }
};

