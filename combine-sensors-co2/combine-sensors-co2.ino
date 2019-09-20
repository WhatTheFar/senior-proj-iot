// Sensors :
// CO2 Sensor --> CO2(ppm)

// CO2 Define
#include <Wire.h>
#include <ZG09.h>
ZG09 zg09 = ZG09();
float co2,CO2Result;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  zg09.begin();
  for (int i = 0; i < 3; i++) {
    co2 = zg09.readData();
    delay(100);
  }
}

void loop() {

  CO2Result = CO2Reader();
  Serial.print("CO2: "); Serial.print(CO2Result); Serial.println("ppm");
  delay(1000);

}


float CO2Reader() {
  int ret = zg09.readData();
  if (ret) {
    co2 = zg09.getCo2();
  }
  return co2;
}
