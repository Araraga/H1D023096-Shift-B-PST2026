# Jawaban Pertanyaan Praktikum Modul 4: Pulse Width Modulation (PWM)

---

## 1. Mekanisme Pengaturan Kecerahan LED Melalui `analogWrite()`

Fungsi `analogWrite()` tidak mengeluarkan tegangan analog murni (seperti tegangan konstan 2.5V), melainkan mengimplementasikan teknik _Pulse Width Modulation_ (PWM). Fungsi ini mengontrol kecerahan LED dengan cara menghidupkan dan mematikan pin keluaran digital secara sangat cepat (sekitar 490 Hz hingga 980 Hz pada Arduino Uno).

Variabel yang dikendalikan adalah _duty cycle_ (siklus kerja), yaitu persentase durasi sinyal berada pada kondisi logika HIGH (5V) dibandingkan dengan total waktu satu periode gelombang. Jika fungsi diberikan nilai 127 (50% _duty cycle_), maka LED akan menyala setengah waktu dan mati setengah waktu secara bergantian. Karena frekuensi kedipan ini jauh melampaui batas kemampuan tangkap visual mata manusia (_persistence of vision_), mata mengartikan pergantian cepat tersebut sebagai nilai rata-rata tegangan efektif. Semakin besar nilai _duty cycle_, semakin tinggi tegangan rata-rata yang diterima LED, sehingga iluminasi cahaya yang dipancarkan akan terlihat semakin terang.

---

## 2. Hubungan Korelasi Antara Nilai ADC (0–1023) dan PWM (0–255)

Hubungan antara kedua metrik ini didasari oleh perbedaan resolusi perangkat keras komputasi pada mikrokontroler.

- **Nilai ADC (0-1023):** Modul penerima _Analog to Digital Converter_ pada Arduino Uno berbasis pada arsitektur 10-bit. Artinya, rentang tegangan masukan analog 0-5V dikonversi menjadi $2^{10}$ atau 1024 tingkatan nilai diskrit yang berbeda (0 hingga 1023).
- **Nilai PWM (0-255):** Register pembangkit sinyal PWM pada Arduino Uno merupakan perangkat keras 8-bit. Ini berarti siklus _duty cycle_ 0% hingga 100% hanya dapat direpresentasikan oleh $2^{8}$ atau 256 tingkatan (0 hingga 255).

Untuk menghubungkan data masukan ADC sebagai pengendali keluaran PWM, nilai 10-bit tersebut harus dipampatkan (_downscaled_) ke dalam resolusi 8-bit. Karena rasio perbandingannya adalah 1024 : 256 (atau 4 : 1), setiap perubahan 4 poin pada pembacaan nilai ADC ekuivalen dengan perubahan 1 poin pada nilai PWM. Penyesuaian matematis ini dieksekusi secara terstruktur melalui komputasi fungsi `map(nilaiADC, 0, 1023, 0, 255)` atau sekadar pembagian nilai `nilaiADC / 4`.

---

## 3. Modifikasi Program PWM (Rentang Kecerahan Terbatas 50–200)

Berikut adalah skrip modifikasi untuk mengendalikan LED pada rentang tingkat kecerahan sedang, beserta penjelasan yang diformat untuk dokumentasi _README.md_.

```cpp
/* Program Modifikasi Pengendalian LED (Rentang Kecerahan Terbatas)
   Target: Pembatasan rentang PWM aktif pada 50 hingga 200
*/

#include <Arduino.h>

const int PIN_POT = A0;   // Menetapkan pin analog masukan untuk potensiometer
const int PIN_LED = 9;    // Menetapkan pin PWM keluaran untuk LED

void setup() {
  Serial.begin(9600);        // Inisialisasi komunikasi serial
  pinMode(PIN_LED, OUTPUT);  // Konfigurasi pin LED sebagai aktuator keluaran

  Serial.println("Sistem Kontrol Kecerahan LED (Batas PWM: 50-200)");
}

void loop() {
  // Mengambil sampel tegangan analog dan menyimpannya dalam resolusi 10-bit
  int nilaiADC = analogRead(PIN_POT);

  // Memetakan resolusi ADC penuh ke rentang PWM yang direstriksi
  int nilaiPWM = map(nilaiADC, 0, 1023, 50, 200);

  // Mengaplikasikan nilai PWM yang telah diproses ke terminal LED
  analogWrite(PIN_LED, nilaiPWM);

  // Visualisasi parameter pemrosesan melalui antarmuka serial
  Serial.print("Data Masukan ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | Sinyal Keluaran PWM: ");
  Serial.println(nilaiPWM);

  // Interval stabilisasi pembacaan dan pemrosesan modulasi
  delay(20);
}
```
