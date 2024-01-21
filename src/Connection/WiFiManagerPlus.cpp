#include "WiFiManagerPlus.h"
#include "Utility/StorageHandler.h"

WiFiManagerPlus::WiFiManagerPlus() : WiFiManager_()
{
    if (StorageHandler::checkDir("/config") == false)
    {
        StorageHandler::createDir("/config");
    }

    if (StorageHandler::checkFile("/config/wifi.txt") == false)
    {
        StorageHandler::writeFile("/config/wifi.txt", " ");
        StorageHandler::appendFile("/config/wifi.txt", " ");
    }

    TimerReconnect.setInterval(DEFAULT_RECONNECT_INTERVAL);
    TimerReconnect.setCallback([this]()
                               { this->reconnect(); });
}

WiFiManagerPlus::~WiFiManagerPlus()
{
}

bool WiFiManagerPlus::isConnected()
{
    return this->wifi_connected;
}

String WiFiManagerPlus::getSSID()
{
    return this->SSID;
}

String WiFiManagerPlus::getPassword()
{
    return this->password;
}

String WiFiManagerPlus::readSSID()
{
    String *file = StorageHandler::readFile("/config/wifi.txt");
    String res = file[0];
    delete file;
    return res;
}

String WiFiManagerPlus::readPassword()
{
    String *file = StorageHandler::readFile("/config/wifi.txt");
    String res = file[1];
    delete file;
    return res;
}

bool WiFiManagerPlus::begin(String APName, String APPassword, WiFiManagerPlusMode mode, int timeout)
{
    bool res;

    this->AP_Name = APName;
    this->AP_Password = APPassword;

#ifdef DEBUG_WIFI_MANAGER
    Serial.println("WiFiManager Started");
#endif

    WiFiManager_.setConfigPortalTimeout(timeout);
    if (mode == WMPS_ONDEMAND)
    {
        // Settingan selalu menyalakan WiFi Manager Ketika Menyala
        res = WiFiManager_.startConfigPortal(this->AP_Name.c_str(), this->AP_Password.c_str());
    }
    else if (mode == WMPS_AUTOCONNECT)
    {
        // Settingngan autoconnect kalau pernah konek ke wifi sebelumnya jadi ngga akan nyalain WiFi Manager
        res = WiFiManager_.autoConnect(this->AP_Name.c_str(), this->AP_Password.c_str()); // password protected ap
    }

    if (!res)
    {
#ifdef DEBUG_WIFI_MANAGER
        Serial.println("Failed to connect");
#endif
        // ESP.restart();
        this->wifi_connected = false;
        this->SSID = readSSID();
        this->password = readPassword();
    }
    else
    {
// if you get here you have connected to the WiFi
#ifdef DEBUG_WIFI_MANAGER
        this->saved_wifi_ssid = wm.getWiFiSSID();
        this->saved_wifi_password = wm.getWiFiPass();
        Serial.println("connected...yeey :)");
#endif
        Serial.println("WiFi connected");
        Serial.println("Local IP");
        Serial.println(WiFi.localIP());

        String *file = StorageHandler::readFile("/config/wifi.txt");

        if (file[0] != WiFiManager_.getWiFiSSID() || file[1] != WiFiManager_.getWiFiPass())
        {
            StorageHandler::writeFile("/config/wifi.txt", WiFiManager_.getWiFiSSID().c_str());
            StorageHandler::appendFile("/config/wifi.txt", WiFiManager_.getWiFiPass().c_str());
        }

        SSID = WiFiManager_.getWiFiSSID();
        password = WiFiManager_.getWiFiPass();

        this->wifi_connected = true;

        delete file;
    }

#ifdef DEBUG_WIFI_MANAGER

#endif

    return res;
}

bool WiFiManagerPlus::reconnect()
{
    if (WiFi.begin(this->SSID.c_str(), this->password.c_str()) == WL_CONNECTED)
    {
        this->wifi_connected = true;
        return true;
    }
    else
    {
        this->wifi_connected = false;
        return false;
    }
    return false;
}

void WiFiManagerPlus::disconnect()
{
    WiFi.disconnect();
    this->wifi_connected = false;
}

void WiFiManagerPlus::loop()
{
    TimerReconnect.loop();
    if (WiFi.status() != WL_CONNECTED)
    {
        TimerReconnect.setEnable(true);
    }
    else
    {
        TimerReconnect.setEnable(false);
    }
}
