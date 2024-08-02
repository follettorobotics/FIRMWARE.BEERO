#ifndef MOTORHANDLER_H
#define MOTORHANDLER_H

#include "RelayHandler.h"
#include "SensorHandler.h"
#include <Arduino.h>

#define EX_MOTOR        4

#define PIN_PC5         32
#define PIN_PC4         33
#define PIN_PC3         34
#define PIN_PC2         35

#define PIN_PB7         13
#define PIN_PB6         12
#define PIN_PB5         11
#define PIN_PB4         10

#define MOTOR_DELAY     600

struct ExMotorPins{
    int8_t DIR;
    int8_t PWM; 
};

const ExMotorPins exMotorPinAddress[EX_MOTOR] = {
   {PIN_PC5,PIN_PB7},
   {PIN_PC4,PIN_PB6},
   {PIN_PC3,PIN_PB5},
   {PIN_PC2,PIN_PB4} 
}; 

// motor direction
#define doorOpen                    0x00
#define doorClose                   0xff

#define motorRunRspCommand          0XC2

class MotorSetup {
public:
    static void initializePins(){
        for (int i=0; i<EX_MOTOR; i++){
            int8_t dir = exMotorPinAddress[i].DIR;
            int8_t pwm = exMotorPinAddress[i].PWM;
            pinMode(dir, OUTPUT);
            pinMode(pwm, OUTPUT);
            digitalWrite(dir, false);
            digitalWrite(pwm, false);
        }   
    }
}; 

class MotorHandler {
private:

    uint8_t motorNumber;
    uint8_t motorDir;     
    uint16_t motorStep;
    uint16_t motorAddStep;
    uint8_t relayBrake; 
    uint8_t sensorLimit;
    uint8_t errorCheckSensorLimit; 

    uint8_t dirPin; 
    uint8_t pwmPin;

    uint16_t currentStep = 0; 

    bool sensor = false; 
    bool errorCheckSensor = false; 
    bool sensorCheckRequire = false; 

    unsigned int startTime; 
    unsigned int currentTime; 

    uint8_t packet_number; 

public:

    MotorHandler(int motorNumber, uint8_t motorDir, uint16_t motorStep, 
        uint16_t motorAddStep, uint8_t relayBrake, uint8_t sensorLimit, uint8_t errorCheckSensorLimit, uint8_t packet_number):
    motorNumber(motorNumber), motorDir(motorDir), motorStep(motorStep), 
    motorAddStep(motorAddStep), relayBrake(relayBrake), sensorLimit(sensorLimit), errorCheckSensorLimit(errorCheckSensorLimit), packet_number(packet_number){

        dirPin = exMotorPinAddress[motorNumber].DIR;
        pwmPin = exMotorPinAddress[motorNumber].PWM;

        if (sensorLimit != 16){
            sensorCheckRequire = true; 
        }else{
            sensorCheckRequire = false; 
        }

        bool direction; 
        if (motorDir == doorOpen){
            // down
            direction = true; 
        }else{
            direction = false;
        }

        // motor dir 
        if (motorNumber == 0){
            if (direction){
                digitalWrite(dirPin, true);
            }else{
                digitalWrite(dirPin, false);
            }

        }else if (motorNumber == 1){
            if (direction){
                digitalWrite(dirPin, false);
            }else{
                digitalWrite(dirPin, true);
            }

        }else{
            // motorNumber == 2
            if (direction){
                digitalWrite(dirPin, false);
            }else{
                digitalWrite(dirPin, true);
            }
        }
    }

    bool execute(); 
    bool getSensorLimitValue(uint8_t sensorLimit);
    size_t response(byte* exMotorControlRsp); 

}; 

#endif