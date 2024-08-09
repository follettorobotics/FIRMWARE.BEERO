#include "MotorHandler.h"

// concurrency 
bool MotorHandler::execute(){
    if (currentStep == 0){
        // if the relay brake exists, relay ON
        if (relayBrake != 0x00){
            RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, true, packet_number); 
            delete relayHandler;
        }
        startTime = micros();
        digitalWrite(pwmPin, true);
        currentStep++; 
        digitalWrite(pwmPin, false);
        return false; 
    }

    currentTime = micros(); 
    unsigned int elapsedTime = currentTime - startTime; 

    if (elapsedTime >= MOTOR_DELAY){
        // delay check 
        if (sensorCheckRequire && !sensor){
            // // senor check needed but sensor OFF
            // Serial.println("sensor read");
            if (getSensorLimitValue(sensorLimit)){
                Serial.print("sensor on"); 
                sensor = true; 
            }
        }else if (sensorCheckRequire && sensor){
            // senor check needed and sensor ON
            // Serial.println("sensor on");
            if (motorAddStep == 0){
                Serial.println("add done");
                if (relayBrake != 0x00){
                    RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, false, packet_number); 
                    delete relayHandler;
                }
                return true; 
            }else{
                // Serial.println("add");
                motorAddStep--; 
            }
        }else{
            // senor check not needed
            // Serial.println("sensor not need");
            if (currentStep == motorStep){
                if (relayBrake != 0x00){
                    RelayHandler* relayHandler = new RelayHandler(relayBrake, 0, false, packet_number); 
                    delete relayHandler;
                }
                return true; 
            }
        }

        digitalWrite(pwmPin, true);
        currentStep++; 
        startTime = micros(); 
        digitalWrite(pwmPin, false);
    }
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
    exMotorControlRsp[rspIndex++] = packet_number;
    exMotorControlRsp[rspIndex++] = endByte;

    return rspIndex; 
}
