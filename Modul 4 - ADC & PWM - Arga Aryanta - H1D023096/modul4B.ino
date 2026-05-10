// Definisi Pin
const int POT_PIN = A0;  // Pin analog untuk potensiometer
const int LED_PIN = 11;  // Pin digital PWM untuk LED (misal pin 11)

void setup() {
  pinMode(LED_PIN, OUTPUT); // Mengatur pin LED sebagai output
  Serial.begin(9600);       // Inisialisasi komunikasi serial
}

void loop() {
  // Membaca nilai analog dari potensiometer (0 - 1023)
  int adcValue = analogRead(POT_PIN);
  
  // Mengonversi nilai ADC (0-1023) ke rentang PWM (0-255)
  // Berdasarkan Dasar Teori PWM pada modul 
  int pwmValue = map(adcValue, 0, 1023, 0, 255);
  
  // Mengatur intensitas cahaya LED menggunakan PWM
  analogWrite(LED_PIN, pwmValue);
  
  // Menampilkan data ke Serial Monitor
  Serial.print("Input ADC: ");
  Serial.print(adcValue);
  Serial.print(" -> Output PWM: ");
  Serial.println(pwmValue);
  
  delay(10); // Delay singkat untuk responsivitas sistem
}