#include <Ethernet.h>
#include <SPI.h>
#include <Dns.h>
#include <Dhcp.h>
#include <avr/interrupt.h>

#include "TCPHandler.h"
#include "RelayHandler.h"
#include "SensorHandler.h"
#include "MotorHandler.h"
#include "LoadcellHandler.h"

#define SERVER_PORT 502
#define sspin       53

IPAddress ip(192, 168, 0, 141);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 

TCPHandler& tcpHandler = TCPHandler::getInstance();

void setup()
{
	Serial.begin(115200);
    Ethernet.init(sspin);

    // sensor initial
    SensorHandler& sensorHandler = SensorHandler::getInstance();
    sensorHandler.initialSensorHandler(); 

    // relay initial
    Serial.println("relay");
    RelaySetup::initializePins(); 

    // motor initial
    Serial.println("motor");
    MotorSetup::initializePins(); 

    // loadcell pin setup
    Serial.println("loadcell");
    for (int index=0; index<3; index++){
        scales[index].begin(LOADCELL_DOUT_PINS[index], LOADCELL_SCK_PINS[index]);
    }

    // Ethernet and TCPserver starts
    tcpHandler.begin(mac, ip);
    Serial.println("tcp handler");
    
    Serial.print("IP: ");
    Serial.println(ip);
}

void loop()
{
	TCPHandler& tcpHandler = TCPHandler::getInstance(); 
    tcpHandler.clientHandle();
}
