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

    if (errorCheckSensorLimit != 16){
        errorCheckSensor = getSensorLimitValue(errorCheckSensorLimit);
    }
    return true; 
}

bool MotorHandler::getSensorLimitValue(uint8_t sensorLimitParameter){
    SensorHandler& sensorHandler = SensorHandler::getInstance();
    sensorHandler.execute(); 
    uint16_t sensorValue = sensorHandler.getSensorValue();

    bool isBit = (sensorValue >> sensorLimitParameter) & 1; 

    if (sensorLimitParameter > 2){
        return isBit==1; 
    }else{
        return isBit==0; 
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
    exMotorControlRsp[rspIndex++] = errorCheckSensor; 
    exMotorControlRsp[rspIndex++] = endByte;

    return rspIndex; 
}
