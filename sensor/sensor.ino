/*
 * (C) 2020 Jack Hay
 */
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <dht.h>

#define AMAX 1023
#define ANALOG0 A0
#define DIGITAL7 7

dht DHT;

void setup() {
  WiFi.begin("ssid","pswd");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {

  /*
   * Get the soil moisture reading from the sensor
   */
  double value = AMAX - (double) analogRead(ANALOG0);

  HTTPClient http;

  http.begin("http://10.0.0.149/api/v1/push");
  http.addHeader("Content-Type","application/json");

  http.POST("{\"name\":\"lemon_tree\",\"family\":\"soil_moisture\",\"value\": " + String(value) + "}");

  /*
   * Get the temperature and humidity readings
   */
  DHT.read11(DHT11_PIN);

  /*
   * Post the temperature
   */

  http.begin("http://10.0.0.149/api/v1/push");
  http.addHeader("Content-Type","application/json");

  http.POST("{\"name\":\"lemon_tree\",\"family\":\"temp\",\"value\": " + String(DHT.temperature) + "}");

  /*
   * Post the humidity
   */

  http.begin("http://10.0.0.149/api/v1/push");
  http.addHeader("Content-Type","application/json");

  http.POST("{\"name\":\"lemon_tree\",\"family\":\"humidity\",\"value\": " + String(DHT.humidity) + "}");

  //delay for 60 seconds
  delay(60000);
}
