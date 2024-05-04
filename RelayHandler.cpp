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