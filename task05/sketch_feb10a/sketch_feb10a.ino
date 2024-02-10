#include <PID_v1.h>

#define NUMSAMPLES 5
#define THERMISTOR_PIN A0
#define R1 100000
#define NUM_MOSFETS 5
#define NUM_REGULATORS 5
#define CURRENT_SENSOR_PIN A6
#define BATTERY_PLUS_PIN A1
#define BATTERY_MINUS_PIN A2
#define BATTERY_MIDDLE_PIN A3

// Struktura za predstavljanje NTC senzora
struct NTC {
  int pin;
  float c1, c2, c3;
};

// Struktura za predstavljanje uređaja (MOSFET ili regulator)
struct Device {
  int pin;
};

// Bolja enkapsulacija
class Controller {
private:
  double setpoint;
  double input, output;
  PID pid;
  NTC thermistor;
  NTC battery_plus;
  NTC battery_minus;
  NTC battery_middle;
  Device mosfets[NUM_MOSFETS];
  Device regulators[NUM_REGULATORS];
public:
  Controller(double Kp, double Ki, double Kd, double _setpoint, Device _mosfets[], Device _regulators[])
    : setpoint(_setpoint), pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT) {
    memcpy(mosfets, _mosfets, sizeof(mosfets));
    memcpy(regulators, _regulators, sizeof(regulators));
  }

  void setup() {
    Serial.begin(9600);
    pinMode(CURRENT_SENSOR_PIN, INPUT);
    pinMode(BATTERY_PLUS_PIN, INPUT);
    pinMode(BATTERY_MINUS_PIN, INPUT);
    pinMode(BATTERY_MIDDLE_PIN, INPUT);
    for (int i = 0; i < NUM_MOSFETS; i++) {
      pinMode(mosfets[i].pin, OUTPUT);
    }
    for (int i = 0; i < NUM_REGULATORS; i++) {
      pinMode(regulators[i].pin, OUTPUT);
    }
    pid.SetMode(AUTOMATIC);
  }

  float readTemperatureSum(byte samples, NTC sensor) {
    float sum = 0;
    for (int i = 0; i < samples; i++) {
      float Vo = analogRead(sensor.pin);
      float R2 = R1 * (1023.0 / Vo - 1.0);
      float logR2 = log(R2);
      float celsius = (1.0 / (sensor.c1 + sensor.c2 * logR2 + sensor.c3 * logR2 * logR2 * logR2)) - 273.15;
      sum += celsius;
      delay(10);
    }
    return sum / samples;
  }

  void loop() {
    float averageTemperature = readTemperatureSum(NUMSAMPLES, thermistor);
    float batteryPlusVoltage = analogRead(BATTERY_PLUS_PIN) * (5.0 / 1023.0);
    float batteryMinusVoltage = analogRead(BATTERY_MINUS_PIN) * (5.0 / 1023.0);
    float batteryMiddleVoltage = analogRead(BATTERY_MIDDLE_PIN) * (5.0 / 1023.0);

    input = analogRead(CURRENT_SENSOR_PIN);
    pid.Compute();
    for (int i = 0; i < NUM_MOSFETS; i++) {
      digitalWrite(mosfets[i].pin, output > 0 ? HIGH : LOW);
    }
    for (int i = 0; i < NUM_REGULATORS; i++) {
      analogWrite(regulators[i].pin, 128);
    }
    Serial.print("Temperature (Thermistor): ");
    Serial.print(averageTemperature);
    Serial.println(" °C");

    Serial.print("Battery Plus Voltage: ");
    Serial.print(batteryPlusVoltage, 2);
    Serial.println(" V");

    Serial.print("Battery Minus Voltage: ");
    Serial.print(batteryMinusVoltage, 2);
    Serial.println(" V");

    Serial.print("Battery Middle Voltage: ");
    Serial.print(batteryMiddleVoltage, 2);
    Serial.println(" V");

    delay(1000);
  }
};

NTC thermistor = { THERMISTOR_PIN, 1.009249522e-03, 2.378405444e-04, 2.019202697e-07 };

Device mosfets[NUM_MOSFETS] = { { 9 }, { 10 }, { 11 }, { 12 }, { 13 } };
Device regulators[NUM_REGULATORS] = { { 2 }, { 3 }, { 4 }, { 5 }, { 6 } };

double Kp = 2, Ki = 5, Kd = 1;
double setpoint = 512.0;

Controller controller(Kp, Ki, Kd, setpoint, mosfets, regulators);

void setup() {
  controller.setup();
}

void loop() {
  controller.loop();
}
