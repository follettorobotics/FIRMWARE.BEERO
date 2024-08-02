#include "LoadCellHandler.h"

HX711 scales[NUM_LOADCELLS];

float CALIBRATION_FACTORS[5] = {
    -744.61f,
    -1921.96f,
    -1998.84f,
    -744.61f,
    -744.61f
};

bool LoadcellSetup::execute(){
    if (initial_step == 0){
        scales[initial_step].set_scale(CALIBRATION_FACTORS[initial_step]);
        scales[initial_step].tare(); 
        initial_step++;
        return false;
    }else if(initial_step == 1){
        scales[initial_step].set_scale(CALIBRATION_FACTORS[initial_step]);
        scales[initial_step].tare(); 
        initial_step++;
        return false; 
    }else{
        scales[initial_step].set_scale(CALIBRATION_FACTORS[initial_step]);
        scales[initial_step].tare(); 
        initial_step = 0;
        // Serial.println("loadcell initial finished"); 
        return true;
    }
}

size_t LoadcellSetup::response(byte* loadcellSetupRsp){
    size_t index = 0;

    loadcellSetupRsp[index++] = startByte; 
    loadcellSetupRsp[index++] = loadcellReadRspCommand;
    loadcellSetupRsp[index++] = packet_number; 
    loadcellSetupRsp[index++] = endByte;
    return index; 
}

bool LoadCellHandler::execute() {
    bool success = true;

    // read loadcell 
    for (int i=0; i<3; i++){
        loadcellValues[i] = scales[i].get_units();
        // Serial.print("loadcell value ");
        // Serial.print(i);
        // Serial.print(" ");
        // Serial.println(loadcellValues[i]);

        delay(10); 
    }
    return success;
}

size_t LoadCellHandler::response(byte* loadcellRsp) {
    // Serial.println("make the load cell response");
    size_t index = 0;
    loadcellRsp[index++] = startByte; 
    loadcellRsp[index++] = loadcellReadRspCommand;

    for (int i=0; i<3; i++){
        float value = loadcellValues[i];
        byte* bytePointer = (byte*) &value;  
        for (int j = 0; j < sizeof(float); j++) {
            loadcellRsp[index++] = bytePointer[j];
        }
    }
    loadcellRsp[index++] = packet_number;
    loadcellRsp[index++] = endByte;
    return index; 
}
