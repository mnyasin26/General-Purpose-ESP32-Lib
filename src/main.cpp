#include <main_header.h>
#include <Connection/Mqtt.h>
#include <Connection/WiFiManagerPlus.h>

WiFiManagerPlus WifiManager_h;
Mqtt Mqtt_h("ESP32-TEST");

void setup()
{
	Serial.begin(115200);
	delay(1000);
	Serial.println("Hello world");
	WifiManager_h.begin("AP-ESP32", "12345678", WMPS_ONDEMAND);
	Mqtt_h.connect("broker.hivemq.com");
}

void loop()
{
	// maintain objects loop
	WifiManager_h.loop();
	Mqtt_h.loop();
}
