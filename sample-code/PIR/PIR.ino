int inputPin = D6;

void setup() {
  pinMode(inputPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int value = digitalRead(inputPin);
  Serial.println(value);
  delay(1000);
}
