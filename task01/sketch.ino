const byte led1Pin = 3;
const byte led2Pin = 5;

const int LEDOnTime = 500;
const int LEDOffTime1 = 1500;
const int LEDOffTime2 = 2500;

void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
}

void blinkLED(const byte pin, const int onTime, const int offTime) {
  digitalWrite(pin, HIGH);
  delay(onTime);
  digitalWrite(pin, LOW);
  delay(offTime);
}

void loop() {
  blinkLED(led1Pin, LEDOnTime, LEDOffTime1);
  blinkLED(led2Pin, LEDOnTime, LEDOffTime2);
}
