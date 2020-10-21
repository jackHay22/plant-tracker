/*
 * (C) 2020 Jack Hay
 */
 #include <ESP8266HTTPClient.h>
 #include <ESP8266WiFi.h>

 #define SSID "ssid"
 #define PSWD "password"

void setup() {
  Serial.begin(9600);
  WiFi.begin(SSID,PSWD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to " + SSID + "...");
    delay(500);
  }
}

void loop() {

  /*
   * Get the reading from the sensor
   */
  double value = (double) analogRead(A0);

  Serial.println(value);
  
  HTTPClient http;

  http.begin("http://10.0.0.149/api/v1/push");
  http.addHeader("Content-Type","application/json");

  int status = http.POST("{\"name\":\"lemon_tree\",\"family\":\"soil_moisture\",\"value\": " + String(value) + "}");

  Serial.print("Status: ");
  Serial.println(status);

  //delay for 10 seconds
  delay(10000);
}
