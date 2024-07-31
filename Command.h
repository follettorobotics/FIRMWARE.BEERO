#ifndef COMMAND_H
#define COMMAND_H

#include "SensorHandler.h"
#include "RelayHandler.h"
#include "MotorHandler.h"
#include "DeviceResponse.h"
#include "LoadcellHandler.h"

class Command {
public:
    virtual ~Command() {}
    virtual bool execute() = 0;
    virtual void getResponse(byte* data) = 0; 
};

class ReadSensorCommand : public Command {
private:
    SensorHandler& sensorHandler;

public:
    ReadSensorCommand() : sensorHandler(SensorHandler::getInstance()) {}
    
    bool execute() override {
        return sensorHandler.execute(); 
    }

    void getResponse(byte *data) override {
        
    }

};

class RelayHandlerCommand : public Command {
private:
    RelayHandler* relayHandler;
    unsigned long realTime; 

public: 

    RelayHandlerCommand(RelayHandler* handler) : relayHandler(handler) {}
    virtual ~RelayHandlerCommand() {
        delete relayHandler;
    }

    bool execute() override {
        // call the execute function of RelayHandler
        bool isCompleted = relayHandler->execute();

        if (isCompleted){
            // get realTime of control 
            realTime = relayHandler->getRealTime(); 
        }
        return isCompleted; 
    }

    void getResponse(byte* rsp) override {
        size_t rspSize = relayHandler->response(rsp);
        DeviceResponse& deviceRsp = DeviceResponse::getInstance(); 
        deviceRsp.appendResponse(rsp, rspSize);
    }

};

class MotorHandlerCommand : public Command {
private:
    MotorHandler* motorHandler;

public:
    MotorHandlerCommand(MotorHandler* handler) : motorHandler(handler) {}

    virtual ~MotorHandlerCommand() {
        delete motorHandler;
    }

    bool execute() override {
        bool isCompleted = motorHandler->execute();
        return isCompleted; 
    }

    void getResponse(byte* rsp) override {
        size_t rspSize = motorHandler->response(rsp);
        DeviceResponse& deviceRsp = DeviceResponse::getInstance();
        deviceRsp.appendResponse(rsp, rspSize); 
    }
    
};

class LoadCellInitialCommand : public Command {
private:
    LoadcellSetup* loadCellInitial;

public:
    LoadCellInitialCommand(LoadcellSetup* handler) : loadCellInitial(handler) {}

    virtual ~LoadCellInitialCommand() {
        delete loadCellInitial;
    }

    bool execute() override {
        bool isCompleted = loadCellInitial->execute();
        return isCompleted; 
    }

    void getResponse(byte* rsp) override {
        size_t rspSize = loadCellInitial->response(rsp);
        DeviceResponse& deviceRsp = DeviceResponse::getInstance();
        deviceRsp.appendResponse(rsp, rspSize); 
    }
};

class LoadCellReadCommand : public Command {
private:
    LoadCellHandler* loadCellRead;

public:
    LoadCellReadCommand(LoadCellHandler* handler) : loadCellRead(handler) {}

    virtual ~LoadCellReadCommand() {
        delete loadCellRead;
    }

    bool execute() override {
        bool isCompleted = loadCellRead->execute();
        return isCompleted; 
    }

    void getResponse(byte* rsp) override {
        size_t rspSize = loadCellRead->response(rsp);
        DeviceResponse& deviceRsp = DeviceResponse::getInstance();
        deviceRsp.appendResponse(rsp, rspSize); 
    }
    
};

#endif