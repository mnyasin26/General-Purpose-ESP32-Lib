#include "WiFiManagerPlus.h"
#include "Utility/StorageHandler.h"

WiFiManagerPlus::WiFiManagerPlus() : WiFiManager_()
{
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
    vector<String> *file = StorageHandler::readFile("/config/wifi.txt");
    String res = file->at(0);
    delete file;
    return res;
}

String WiFiManagerPlus::readPassword()
{
    vector<String> *file = StorageHandler::readFile("/config/wifi.txt");
    String res = file->at(1);
    delete file;
    return res;
}

bool WiFiManagerPlus::begin(String APName, String APPassword, WiFiManagerPlusMode mode, int timeout)
{
    bool res;

    this->AP_Name = APName;
    this->AP_Password = APPassword;

    DEBUG_PRINTLN("Starting WiFiManager");

    StorageHandler::begin();

    if (StorageHandler::checkDir("/config") == false)
    {
        DEBUG_PRINTLN("Directory /config not exist");
        StorageHandler::createDir("/config");
    }
    else
    {
        DEBUG_PRINTLN("Directory /config already exist");
    }

    if (StorageHandler::checkFile("/config/wifi.txt") == false)
    {
        DEBUG_PRINTLN("File /config/wifi.txt not exist");
        StorageHandler::writeFile("/config/wifi.txt", " ");
        StorageHandler::appendFile("/config/wifi.txt", " ");
    }

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
        DEBUG_PRINTLN("Failed to connect or hit timeout");

        // ESP.restart();
        this->wifi_connected = false;
        this->SSID = readSSID();
        this->password = readPassword();
    }
    else
    {
        DEBUG_PRINTLN("connected...yeey :)");
        DEBUG_PRINT("WiFi connected with IP: ");
        DEBUG_PRINTLN(WiFi.localIP());

        vector<String> *file = StorageHandler::readFile("/config/wifi.txt");

        DEBUG_PRINTLN("saved wifi");
        DEBUG_PRINTF("SSID: %s\n", file->at(0).c_str());
        DEBUG_PRINTF("Password: %s\n", file->at(1).c_str());
        DEBUG_PRINTLN("from wm");
        DEBUG_PRINTF("SSID: %s\n", WiFiManager_.getWiFiSSID().c_str());
        DEBUG_PRINTF("Password: %s\n", WiFiManager_.getWiFiPass().c_str());
        DEBUG_PRINTLN("true or false");
        DEBUG_PRINTF("state SSID: %d\n", file->at(0) != WiFiManager_.getWiFiSSID());
        DEBUG_PRINTF("state Password: %d\n", file->at(1) != WiFiManager_.getWiFiPass());
        DEBUG_PRINTLN("saved wifi");
        DEBUG_PRINTF("Length SSID: %d\n", file->at(0).length());
        DEBUG_PRINTF("Length Password: %d\n", file->at(1).length());
        DEBUG_PRINTLN("from wm");
        DEBUG_PRINTLN(WiFiManager_.getWiFiSSID().length());
        DEBUG_PRINTLN(WiFiManager_.getWiFiPass().length());

        if (file->at(0) != WiFiManager_.getWiFiSSID() || file->at(1) != WiFiManager_.getWiFiPass())
        {
            DEBUG_PRINTLN("TESPOINT 5.1");

            StorageHandler::writeFile("/config/wifi.txt", WiFiManager_.getWiFiSSID().c_str());
            StorageHandler::appendFile("/config/wifi.txt", WiFiManager_.getWiFiPass().c_str());
        }

        SSID = WiFiManager_.getWiFiSSID();
        password = WiFiManager_.getWiFiPass();

        this->wifi_connected = true;

        delete file;
    }
    return res;
}

void WiFiManagerPlus::reconnect()
{
    DEBUG_PRINTLN("Reconnecting to WiFi");
    DEBUG_PRINTF("SSID: %s\n", this->SSID.c_str());
    DEBUG_PRINTF("Password: %s\n", this->password.c_str());
    WiFi.begin(this->SSID.c_str(), this->password.c_str());
    DEBUG_PRINTF("Status: %d\n", WiFi.status());
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
        if (this->wifi_connected == true)
        {
            DEBUG_PRINTLN("WiFi Disconnected");
            this->wifi_connected = false;
        }

        TimerReconnect.setEnable(true);
    }
    else
    {
        if (this->wifi_connected == false)
        {
            DEBUG_PRINTLN("WiFi Connected");
            this->wifi_connected = true;
        }

        TimerReconnect.setEnable(false);
    }
}
