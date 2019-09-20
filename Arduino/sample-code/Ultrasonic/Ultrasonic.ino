#define ECHO 13
#define TRIG 12

int counter = 0;

void setup() {
  Serial.begin (9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  long duration, distance;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance < 20) {
    counter = counter + 1;
  }

  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
  
  Serial.print("#People : ");
  Serial.print(counter);
  Serial.println(" people");
  delay(500);
}
