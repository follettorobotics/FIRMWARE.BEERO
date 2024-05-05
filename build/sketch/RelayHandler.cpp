#line 1 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\RelayHandler.cpp"
#include "RelayHandler.h"

bool RelayHandler::execute(){

    if (controlTime != 0){
        digitalWrite(relayPin, true);
        delay(controlTime); 
        digitalWrite(relayPin, false);
        return true; 
    }else{
        return true; 
    }

}

size_t RelayHandler::response(byte* relayControlRsp){
    size_t index = 0;

    relayControlRsp[index++] = startByte;
    relayControlRsp[index++] = relayHandlerRspCommand; 

    relayControlRsp[index++] = getRelayNumber();

    // get relay control (real time) 
    if (controlTime != 0){
        unsigned long realTimeVal = getRealTime();
        // split the time 
        byte* integerByte;
        byte* fractionByte;

        decimalToHex(controlTime, integerByte, fractionByte);
        relayControlRsp[index++] = *integerByte;
        relayControlRsp[index++] = *fractionByte;
    }

    relayControlRsp[index++] = endByte;

    return index;
    
}

void RelayHandler::decimalToHex(unsigned long time, byte* integerByte, byte* fractionByte){
    *integerByte = time / 1000;
    unsigned long rawFraction = time % 1000;

    if (rawFraction % 100 == 0){
        *fractionByte = rawFraction / 100;
    }else if (rawFraction % 10 == 0){
        *fractionByte = rawFraction / 10;
    }else{
        *fractionByte = rawFraction; 
    }

}