# 1 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino"
# 2 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino" 2
# 3 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino" 2
# 4 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino" 2
# 5 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino" 2
# 6 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino" 2

# 8 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino" 2
# 9 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino" 2
# 10 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino" 2
# 11 "C:\\Users\\escap\\Desktop\\beero\\FIRMWARE.BEERO\\FIRMWARE.BEERO.ino" 2




IPAddress ip(192, 168, 0, 141);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

TCPHandler& tcpHandler = TCPHandler::getInstance();

void setup()
{
   Serial.begin(115200);
    Ethernet.init(53);

    // Ethernet and TCPserver starts
    tcpHandler.begin(mac, ip);
    Serial.println("tcp handler");

    Serial.print("IP: ");
    Serial.println(ip);

    // sensor initial
    SensorHandler& sensorHandler = SensorHandler::getInstance();
    sensorHandler.initialSensorHandler();

    // relay initial
    Serial.println("relay");
    RelaySetup::initializePins();

    // motor initial
    Serial.println("motor");
    MotorSetup::initializePins();
}

void loop()
{
 TCPHandler& tcpHandler = TCPHandler::getInstance();
    tcpHandler.clientHandle();
}
