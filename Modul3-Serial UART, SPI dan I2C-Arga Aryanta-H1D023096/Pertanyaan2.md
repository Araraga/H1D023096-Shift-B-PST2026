# Jawaban Pertanyaan Praktikum Modul 3: Inter-Integrated Circuit (I2C)

---

## 1. Analisis Cara Kerja Komunikasi I2C antara Arduino dan LCD

Komunikasi protokol _Inter-Integrated Circuit_ (I2C) pada rangkaian yang diuji coba beroperasi menggunakan arsitektur _Master-Slave_ yang tersinkronisasi. Berikut adalah urutan dan mekanisme kerjanya:

1. **Inisialisasi Jalur Komunikasi:** Mikrokontroler Arduino Uno bertindak sebagai pengontrol utama (_Master_), sedangkan modul layar LCD I2C bertindak sebagai klien pasif (_Slave_). Keduanya dihubungkan secara paralel melalui dua kawat utama: **SDA** (_Serial Data_ pada pin A4) untuk transfer paket informasi, dan **SCL** (_Serial Clock_ pada pin A5) untuk menyelaraskan ketukan (detak) pengiriman data.
2. **Identifikasi Alamat Heksadesimal:** Untuk mengawali instruksi cetak karakter ke layar, _Master_ (Arduino) memancarkan sinyal kondisi _Start_ ke jalur jaringan, dilanjutkan dengan mengirimkan kode alamat I2C spesifik dari layar LCD (umumnya `0x27` atau `0x3F`).
3. **Verifikasi dan Transfer Data:** Modul _Slave_ yang mengenali panggilan alamat tersebut akan membalas dengan sinyal _Acknowledge_ (ACK). Setelah koneksi tervalidasi, Arduino mengonversi instruksi tekstual menjadi himpunan _byte_ data dan mentransmisikannya ke cip antarmuka pembantu (biasanya PCF8574) pada punggung LCD. Cip ini kemudian mengatur suplai voltase pada matriks kristal cair untuk memunculkan piktogram huruf sesuai perintah.

---

## 2. Analisis Konfigurasi Polaritas Pin Potensiometer

**Jawaban:** Tidak, konfigurasi kedua pin ujung (kiri dan kanan) pada potensiometer tidak bersifat mengikat secara mutlak terhadap kutub suplai daya dan _ground_.

**Penjelasan apabila pin kiri dan kanan ditukar:**
Potensiometer bekerja secara fisis sebagai komponen pembagi tegangan (_voltage divider_) mekanis. Kaki pin bagian tengah (_wiper_) bertugas membaca resistansi aktual, sedangkan kedua kaki luarnya dihubungkan secara berseberangan pada arus tegangan tinggi (VCC 5V) dan arus rendah (GND).
Jika konfigurasi instalasi pin kiri dan pin kanan dibalik letaknya pada _breadboard_, efek yang terjadi hanyalah pembalikan gradasi kurva arah putaran (_reversed mapping_).

- **Kondisi Normal:** Pemutaran tuas searah jarum jam menghasilkan peningkatan kuantitas voltase (Nilai ADC naik dari 0 menuju 1023).
- **Kondisi Tertukar:** Pemutaran tuas pada arah yang sama (searah jarum jam) akan menghasilkan penurunan kuantitas voltase (Nilai ADC menurun dari 1023 menuju 0).

Kesalahan penukaran kutub pada komponen pasif ini tidak akan memicu risiko korsleting atau kerusakan fisis pada papan mikrokontroler.

---

## 3. Modifikasi Program Integrasi Output UART dan I2C

Kode di bawah ini dimodifikasi agar mikrokontroler mampu membaca nilai tegangan komparatif, mengalkulasinya, dan mengirimkan paket _output_ informasi tersebut menuju antarmuka serial (komputer) dan antarmuka I2C (layar LCD) secara bersamaan.

