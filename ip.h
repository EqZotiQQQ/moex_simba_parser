#pragma once

#include <cstdint>
#include <fstream>
#include <array>
#include <iomanip>
#include <unordered_map>

#include "types.h"
#include "constants.h"
#include "parsers.h"
#include "udp.h"

class IPHeader {
private:
    u32 magic_number {};
    u16 version_major {};
    u16 version_minor {};
    i32 time_zone {};
    u32 sig_figs {};
    u32 snap_len {};
    u32 network {}; // replace it with LinkType later ...

    Endian endian {};

public:
    static constexpr u8 size = {24};

    friend std::ostream& operator<<(std::ostream& os, const IPHeader& pcap);
    Endian parse(std::ifstream& file) {
        std::array<int, 4> buffer {};
        for (int i = 0; i < 4; i++) {
            buffer[i] = file.get();
        }
        magic_number = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];
        validate_endians();
        // Dream: parse<u16>();
        version_major = Parsers::parse_u16(file, endian);
        version_minor = Parsers::parse_u16(file, endian);
        time_zone = Parsers::parse_i32(file, endian);
        sig_figs = Parsers::parse_u32(file, endian);
        snap_len = Parsers::parse_u32(file, endian);
        network = Parsers::parse_u32(file, endian);
        return endian;
    };

    void validate_endians() {
        switch (magic_number) {
            case Constants::little_endian_milliseconds: endian = Endian::big_endian;
            case Constants::little_endian_nanoseconds: endian = Endian::big_endian; break;
            case Constants::big_endian_milliseconds: endian = Endian::little_endian;
            case Constants::big_endian_nanoseconds: endian = Endian::little_endian; break;
            default: throw InvalidMagicNumberException();
        }
    }
};

class IPPacket {
    IPHeader ip_header;
    UDPPacket udp_packet;
    Endian endian {};
friend std::ostream& operator<<(std::ostream& os, const IPPacket& ip);
public:
    IPPacket() {}

    void parse(std::ifstream& file) {
        endian = ip_header.parse(file);
        int i {};
        while (!file.eof()) {
            i++;
            std::cout << "================================ Packet number " << i << '\n';
            udp_packet.parse(file, endian);
            std::cout << udp_packet << '\n';
        }
    }
};

std::ostream& operator<<(std::ostream& os, const IPPacket& ip) {
    os << ip.ip_header << '\n';
    os << ip.udp_packet << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const IPHeader& pcap) {
    os << "===============IP Header=================\n";
    os << std::setw(2) << std::right << std::hex << "Magic numer: " << ((pcap.magic_number >> 24)&0xFF) << ' ' << ((pcap.magic_number >> 16)&0xFF) << ' ' << ((pcap.magic_number >> 8)&0xFF) << ' ' << ((pcap.magic_number)&0xFF) << '\n';
    os << "Version Major: " << std::dec << pcap.version_major << '\n';
    os << "Version Minor: " << pcap.version_minor << '\n';
    os << "Time zone: " << pcap.time_zone << '\n';
    os << "Sig figs: " << pcap.sig_figs << '\n';
    os << "Snap len: " << pcap.snap_len << '\n';
    os << "Network: " << pcap.network << '\n';
    os << "===============IP Header end==============\n";
    return os;
}
