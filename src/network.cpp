#include "network.h"
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int openRawSocket(const std::string& interfaceName) {
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) {
        std::cerr << "Socket konnte nicht erstellt werden!\n";
        return -1;
    }

    int ifIndex = if_nametoindex(interfaceName.c_str());
    if (ifIndex == 0) {
        std::cerr << "Interface nicht gefunden!\n";
        close(sock);
        return -1;
    }

    std::cout << "Raw Socket auf Interface " << interfaceName << " geöffnet (ID " << ifIndex << ")\n";
    return sock;
}
