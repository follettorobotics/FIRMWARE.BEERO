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
        Serial.println("start byte error"); 
        // start byte error
        response[responseIndex++] = startByte;
        response[responseIndex++] = startByteErrorCommand;
        response[responseIndex++] = endByte;

        return responseIndex;
    }

    if (request[requestSize - 1] != endByte){
        Serial.println("end byte error"); 
        // end byte error
        response[responseIndex++] = startByte;
        response[responseIndex++] = endByteErrorCommand;
        response[responseIndex++] = endByte;

        return responseIndex; 
    }

    index++; 

    if (request[index] == sensorReandReqCommand){
        Serial.println("sensor request dispatch"); 
        // sensor read
        // call class SensorHandler
        SensorHandler& sensorHandler = SensorHandler::getInstance();
        sensorHandler.execute();
        uint16_t sensorValue = sensorHandler.getSensorValue();
        byte sensorLw = sensorValue & 0xFF;
        byte sensorUp = (sensorValue >> 8) & 0xFF;

        response[responseIndex++] = startByte;
        response[responseIndex++] = sensorReadRspCommand;
        response[responseIndex++] = sensorLw;
        response[responseIndex++] = sensorUp;
        response[responseIndex++] = endByte;
        
        return responseIndex; 
    }else if (request[index] == relayHandlerReqCommand){
        Serial.println("relay control with time dispatch"); 
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

    }else if (request[index] == motorRunReqCommand){
        Serial.println("motor control dispatch");
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
        Serial.print("motor_number");
        Serial.println(motorNumber); 
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

        byte errorCheckSensorLimit = request[index++]; 

        // motor class
        // Serial.println("motor run execute"); 
        MotorHandler* motorHandler = new MotorHandler(motorNumber, motorDir, motorStep, motorAdd, relayBrake, sensorLimit, errorCheckSensorLimit); 
        motorHandler->execute(); 
        responseIndex = motorHandler->response(response); 

        delete(motorHandler); 
        
        return responseIndex; 

    }else if(request[index] == relayOnOffReqCommand){
        Serial.println("relay on/off handler");
        index++;

        byte relayNumber = request[index++];

        if (relayNumber < 1 || relayNumber > RELAY){

            response[responseIndex++] = startByte;
            response[responseIndex++] = relayOnOffErrorCommand;
            response[responseIndex++] = relayNumberErrorByte;
            response[responseIndex++] = endByte;

            return responseIndex; 
        }

        byte relayControl = request[index++];
        if (relayControl != relayOn && relayControl != relayOff){
            response[responseIndex++] = startByte;
            response[responseIndex++] = relayOnOffErrorCommand;
            response[responseIndex++] = relayControlErrorByte;
            response[responseIndex++] = endByte;

            return responseIndex; 
        }

        bool On = false;
        if (relayControl == relayOn){
            On = true; 
        }

        // relayHandler instance 
        RelayHandler* relayHandler = new RelayHandler(relayNumber, 0x00, On);
        delete(relayHandler);

        //response
        response[responseIndex++] = startByte;
        response[responseIndex++] = relayOnOffRspCommand;
        response[responseIndex++] = relayNumber;
        response[responseIndex++] = relayControl;
        response[responseIndex++] = endByte;

        return responseIndex; 
        
    }else if(request[index] == loadcellInitialReqCommand){
        Serial.println("loadcell initial handler");
        index++;
        
        // initialize all the three loadcell
        for (int i=0; i<3; i++){
            LoadcellSetup::initializePins(i);
        }
        
        // make the response
        response[responseIndex++] = startByte;
        response[responseIndex++] = loadcellInitialRspCommand;
        response[responseIndex++] = endByte;

        return responseIndex;
        
    }else if(request[index] == loadcellReadReqCommand){
        Serial.println("laodcell read handler");
        index++; 

        // read all the three loadcell
        LoadCellHandler* loadcellHandler = new LoadCellHandler(); 
        loadcellHandler->execute(); 

        // make the response
        responseIndex = loadcellHandler->response(response); 

        // delete the class
        delete(loadcellHandler); 

        return responseIndex; 
        
    }else{
        // non request, error occur 
        response[responseIndex++] = startByte;
        response[responseIndex++] = nonExistErrorCommand;
        response[responseIndex++] = endByte;
        return responseIndex;
    }
    return 0; 
}

unsigned long Dispatcher::hexToDecimal(const byte data1, const byte data2){
    Serial.print("data 1 (integer part): ");
    Serial.println(data1);

    unsigned long integerPart = data1 * 1000;
    Serial.print("data 1 (integer part) * 1000: ");
    Serial.println(integerPart);
    unsigned long decimalPart = (unsigned long) data2;
    unsigned long millisecTime = 0;
    
    if (decimalPart < 10){
        millisecTime = (integerPart + (decimalPart * 100));
    }else if (decimalPart < 100){
        millisecTime = (integerPart + (decimalPart * 10));
    }else{
        millisecTime = (integerPart + decimalPart);
    }

    return millisecTime; 
}