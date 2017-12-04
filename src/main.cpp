/**
* @file main.cpp
* @author Robson Costa
* @date 3 Dec 2017
* @copyright 2017 Robson Costa
* @brief Main file for SmartHome - EndNode project.
*/

#include <Arduino.h>
#include "DHT_U.h"

/** Main execution period (in MS). */
#define PERIOD      1000

/** Define if the LDR sensor is enabled or not. */
#define SENSOR_LDR  1

/** Define if the PIR sensor is enabled or not. */
#define SENSOR_PIR  1

/** Define if the DHT sensor is enabled or not. */
#define SENSOR_DHT  1

/** @defgroup SENSOR_LDR LDR sensor group.
 *  @{
 */
#if defined (SENSOR_LDR)
  /** LDR sensor pin. */
  #define SENSOR_LDR_PIN    A0
  /** LDR sensor scale to Lux converter. */
  #define LUX_CALC_SCALAR   12518931
  /** LDR sensor exponent to Lux converter. */
  #define LUX_CALC_EXPONENT -1.405
  /** LDR sensor resistor value. */
  #define REF_RESISTANCE    10000
  /** LDR sensor maximum ADC reading. */
  #define MAX_ADC_READING   1023
  /** LDR sensor voltage. */
  #define ADC_REF_VOLTAGE   5.0
#endif
/** @} */

// PIR sensor configurations
#if defined (SENSOR_PIR)
  /** PIR sensor pin. */
  #define SENSOR_PIR_PIN  7
#endif

/** @defgroup SENSOR_DHT DHT sensor group.
 *  @{
 */
 #if defined (SENSOR_DHT)
  /** DHT sensor type. */
  #define SENSOR_DHT_TYPE DHT11
  /** DHT sensor pin. */
  #define SENSOR_DHT_PIN  A1
#endif
/** @} */

// Functions declaration
#if defined (SENSOR_LDR)
  int getLDRValue(int samples, int interval);
  float getLDRValueinLux(int samples, int interval);
  float getTemperatureValue(int samples, int interval);
  float getHumidityValue(int samples, int interval);
#endif

// Global variables
DHT dht(SENSOR_DHT_PIN, SENSOR_DHT_TYPE); /*!< DHT global variable. */

/**
* @brief Setup function.
* @param none
* @return none
* @details Setup function used to system configuration.
*/
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

/**
* @brief Loop function for Arduino project.
* @param none
* @return none
* @details Main loop function.
*/
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
/**
* @brief Get the digital LDR value.
* @param [in] <sample> number of samples used.
* @param [in] <interval> interval between each sample.
* @return Media between all values sampled.
* @details Get the digital LDR value (0 -- 1023) converted from an
* analogic input and based on a 10 bits ADC.
*/
int getLDRValue(int samples, int interval) {
  unsigned int value = 0;
  for (int i = 0; i < samples; i++) {
    value += analogRead(SENSOR_LDR_PIN);
    delay(interval);
  }
  return value/samples;
}

/**
* @brief Get the LDR value (in LUX).
* @param [in] <sample> number of samples used.
* @param [in] <interval> interval between each sample.
* @return Media between all values sampled.
* @details Get the LDR value in LUX scale.
*/
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
 /**
 * @brief Get the temperature.
 * @param [in] <sample> number of samples used.
 * @param [in] <interval> interval between each sample.
 * @return Media between all values sampled.
 * @details Get the temperature value (in Celsius) from DHT sensor.
 */
float getTemperatureValue(int samples, int interval) {
  float value = 0;
  for (int i = 0; i < samples; i++) {
    value += dht.readTemperature();
    delay(interval);
  }
  return value/samples;
}

/**
* @brief Get the humidity.
* @param [in] <sample> number of samples used.
* @param [in] <interval> interval between each sample.
* @return Media between all values sampled.
* @details Get the humidity value (in %) from DHT sensor.
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
