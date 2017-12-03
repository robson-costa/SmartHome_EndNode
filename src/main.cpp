#include <Arduino.h>
//#include "DHT.h"

// Main execution period (ms)
#define PERIOD      1000

// Define if the sensor is enabled (1) or not (0)
#define SENSOR_LDR  1
#define SENSOR_PIR  1
#define SENSOR_DHT  1

// LDR sensor configurations
#ifdef SENSOR_LDR
  #define SENSOR_LDR_PIN  A0
#endif

// PIR sensor configurations
#ifdef SENSOR_PIR
  #define SENSOR_PIR_PIN  7
#endif

// DHT sensor configurations
#ifdef SENSOR_DHT
  #define SENSOR_DHT_TYPE DHT11
  #define SENSOR_DHT_PIN  A1
#endif

// Functions declaration
#ifdef SENSOR_LDR
int getLDRValue(int samples, int interval);
#endif

void setup() {
    // initialize LED digital pin as an output
    pinMode(LED_BUILTIN, OUTPUT);

    #ifdef SENSOR_PIR
      // initialize PIR pin as an output
      pinMode(SENSOR_LDR_PIN, OUTPUT);
    #endif

    #ifdef SENSOR_DHT

    #endif
}

void loop() {
    // turn the LED on/off
    if (digitalRead(LED_BUILTIN) == LOW) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }

    #ifdef SENSOR_LDR
      // Get the luminosity value
      int ldrValue = getLDRValue(10, 1);
    #endif

    // Print the values on serial
    Serial.print("#############################\r\n");
    #ifdef SENSOR_PIR
      Serial.print("Luminosity: ");
      Serial.print(ldrValue);
    #endif
    Serial.print("\r\n#############################");

    // Wait the Main Execution Period
    delay(PERIOD);
}

#ifdef SENSOR_PIR
/* Get the LDR ldrValue
 * params: int samples - number of samples
 *         int interval - interval between samples
 * return: int media between LDR values sampled
 */
int getLDRValue(int samples, int interval) {
  unsigned int value = 0;
  for (int i = 0; i < samples; i++) {
    value += analogRead(SENSOR_LDR_PIN);
    delay(interval);
  }
  return value/samples;
}
#endif
