#line 1 "C:\\Users\\Owner\\OneDrive\\바탕 화면\\FIRMWARE.BEERO\\SensorHandler.h"
#ifndef SENSORHANDLER_H 
#define SENSORHANDLER_H

#define LOAD 49 // PL=PL0=49 //out
#define DIN  48 // Q7=PL1=48 //in
#define CLK  46 // CP=PL3=46 //out
#define CE   16 // CE=PH1=16 //out

#define NUMBER_OF_SHIFT_CHIPS 2
#define DATA_WIDTH  NUMBER_OF_SHIFT_CHIPS * 8

#include <stdint.h> 
#include <Arduino.h>

// must be signle-ton pattern 
class SensorHandler {
private:

    // single-ton pattern 
    SensorHandler();
    SensorHandler(const SensorHandler&) = delete;
    SensorHandler& operator=(const SensorHandler&) = delete;

    // sensor value
    byte sensor_value_1 = 0x00;
    byte sensor_value_2 = 0x00; 
    uint16_t sensor_value = 0x00; 
    

public:

    static SensorHandler& getInstance(); 

    // access to private variables
    uint8_t getSensorValue1() const {return sensor_value_1;}
    uint8_t getSensorValue2() const {return sensor_value_2;}

    uint16_t getSensorValue() const {return sensor_value;}

    void initialSensorHandler();
    bool execute();
    void makeSensorValue(byte* sensorValue1, byte* sensorValue2); 

};

#endif