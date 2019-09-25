// Sensors :
// 1. SHT31               --> Humidity(%) and Temperature(*C)
// 2. LDR Photosensitive  --> Illuminance(Lux)

// Library
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> //Version 5
#include <math.h>
#include <MicroGear.h>

const char* ssid = "true_home2G_Up7";
const char* password = "vDcqdQQq";

// Device No.
const int DEVICENO = 1;

// 1. SHT Define
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// 2. LDR Define
const int ANALOGPIN = A0;  // Set to any analog input pin

// Netpie Define
#define APPID "seniorproj"
#define KEY "6K6MhlDiz7v9LsY" // Device Key
#define SECRET "rnhMk2Mui528uoe1NPZTSLDfz"
#define ALIAS  "MultiSensor1"
#define TOPIC "/iot"
WiFiClient client;
MicroGear microgear(client);

// Global Variable
float temperature, humidity, light;

void setup() {
  Serial.begin(115200);

  // Microgear event listener
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(CONNECTED, onConnected);

  // Wifi setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  delay(1000);

  // Microgear init
  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);

  // 1. SHT Setup
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  // 2. LDR Setup
  pinMode (ANALOGPIN, INPUT);
}

void loop() {

  //  temperature = roundf(shtTempReader() * 1000) / 1000;
  //  humidity = roundf(shtHumReader() * 1000) / 1000;
  //  light = roundf(ldrReader() * 1000) / 1000;

  if (WiFi.status() == WL_CONNECTED) {

    // put your main code here, to run repeatedly:
    if (microgear.connected())
    {
      microgear.loop();
      Serial.println("connect...");
    }
    else
    {
      Serial.println("connection lost, reconnect...");
      microgear.connect(APPID);
    }
    delay(250);

    //    StaticJsonBuffer<256> jsonBuffer;
    //    JsonObject& data = jsonBuffer.createObject();
    //    //    data["timestamp"] = "10:00:00";
    //    data["temp"] = temperature;
    //    data["hum"] = humidity;
    //    data["light"] = light;
    //
    //    char JSONmessageBuffer[256];
    //    data.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    //    Serial.println(JSONmessageBuffer);
    //
    //    HTTPClient http;
    //    http.begin("http://192.168.1.34:3000/test"); //destination
    //    http.addHeader("Content-Type" , "application/x-www-form-urlencoded"); // content-type, header
    //    int httpResponseCode = http.POST(JSONmessageBuffer);
    //    if (httpResponseCode > 0) {
    //      String response = http.getString();
    //    } else {
    //      Serial.print("Error on sending POST:  ");
    //      Serial.print(httpResponseCode);
    //    }
    //    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
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

// Netpie Functions
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  Serial.print(topic);
  Serial.print(" : ");
  char strState[msglen];
  for (int i = 0; i < msglen; i++)
  {
    strState[i] = (char)msg[i];
    Serial.print((char)msg[i]);
  }
  Serial.println();

  // stateStr = ISO Time (String)
  String stateStr = String(strState).substring(0, msglen);
  postRequest(stateStr);

  //  Serial.println(stateStr);
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
  microgear.subscribe(TOPIC);
}

void postRequest(String date) {
  temperature = roundf(shtTempReader() * 1000) / 1000;
  humidity = roundf(shtHumReader() * 1000) / 1000;
  light = roundf(ldrReader() * 1000) / 1000;

  StaticJsonBuffer<256> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  data["date"] = date;
  data["device"] = DEVICENO;
  data["temp"] = temperature;
  data["hum"] = humidity;
  data["light"] = light;

  char JSONmessageBuffer[256];
  data.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  HTTPClient http;
  http.begin("http://192.168.1.34:3000/iot/sensor/multi"); //destination
  http.addHeader("Content-Type" , "application/json"); // content-type, header
  int httpResponseCode = http.POST(JSONmessageBuffer);
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST:  ");
    Serial.print(httpResponseCode);
  }
  http.end();
}
