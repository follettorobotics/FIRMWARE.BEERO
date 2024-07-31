#ifndef Operator_H
#define Operator_H

#include "Command.h"

#define     MAX_COMMANDS     15
#define     poll_timer       50000  // 0.05s

// single-ton pattern
class Operator {
private:

    Command* commands[MAX_COMMANDS]; 
    int commandCount = 0; 

    Operator() {

        Command* readSensorCmd = new ReadSensorCommand(); 

        for (int i=0; i<MAX_COMMANDS; i++){
            commands[i] = nullptr; 
        }
        
        addCommand(readSensorCmd);
    }

    Operator(const Operator&) = delete;
    Operator& operator=(const Operator&) = delete; 

    int count = 0; 

public:

    static Operator& getInstance() {
        static Operator instance;
        return instance; 
    }

    bool addCommand(Command* command) {
        if (commandCount >= MAX_COMMANDS) {
            return false; 
        }
        for (int i=0; i<MAX_COMMANDS; i++){
            if (commands[i] == nullptr){
                commands[i] = command;
                commandCount++; 
                return true;
            }
        }
    }

    void executeCommands() {
        // Sensor Handler execute
        commands[0]->execute(); 
        
        for (int i = 1; i < MAX_COMMANDS; i++) {
            if (commands[i] != nullptr) {
                bool result = commands[i]->execute();
                if (result){
                    // success
                    byte data[20]; 
                    commands[i]->getResponse(data); 
                    delete commands[i]; 
                    commandCount--; 
                    commands[i] = nullptr;
                }
            }
        }
    }
};

#endif