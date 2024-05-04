#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <Ethernet.h>
#include "dispatch.h"
#include "BitStuffing.h"


// TCPHandler singleton pattern 
class TCPHandler{
private:

    TCPHandler() : TCPserver(TCPHandler::port) {}
    TCPHandler(const TCPHandler&) = delete;
    TCPHandler& operator=(const TCPHandler&) = delete;

    EthernetServer TCPserver;
    EthernetClient TCPclient; 
    static uint16_t port;

public:

    static TCPHandler& getInstance();
    void begin(byte mac[], IPAddress ip);
    void clientHandle(); 
    void messageHandle();
    bool sendMessageToClient(const byte *message, size_t size);
};

#endif