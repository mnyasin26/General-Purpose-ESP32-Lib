#include <Arduino.h>
#include <Connection/Mqtt.h>


Mqtt mqtt("ESP32-NF08");

void setup() {
	Serial.begin(115200);
	delay(1000);
	Serial.println("Hello world");
	mqtt.connect("broker.hivemq.com");
}

void loop() {
	mqtt.loop();
	delay(1000);
}
