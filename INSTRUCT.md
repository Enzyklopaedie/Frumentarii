# How to realise the project

## ARP
### `arp.cpp`

1. get all network interfaces and pick one with an IP (not loopback or virtual)
2. use the interface name to get the MAC-Address of the NIC
3. get the IP of network interface
4. build ARP request
5. send ARP request via rawSocket
6. recieve ARP response via rawSocket
