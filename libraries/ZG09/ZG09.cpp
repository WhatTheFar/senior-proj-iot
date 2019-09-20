#include <Arduino.h>
#include <ZG09.h>
#include <Wire.h>

ZG09::ZG09()
{

}

void ZG09::begin()
{
  //set_register(0x69,0x01,0x06);
}

bool ZG09::readData()
{
  int data_count = 0;

  //RecieveDataI2c(0x69, 0x02, &_buf[2], 1); // Read Status
  //if((_buf[2] & 0x80) == 0)  // BUSY時はデータが化ける為取り込まない様対策
  //{
    data_count = RecieveDataI2c(0x68, 0x03, &_buf[0], 2); // Read DAL
    //data_count += RecieveDataI2c(0x69, 0x04, &_buf[1], 1); // Read DAH
    _co2ppm = (_buf[1] << 8)| _buf[0];
  //}
  if(data_count == 2) return true;

  return(false);
}
char ZG09::getcalibration()
{
  char buf[0];
  RecieveDataI2c(0x69, 0x02, &buf[0], 1); // Read Status
  return(buf[0]);

}

void ZG09::calibration(char cal)
{
  set_register(0x69,0x12,cal);
  set_register(0x69,0x0e,1);

}

void ZG09::initWait()
{

}

bool ZG09::getInitEndStatus()
{

}

int ZG09::getCo2()
{
  return _co2ppm;
}

int ZG09::getTemperature()
{

}

void ZG09::set_register(int address, unsigned char r, unsigned char v)
{    
  //Serial.println("a");
  Wire.beginTransmission(address);    
  //Serial.println("b");
  Wire.write(r);   
  //Serial.println("c");
  Wire.write(v);   
  //Serial.println("d");
  Wire.endTransmission();
  //Serial.println("e");
}

int ZG09::RecieveDataI2c(int address, unsigned char reg, char* data, int size)
{
  int i = 0;
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();    // stop transmitting
  Wire.requestFrom(address,size);

  while(Wire.available()){
    data[i] = Wire.read();
    i++;
  }
  return i;
}
