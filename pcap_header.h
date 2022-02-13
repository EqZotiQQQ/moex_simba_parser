#pragma once

#include <cstdint>
#include <fstream>
#include <array>
#include <iomanip>

#include "types.h"
#include "constants.h"
#include "parsers.h"

struct PcapHeader {
    u32 magic_number {};
    u16 version_major {};
    u16 version_minor {};
    i32 time_zone {};
    u32 sig_figs {};
    u32 snap_len {};
    u32 network {}; // replace it with LinkType later ...
    static constexpr u8 size_bytes = {24};

    static PcapHeader parse_pcap_header(std::ifstream& file) {
        PcapHeader header {};
        std::array<int, 4> buffer {};
        for (int i = 0; i < 4; i++) {
            buffer[i] = file.get();
        }
        header.magic_number = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];
        Endian endian = validate_endians(header);

        // Dream: parse<u16>();
        header.version_major = Parsers::parse_u16(file, endian);
        header.version_minor = Parsers::parse_u16(file, endian);
        header.time_zone = Parsers::parse_i32(file, endian);
        header.sig_figs = Parsers::parse_u32(file, endian);
        header.snap_len = Parsers::parse_u32(file, endian);
        header.network = Parsers::parse_u32(file, endian);
        return header;
    };

    static Endian validate_endians(PcapHeader& header) {
        switch (header.magic_number) {
            case Constants::little_endian_milliseconds: {
                return Endian::big_endian;
//                time_stamp = TimeStamp::milliseconds;
//                break;
            }
            case Constants::big_endian_milliseconds: {
                return Endian::little_endian;
//                time_stamp = TimeStamp::milliseconds;
//                break;
            }
            case Constants::little_endian_nanoseconds: {
                return Endian::big_endian;
//                time_stamp = TimeStamp::nanoseconds;
//                break;
            }
            case Constants::big_endian_nanoseconds: {
                return Endian::little_endian;
//                time_stamp = TimeStamp::nanoseconds;
//                break;
            }
            default: {
                throw InvalidMagicNumberException();
            }
        }
    }
};

std::ostream& operator<<(std::ostream& os, const PcapHeader& pcap) {
    os << "Header: =======================================\n";
    os << std::setw(2) << std::right << std::hex << "Magic numer: " << ((pcap.magic_number >> 24)&0xFF) << ' ' << ((pcap.magic_number >> 16)&0xFF) << ' ' << ((pcap.magic_number >> 8)&0xFF) << ' ' << ((pcap.magic_number)&0xFF) << '\n';
    os << "Version Major: " << std::dec << pcap.version_major << '\n';
    os << "Version Minor: " << pcap.version_minor << '\n';
    os << "Time zone: " << pcap.time_zone << '\n';
    os << "Sig figs: " << pcap.sig_figs << '\n';
    os << "Snap len: " << pcap.snap_len << '\n';
    os << "Network: " << pcap.network << '\n';
    os << "Header end: ++++++++++++++++++++++++++++++++++++\n";
    return os;
}

// http://www.kroosec.com/2012/10/a-look-at-pcap-file-format.html
// https://wiki.wireshark.org/Development/LibpcapFileFormat
// https://www.slideshare.net/ShravanKumarCEHOSCP/pcap-headers-description

// GLOBAL HEADER
//typedef struct pcap_hdr_s {
//    guint32 magic_number;   /* magic number */
//    guint16 version_major;  /* major version number */
//    guint16 version_minor;  /* minor version number */
//    gint32  thiszone;       /* GMT to local correction */
//    guint32 sigfigs;        /* accuracy of timestamps */
//    guint32 snaplen;        /* max length of captured packets, in octets */
//    guint32 network;        /* data link type */
//} pcap_hdr_t;
//
//Header size = 24 bytes:
//
//magic_number = 4 bytes (d4 c3 b2 a1)
//version_major = 2 bytes (02 00)
//version_minor = 2 bytes (04 00) *in our case 2.4. (little endian)
//thiszone = 4 bytes (00 00 00 00) *usually set to 0
//sigfigs = 4 bytes (00 00 00 00) *usually set to 0
//snaplen = 4 bytes (FF FF 00 00) *maximum length of the captured packets (data#) in bytes, here its 65535 (0xffff) which is default value for tcpdump and wireshark)
//network = 4 bytes (01 00 00 00) *0x1 which indicates that the link-layer protocol is Ethernet. Full list: http://www.tcpdump.org/linktypes.html
