# Jawaban Pertanyaan Praktikum Modul 3: Protokol Komunikasi (UART)

---

## 1. Analisis Proses Transmisi Data: Input Keyboard hingga Aktivasi LED

Proses komunikasi data dari terminal komputer hingga menghasilkan respon fisik pada LED mengikuti tahapan berikut:

1. **Input Pengguna:** Pengguna mengetikkan karakter (misalnya '1' atau '0') pada kolom input _Serial Monitor_ di Arduino IDE.
2. **Transmisi Serial (UART):** Karakter tersebut dikonversi menjadi data biner berdasarkan standar kode ASCII. Data dikirimkan secara serial (bit per bit) melalui kabel USB menggunakan protokol UART (_Universal Asynchronous Receiver-Transmitter_).
3. **Penerimaan pada Buffer:** Data biner diterima oleh chip komunikasi pada Arduino Uno dan disimpan sementara di dalam _RX Buffer_ (memori penampung penerimaan).
4. **Eksekusi Program:** Fungsi `Serial.read()` mengambil data dari _buffer_ tersebut dan menyimpannya ke dalam variabel (misalnya variabel `data`).
5. **Logika Pengondisian:** Mikrokontroler melakukan perbandingan nilai variabel melalui struktur `if-else`. Jika variabel berisi karakter '1', instruksi `digitalWrite(PIN_LED, HIGH)` dieksekusi.
6. **Output Fisik:** Pin digital yang ditentukan (Pin 8) mengeluarkan tegangan sebesar 5V, sehingga arus mengalir melalui LED dan menyebabkan LED menyala. Proses yang sama berlaku untuk instruksi mematikan LED dengan sinyal `LOW` (0V).

---

## 2. Fungsi dan Signifikansi `Serial.available()`

**Kegunaan:**
Fungsi `Serial.available()` digunakan untuk memeriksa apakah terdapat data yang telah masuk dan tersimpan di dalam _buffer_ penerimaan serial. Fungsi ini mengembalikan nilai integer yang menunjukkan jumlah _byte_ data yang siap untuk dibaca.

**Analisis Jika Baris Dihilangkan:**
Apabila baris `if (Serial.available() > 0)` dihilangkan, maka fungsi `Serial.read()` akan terus dieksekusi pada setiap iterasi perulangan `loop()`, tanpa mempedulikan apakah ada data yang masuk atau tidak. Hal ini mengakibatkan:

- **Nilai Kembalian Tidak Valid:** Jika _buffer_ kosong, `Serial.read()` akan mengembalikan nilai -1.
- **Instruksi Berulang yang Membebani:** Program mungkin akan terus menjalankan logika "Perintah tidak dikenal" secara berulang-ulang karena variabel terus diisi dengan nilai kosong, sehingga membebani kinerja prosesor dan mengacaukan tampilan pada _Serial Monitor_.

---

## 3. Modifikasi Program: Fitur LED Berkedip (Input '2')

Program di bawah ini telah dimodifikasi agar LED dapat berkedip secara kontinu ketika menerima input '2'. Status berkedip akan dipertahankan hingga terdapat perintah baru ('0' atau '1'). Penjelasan fungsi baris kode tersedia dalam komentar program.

```cpp
/* Program Kontrol LED via UART (UART Control with Persistent Blink)
   Target: Pin 8 (LED) sesuai Tabel 3.1 Modul
*/

#include <Arduino.h>

const int PIN_LED = 8;       // Menetapkan pin 8 sebagai jalur output LED
char statusPerintah = '0';   // Variabel global untuk menyimpan status terakhir
unsigned long waktuLalu = 0; // Menyimpan waktu terakhir LED berubah status
bool ledState = LOW;         // Menyimpan status logika LED (Nyala/Mati)
const int interval = 500;    // Interval kedipan dalam milidetik

void setup() {
  Serial.begin(9600);       // Memulai komunikasi serial pada kecepatan 9600 bps
  pinMode(PIN_LED, OUTPUT); // Mengonfigurasi pin 8 sebagai OUTPUT

  Serial.println("Kontrol LED Ready:");
  Serial.println("1: ON | 0: OFF | 2: BLINK");
}

void loop() {
  // Memeriksa keberadaan data baru di buffer serial
  if (Serial.available() > 0) {
    statusPerintah = Serial.read(); // Memperbarui status berdasarkan input baru
  }

  // Eksekusi Logika berdasarkan status terakhir yang diterima
  if (statusPerintah == '1') {
    digitalWrite(PIN_LED, HIGH);   // LED Menyala menetap
  }
  else if (statusPerintah == '0') {
    digitalWrite(PIN_LED, LOW);    // LED Mati menetap
  }
  else if (statusPerintah == '2') {
    // Implementasi Blink Non-Blocking menggunakan millis()
    unsigned long waktuSekarang = millis();

    if (waktuSekarang - waktuLalu >= interval) {
      waktuLalu = waktuSekarang;   // Memperbarui referensi waktu
      ledState = !ledState;        // Membalikkan status LED (Toggle)
      digitalWrite(PIN_LED, ledState); // Menerapkan status logika yang baru
    }
  }
}
```

---

## 4. Penentuan Metode Pewaktuan: `delay()` vs `millis()`

**Keputusan:** Dalam sistem kontrol interaktif seperti ini, penggunaan fungsi **`millis()`** adalah metode yang lebih tepat dan diwajibkan dibandingkan `delay()`.

**Analisis Pengaruh terhadap Sistem:**

1. **Sifat `delay()` (Pemblokiran/Blocking):** Jika menggunakan `delay()`, mikrokontroler akan berhenti bekerja sepenuhnya selama durasi waktu tunggu yang ditetapkan. Hal ini mengakibatkan mikrokontroler kehilangan kemampuan untuk membaca input baru dari _Serial Monitor_ saat proses jeda berlangsung. Jika pengguna mengirimkan perintah penghentian ('0') saat sistem sedang menahan nyala LED dalam fungsi `delay()`, respon sistem akan terlambat atau instruksi tersebut gagal terdeteksi seluruhnya.
2. **Sifat `millis()` (Non-blocking):** Fungsi `millis()` bekerja dengan memantau waktu internal sejak program dijalankan tanpa menghentikan eksekusi kode di bawahnya. Hal ini memungkinkan mikrokontroler untuk melakukan pencatatan waktu secara asinkron. Dengan demikian, sistem tetap dapat memeriksa masuknya input serial baru pada setiap siklus `loop()`, menjamin papan mikrokontroler tetap responsif terhadap perintah interupsi seketika meskipun mode _blink_ sedang berjalan.
