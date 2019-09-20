/*
Photoresistor Light Sensor Module Test

Basic code for reading the analog output of the light sensor module
*/
const int ANALOGPIN = A0;  // Set to any analog input pin
int rawValue = 0;          // Returned ADC reading from sensor
float lux=0.00,ADC_value=0.0048828125;

//===============================================================================
//  Initialization
//===============================================================================
void setup() 
{ 
  pinMode (ANALOGPIN, INPUT);
  Serial.begin(9600);         // Set comm speed for debug window messages
}

//===============================================================================
//  Main
//===============================================================================
void loop() 
{
  rawValue = analogRead (ANALOGPIN);  // Read sensor input
  lux=(250.000000/(ADC_value*rawValue))-50.000000;
  Serial.print ("Reading Raw:");
  Serial.print (rawValue);
  Serial.print (" = ");
  Serial.print (lux);
  Serial.println (" LUX.");
  delay (1000);
}
