/*
 * (C) 2020 Jack Hay
 */
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define AMAX 1023

void setup() {
  WiFi.begin("ssid","pswd");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {

  /*
   * Get the reading from the sensor
   */
  double value = AMAX - (double) analogRead(A0);

  HTTPClient http;

  http.begin("http://10.0.0.149/api/v1/push");
  http.addHeader("Content-Type","application/json");

  int status = http.POST("{\"name\":\"lemon_tree\",\"family\":\"soil_moisture\",\"value\": " + String(value) + "}");

  //delay for 10 seconds
  delay(10000);
}
