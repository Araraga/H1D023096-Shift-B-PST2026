# Jawaban Pertanyaan Percabangan

## 1. Kondisi Program Masuk ke Blok `if`

Program akan masuk dan mengeksekusi instruksi di dalam blok `if` apabila kondisi evaluasi persyaratan bernilai benar (_true_). [cite_start]Berdasarkan skrip kode percobaan, mikrokontroler akan mengeksekusi blok ini secara spesifik ketika nilai pada variabel `timeDelay` kurang dari atau sama dengan 100[cite: 92].

## 2. Kondisi Program Masuk ke Blok `else`

[cite_start]Program akan beralih dan mengeksekusi instruksi di dalam blok `else` apabila kondisi utama yang diuji pada blok `if` tidak terpenuhi atau bernilai salah (_false_)[cite: 92, 95]. Dalam konteks operasional kode percobaan, blok `else` dieksekusi secara berulang selama nilai variabel `timeDelay` masih lebih besar dari angka 100.

## 3. Fungsi Perintah `delay(timeDelay)`

[cite_start]Perintah `delay(timeDelay)` berfungsi untuk memberikan penundaan (jeda waktu) pada eksekusi siklus instruksi mikrokontroler[cite: 87, 90]. Durasi penundaan ini bersifat dinamis, bergantung pada nilai numerik yang tersimpan di dalam variabel `timeDelay` (dalam satuan milidetik). [cite_start]Fungsi ini esensial untuk menahan _state_ fisik pin digital, sehingga LED dapat dipertahankan dalam kondisi menyala (`HIGH`) atau mati (`LOW`) cukup lama agar efek visualnya dapat diamati oleh mata manusia [cite: 86-90].

---

## 4. Modifikasi Alur Percabangan: Cepat → Sedang → Mati

Program berikut merupakan modifikasi dari logika sistem dasar. Alur kontrol disesuaikan melalui percabangan bertingkat agar durasi kedipan dimulai dari fase **Cepat**, bertransisi ke fase **Sedang**, lalu menahan status instrumen pada fase **Mati** secara penuh sebelum siklus diulang kembali secara otomatis.

### Kode Program dan Penjelasan Detail

```cpp
/*
  Modifikasi Program Percabangan LED (Cepat -> Sedang -> Mati)
  Deskripsi: Mengontrol LED pada pin 5 untuk berkedip dengan siklus waktu
             yang berubah secara berurutan sesuai fase transisi logika.
*/

// --- INISIALISASI PARAMETER AWAL ---
const int ledPin = 5;      // Mendefinisikan pin digital 5 sebagai jalur output ke komponen LED
int timeDelay = 100;       // Mengatur nilai jeda waktu awal sebesar 100 ms (Memulai sistem di fase 'Cepat')

void setup() {
  pinMode(ledPin, OUTPUT); // Mengonfigurasi pin 5 agar beroperasi dalam mode keluaran sinyal (OUTPUT)
}

void loop() {
  // --- BLOK KONTROL VISUALISASI FISIK ---

  digitalWrite(ledPin, HIGH); // Mengirimkan tegangan positif (5V) untuk menyalakan LED
  delay(timeDelay);           // Menahan kondisi LED menyala selama durasi variabel timeDelay saat ini

  digitalWrite(ledPin, LOW);  // Menghentikan tegangan (0V) untuk mematikan LED
  delay(timeDelay);           // Menahan kondisi LED mati selama durasi variabel timeDelay saat ini

  // --- BLOK EVALUASI LOGIKA PERCABANGAN BERTINGKAT ---

  // Evaluasi Kondisi 1: Apakah sistem saat ini berada di fase 'Cepat'?
  if (timeDelay == 100) {
    // Jika benar, perbarui parameter waktu menjadi 500 ms
    // Hal ini akan memperlambat kedipan LED menjadi 'Sedang' pada siklus loop berikutnya
    timeDelay = 500;
  }

  // Evaluasi Kondisi 2: Apakah sistem saat ini berada di fase 'Sedang'?
  else if (timeDelay == 500) {
    // Jika benar, paksa LED masuk ke fase 'Mati'
    digitalWrite(ledPin, LOW); // Memastikan LED dalam keadaan mati penuh tanpa sisa sinyal HIGH
    delay(4000);              // Menghentikan seluruh eksekusi loop selama 4000 ms (4 detik)

    // Setelah masa jeda 'Mati' selesai, kembalikan parameter ke 100 ms
    // Hal ini mereset sistem untuk mengulang kembali kedipan 'Cepat'
    timeDelay = 100;
  }
}
```
