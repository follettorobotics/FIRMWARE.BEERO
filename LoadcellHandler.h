#ifndef LOADCELLHANDLER_H
#define LOADCELLHANDLER_H

#include <HX711.h>
#include <Arduino.h>

const int NUM_LOADCELLS = 16;

extern float CALIBRATION_FACTORS[3];

// #define calibration_factor          -7050.0

#define startByte                   0x7E
#define loadcellInitialRspCommand   0xC4
#define loadcellReadRspCommand      0xC5
#define endByte                     0xAA

#define SET_BIT(port, bit) ((port) |= (1 << (bit)))
#define CLEAR_BIT(port, bit) ((port) &= ~(1 << (bit)))

// DOUT 
#define DOUT_1          78
#define DOUT_2          84
#define DOUT_3          77
#define DOUT_4          85
#define DOUT_5          38
#define DOUT_6          82
#define DOUT_7          79
#define DOUT_8          83 
#define DOUT_9          45
#define DOUT_10         80
#define DOUT_11         73
#define DOUT_12         81
#define DOUT_13         37
#define DOUT_14         15
#define DOUT_15         36
#define DOUT_16         14

// SCKD
#define SCKD_1          60
#define SCKD_2          68
#define SCKD_3          61
#define SCKD_4          69
#define SCKD_5          58
#define SCKD_6          66
#define SCKD_7          59
#define SCKD_8          67
#define SCKD_9          56
#define SCKD_10         64
#define SCKD_11         57
#define SCKD_12         65
#define SCKD_13         54
#define SCKD_14         62
#define SCKD_15         55
#define SCKD_16         63


const int LOADCELL_DOUT_PINS[NUM_LOADCELLS] = {
    DOUT_1,        
    DOUT_2,           
    DOUT_3,          
    DOUT_4,         
    DOUT_5,        
    DOUT_6,       
    DOUT_7,      
    DOUT_8,     
    DOUT_9,    
    DOUT_10,   
    DOUT_11,
    DOUT_12, 
    DOUT_13,
    DOUT_14,
    DOUT_15,
    DOUT_16 
};

const int LOADCELL_SCK_PINS[NUM_LOADCELLS] = {
    SCKD_1,
    SCKD_2,
    SCKD_3,
    SCKD_4,
    SCKD_5,
    SCKD_6,
    SCKD_7,
    SCKD_8,
    SCKD_9,
    SCKD_10,
    SCKD_11,
    SCKD_12,
    SCKD_13,
    SCKD_14,
    SCKD_15,
    SCKD_16 
};

extern HX711 scales[NUM_LOADCELLS];

class LoadcellSetup {
public:
    static bool initializePins(int index) {
        scales[index].begin(LOADCELL_DOUT_PINS[index], LOADCELL_SCK_PINS[index]);
        scales[index].set_scale(CALIBRATION_FACTORS[index]);
        scales[index].tare(20);

        delay(100);
    }
};

class LoadCellHandler {
private:
    float loadcellValues[NUM_LOADCELLS];

public:
    LoadCellHandler(){
        for (int i = 0; i < NUM_LOADCELLS; i++) {
            loadcellValues[i] = 0.0;
        }
    }

    bool execute();
    size_t response(byte* loadcellRsp);
};

#endif