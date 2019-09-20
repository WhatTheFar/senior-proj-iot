// Sensors :
// 1. SHT31               --> Humidity(%) and Temperature(*C)
// 2. LDR Photosensitive  --> Illuminance(Lux)

// Library
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> //Version 5

const char* ssid = "true_home2G_Up7";
const char* password = "vDcqdQQq";

#include <math.h>

// 1. SHT Define
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// 2. LDR Define
const int ANALOGPIN = A0;  // Set to any analog input pin

// Global Variable
float temperature, humidity, light;

void setup() {
  Serial.begin(9600);

  // Wifi setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  delay(1000);

  // 1. SHT Setup
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  // 2. LDR Setup
  pinMode (ANALOGPIN, INPUT);
}

void loop() {

  temperature = roundf(shtTempReader() * 1000) / 1000;
  humidity = roundf(shtHumReader() * 1000) / 1000;
  light = roundf(ldrReader() * 1000) / 1000;

  //  Serial.print("Temperature = "); Serial.print(temperature); Serial.print(" *C");
  //  Serial.print(" ,Humidity = "); Serial.print(humidity); Serial.print(" %");
  //  Serial.print(" ,Light = "); Serial.print(light); Serial.println(" Lux");
  //  delay(1000);

  if (WiFi.status() == WL_CONNECTED) {

    StaticJsonBuffer<256> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();
    //    data["timestamp"] = "10:00:00";
    data["temp"] = temperature;
    data["hum"] = humidity;
    data["light"] = light;

    char JSONmessageBuffer[256];
    data.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);

    HTTPClient http;
    http.begin("http://192.168.1.34:3000/test"); //destination
    http.addHeader("Content-Type" , "application/x-www-form-urlencoded"); // content-type, header
    int httpResponseCode = http.POST(JSONmessageBuffer);
    if (httpResponseCode > 0) {
      String response = http.getString();
    } else {
      Serial.print("Error on sending POST:  ");
      Serial.print(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
  delay(3000);  // post
}


float shtTempReader() {
  float temp = sht31.readTemperature();
  return temp;
}


float shtHumReader() {
  float hum = sht31.readHumidity();
  return hum;
}

float ldrReader() {
  int rawValue = 0;          // Returned ADC reading from sensor
  float lux = 0.00, ADC_value = 0.0048828125;
  rawValue = analogRead (ANALOGPIN);  // Read sensor input
  lux = (250.000000 / (ADC_value * rawValue)) - 50.000000;
  return lux;
}
