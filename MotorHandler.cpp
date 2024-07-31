#include "MotorHandler.h"

// non-concurrency 
bool MotorHandler::execute(){
    // int j = 0; 
    // Serial.print("first current step: ");
    // Serial.println(currentStep); 
    if (relayBrake != 0x00){
        RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, true); 
        delete relayHandler; 
    }
    for (int i=0; i<motorStep; i++){
        if ((sensorLimit < 16)){
            if (!sensor){
                if (getSensorLimitValue(sensorLimit)){
                    sensor = true; 
                    // Serial.print("motor step when sensor value on");
                    // Serial.println(currentStep); 
                }
            }else{
                if (motorAddStep != 0){
                    // additional step exists
                    motorAddStep--;  
                }else{
                    // Serial.print("current step: ");
                    // Serial.println(currentStep); 
                    break; 
                }
            }
        }

        digitalWrite(pwmPin, true);
        currentStep++; 
        digitalWrite(pwmPin, false);
        // j++;
        // Serial.print("temp step ");
        // Serial.println(j);
        delayMicroseconds(600); 
    }

    if (relayBrake != 0x00){
        RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, false); 
        delete relayHandler; 
    }

    if (errorCheckSensorLimit != 16){
        errorCheckSensor = getSensorLimitValue(errorCheckSensorLimit);
    }
    // Serial.print("final current step: ");
    // Serial.println(currentStep); 
    return true; 
}

bool MotorHandler::getSensorLimitValue(uint8_t sensorLimitParameter){
    SensorHandler& sensorHandler = SensorHandler::getInstance();
    sensorHandler.execute(); 
    uint16_t sensorValue = sensorHandler.getSensorValue();

    bool isBit = (sensorValue >> sensorLimitParameter) & 1; 

    // if (sensorLimitParameter > 2){
    //     return isBit==1; 
    // }else{
    //     return isBit==0; 
    // }
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
