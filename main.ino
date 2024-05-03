#include <Ethernet.h>
#include <SPI.h>
#include <Dns.h>
#include <Dhcp.h>
#include <avr/interrupt.h>

#include "TCPHandler.h"
#include "RelayHandler.h"
#include "SensorHandler.h"

#define SERVER_PORT 502
#define sspin       53

IPAddress ip(192, 168, 0, 141);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 

void setup()
{
	Serial.begin(115200);
    Ethernet.init(sspin);
    Ethernet.begin(mac, ip);

    // sensor initial
    SensorHandler& sensorHandler = SensorHandler::getInstance();
    sensorHandler.initialSensorHandler(); 

    // relay initial
    Serial.println("relay");
    RelaySetup::initializePins(); 

    // motor initial
    

}

void loop()
{
	TCPHandler& tcpHandler = TCPHandler::getInstance(); 
    tcpHandler.clientHandle();
}