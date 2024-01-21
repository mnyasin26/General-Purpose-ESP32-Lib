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

class Mqtt
{
private:
    PubSubClient Client;
    WiFiClient WifiClient;
    SimpleTimer TimerReconnect;

    String idClient;
    String last_message;
    String server;
    String port;

    void (*onDataAvailable)();
    void (*onConnected)();
    void callback(char *topic, byte *payload, unsigned int length);

public:
    Mqtt(String ID_Client);
    ~Mqtt();

    bool reconnect();
    void setReconnectInterval(unsigned long interval);
    bool connect(String server, String port);
    void disconnect();
    bool send();
    String lastMessage();
    bool isConnected();
    bool isDataAvailable();
    void setOnDataAvailable(void (*callback)());
    void setOnConnected(void (*callback)());
    void loop();
};

Mqtt::Mqtt(String ID_Client) : Client(), WifiClient()
{
    idClient = ID_Client;
    last_message = "";
    onDataAvailable = NULL;
    Client.setClient(WifiClient);
}

Mqtt::~Mqtt()
{
}

void Mqtt::setReconnectInterval(unsigned long interval)
{
    TimerReconnect.setInterval(interval);
}

bool Mqtt::connect(String server, String port)
{
    this->server = server;
    this->port = port;


    Client.setServer(server.c_str(), port.toInt());
    Client.setCallback([this](char *topic, byte *payload, unsigned int length)
                       { this->callback(topic, payload, length); });
    Client.setBufferSize(MQTT_MAX_PACKET_SIZE);

    TimerReconnect.setInterval(DEFAULT_RECONNECT_INTERVAL);
    TimerReconnect.setCallback([this]()
                               { this->reconnect(); });

    DEBUG_PRINTLN(WiFi.isConnected() == WL_CONNECTED ? "[info] WiFi is connected" : "[Warning] WiFi is not connected");
    
    bool result = Client.connect(idClient.c_str());

    DEBUG_PRINTF("MQTT is %s", result ? "connected" : "not connected");

    return result;
}

bool Mqtt::reconnect()
{
    bool result = Client.connect(idClient.c_str()); 
    DEBUG_PRINTF("MQTT is %s", result ? "connected" : "not connected");
    return result;
}

void Mqtt::loop()
{
    TimerReconnect.loop();
    if (Client.connected())
    {
        // Reconnect MQTT Disable, loop Client
        TimerReconnect.setEnable(false);
        Client.loop();
    }
    else
    {
        // Reconnect MQTT Enable
        TimerReconnect.setEnable(true);
    }
}

#endif // MQTT_H

// void connectCallback(Mqtt* mqtt)
// {
//     mqtt->connect(server, port);
// }
