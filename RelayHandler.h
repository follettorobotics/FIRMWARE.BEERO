#ifndef RELAYHANDLER_H
#define RELAYHANDLER_H

#include <Arduino.h>

#define    RELAY   7 

#define startByte                   0x7E
#define relayHandlerRspCommand      0xC1
#define endByte                     0xAA

const uint8_t relayPins[RELAY] = {22,23,24,25,26,27,28}; // PA0~6 

// Relay setup class 
class RelaySetup {
public:
    static void initializePins(){
        for (int i=0; i<RELAY; i++){
            pinMode(relayPins[i], OUTPUT);
            digitalWrite(relayPins[i], false); 
        }
    }
}; 

class RelayHandler {
private:

    int relayNumber; 
    // relayPin determined by relayNumber
    uint8_t relayPin;
    unsigned long controlTime;
    unsigned long startTime;
    unsigned long realTime; 
    bool on; 
    uint8_t packet_number; 

public:

    RelayHandler(int relayNumber, unsigned long controlTime, bool On, uint8_t packet_number) 
        : relayNumber(relayNumber), controlTime(controlTime), on(On), packet_number(packet_number){

        relayPin = relayPins[relayNumber-1];
        if (controlTime != 0){
            // time control 
        }else{
            // ON, OFF control 
            digitalWrite(relayPin, on); 
        }
 
    }

    bool execute();
    size_t response(byte* relayControlRsp); 

    void setRealTime(unsigned long time) {
        realTime = time;
        return; 
    }

    unsigned long getRealTime() const {return realTime;}
    int getRelayNumber() const {return relayNumber;}

    void decimalToHex(unsigned long time, byte* integerByte, byte* fractionByte);
};


#endif