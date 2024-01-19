//analogni pin na arduinu
byte thermistorPin = A0;
//razina napona
float Vo;
//Triba bi bit NTC thermistor => otpor se smanjuje kako se temparatura dize,  otpor rezistora 100kilohm-a
float R1 = 100000;
//algoritam, stvarni otpor thermistora, varijabla za temparaturu
float logR2, R2, celsius;
//Steinhart-Hart => koeficijenti, ovisno o thermistoru => mogu biti drugaciji
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Vo = analogRead(thermistorPin);
  R2 = R1 * (1023.0 / Vo - 1.0); 
  logR2 = log(R2);
  celsius = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)) - 273.15;

  Serial.print("Temparatura: ");
  Serial.print(celsius);
  Serial.println(" C");
  //Serial.println(R2);
  delay(1000);
}

