#include <main_header.h>
#include <Connection/Mqtt.h>
#include <Connection/WiFiManagerPlus.h>
#include <Connection/SerialPlus.h>

WiFiManagerPlus WifiManager_h;
Mqtt Mqtt_h("ESP32-TEST");
SerialPlus SerialPlus_h(1);

void setup()
{
	Serial.begin(115200);
	SerialPlus_h.begin(16, 17);
	delay(1000);
	Serial.println("Hello world");
	WifiManager_h.begin("AP-ESP32", "12345678", WMPS_ONDEMAND);
	Mqtt_h.connect("broker.hivemq.com");
}

void loop()
{
	SerialPlus_h.println("Hello world");

	// maintain objects loop
	SerialPlus_h.loop();
	WifiManager_h.loop();
	Mqtt_h.loop();
}
