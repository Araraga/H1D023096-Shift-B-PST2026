#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinPot = A0;
const int LED_PIN = 7;

void setup() {
  Serial.begin(9600);
  lcd.init();     
  lcd.backlight();  
  PinMode(LED_PIN_OUTPUT)
}

void loop() {
  int nilai = analogRead(pinPot);

  //mulai ada bar ketika membaca nilai mulai dari 100 (putaran sekian derajat)
  int panjangBar = map(nilai, 100, 1023, 0, 16);
  
  //membaca adc mulai dari 100 (putaran sekian derajat) hingga penuh
  int kecerahan = map(adcValue, 100, 1023, 0, 255);

  analogWrite(LED_PIN, kecerahan);

  Serial.print("Nilai ADC: ");
  Serial.println(nilai);

  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  if (kecerahan > 0){
    lcd.print("LED Nyala");
  } else {
    lcd.print("LED Mati");
  }

  lcd.setCursor(0, 2);
  lcd.print("Kecerahan LED: ");
  lcd.print(nilai);
  lcd.print("    ");

  lcd.setCursor(0, 3);
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255);
    } else {
      lcd.print(" ");
    }
  }

  delay(200);
}