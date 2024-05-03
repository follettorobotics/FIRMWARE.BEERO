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
}