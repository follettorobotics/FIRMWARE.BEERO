#include "DeviceResponse.h"

DeviceResponse::DeviceResponse() {}

DeviceResponse& DeviceResponse::getInstance(){
    static DeviceResponse instance;
    return instance;
}

void DeviceResponse::appendResponse(byte *dataToAdd, size_t dataSize) {
    
    byte stuffedRsp[100]; 
    size_t stuffedRspIndex = 0; 

    size_t spaceLeft = RESPONSE - responseSize;
    if (dataSize > spaceLeft){
        Serial.println("long term response overflow ");
    }

    BitStuffing& bitStuffing = BitStuffing::getInstance();

    stuffedRspIndex = bitStuffing.applyBitStuffing(dataToAdd, dataSize, stuffedRsp);

    for (size_t i=0; i < stuffedRspIndex; i++){
        response[responseSize++] = stuffedRsp[i];
    }

    return ;
}

size_t DeviceResponse::getResponse(byte* dataToSend){
    size_t currentSize = getResponseSize();

    if (currentSize == 0){
        return 0; 
    }

    for (int i=0; i< currentSize; i++){
        dataToSend[i] = response[i];
        responseSize--;
    }
    
    return currentSize; 
}
