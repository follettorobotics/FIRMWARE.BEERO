#include "MotorHandler.h"

// concurrency 
bool MotorHandler::execute(){
    if (currentStep == 0 and relayBrake != 0x00){
        // if the relay brake exists, relay ON 
        RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, true); 
        delete relayHandler;
    }
    if (sensorLimit != 16){
        if (getSensorLimitValue(sensorLimit)){
            sensor = true; 
        }
    }else{
        if (currentStep == motorStep){
            if (relayBrake != 0x00){
                RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, false); 
                delete relayHandler;
            }
        return true; 
        }
    }
    if (sensor){
        if (motorAddStep == 0){
            if (relayBrake != 0x00){
                RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, false); 
                delete relayHandler;
            }
            return true; 
        }else{
            motorAddStep--; 
        }
    }

    digitalWrite(pwmPin, true);
    currentStep++; 
    digitalWrite(pwmPin, false);
    return false; 
}

bool MotorHandler::getSensorLimitValue(uint8_t sensorLimitParameter){
    SensorHandler& sensorHandler = SensorHandler::getInstance();
    // sensorHandler.execute(); 
    uint16_t sensorValue = sensorHandler.getSensorValue();

    bool isBit = (sensorValue >> sensorLimitParameter) & 1; 

    return isBit==1; 
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
    exMotorControlRsp[rspIndex++] = errorCheckSensor; 
    exMotorControlRsp[rspIndex++] = endByte;

    return rspIndex; 
}
