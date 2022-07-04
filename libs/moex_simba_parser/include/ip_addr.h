#pragma once

#include <iostream>

struct IpAddr {
    using Value = std::array<uint8_t, 4>;

    Value ip;

    std::string to_string() const {
        return fmt::format("{}", fmt::join(ip, ":"));
    }

    friend std::ostream& operator<<(std::ostream& os, const IpAddr& header) {
        os << header.to_string();
        return os;
    }
};

struct MacAddr {
    using Value = std::array<uint8_t, 6>;

    Value ip;

    std::string to_string() const {
        std::string result;
        for (int i = 0; i < 5; i++) {
            result += fmt::format("{:02X}:", ip[i]);
        }
        result += fmt::format("{:02X}", ip[5]);
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const MacAddr& header) {
        os << header.to_string();
        return os;
    }
};