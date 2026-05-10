# Jawaban Pertanyaan Praktikum Modul 4: ADC

---

## 1. Fungsi Perintah `analogRead()` pada Rangkaian Praktikum

[cite_start]Fungsi `analogRead()` pada rangkaian praktikum ini bertugas untuk membaca nilai tegangan analog variabel yang dikirimkan oleh komponen potensiometer[cite: 454, 455]. [cite_start]Karena inti pemrosesan digital mikrokontroler tidak dapat mengevaluasi sinyal analog secara langsung, fungsi ini menginstruksikan modul _Analog to Digital Converter_ (ADC) internal untuk melakukan konversi[cite: 453]. [cite_start]Konversi tersebut mengubah sinyal tegangan yang bersifat kontinu menjadi besaran data digital berbentuk bilangan bulat dengan rentang 0 hingga 1023 (representasi resolusi 10-bit mikrokontroler) yang selanjutnya diproses untuk menentukan parameter pergerakan aktuator[cite: 453, 457].

---

## 2. Rasionalisasi Penggunaan Fungsi `map()`

Fungsi `map()` mutlak diperlukan untuk mengeksekusi penskalaan proporsi atau interpolasi linear dari satu rentang nilai data menuju rentang nilai yang berbeda. Dalam arsitektur sistem kendali pada praktikum ini, terdapat ketidaksesuaian resolusi antara perangkat masukan dan perangkat keluaran.

Potensiometer (melalui fungsi ADC) mengembalikan nilai masukan dengan jangkauan 0 hingga 1023. Di sisi lain, _library_ standar pengontrol motor servo mengekspektasikan nilai instruksi posisi sudut yang berada pada rentang batas mekanisnya, yakni umumnya 0 hingga 180 derajat. Fungsi `map()` bertindak sebagai jembatan untuk mentranslasikan nilai 0–1023 secara presisi agar sejajar dengan rentang 0–180, sehingga putaran fisik dari ujung servo ekuivalen dan selaras dengan besaran putaran tuas potensiometer.

---

## 3. Modifikasi Program Servo (Rentang Terbatas 30° – 150°)

Berikut adalah modifikasi perangkat lunak sistem tertanam dan dokumentasi teknisnya dalam format _README.md_.

```cpp
/* Program Modifikasi Pengendalian Servo (Rentang Sudut Terbatas)
   Target: Pembatasan rentang mekanis pada 30° hingga 150°
*/

#include <Servo.h>

Servo motorServo;         // Mendeklarasikan objek motor servo
const int PIN_POT = A0;   // Menetapkan pin analog masukan (Potensiometer)
const int PIN_SERVO = 9;  // Menetapkan pin PWM keluaran (Motor Servo)

void setup() {
  Serial.begin(9600);           // Inisialisasi komunikasi serial untuk monitoring
  motorServo.attach(PIN_SERVO); // Mengaitkan objek servo pada pin digital 9

  Serial.println("Sistem Kontrol Servo Berjalan (Batas: 30-150 Derajat)");
}

void loop() {
  // Membaca resolusi 10-bit tegangan potensiometer (Rentang 0 - 1023)
  int nilaiADC = analogRead(PIN_POT);

  // Penskalaan nilai ADC ke rentang sudut yang direstriksi (30 - 150)
  int sudutServo = map(nilaiADC, 0, 1023, 30, 150);

  // Menerapkan instruksi rotasi fisik pada motor servo
  motorServo.write(sudutServo);

  // Visualisasi pemantauan data pada Serial Monitor
  Serial.print("Nilai ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | Posisi Sudut: ");
  Serial.print(sudutServo);
  Serial.println("°");

  // Memberikan jeda waktu minimum agar mekanisme gear servo mencapai posisi target
  delay(15);
}
```
