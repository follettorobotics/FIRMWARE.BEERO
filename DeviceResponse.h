#ifndef DEVICERESPONSE_H
#define DEVICERESPONSE_H

#define RESPONSE 200

#include <Arduino.h>
#include "BitStuffing.h"

class DeviceResponse {
private: 
    // single-ton pattern 
    DeviceResponse();
    DeviceResponse(const DeviceResponse&) = delete;
    DeviceResponse& operator=(const DeviceResponse&) = delete;

    byte response[RESPONSE];
    size_t responseSize = 0; 

public:

    static DeviceResponse& getInstance(); 
    void appendResponse(byte *dataToAdd, size_t dataSize);
    size_t getResponse(byte* dataToSend); 
    size_t getResponseSize() const {return responseSize;}
};

#endif 