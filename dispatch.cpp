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
        // sensorHandler.execute();
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
        Operator& operatorInstance = Operator::getInstance(); 
        Command* relayHandlerCmd = new RelayHandlerCommand(relayHandler); 
        operatorInstance.addCommand(relayHandlerCmd); 

        // ACK 
        response[responseIndex++] = startByte; 
        response[responseIndex++] = relayHandlerACKCommand; 
        response[responseIndex++] = relayNumber; 
        response[responseIndex++] = endByte; 

        return responseIndex;
    }else if (request[index] == motorRunReqCommand){
        // Serial.println("motor run request parsing"); 
        index++;

        byte motorNumber = request[index++];
        if (motorNumber <1 || motorNumber > EX_MOTOR){
            response[responseIndex++] = startByte;
            response[responseIndex++] = motorRunErrorCommand;
            response[responseIndex++] = motorNumberErrorByte;
            response[responseIndex++] = endByte;
            return responseIndex;
        }

        motorNumber--; 

        byte motorDir = request[index++];
        if (motorDir!= doorOpen && motorDir!= doorClose){
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
            response[responseIndex++] = startByte;
            response[responseIndex++] = motorRunErrorCommand;
            response[responseIndex++] = SensorLimitErrorByte;
            response[responseIndex++] = endByte;
            return responseIndex;
        }

        byte errorCheckSensorLimit = request[index++]; 

        // motor class
        MotorHandler* motorHandler = new MotorHandler(motorNumber, motorDir, motorStep, motorAdd, relayBrake, sensorLimit, errorCheckSensorLimit); 
        Operator& operatorInstance = Operator::getInstance(); 
        Command* motorHandlerCommand = new MotorHandlerCommand(motorHandler); 
        operatorInstance.addCommand(motorHandlerCommand);

        // ACK 
        response[responseIndex++] = startByte; 
        response[responseIndex++] = motorRunACKCommand; 
        response[responseIndex++] = motorNumber;
        response[responseIndex++] = endByte;
        
        return responseIndex; 

    }else if(request[index] == relayOnOffReqCommand){
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
        index++;

        LoadcellSetup* loadcellSetup = new LoadcellSetup(); 
        Operator& operatorInstance = Operator::getInstance(); 
        Command* loadcellSetupCommand = new LoadCellInitialCommand(loadcellSetup); 
        operatorInstance.addCommand(loadcellSetupCommand);

        // ACK 
        response[responseIndex++] = startByte; 
        response[responseIndex++] = loadcellInitialACKCommand; 
        response[responseIndex++] = endByte;

        return responseIndex;
        
    }else if(request[index] == loadcellReadReqCommand){
        index++; 
        Serial.println("Loadcell read"); 
        // read all the three loadcell
        LoadCellHandler* loadcellHandler = new LoadCellHandler(); 
        loadcellHandler->execute();
        responseIndex = loadcellHandler->response(response); 
    
        delete(loadcellHandler); 

        // Serial.println("loadcell value response"); 
        // for (int i=0; i<responseIndex; i++){
        //     Serial.print(response[i]);
        //     Serial.print(" "); 
        // }

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
    unsigned long integerPart = data1 * 1000;
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