#include <sys/socket.h>             // for socket()
#include <stdio.h>                  // for std
#include <iostream>                 // for cout
#include <cstring>                  // for strerror()
#include <errno.h>                  // for errno
#include <unistd.h>                 // for close()
#include <arpa/inet.h>              // Host-to-Network-Short htons()
#include <net/ethernet.h>           // ETH_P_ALL

int main(){
    int socketfd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));


    if (socketfd == -1){
        if (errno == EPERM){                                        // Operation not premitted
            std::cout << "Try again with sudo" << std::endl;
        }
        else{
            std::cout << "Socket not successful\r\n" << strerror(errno) << errno << std::endl;
        }
        return 1;
    }
    else{
        std::cout << "File Descriptor is: " << socketfd << std::endl;
    }

    close(socketfd);
    std::cout << "Socket closed" << std::endl;
    return 0;
}