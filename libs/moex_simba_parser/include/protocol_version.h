#pragma once

#include <cstdint>
#include <string>

struct ProtocolVersion {
    uint16_t value;

    ProtocolVersion();

    explicit ProtocolVersion(uint32_t value);

    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& os, const ProtocolVersion& header);
};

