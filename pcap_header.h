#include <cstdint>
#include <iomanip>

#include "types.h"

struct PcapHeader {
    u32 magic_number {};
    u16 version_major {};
    u16 version_minor {};
    i32 time_zone {};
    u32 sig_figs {};
    u32 snap_len {};
    u32 network {}; // replace it with LinkType later ...
};

//Pizdec
std::ostream& operator<<(std::ostream& os, const PcapHeader& pcap) {
    os << std::setfill('0') << std::setw(2) << std::right << std::hex
       << "Magic numer: " << ((pcap.magic_number >> 24)&0xFF) << ' ' << ((pcap.magic_number >> 16)&0xFF) << ' ' << ((pcap.magic_number >> 8)&0xFF) << ' ' << ((pcap.magic_number)&0xFF)
       << "\nVersion Major: " << std::setfill('0') << std::setw(2) << ((pcap.version_major >> 8) & 0xFF) << ' ' << std::setfill('0') << std::setw(2) << ((pcap.version_major) & 0xFF)
       << "\nVersion Minor: " << std::setfill('0') << std::setw(2) << ((pcap.version_minor >> 8) & 0xFF) << ' ' << std::setfill('0') << std::setw(2) << ((pcap.version_minor) & 0xFF)
       << "\nTime zone: " << std::setfill('0') << std::setw(2) << ((pcap.time_zone >> 24)&0xFF) << ' ' << std::setfill('0') << std::setw(2) << ((pcap.time_zone >> 16)&0xFF) << ' ' << std::setfill('0') << std::setw(2) << ((pcap.time_zone >> 8)&0xFF) << ' ' << std::setfill('0') << std::setw(2) << ((pcap.time_zone)&0xFF)
       << "\nSig figs: " <<  std::setfill('0') << std::setw(2) << ((pcap.sig_figs >> 24)&0xFF)  << ' ' << std::setfill('0') << std::setw(2) << ((pcap.sig_figs >> 16)&0xFF)  << ' ' << std::setfill('0') << std::setw(2) << ((pcap.sig_figs >> 8)&0xFF)  << ' ' << std::setfill('0') << std::setw(2) << ((pcap.sig_figs)&0xFF)
       << "\nsnap len: " <<  std::setfill('0') << std::setw(2) << ((pcap.snap_len >> 24)&0xFF)  << ' ' << std::setfill('0') << std::setw(2) << ((pcap.snap_len >> 16)&0xFF)  << ' ' << std::setfill('0') << std::setw(2) << ((pcap.snap_len >> 8)&0xFF)  << ' ' << std::setfill('0') << std::setw(2) << ((pcap.snap_len)&0xFF)
       << "\nnetwork: " <<   std::setfill('0') << std::setw(2) << ((pcap.network >> 24)&0xFF)   << ' ' << std::setfill('0') << std::setw(2) << ((pcap.network >> 16)&0xFF)   << ' ' << std::setfill('0') << std::setw(2) << ((pcap.network >> 8)&0xFF)   << ' ' << std::setfill('0') << std::setw(2) << ((pcap.network)&0xFF);
    return os;
}

