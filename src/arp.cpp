#include "arp.h"
#include <string>
#include <ifaddrs.h>
#include <cstring>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/ether.h>

/*
std::array<uint8_t, 42> buildARPRequest(
    const uint8_t senderMAC[6], 
    const uint8_t senderIP[4], 
    const uint8_t targetIP[4])
{
    std::array<uint8_t, 42> packet{};
    
    // --- Ethernet Header ---
    for(int i=0;i<6;i++) packet[i] = 0xFF;          // Broadcast Ziel-MAC
    for(int i=0;i<6;i++) packet[6+i] = senderMAC[i]; // Sender-MAC
    packet[12] = 0x08;  // EtherType 0x0806
    packet[13] = 0x06;

    // --- ARP Header ---
    packet[14] = 0x00; packet[15] = 0x01;          // HardwareType Ethernet
    packet[16] = 0x08; packet[17] = 0x00;          // ProtocolType IPv4
    packet[18] = 6;  // HardwareSize
    packet[19] = 4;  // ProtocolSize
    packet[20] = 0x00; packet[21] = 0x01;          // Opcode = request

    // Sender MAC
    for(int i=0;i<6;i++) packet[22+i] = senderMAC[i];
    // Sender IP
    for(int i=0;i<4;i++) packet[28+i] = senderIP[i];
    // Target MAC (0 für Request)
    for(int i=0;i<6;i++) packet[32+i] = 0x00;
    // Target IP
    for(int i=0;i<4;i++) packet[38+i] = targetIP[i];

    return packet;
}*/

std::string getDefaultInterface(){
    struct ifaddrs *ifaddr, *ifa;
    std::string defaultInterface;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return defaultInterface;
    }

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;
        if (ifa->ifa_addr->sa_family == AF_INET) { // IPv4-Address
            defaultInterface = ifa->ifa_name;
            break; // First avilable Interface with IP selected
        }
    }

    freeifaddrs(ifaddr);
    return defaultInterface;
};

std::array<uint8_t, 6> getMacAddress(const char* ifname){
    struct ifreq ifr;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    ioctl(sock, SIOCGIFHWADDR, &ifr);
    close(sock);

    return std::array<uint8_t, 6>{
        static_cast<uint8_t>(ifr.ifr_hwaddr.sa_data[0]),
        static_cast<uint8_t>(ifr.ifr_hwaddr.sa_data[1]),
        static_cast<uint8_t>(ifr.ifr_hwaddr.sa_data[2]),
        static_cast<uint8_t>(ifr.ifr_hwaddr.sa_data[3]),
        static_cast<uint8_t>(ifr.ifr_hwaddr.sa_data[4]),
        static_cast<uint8_t>(ifr.ifr_hwaddr.sa_data[5]),
    };
}


std::array<uint8_t, 42> buildARPRequest(
    const uint8_t senderMAC[6], 
    const uint8_t senderIP[4], 
    const uint8_t targetIP[4])
{
    std::array<uint8_t, 42> packet{};

    // Ethernet-Frame Overhead
    for (int i = 0; i < 6; i++){
        packet[i] = 0xFF;
    }

    std::string interfaceName = getDefaultInterface();
    

    std::array<uint8_t, 6> ownMac = getMacAddress();

}