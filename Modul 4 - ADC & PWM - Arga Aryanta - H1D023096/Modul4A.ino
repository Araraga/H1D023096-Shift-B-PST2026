#include <Servo.h>

// Definisi Pin
const int POT_PIN = A0;  // Pin analog untuk potensiometer
const int SERVO_PIN = 9; // Pin digital PWM untuk servo

Servo myservo; // Membuat objek servo

void setup() {
  myservo.attach(SERVO_PIN); // Menghubungkan objek servo ke pin 9
  Serial.begin(9600);        // Inisialisasi komunikasi serial
}

void loop() {
  // Membaca nilai analog dari potensiometer (0 - 1023)
  int adcValue = analogRead(POT_PIN);
  
  // Mengonversi nilai ADC (0-1023) ke sudut servo (0-180)
  int angle = map(adcValue, 0, 1023, 0, 180);
  
  // Menggerakkan servo ke sudut yang telah dikonversi
  myservo.write(angle);
  
  // Menampilkan data ke Serial Monitor untuk pemantauan
  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" | Sudut Servo: ");
  Serial.println(angle);
  
  delay(15); // Delay singkat untuk stabilitas gerakan servo
}