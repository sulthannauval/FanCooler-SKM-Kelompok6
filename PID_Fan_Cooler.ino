#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN 7
#define DHTTYPE DHT11

LiquidCrystal lcd(13, 10, 5, 4, 3, 2);

const int fan = 6;
int motorSpeed = 0;

// PID Constants
double Kp = 100;  // Proportional gain
double Ki = 0.01; // Integral gain
double Kd = 10;   // Derivative gain

double setpoint = 30.0; // Set Point untuk Temperatur
double error, lastError, integral, derivative, PIDValue;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(fan, OUTPUT);
  dht.begin();
}

void loop() {
  float currentTemperature = dht.readTemperature();

  // Kalkulasi PID
  error = currentTemperature - setpoint;
  integral += error;
  derivative = error - lastError;
  PIDValue = Kp * error + Ki * integral + Kd * derivative;

  // Batas pada Kecepatan Kipas
  // motorSpeed = map(motorSpeed, setpoint, 100, 0, 255);
  motorSpeed = constrain(PIDValue, 0, 255);

  // Update Kecepatan Kipas
  analogWrite(fan, motorSpeed);

  // Update Error untuk Iterasi Selanjutnya
  lastError = error;

  // Display Serial Monitor
  Serial.print(currentTemperature);
  Serial.print(" ");
  Serial.println(motorSpeed);

  // Display LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(currentTemperature);
  lcd.print(" C  ");

  lcd.setCursor(0, 1);
  lcd.print("Fan Speed: ");
  lcd.print(motorSpeed);
  lcd.print("   ");

  delay(200); // Delay untuk jeda pembacaan
}