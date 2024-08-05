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
        // Serial.println("tcp client disconnected and new client accept");
    }
    
    // parse the message logic 
    byte stuffedRsp[30]; 
    size_t stuffedRspSize = messageHandle(stuffedRsp); 

    // long-term message
    DeviceResponse& deviceRsp = DeviceResponse::getInstance(); 
    byte dataToSend[100]; 
    size_t dataToSendSize = deviceRsp.getResponse(dataToSend); 

    // Combine immediate and long-term responses
    byte combinedData[130]; // Buffer for combined responses
    size_t combinedDataSize = 0;

    // Copy immediate response
    if (stuffedRspSize > 0) {
        memcpy(combinedData, stuffedRsp, stuffedRspSize);
        combinedDataSize += stuffedRspSize;
    }

    // Copy long-term response
    if (dataToSendSize > 0) {
        memcpy(combinedData + combinedDataSize, dataToSend, dataToSendSize);
        combinedDataSize += dataToSendSize;
    }

    // Send combined data if there is any
    if (combinedDataSize > 0) {
        bool result = sendMessageToClient(combinedData, combinedDataSize);
        if (!result && dataToSendSize > 0) {
            // If failed to send and long-term data exists, append it back to device response
            deviceRsp.appendResponse(dataToSend, dataToSendSize); 
        }
    }

    return;
}

size_t TCPHandler::messageHandle(byte *stuffedRsp){
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

        byte response[50] = {0, };
        size_t responseSize = 0;

        responseSize = dispatch.dispatch(unStuffedReq, unStuffedReqSize, response);

        if (responseSize != 0){
            size_t stuffedRspSize; 
            stuffedRspSize = bitStuffing.applyBitStuffing(response, responseSize, stuffedRsp);          
            // sendMessageToClient(stuffedRsp, sstuffedRspSize); 
            return stuffedRspSize; 
        }
    }
    return 0; 
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
            // Serial.println("tcp not available to send response message");
            return false; 
        }
    }
}