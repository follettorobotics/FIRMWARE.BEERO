#include "SensorHandler.h"

SensorHandler::SensorHandler() {
    
}

SensorHandler& SensorHandler::getInstance(){
    static SensorHandler instance;
    return instance;
}


void SensorHandler::initialSensorHandler(){

    //initialize sensor data 
    pinMode(LOAD, OUTPUT);
    pinMode(DIN, INPUT);
    pinMode(CLK, OUTPUT);
    pinMode(CE, OUTPUT);
    digitalWrite(CLK, false);
    digitalWrite(LOAD, true);
    digitalWrite(CE, false); 
}


bool SensorHandler::execute(){ 
    digitalWrite(LOAD, LOW);
    digitalWrite(LOAD, HIGH);
    
    uint16_t temp = 0;
    sensor_value = 0; 

    for (int i=0; i<16; i++){
        int value = digitalRead(DIN);
        temp |= (value << ((DATA_WIDTH-1) - i));
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    sensor_value = temp;

    return false;
}

void SensorHandler::makeSensorValue(byte *sensorValue1, byte *sensorValue2){
    *sensorValue1 = getSensorValue1();
    *sensorValue2 = getSensorValue2();
    return;
}

