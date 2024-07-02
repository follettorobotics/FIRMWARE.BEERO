#include "LoadCellHandler.h"

HX711 scales[NUM_LOADCELLS];

bool LoadCellHandler::execute() {
    bool success = true;

    // read loadcell 
    for (int i=0; i<3; i++){
        loadcellValues[i] = scales[i].get_units();
        delay(10); 
    }
    return success;
}

size_t LoadCellHandler::response(byte* loadcellRsp) {
    size_t index = 0;
    loadcellRsp[index++] = startByte; 
    loadcellRsp[index++] = loadCellRspCommand;

    for (int i=0; i<3; i++){
        float value = loadcellValues[i];
        byte* bytePointer = (byte*) &value;  
        for (int j = 0; j < sizeof(float); j++) {
            loadcellRsp[index++] = bytePointer[j];
        }
    }
    
    loadcellRsp[index++] = endByte;
    return index; 
}