```cpp
/* Program Modifikasi: Integrasi Protokol UART & I2C
  Membaca sinyal Potensiometer dan menampilkannya pada Serial Monitor & LCD
*/

#include <Wire.h>               // Memuat library protokol I2C bawaan
#include <LiquidCrystal_I2C.h>  // Memuat library pengontrol chip LCD I2C

// Menginisialisasi objek LCD dengan alamat heksadesimal 0x27, dimensi 16 kolom x 2 baris
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinPot = A0;          // Menetapkan variabel pin analog A0 untuk input potensiometer

void setup() {
  Serial.begin(9600);           // Mengaktifkan antarmuka komunikasi UART dengan kecepatan 9600 bps
  lcd.init();                   // Melakukan inisialisasi awal pada modul layar LCD
  lcd.backlight();              // Mengaktifkan lampu latar (backlight) layar LCD
}

void loop() {
  // --- FASE PEMBACAAN DAN KALKULASI ---

  // Membaca sinyal analog dari potensiometer (rentang resolusi 10-bit: 0 hingga 1023)
  int nilaiADC = analogRead(pinPot);

  // Mengonversi data numerik ADC menjadi estimasi tegangan Volt (referensi atas 5.0V)
  float voltase = (nilaiADC / 1023.0) * 5.0;

  // Mengonversi data numerik ADC menjadi estimasi skala rasio persentase (0% - 100%)
  int persen = map(nilaiADC, 0, 1023, 0, 100);

  // Memetakan nilai ADC untuk merepresentasikan kuantitas blok baris visual (0 hingga 16 blok)
  int panjangBar = map(nilaiADC, 0, 1023, 0, 16);

  // --- FASE OUTPUT UART (SERIAL MONITOR) ---

  // Mencetak format teks berurutan ke antarmuka Serial Monitor sesuai spesifikasi
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" Volt: ");
  Serial.print(voltase, 2);     // Mencetak nilai tegangan dengan ketelitian 2 angka desimal
  Serial.print(" V Persen: ");
  Serial.print(persen);         // Mencetak data persentase sebagai integer utuh
  Serial.println("%");          // Mencetak simbol persen dan membuat jeda baris baru

  // --- FASE OUTPUT I2C (LCD DISPLAY) ---

  // Baris 1 LCD: Mencetak teks gabungan "ADC: [nilai] [persen]%"
  lcd.setCursor(0, 0);          // Memindahkan kursor pointer teks ke kolom 0, baris 0
  lcd.print("ADC: ");
  lcd.print(nilaiADC);
  lcd.print(" ");               // Menyediakan spasi pemisah antar variabel
  lcd.print(persen);
  lcd.print("%    ");           // Mencetak karakter kosong berlebih untuk membersihkan sisa angka lama

  // Baris 2 LCD: Mencetak visualisasi indikator grafis batang (Level Bar)
  lcd.setCursor(0, 1);          // Memindahkan kursor pointer teks ke kolom 0, baris 1
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.print((char)255);     // Mencetak grafis blok solid hitam untuk mengisi indikator bar
    } else {
      lcd.print(" ");           // Mencetak karakter kosong (spasi) untuk menghapus sisa indikator panjang
    }
  }

  // Menahan rotasi eksekusi program selama 200 milidetik untuk stabilisasi frekuensi refresh data
  delay(200);
}
```

---

## 4. Tabel Hasil Pengamatan Konversi ADC pada Serial Monitor

Tabel berikut menunjukkan hasil pengamatan dan kalkulasi komputasi numerik di mana representasi voltase diperoleh menggunakan formula `Volt = (ADC / 1023) * 5.0` dan persentase diformulasikan dari `Persen = (ADC / 1023) * 100`.

| ADC    | Volt (V) | Persen (%) |
| :----- | :------- | :--------- |
| **1**  | 0.00 V   | 0 %        |
| **21** | 0.10 V   | 2 %        |
| **49** | 0.24 V   | 4 %        |
| **74** | 0.36 V   | 7 %        |
| **96** | 0.47 V   | 9 %        |

_(Catatan ketelitian: Nilai Voltase dibulatkan menjadi 2 angka desimal sesuai sintaks `Serial.print(voltase, 2)`. Sedangkan nilai Persentase dibulatkan menjadi bilangan bulat sesuai operasi `map()` pada program)._
