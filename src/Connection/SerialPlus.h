#ifndef SERIAL_PLUS_H
#define SERIAL_PLUS_H

#include <main_header.h>
#include <HardwareSerial.h>

#if ENABLE_DEBUG == true
#if MQTT_DEBUG == true
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(x, y) Serial.printf(x, y)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(x, y)
#endif
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(x, y)
#endif

class SerialPlus
{
private:
    HardwareSerial serialPort;
    uint8_t port;
    int baudrate;
    bool isBegin = false;
    int address;
    uint8_t RX;
    uint8_t TX;
    String previous_msg = "";
    String latest_msg = "";
    String sourceAddressLatestMsg = "";
    String sourceAddressPreviousMsg = "";
    bool isLatestMsg = false;
    bool isPreviousMsg = false;
    bool isListen = false;
    char terminator = '\n';
    String buffer = "";

    bool isAddressMode = false;

    void listen();

public:
    SerialPlus(uint8_t port = 1);

    bool begin(uint8_t RX = 16, uint8_t TX = 17, int baudrate = 115200, bool isAddressMode = false, int address = 0);
    void write(int desAddress, String message);
    void write(String message);
    void print(int desAddress, String message);
    void print(String message);
    void println(int desAddress, String message);
    void println(String message);

    void setAddressMode(bool isAddressMode);
    void setTerminator(char terminator);
    char getTerminator();

    String getLatestMsg();
    String getSourceAddressLatestMsg();
    String getPreviousMsg();
    String getSourceAddressPreviousMsg();
    bool isMsgAvailable();
    

    void pauseListen();
    void resumeListen();
    void end();
    void loop();

    ~SerialPlus();
};

#endif