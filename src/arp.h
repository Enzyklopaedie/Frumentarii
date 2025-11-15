#pragma once
#include <cstdint>
#include <array>

struct EthernetHeader {
    std::array<uint8_t, 6> destMAC;   // Destination-MAC
    std::array<uint8_t, 6> srcMAC;    // Source-MAC
    uint16_t etherType;               // 0x0806 for ARP
};

struct ArpHeader {
    uint16_t hardwareType;      // Hardwaretype (e.g., Ethernet = 1)
    uint16_t protocolType;      // Protocoltype (e.g. IPv4 = 0x0800)
    uint8_t hardwareSize;       // Length of Hardwareaddress (e.g. MAC = 6)
    uint8_t protocolSize;       // Length of Protocoladdress (e.g. IPv4 = 4)
    uint16_t opcode;            // ARP-Operation (1 = Request, 2 = Response)
    std::array<uint8_t, 6> senderMac;   // MAC-Address of Source
    std::array<uint8_t, 4> senderIp;    // IP-Address of Source
    std::array<uint8_t, 6> targetMac;   // MAC-Address of Target
    std::array<uint8_t, 4> targetIp;    // IP-Address of Target
};

std::array<uint8_t, 42> buildArpRequest(
    const std::array<uint8_t, 6>& srcMac,
    const std::array<uint8_t, 4>& srcIp,
    const std::array<uint8_t, 4>& targetIp
);