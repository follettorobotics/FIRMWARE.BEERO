#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <Arduino.h>
#include "SensorHandler.h"
#include "RelayHandler.h"
#include "MotorHandler.h"
#include "LoadcellHandler.h"

#define RELAY                       7
#define EX_MOTOR                    4
#define sensorValueIndex            16

#define startByte                   0x7E
#define endByte                     0xAA

// Request
#define sensorReandReqCommand       0xB0 
#define relayHandlerReqCommand      0xB1
#define motorRunReqCommand          0xB2
#define relayOnOffReqCommand        0xB3
#define loadcellInitialReqCommand   0xB4
#define loadcellReadReqCommand      0xB5

// Response 
#define sensorReadRspCommand        0xC0
#define relayHandlerRspCommand      0xC1
#define motorRunRspCommand          0XC2
#define relayOnOffRspCommand        0xC3
#define loadcellInitialRspCommand   0xC4
#define loadcellReadRspCommand      0xC5

// Error code command
#define startByteErrorCommand       0xE0
#define endByteErrorCommand         0xE1
#define relayHandlerErrorCommand    0xE2
#define motorRunErrorCommand        0xE3
#define relayOnOffErrorCommand      0xE4

#define nonExistErrorCommand        0xEA

// Error code specific
#define relayNumberErrorByte        0xD0
#define motorNumberErrorByte        0xD1
#define relayBrakeErrorByte         0xD2
#define motorDirErrorByte           0xD4
#define SensorLimitErrorByte        0xD5
#define relayControlErrorByte       0xD6

// motor direction
#define doorDown                    0x00
#define doorUp                      0xff

// relay ON OFF
#define relayOn                     0x01
#define relayOff                    0x00

class Dispatcher {
private:

    Dispatcher();
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

public:
    // get instance
    static Dispatcher& getInstance(); 

    size_t dispatch(byte* request, size_t requestSize, byte* response);
    unsigned long hexToDecimal(const byte data1, const byte data2);
};


#endif 