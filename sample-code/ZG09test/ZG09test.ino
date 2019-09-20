/***************************************************************************
 
 ***************************************************************************/

#include <Wire.h>
#include <ZG09.h>

ZG09 zg09 = ZG09();

uint16_t co2;

void setup() {
  int i;

  Wire.begin();
  Serial.begin(9600);
  Serial.println("in setup");
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
  
  Serial.println("ZG09 test");

  Serial.println("Initialize ZG09");
  zg09.begin();
  for (i = 0; i < 3;i++) {
    co2 = zg09.readData();
    delay(100);
 }
}

void loop() {
  int ret;

  ret = zg09.readData();
  if (ret) {
    co2 = zg09.getCo2();
    Serial.print("CO2: ");
    Serial.println(co2);
  }
  
  delay(1000);
}
