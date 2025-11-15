#include "arp.h"
#include <string>
#include <ifaddrs.h>
#include <cstring>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/ether.h>
#include <stdexcept>

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

std::array<uint8_t, 4> getIpAddress(const char* ifname) {
    struct ifaddrs *ifaddr, *ifa;
    std::string ipAddress;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        throw std::runtime_error("Failed to get network interfaces");
    }

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;

        // Check if this is the interface we want and if it's IPv4
        if (strcmp(ifa->ifa_name, ifname) == 0 && ifa->ifa_addr->sa_family == AF_INET) {
            void* addr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, addr, ip, INET_ADDRSTRLEN);
            ipAddress = ip;
            break;
        }
    }

    freeifaddrs(ifaddr);

    if (ipAddress.empty()) {
        throw std::runtime_error("No IPv4 address found for interface");
    }

    std::array<uint8_t, 4>resultIpAddress = {};

    // Convert IP-String ("192.168.1.1") to uint8_t[4]-Array
    struct in_addr ipAddr;
    if (inet_pton(AF_INET, ipAddress.c_str(), &ipAddr) != 1) {
        throw std::runtime_error("Failed to convert IP address");
    }

    // Copy 4 Bytes of IP-Adresse to Result-Array
    const uint8_t* ipBytes = reinterpret_cast<const uint8_t*>(&ipAddr.s_addr);
    for (int i = 0; i < 4; i++) {
        resultIpAddress[i] = ipBytes[i];
    }

    return resultIpAddress;
}

std::array<uint8_t, 60> buildARPRequest(
    const uint8_t senderMAC[6], 
    const uint8_t senderIP[4], 
    const uint8_t targetIP[4])
{
    std::array<uint8_t, 60> packet{};           // Ethernet-Frame

    

    // - - - - -

    // Ethernet-Frame Start
    // Target MAC (Broadcast -> FF:FF:FF:FF:FF:FF)
    for (int i = 0; i < 6; i++){
        packet[i] = 0xFF;
    }

    // Source MAC
    for (int i = 0; i < 6; i++){
        packet[6+i] = senderMAC[i];
    }

    // Ethertype 0x0806 for ARP according to IEEE
    packet[12] = 0x08;
    packet[13] = 0x06;


    // ----- DATA -----
    // Hardware Type
    packet[14] = 0x00;
    packet[15] = 0x01;

    // Protocol Type
    packet[16] = 0x08;
    packet[17] = 0x00;

    // Address Size
    packet[18] = 6;

    // Protocol Size
    packet[19] = 4;

    // Operation
    packet[20] = 0x00;
    packet[21] = 0x01;

    // Source MAC
    for (int i = 0; i < 6; i++){
        packet[22 + i] = senderMAC[i];
    }

    // Source IP
    for (int i = 0; i < 4; i++){
        packet[28 + i] = senderIP[i];
    }

    // Target MAC in ARP Request 00:00:00:00:00:00
    for (int i = 0; i < 6; i++){
        packet[32 + i] = 0x00;
    }

    // Target IP
    for (int i = 0; i < 4; i++){
        packet[38 + i] = targetIP[i];
    }

    // Meet minimum requirements for Ethernet Payload
    for (int i = 0; i < 18; i++){
        packet[42 + i] = 0x00;
    }

    return packet;
}

std::string actuallySendingTheARPRequest(){
    std::string ifname = getDefaultInterface();
    if (ifname.empty()){
        std::__throw_runtime_error("No network interface found!");
    }
    std::array<uint8_t, 6> ownMac = getMacAddress(ifname.c_str());
    std::array<uint8_t, 4> ownIp = getIpAddress(ifname.c_str());

    // Assuming target subnet is the same as own IP subnet and 255.255.255.0
    std::array<uint8_t, 4> targetIp = ownIp;

    for (int i = 0; i < 256; i++){
        targetIp[3] = i; // Targeting .0 to .255 addresses
        buildARPRequest(ownMac.data(), ownIp.data(), targetIp.data());
    }

    return "Super";
}