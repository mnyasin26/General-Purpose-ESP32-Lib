#include <Arduino.h>

// DEBUG
#define ENABLE_DEBUG true

#ifdef ENABLE_DEBUG
#define WIFI_DEBUG true
#define MQTT_DEBUG false
#define STORAGE_HANDLER_DEBUG true
#endif

// TESTING
#define TESTING true

#if TESTING == true
#define STORAGE_HANDLER_TEST true

#endif


// #if ENABLE_DEBUG == true
// #if WIFI_DEBUG == true
// #define DEBUG_PRINT(x) Serial.print(x)
// #define DEBUG_PRINTLN(x) Serial.println(x)
// #define DEBUG_PRINTF(x, y) Serial.printf(x, y)
// #else
// #define DEBUG_PRINT(x)
// #define DEBUG_PRINTLN(x)
// #define DEBUG_PRINTF(x, y)
// #endif
// #else
// #define DEBUG_PRINT(x)
// #define DEBUG_PRINTLN(x)
// #define DEBUG_PRINTF(x, y)
// #endif