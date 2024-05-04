#include "BitStuffing.h"

BitStuffing::BitStuffing() {}

BitStuffing& BitStuffing::getInstance(){
    static BitStuffing instance;
    return instance;
}


/*
    0x7E(start)		->	 0x7D 0x5E
    0xAA(end)		->	 0x7D 0x2A
    0x7D(replaced)	->	 0x7D 0x5D
*/
size_t BitStuffing::applyBitStuffing(byte* message, size_t size, byte* stuffedMessage){
    size_t index = 0;

    // start byte 
    stuffedMessage[index++] = message[0];

    for (size_t i=1; i < size-1; i++){
        switch (message[i])
        {
        case 0x7E:
            stuffedMessage[index++] = 0x7D;
            stuffedMessage[index++] = 0x5E;
            break;

        case 0xAA:
            stuffedMessage[index++] = 0x7D;
            stuffedMessage[index++] = 0x2A;

        case 0x7D:
            stuffedMessage[index++] = 0x7D;
            stuffedMessage[index++] = 0x5D;
        
        default:
            stuffedMessage[index++] = message[i];
        }
    }

    // end byte 
    stuffedMessage[index++] = message[size-1];
    
    return index;
}

size_t BitStuffing::removeBitStuffing(byte* stuffedMessage, size_t stuffedSize, byte* message){
    
    size_t index = 0;

    // start byte
    message[index++] = stuffedMessage[0];

    for (size_t i=1; i <stuffedSize-1; i++){
        if (stuffedMessage[i] == 0x7D && (i+1) < stuffedSize){
            switch (stuffedMessage[i+1])
            {
                case 0x5E:
                    message[index++] = 0x7E;
                    i++;
                    break;
                case 0x2A:
                    message[index++] = 0xAA;
                    i++;
                    break;
                case 0x5D:
                    message[index++] = 0x7D;
                    i++;
                    break;
            }
        }else{
            message[index++] = stuffedMessage[i];
        }
    }
    // end-byte
    message[index++] = stuffedMessage[stuffedSize-1];

    // unstuffed size 
    return index;
}