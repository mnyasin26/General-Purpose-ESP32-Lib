#include <main_header.h>

#if TESTING == false

#include <TaskScheduler.h>
#include <Connection/Mqtt.h>
#include <Connection/WiFiManagerPlus.h>
#include <Connection/SerialPlus.h>


void t1Callback();

//Tasks
Task t1(1000, 10, &t1Callback);


Scheduler runner;


WiFiManagerPlus WifiManager_h;
Mqtt Mqtt_h("ESP32-TEST");
SerialPlus SerialPlus_h(1);

void setup()
{
	Serial.begin(115200);
	// SerialPlus_h.begin(16, 17);
	delay(1000);
	Serial.println("Hello world");
	WifiManager_h.begin("AP-ESP32", "12345678", WMPS_ONDEMAND);
	// Mqtt_h.connect("broker.hivemq.com");

	runner.init();
	runner.addTask(t1);
	t1.enable();
}

void loop()
{
	// SerialPlus_h.println("Hello world");
	// maintain objects loop
	// SerialPlus_h.loop();
	// Mqtt_h.loop();
	
	WifiManager_h.loop();

	runner.execute();
}

void t1Callback()
{
	Serial.print("t1: ");
	Serial.println(millis());
}

#endif
