#ifndef MQTT_H
#define MQTT_H

#include <main_header.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <Utility/SimpleTimer.h>

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

#define MQTT_MAX_PACKET_SIZE 4096
#define DEFAULT_RECONNECT_INTERVAL 1000

/// @brief A class to handle MQTT connection
class Mqtt
{
private:
    PubSubClient Client;
    WiFiClient WifiClient;
    SimpleTimer TimerReconnect;

    String idClient;
    String previous_msg;
    String latest_msg;
    String server;
    String port;

    void (*onDataAvailable)();
    void (*onConnected)();
    void callback(char *topic, byte *payload, unsigned int length);

public:
    Mqtt(String ID_Client);
    ~Mqtt();

    bool connect(String server, String port = "1883");
    bool reconnect();
    void disconnect();

    String latestMsg();
    String previousMsg();
    bool sendMsg(String topic, String msg);
    
    bool isConnected();
    bool isDataAvailable();
    
    void loop();

    void setReconnectInterval(unsigned long interval);
    void setOnDataAvailable(void (*callback)());
    void setOnConnected(void (*callback)());
};

#endif // MQTT_H