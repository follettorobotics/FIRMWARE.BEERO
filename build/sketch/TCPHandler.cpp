#line 1 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\TCPHandler.cpp"
#include "TCPHandler.h"

uint16_t TCPHandler::port = 502;

TCPHandler& TCPHandler::getInstance(){
    static TCPHandler instance;
    return instance;
}

void TCPHandler::begin(byte mac[], IPAddress ip){
    Serial.println("tcp begin");
    Ethernet.begin(mac, ip);
    TCPserver.begin();
    Serial.println("tcp begin starts!");
}


void TCPHandler::clientHandle(){
    EthernetClient newClient = TCPserver.available();

    if (newClient && !TCPclient.connected()){
        TCPclient = newClient;  // over-write TCPclient
        Serial.println("tcp client disconnected and new client accept");
    }

    // parse the message logic 
    messageHandle(); 

    return;
}

void TCPHandler::messageHandle(){
    byte request[20];
    size_t requestSize = 0;

    byte unStuffedReq[20];
    size_t unStuffedReqSize = 0; 

    // check the data 
    // if the client disconnected, client.available() return 0
    int dataSize = TCPclient.available();
    if (dataSize > 0){
        for (int i=0; i<dataSize; i++){
            byte data = TCPclient.read();
            request[requestSize++] = data; 
        }
    }
    
    if (requestSize != 0){
        Dispatcher& dispatch = Dispatcher::getInstance();

        BitStuffing& bitStuffing = BitStuffing::getInstance(); 
        unStuffedReqSize = bitStuffing.removeBitStuffing(request, requestSize, unStuffedReq);

        byte response[20] = {0, };
        size_t responseSize = 0;

        responseSize = dispatch.dispatch(unStuffedReq, unStuffedReqSize, response);

        byte stuffedRsp[20];
        size_t sstuffedRspSize; 
        sstuffedRspSize = bitStuffing.applyBitStuffing(response, responseSize, stuffedRsp);

        sendMessageToClient(stuffedRsp, sstuffedRspSize); 
    }

}

bool TCPHandler::sendMessageToClient(const byte* message, size_t size){
    if (TCPclient.connected()){
        TCPclient.write(message, size);
        return true; 
    }else{
        EthernetClient newClient = TCPserver.available();
        if (newClient){
            TCPclient = newClient;
            TCPclient.write(message, size); 
            return true; 
        }
        else{
            Serial.println("tcp not available to send response message");
            return false; 
        }
    }
}