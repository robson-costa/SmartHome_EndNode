#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Main execution period (ms)
#define PERIOD      1000

// Define if the sensor is enabled (1) or not (0)
#define SENSOR_LDR  1
#define SENSOR_PIR  1
#define SENSOR_DHT  1

// LDR sensor configurations
#if defined (SENSOR_LDR)
  #define SENSOR_LDR_PIN    A0
  #define LUX_CALC_SCALAR   12518931
  #define LUX_CALC_EXPONENT -1.405
  #define REF_RESISTANCE    10000
  #define MAX_ADC_READING   1023
  #define ADC_REF_VOLTAGE   5.0
#endif

// PIR sensor configurations
#if defined (SENSOR_PIR)
  #define SENSOR_PIR_PIN  7
#endif

// DHT sensor configurations
#if defined (SENSOR_DHT)
  #define SENSOR_DHT_TYPE DHT11
  #define SENSOR_DHT_PIN  A1
#endif

// Functions declaration
#if defined (SENSOR_LDR)
  int getLDRValue(int samples, int interval);
  float getLDRValueinLux(int samples, int interval);
  float getTemperatureValue(int samples, int interval);
  float getHumidityValue(int samples, int interval);
#endif

// Global variables
DHT dht(SENSOR_DHT_PIN, SENSOR_DHT_TYPE);

void setup() {
    // initialize LED digital pin as an output
    pinMode(LED_BUILTIN, OUTPUT);

    #if defined (SENSOR_PIR)
      // initialize PIR pin as an output
      pinMode(SENSOR_LDR_PIN, INPUT);
    #endif

    #if defined (SENSOR_DHT)
      // initialize DHT sensor
      dht.begin();
    #endif

    // Configure serial OUTPUT
    Serial.begin(115200);
}

void loop() {
    // turn the LED on/off
    if (digitalRead(LED_BUILTIN) == LOW) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }

    #if defined (SENSOR_LDR)
      // Get the LDR (Luminosity) value
      int ldrValue = getLDRValue(10, 1);
      float lux = getLDRValueinLux(10, 1);
    #endif

    #if defined (SENSOR_PIR)
      // Get the PIR (Presence) value
      int pirValue = digitalRead(SENSOR_PIR_PIN);
    #endif

    #if defined (SENSOR_DHT)
      // Get the DHT (Temperature) value
      int tempValue = getTemperatureValue(10, 1);

      // Get the DHT (Humidity) value
      int humValue = getHumidityValue(10, 1);
    #endif

    // Print the values on serial
    Serial.print("\r\n#############################");
    #if defined (SENSOR_LDR)
      Serial.print("\r\nLuminosity (0-1023): ");
      Serial.print(ldrValue);
      Serial.print("\r\nLuminosity (in Lux): ");
      Serial.print(lux);
    #endif
    #if defined (SENSOR_PIR)
      Serial.print("\r\nPresence: ");
      if (pirValue == LOW) {
        Serial.print("STOPPED!!!");
      } else {
        Serial.print("MOVIMENT!!!");
      }
    #endif
    #if defined (SENSOR_DHT)
      Serial.print("\r\nTemperature: ");
      Serial.print(tempValue);
      Serial.print(" oC\r\nHumidity: ");
      Serial.print(humValue);
      Serial.print("%");
    #endif
    Serial.print("\r\n#############################\r\n");

    // Wait the Main Execution Period
    delay(PERIOD);
}

#if defined (SENSOR_LDR)
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

float getLDRValueinLux(int samples, int interval) {
  // Perform the analog to digital conversion
	int ldrRawData = getLDRValue(samples, interval);

  // RESISTOR VOLTAGE_CONVERSION
	// Convert the raw digital data back to the voltage that was measured on the analog pin
	float resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;

  // voltage across the LDR is the 5V supply minus the 10k resistor voltage
  float ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;

  // LDR_RESISTANCE_CONVERSION
	// resistance that the LDR would have for that voltage
	float ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;

  // LDR_LUX
	// Change the code below to the proper conversion from ldrResistance to
	// ldrLux
	float ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);

  return ldrLux;
}
#endif

#if defined (SENSOR_DHT)
/* Get the DHT (Temperature) value
 * params: int samples - number of samples
 *         int interval - interval between samples
 * return: float media between temperature values sampled
 */
float getTemperatureValue(int samples, int interval) {
  float value = 0;
  for (int i = 0; i < samples; i++) {
    value += dht.readTemperature();
    delay(interval);
  }
  return value/samples;
}

/* Get the DHT (Humidity) value
 * params: int samples - number of samples
 *         int interval - interval between samples
 * return: float media between humidity values sampled
 */
float getHumidityValue(int samples, int interval) {
  float value = 0;
  for (int i = 0; i < samples; i++) {
    value += dht.readHumidity();
    delay(interval);
  }
  return value/samples;
}
#endif
