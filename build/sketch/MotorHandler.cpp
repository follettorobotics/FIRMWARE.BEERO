#line 1 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\MotorHandler.cpp"
#include "MotorHandler.h"

// non-concurrency 
bool MotorHandler::execute(){
  Serial.println("motor execute start"); 
    for (int i=0; i<motorStep; i++){
        if (i == 0){
            // motor step 
            Serial.println("motor step 0");
            if (relayBrake != 0x00){
                RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, true); 
                delete relayHandler; 
            }
        }else if (currentStep == motorStep){
            // when motorStep 
            Serial.println("motor run end");
            if (relayBrake != 0x00){
                RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, false); 
                delete relayHandler; 
                break; 
            }
        
        }

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
                    Serial.println("motor run end (add step)");
                    if (relayBrake != 0x00){
                        RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, false); 
                        delete relayHandler; 
                    }
                    break; 
                }
            }
        }

        digitalWrite(pwmPin, true);
        currentStep++; 
        digitalWrite(pwmPin, false);
        delayMicroseconds(0.5); 
    }

    Serial.println("motor execute end"); 
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
    exMotorControlRsp[rspIndex++] = motorNumber;
    exMotorControlRsp[rspIndex++] = currentStep & 0xFF;
    exMotorControlRsp[rspIndex++] = (currentStep >> 8) & 0xFF;
    exMotorControlRsp[rspIndex++] = sensor; 
    exMotorControlRsp[rspIndex++] = endByte;

    return rspIndex; 
}
