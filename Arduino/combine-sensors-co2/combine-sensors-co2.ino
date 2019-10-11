// Sensors :
// CO2 Sensor --> CO2(ppm)

// Library
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> //Version 5
#include <math.h>
#include <MicroGear.h>

// NTP Server
#include <NTPClient.h>
#include <WiFiUdp.h>
const char *NTPServer = "1.th.pool.ntp.org";
int timeOffset = 0 * 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTPServer , timeOffset, 60000);

//const char* ssid = "true_home2G_Up7";
//const char* password = "vDcqdQQq";
const char* ssid = "Chula FIRST";
const char* password = "0825519444";

// Https request URL and Key
String httpsURL = "https://seniorproj.thinc.in.th/iot/sensor/co2";
const char *CAcert = "CC 42 E6 4C EB C9 3E 87 9B 66 E6 C5 D8 79 41 FE 12 AC D7 35";

// Device No.
const int DEVICENO = 5;

// CO2 Define
#include <Wire.h>
#include <ZG09.h>
ZG09 zg09 = ZG09();

float co2, CO2Result;
String NTPtime;

// Netpie Define
#define APPID "seniorproj"
#define KEY "6K6MhlDiz7v9LsY" // Device Key
#define SECRET "rnhMk2Mui528uoe1NPZTSLDfz"
#define ALIAS  "CO2Sensor"
#define TOPIC "/iot"
WiFiClient client;
MicroGear microgear(client);

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

  // NTP init
  timeClient.begin();

  // 1. CO2 Setup
  Wire.begin();
  zg09.begin();
  for (int i = 0; i < 3; i++) {
    co2 = zg09.readData();
    delay(100);
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
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
  } else {
    Serial.println("Error in WiFi connection");
    delay(250);
  }
}

float CO2Reader() {
  int ret = zg09.readData();
  if (ret) {
    co2 = zg09.getCo2();
  }
  return co2;
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
  CO2Result = CO2Reader();
  NTPtime = timeClient.getFormattedDate();

  Serial.print("NTP time : ");
  Serial.println(NTPtime);

  StaticJsonBuffer<256> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  data["date"] = date;
  data["actualDate"] = NTPtime;
  data["device"] = DEVICENO;
  data["co2"] = CO2Result;

  char JSONmessageBuffer[256];
  data.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  HTTPClient http;
  http.begin("https://seniorproj.thinc.in.th/iot/sensor/multi","CC 42 E6 4C EB C9 3E 87 9B 66 E6 C5 D8 79 41 FE 12 AC D7 35"); //destination
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
