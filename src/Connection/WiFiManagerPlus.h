#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <main_header.h>
#include <WiFiManager.h>
#include <WiFi.h>
#include <Utility/SimpleTimer.h>

#if ENABLE_DEBUG == true
#if WIFI_DEBUG == true
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

#define DEFAULT_RECONNECT_INTERVAL 1000

enum WiFiManagerPlusMode
{
    WMPS_AUTOCONNECT,
    WMPS_ONDEMAND
};

class WiFiManagerPlus
{
private:
    WiFiManager WiFiManager_;
    SimpleTimer TimerReconnect;

    String AP_Name;
    String AP_Password;

    bool wifi_connected = false;

    String SSID;
    String password;
    String readSSID();
    String readPassword();

public:
    WiFiManagerPlus();
    ~WiFiManagerPlus();

    bool begin(String APName = "ESP32-WM", String APPassword = "12345678", WiFiManagerPlusMode mode = WMPS_AUTOCONNECT, int timeout = 60);
    void reconnect();
    void disconnect();
    bool isConnected();

    String getSSID();
    String getPassword();

    void loop();
};

#endif