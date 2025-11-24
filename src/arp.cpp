#include "arp.h"
#include <iostream>
#include <sys/ioctl.h>
#include <sys/socket.h>         // for socket
#include <arpa/inet.h>          // for htons
#include <net/if.h>
#include <string.h>             // for memset


std::array<uint8_t, 6> getMacAddress(std::string if_name){

    int if_check = socket(AF_INET, SOCK_DGRAM, 0);              // Address Family IPv4 (INET)

    if (if_check == -1){
        std::cout << "Socket Error" << std::endl;
        return {};
    }

    struct ifreq ifr{};                 // data structure ifreq is applied to ifr
    strcpy(ifr.ifr_name, if_name.c_str());


    std::cout << "MAC: " << if_check << "\nIFR_Name: " << ifr.ifr_name;


    std::array<uint8_t, 6> mac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    return mac;
}


int main(){
    getMacAddress("wlan0");
}

