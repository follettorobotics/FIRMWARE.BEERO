#line 1 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\MotorHandler.cpp"
#include "MotorHandler.h"

// non-concurrency 
bool MotorHandler::execute(){
    if (relayBrake != 0x00){
        RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, true); 
        delete relayHandler; 
    }
    for (int i=0; i<motorStep; i++){
        if ((sensorLimit < 16)){
            if (!sensor){
                if (getSensorLimitValue(sensorLimit)){
                    sensor = true; 
                }
            }else{
                if (motorAddStep != 0){
                    // additional step exists
                    motorAddStep--; 
                }else{
                    break; 
                }
            }
        }

        digitalWrite(pwmPin, true);
        currentStep++; 
        digitalWrite(pwmPin, false);
        delayMicroseconds(300); 
    }

    if (relayBrake != 0x00){
        Serial.println("relay off timing"); 
        RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, false); 
        delete relayHandler; 
    }
    return true; 
}

bool MotorHandler::getSensorLimitValue(uint8_t sensorLimit){
    SensorHandler& sensorHandler = SensorHandler::getInstance();
    sensorHandler.execute(); 
    uint16_t sensorValue = sensorHandler.getSensorValue();

    bool isBit = (sensorValue >> sensorLimit) & 1; 

    if (sensorLimit == 0 || sensorLimit == 1 || sensorLimit == 2){
        return isBit==0; 
    }else{
        return isBit==1; 
    }
}

size_t MotorHandler::response(byte* exMotorControlRsp){
    size_t rspIndex = 0;

    exMotorControlRsp[rspIndex++] = startByte;
    exMotorControlRsp[rspIndex++] = motorRunRspCommand;
    exMotorControlRsp[rspIndex++] = (motorNumber + 1);
    exMotorControlRsp[rspIndex++] = motorDir; 
    exMotorControlRsp[rspIndex++] = currentStep & 0xFF;
    exMotorControlRsp[rspIndex++] = (currentStep >> 8) & 0xFF;
    exMotorControlRsp[rspIndex++] = sensor; 
    exMotorControlRsp[rspIndex++] = endByte;

    return rspIndex; 
}
