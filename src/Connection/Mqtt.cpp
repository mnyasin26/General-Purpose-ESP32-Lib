#include "Mqtt.h"

/// @brief 
/// @param ID_Client ID of MQTT client (must be unique)
Mqtt::Mqtt(String ID_Client) : Client(), WifiClient()
{
    idClient = ID_Client;
    latest_msg = "";
    onDataAvailable = NULL;
    Client.setClient(WifiClient);
}

Mqtt::~Mqtt()
{
}

/// @brief Connect to MQTT server
/// @param server domain name or IP address of MQTT server
/// @param port default is 1883
/// @return true or false if MQTT is connected or not
bool Mqtt::connect(String server, String port)
{
    this->server = server;
    this->port = port;

    Client.setServer(server.c_str(), port.toInt());
    Client.setCallback([this](char *topic, byte *payload, unsigned int length)
                       { this->callback(topic, payload, length); });
    Client.setBufferSize(MQTT_MAX_PACKET_SIZE_LOCAL);

    TimerReconnect.setInterval(DEFAULT_RECONNECT_INTERVAL);
    TimerReconnect.setCallback([this]()
                               { this->reconnect(); });

    DEBUG_PRINTLN(WiFi.isConnected() == WL_CONNECTED ? "[info] WiFi is connected" : "[Warning] WiFi is not connected");

    bool result = Client.connect(idClient.c_str());

    DEBUG_PRINTF("MQTT is %s", result ? "connected" : "not connected");

    return result;
}

/// @brief Reconnect to MQTT server that has been connected before
/// @return true or false if MQTT is connected or not
bool Mqtt::reconnect()
{
    bool result = Client.connect(idClient.c_str());
    DEBUG_PRINTF("MQTT is %s", result ? "connected" : "not connected");
    return result;
}

/// @brief Disconnect from MQTT server
void Mqtt::disconnect()
{
    Client.disconnect();
}

/// @brief get latest message from MQTT server
/// @return a string of latest message
String Mqtt::latestMsg()
{
    return latest_msg;
}

/// @brief get previous message from MQTT server
/// @return a string of previous message
String Mqtt::previousMsg()
{
    return previous_msg;
}

/// @brief send message to MQTT server
/// @param topic topic of MQTT server that will be sent
/// @param msg the message that will be sent
/// @return boolean if message is sent or not
bool Mqtt::sendMsg(String topic, String msg)
{
    if (Client.connected())
    {
        DEBUG_PRINTLN("[info] Sending message");
        DEBUG_PRINTF("[info] Topic: %s\n", topic.c_str());
        DEBUG_PRINTF("[info] Payload: %s\n", msg.c_str());
        return Client.publish(topic.c_str(), msg.c_str());
    }
    else
    {
        DEBUG_PRINTLN("[Warning] MQTT is not connected");
        return false;
    }
}

/// @brief check if MQTT is connected or not
/// @return boolean if MQTT is connected or not
bool Mqtt::isConnected()
{
    return Client.connected();
}

/// @brief check if there is a new message from MQTT server
/// @return boolean if there is a new message or not
bool Mqtt::isDataAvailable()
{
    return latest_msg != previous_msg;
}

/// @brief used to maintain MQTT connection including reconnecting if MQTT is disconnected within a certain time (interval)
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

/// @brief set reconnect interval
void Mqtt::setReconnectInterval(unsigned long interval)
{
    TimerReconnect.setInterval(interval);
}

/// @brief set callback function when there is a new message from MQTT server
void Mqtt::setOnDataAvailable(void (*callback)())
{
    onDataAvailable = callback;
}

/// @brief set callback function when MQTT is connected
void Mqtt::setOnConnected(void (*callback)())
{
    onConnected = callback;
}

/// @brief this function is called when there is a new message from MQTT server
/// @param topic topic of the message arrived
/// @param payload contains the message arrived
/// @param length length of the message arrived
void Mqtt::callback(char *topic, byte *payload, unsigned int length)
{
    DEBUG_PRINTLN("[info] Message arrived");
    DEBUG_PRINTF("[info] Topic: %s\n", topic);
    DEBUG_PRINT("[info] Payload: ");
    for (unsigned int i = 0; i < length; i++)
    {
        DEBUG_PRINT((char)payload[i]);
    }
    DEBUG_PRINTLN();

    previous_msg = latest_msg;
    latest_msg = "";
    for (unsigned int i = 0; i < length; i++)
    {
        latest_msg += (char)payload[i];
    }

    if (onDataAvailable != NULL)
    {
        onDataAvailable();
    }
}