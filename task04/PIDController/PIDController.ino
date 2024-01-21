#include <PID_v1.h>
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
double setpoint = 25.0; // zeljena temperatura u stupnjevima Celzijusa
double input, output;
//triba sve ovo nastimati
double Kp = 20.0, Ki = 0.2, Kd = 0.1; 


PID myPID(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC);
  //izlazni pin za kontrolu mosfeta
  pinMode(mosfetPin, OUTPUT);

}

void loop() {
  Vo = analogRead(thermistorPin);
  R2 = R1 * (1023.0 / Vo - 1.0); 
  logR2 = log(R2);
  celsius = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)) - 273.15;

  input = celsius;

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


