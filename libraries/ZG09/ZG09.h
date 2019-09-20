
#ifndef _ZG09_H_
#define _ZG09_H_

//#include <WioLTEforArduino.h>

class ZG09 {
public:

  ZG09();
  void begin();
  bool readData();
  void calibration(char buf);
  void initWait();
  bool getInitEndStatus();
  int getCo2();
  int getTemperature();
  char getcalibration();

private:
  void set_register(int address, unsigned char r, unsigned char v);
  int RecieveDataI2c(int address, unsigned char reg, char* data, int size);
  char _buf[9];
  int _co2ppm;
  int _temperature;
};

#endif
