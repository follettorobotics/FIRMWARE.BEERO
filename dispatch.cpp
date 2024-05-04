#include "dispatch.h"

Dispatcher::Dispatcher() {
    
}

Dispatcher& Dispatcher::getInstance(){
    static Dispatcher instance;
    return instance;
}

size_t Dispatcher::dispatch(byte* request, size_t requestSize, byte* response){
    size_t responseIndex = 0;
    size_t index = 0; 

     if (request[index] != startByte){
        // start byte error
        response[responseIndex++] = startByte;
        response[responseIndex++] = startByteErrorCommand;
        response[responseIndex++] = endByte;

        return responseIndex;
    }

    if (request[requestSize - 1] != endByte){
        // end byte error
        response[responseIndex++] = startByte;
        response[responseIndex++] = endByteErrorCommand;
        response[responseIndex++] = endByte;

        return responseIndex; 
    }

    index++; 

    if (request[index] == sensorReandReqCommand){
        // sensor read
        // call class SensorHandler
        SensorHandler& sensorHandler = SensorHandler::getInstance();
        uint16_t sensorValue = sensorHandler.getSensorValue();
        byte sensorLw = sensorValue & 0xFF;
        byte sensorUp = (sensorValue >> 8) & 0xFF;

        response[responseIndex++] = startByte;
        response[responseIndex++] = sensorReadRspCommand;
        response[responseIndex++] = sensorLw;
        response[responseIndex++] = sensorUp;
        response[responseIndex++] = endByte;
        
        return responseIndex; 
    }else if (request[index] == relayHandlerRspCommand){
        // relay 
        index++; 

        // relay number
        int relayNumber = request[index++];

        if (relayNumber < 1 || relayNumber > RELAY){

            response[responseIndex++] = startByte;
            response[responseIndex++] = relayHandlerErrorCommand;
            response[responseIndex++] = relayNumberErrorByte;
            response[responseIndex++] = endByte;

            return responseIndex; 
        }


        // Relay Handler
        byte integerTime = request[index++];
        byte fractionalTime = request[index++]; 
        
        // controlTime: millisec 
        unsigned long controlTime = hexToDecimal(integerTime, fractionalTime); 

        bool on = false; 

        // relayHandler instance 
        RelayHandler* relayHandler = new RelayHandler(relayNumber, controlTime, on);
        relayHandler->execute();

        responseIndex = relayHandler->response(response); 

        delete(relayHandler); 
        return responseIndex;

    }else if (request[index] == motorRunRspCommand){
        // external motor 
        index++;

        byte motorNumber = request[index++];
        if (motorNumber <1 || motorNumber > EX_MOTOR){
            // Serial.println("motor number error");
            response[responseIndex++] = startByte;
            response[responseIndex++] = motorRunErrorCommand;
            response[responseIndex++] = motorNumberErrorByte;
            response[responseIndex++] = endByte;
            return responseIndex;
        }

        motorNumber--; 

        byte motorDir = request[index++];
        if (motorDir!= doorOpen && motorDir!= doorClose){
            // Serial.println("motor dir error");
            response[responseIndex++] = startByte;
            response[responseIndex++] = motorRunErrorCommand;
            response[responseIndex++] = motorDirErrorByte;
            response[responseIndex++] = endByte;
            return responseIndex;
        }

        byte motorStepLw = request[index++];
        byte motorStepUp = request[index++];

        uint16_t motorStep = (motorStepUp << 8) | motorStepLw;

        byte motorAddLw = request[index++];
        byte motorAddUp = request[index++];

        uint16_t motorAdd = (motorAddUp << 8) | motorAddLw; 
    
        byte relayBrake = request[index++]; 

        if (relayBrake < 1 || relayBrake > RELAY){

            response[responseIndex++] = startByte;
            response[responseIndex++] = motorRunErrorCommand;
            response[responseIndex++] = relayNumberErrorByte;
            response[responseIndex++] = endByte;

            return responseIndex; 
        }

        byte sensorLimit = request[index++]; 

        if (sensorLimit < 0 || sensorLimit > sensorValueIndex){
            // Serial.println("sensor limit out of range in motor run");
            response[responseIndex++] = startByte;
            response[responseIndex++] = motorRunErrorCommand;
            response[responseIndex++] = SensorLimitErrorByte;
            response[responseIndex++] = endByte;
            return responseIndex;
        }

        // motor class
        MotorHandler* motorHandler = new MotorHandler(motorNumber, motorDir, motorStep, motorAdd, relayBrake, sensorLimit); 
        motorHandler->execute(); 
        responseIndex = motorHandler->response(response); 

        delete(motorHandler); 
        
        return responseIndex; 

    }else{
        // non request 
        response[responseIndex++] = startByte;
        response[responseIndex++] = nonExistErrorCommand;
        response[responseIndex++] = endByte;
        return responseIndex;
    }
    return 0; 
}

unsigned long Dispatcher::hexToDecimal(const byte data1, const byte data2){
    unsigned long integerPart = data1 * 1000;
    unsigned long decimalPart = (unsigned long) data2;
    unsigned long millisecTime;
    
    if (decimalPart < 10){
        millisecTime = (integerPart + (decimalPart * 100));
    }else if (decimalPart < 100){
        millisecTime = (integerPart + (decimalPart * 10));
    }else{
        millisecTime = (integerPart + decimalPart);
    }

    return millisecTime; 
}