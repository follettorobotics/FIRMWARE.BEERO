#include "RelayHandler.h"

bool RelayHandler::execute(){

    unsigned long currentTime = millis();
    unsigned long realTimeVal = currentTime - startTime; 
    if (realTimeVal >= controlTime){
        // stop timing 
        digitalWrite(relayPin, false);
        // real time value set 
        setRealTime(realTimeVal); 

        return true; 
    }

    return false; 

}

size_t RelayHandler::response(byte* relayControlRsp){
    size_t index = 0;

    relayControlRsp[index++] = startByte;
    relayControlRsp[index++] = relayHandlerRspCommand; 

    relayControlRsp[index++] = getRelayNumber();

    // get relay control (real time) 
    unsigned long realTimeVal = getRealTime();

    // split the time 
    byte* integerByte;
    byte* fractionByte;

    decimalToHex(realTimeVal, integerByte, fractionByte);

    relayControlRsp[index++] = *integerByte;
    relayControlRsp[index++] = *fractionByte;

    relayControlRsp[index++] = endByte;

    return index;
    
}