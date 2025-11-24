#include "arp.h"
#include <iostream>
#include <sys/ioctl.h>
#include <sys/socket.h>         // for socket
#include <arpa/inet.h>          // for htons
#include <net/if.h>
#include <string.h>             // for memset
#include <ifaddrs.h>
#include <netinet/in.h>


std::array<uint8_t, 6> getMacAddress(std::string if_name){

    int file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);              // Address Family IPv4 (INET)

    if (file_descriptor == -1){
        std::cout << "Socket Error" << std::endl;
        return {};
    }

    struct ifreq ifr{};                 // data structure ifreq is applied to ifr
    strcpy(ifr.ifr_name, if_name.c_str());
    
    int success = ioctl(file_descriptor, SIOCGIFHWADDR, &ifr);
    if (success == -1){
        std::cout << "Error Code: " << errno << "\n\n";
    }

    // Debug print
    std::cout << "File Descriptor: " << file_descriptor << "\nIFR_Name: " << ifr.ifr_name << "\n" << "IOCTL: " << success << "\n";


    std::array<uint8_t, 6> mac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    return mac;
}

int getAllInterfaces(){
    struct ifaddrs *address;
    if (getifaddrs(&address) == 0){                     // fill out the ifaddrs structure with data        
        while (address != nullptr){
            if (address->ifa_addr != nullptr && address->ifa_addr->sa_family == AF_INET){       // check address type for IPv4
                struct sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(address->ifa_addr);
                std::cout << address->ifa_name << "\n";
                std::cout << inet_ntoa(ipv4->sin_addr) << "\n";
            }
            if (address->ifa_addr != nullptr && address->ifa_addr->sa_family == AF_INET6){
                struct sockaddr_in6 *ipv6 = reinterpret_cast<sockaddr_in6 *>(address->ifa_addr);
                std::cout << address->ifa_name << "\n";
                char ipv6_addr[INET6_ADDRSTRLEN];                       // make a little mem for the IPv6 to be stored in
                std::cout << inet_ntop(AF_INET6, &ipv6->sin6_addr, ipv6_addr, INET6_ADDRSTRLEN) << "\n";                        // requires a few more params
            }
            if (adf)
            address = address->ifa_next;
        }
    }

    

    std::cout << "Interface Retrieval Successful?\n+positive" << "\n";

    return 0;
}


int main(){
    getAllInterfaces();
    getMacAddress("eht0");
}

