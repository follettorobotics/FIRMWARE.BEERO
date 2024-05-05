#line 1 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\BitStuffing.h"
#ifndef BITSTUFFING_H
#define BITSTUFFING_H

#include <Arduino.h>

// have to sinlge-ton value 
class BitStuffing {
private:

    BitStuffing();
    BitStuffing(const BitStuffing&) = delete;
    BitStuffing& operator=(const BitStuffing&) = delete;

public:
    static BitStuffing& getInstance(); 
    
    size_t applyBitStuffing(byte* message, size_t size, byte* stuffedMessage);
    size_t removeBitStuffing(byte* stuffedMessage, size_t stuffedSize, byte* message); 
}; 

#endif 