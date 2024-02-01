#include <PID_v1.h>
//za for petlju
#define NUMSAMPLES 5
//analogni pin na arduinu
byte thermistorPin = A0;
//digitalni pin za kontrolu MOSFET-a
byte mosfetPin = 9;
//razina napona
float Vo;
//Triba bi bit NTC thermistor => otpor se smanjuje kako se temparatura dize,  otpor rezistora 100kilohm-a
float R1 = 100000;
//algoritam, stvarni otpor thermistora, varijabla za temparaturu
float logR2, R2, celsius;
//Steinhart-Hart => koeficijenti, ovisno o thermistoru => mogu biti drugaciji
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// PID parameteri
//triba vidit do koliko dozvolit?
double setpoint = 80.0; // temperatura u stupnjevima Celzijusa do koje ce biti neki output jer je direct
double input, output;
//triba sve ovo nastimati
double Kp = 3.1, Ki = 0.1, Kd = 0.1; 

PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

float readTemperatureSum(byte samples) {
  float sum = 0;

  for (int i = 0; i < samples; i++) {
    Vo = analogRead(thermistorPin);
    R2 = R1 * (1023.0 / Vo - 1.0);
    logR2 = log(R2);
    celsius = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)) - 273.15;
    sum += celsius;
    delay(10);
  }
  return sum / samples;
}

void setup() {
  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC);
  //izlazni pin za kontrolu mosfeta
  pinMode(mosfetPin, OUTPUT);

}

void loop() {
   float averageTemperature = readTemperatureSum(NUMSAMPLES);

  //sigurnije malo
   input = averageTemperature;

  myPID.Compute();

  Serial.print("Temparatura: ");
  Serial.print(celsius);
  Serial.print(" C");
  Serial.print(", PID izlaz: ");
  Serial.println(output);
  // Upravljanje mosfetom s izlazom PID regulatora
  analogWrite(mosfetPin, output);
  //Serial.println(R2);
  delay(1000);
}