#include "arp.h"
#include <iostream>
#include <sys/ioctl.h>
#include <sys/socket.h>         // for socket
#include <arpa/inet.h>          // for htons
#include <net/if.h>
#include <string.h>             // for memset
#include <vector>
#include <array>
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

std::vector<InterfaceInfo> getAllInterfaces(){
    struct ifaddrs *address;
    if (getifaddrs(&address) == 0){                     // fill out the ifaddrs structure with data
        // int cnt = 0;
        std::vector<InterfaceInfo> interfaces;                       // store more than one struct to return
        while (address != nullptr){
            if (address->ifa_addr != nullptr && address->ifa_addr->sa_family == AF_INET){       // check address type for IPv4
                struct sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(address->ifa_addr);
                
                bool existing = false;

                for (int i = 0; i < interfaces.size(); i++){
                    if (interfaces[i].name == address->ifa_name){
                        existing = true;
                        interfaces[i].ipv4_addresses.push_back(inet_ntoa(ipv4->sin_addr));
                        // continue;
                        break;
                    }
                }

                if (existing == false){
                    // create new entry for the interface
                    struct InterfaceInfo interface1;
                    interface1.name = address->ifa_name;
                    interface1.ipv4_addresses.push_back(inet_ntoa((ipv4->sin_addr)));           // adding IPv4 to IPv4 vector in interfaces vector
                    interfaces.push_back(interface1);
                }
            }
            if (address->ifa_addr != nullptr && address->ifa_addr->sa_family == AF_INET6){
                struct sockaddr_in6 *ipv6 = reinterpret_cast<sockaddr_in6 *>(address->ifa_addr);

                bool existing = false;

                for (int i = 0; i < interfaces.size(); i++){
                    if (interfaces[i].name == address->ifa_name){
                        existing = true;
                        char ipv6_addr[INET6_ADDRSTRLEN];               // make a little mem for the IPv6 to be stored
                        interfaces[i].ipv6_addresses.push_back(inet_ntop(AF_INET6, &ipv6->sin6_addr, ipv6_addr, INET6_ADDRSTRLEN));
                        // continue;
                        break;
                    }
                }

                if (!existing){
                    char ipv6_addr[INET6_ADDRSTRLEN];

                    struct InterfaceInfo interface1;
                    interface1.name = address->ifa_name;
                    interface1.ipv6_addresses.push_back(inet_ntop(AF_INET6, &ipv6->sin6_addr, ipv6_addr, INET6_ADDRSTRLEN));        // requires a few more params
                    interfaces.push_back(interface1);
                }                       
            }
            address = address->ifa_next;
        }
        return interfaces;
    }
}


int main(){
    std::vector<InterfaceInfo> interfaces;
    interfaces = getAllInterfaces();
    std::cout << "Available devices: |\n";
    std::cout << "--------------------\n\n";
    for (int i = 0; i < interfaces.size(); i++){
        std::cout << "Interface " << i << ": \n" << interfaces[i].name << "\n";
        for (int j = 0; j < interfaces[i].ipv4_addresses.size(); j++){
            std::cout << interfaces[i].ipv4_addresses[j] << "\n";
        }
        for (int j = 0; j < interfaces[i].ipv6_addresses.size(); j++){
            std::cout << interfaces[i].ipv6_addresses[j] << "\n";
        }
        std::cout << "--------------------\n";   
    }
    std::cout << "\n\n------------------------ Debug ------------------------\n\n";
    getMacAddress("eth0");
}

