# Jawaban Pertanyaan Praktikum Modul 3: Evaluasi Teori dan Analisis (Bab 3.7)

---

## 1. Analisis Keuntungan dan Kerugian Protokol UART dan I2C

Kedua protokol ini memiliki karakteristik arsitektur yang berbeda sehingga peruntukannya disesuaikan dengan kebutuhan topologi perangkat keras.

### A. Universal Asynchronous Receiver-Transmitter (UART)

**Keuntungan:**

1. **Kesederhanaan Perangkat Keras:** Tidak memerlukan jalur _clock_ (SCL) karena beroperasi secara asinkron. Hanya membutuhkan dua kabel data (TX dan RX) atau bahkan satu kabel jika komunikasi hanya berjalan searah (Simpleks).
2. **Komunikasi _Full-Duplex_:** Dapat mengirim dan menerima data secara bersamaan dalam waktu yang persis sama.
3. **Kemudahan Implementasi:** Sangat ideal untuk komunikasi _point-to-point_ antardua perangkat dasar (seperti Arduino ke Komputer).

**Kerugian:**

1. **Skalabilitas Sangat Terbatas:** Arsitekturnya murni _point-to-point_, sehingga tidak dapat menyambungkan banyak perangkat sensor ke dalam satu jalur (hanya mendukung rasio 1:1).
2. **Ketergantungan _Baud Rate_:** Kedua perangkat keras harus diatur pada kecepatan transmisi (_baud rate_) yang identik. Jika ada selisih kalibrasi kecepatan lebih dari sekitar 3%, data yang terbaca akan menjadi karakter tidak bermakna (_garbage data_).

### B. Inter-Integrated Circuit (I2C)

**Keuntungan:**

1. **Efisiensi Jaringan (_Multiple Slaves_):** Mampu menghubungkan puluhan hingga ratusan perangkat keras (sensor, modul, aktuator) hanya dengan menumpang pada dua kabel _bus_ yang sama (SDA dan SCL).
2. **Fleksibilitas Pengontrol (_Multiple Masters_):** Protokol ini mendukung sistem di mana lebih dari satu mikrokontroler bertindak sebagai _Master_ pada jaringan yang sama.
3. **Validasi Penerimaan Data:** Memiliki mekanisme bawaan sinyal _Acknowledge/Not-Acknowledge_ (ACK/NACK) dari perangkat penerima, memastikan _Master_ tahu apakah datanya berhasil sampai atau tidak.

**Kerugian:**

1. **Kecepatan Terbatas dan _Half-Duplex_:** Transmisi data tidak bisa dikirim dan diterima secara bersamaan (bergantian), dan umumnya memiliki throughput kecepatan yang lebih lambat dibanding SPI.
2. **Kompleksitas Sirkuit:** Memerlukan konfigurasi _pull-up resistor_ pada jalur SDA dan SCL agar jaringan dapat beroperasi secara stabil.
3. **Potensi Konflik Alamat:** Jika terdapat dua sensor sejenis dari pabrikan yang sama di satu jalur, alamat heksadesimalnya dapat berbenturan (konflik) dan melumpuhkan komunikasi.

---

## 2. Peran Alamat I2C pada Layar LCD (Contoh: `0x27` vs `0x20`)

**Penjelasan Peran:**
Protokol I2C menggunakan arsitektur jalur berbagi (_shared bus_). Bayangkan SDA dan SCL sebagai sebuah jalan raya yang dilewati oleh semua modul sensor dan LCD. Agar mikrokontroler (_Master_) dapat "berbicara" secara spesifik kepada layar LCD tanpa memicu respon dari modul lain yang terpasang di kabel yang sama, layar LCD wajib memiliki **Identitas Unik**.

Identitas inilah yang direpresentasikan dalam bentuk **Alamat Heksadesimal (I2C Address)**, seperti `0x27`, `0x3F`, atau `0x20`.

**Mekanisme dan Dampaknya:**

1. **Panggilan _Routing_:** Saat Arduino ingin mencetak teks, ia pertama-tama memancarkan alamat `0x27` ke seluruh jaringan kabel.
2. **Penyaringan Klien:** Hanya komponen yang perangkat kerasnya disetel pada alamat `0x27` (dalam hal ini, _chip_ PCF8574T di belakang LCD) yang akan "menjawab" dengan sinyal setuju (ACK) dan membuka pintunya untuk menerima data teks. Modul lain akan mengabaikan paket data tersebut.
3. **Kesalahan Alamat:** Jika kita salah memasukkan alamat pada kode program (misalnya program disetel `0x20` tetapi perangkat kerasnya adalah `0x27`), maka data akan dikirim ke "rumah yang salah". LCD tidak akan merespons, dan layar akan tetap kosong meskipun tidak ada pesan _error_ pada saat kompilasi (_compile_).

---

## 3. Alur Kerja Sistem Gabungan dan Manajemen Dual-Protokol Arduino

Ketika sistem menggabungkan UART (sebagai _input_ dari Komputer) dan I2C (sebagai _output_ ke LCD), sistem bekerja secara harmonis meskipun Arduino Uno (ATmega328P) pada dasarnya adalah prosesor _single-core_ (inti tunggal).

**Alur Kerja Sistem:**

1. **Fase Interupsi Masukan (UART):** Komputer mengirimkan _string_ (misal: "Halo") melalui USB. Cip UART Arduino menerima data listrik ini, dan secara _hardware interrupt_ menyimpannya ke dalam memori _buffer_ serial tanpa mengganggu program utama.
2. **Fase Pemrosesan:** Di dalam fungsi `loop()`, mikrokontroler mendeteksi adanya data di _buffer_ menggunakan `Serial.available()`. Data tersebut kemudian ditarik menggunakan `Serial.read()` dan diproses ke dalam variabel _string_.
3. **Fase Eksekusi Keluaran (I2C):** Arduino mengambil variabel _string_ tersebut, memformulasikannya, dan membuka gerbang komunikasi I2C. Ia mengirimkan pulsa _clock_ (SCL) beserta _byte_ data (SDA) secara terstruktur menuju alamat LCD. Teks pun muncul di layar.

**Bagaimana Arduino Mengelola Keduanya Sekaligus?**
Arduino tidak benar-benar mengeksekusi kedua protokol tersebut secara "bersamaan mutlak" (_parallel_), melainkan mengeksekusinya secara **sekuensial dan asinkron** yang diatur oleh penjadwalan perangkat keras:

- **Isolasi Perangkat Keras:** UART dan I2C memiliki register memori dan pin fisik perangkat keras di dalam _chip_ ATmega328P yang sama sekali terpisah (UART menggunakan TX/RX independen, I2C menggunakan modul _Two-Wire Interface_ / TWI).
- **Sistem _Buffer_:** Ketika Arduino sedang sibuk mengendalikan _clock_ I2C untuk mencetak teks ke LCD (yang memakan waktu sepersekian milidetik), ia tidak akan "kehilangan" data UART yang tiba-tiba masuk. Data dari komputer akan ditampung secara otomatis oleh sistem _hardware interrupt_ ke dalam _RX Buffer_ UART (kapasitas 64 _bytes_).
- **Ekseskusi Bergantian Berkecepatan Tinggi:** Setelah Arduino selesai mengirim _byte_ ke I2C, ia akan kembali memeriksa _buffer_ UART. Karena transisi antar-instruksi ini berlangsung dalam hitungan mikrodetik, dari sudut pandang manusia, proses pembacaan sensor serial dan penampilan di LCD tampak terjadi secara simultan/bersamaan.
