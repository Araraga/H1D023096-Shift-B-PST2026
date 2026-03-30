1. Kondisi Program Masuk ke Blok if
Program masuk ke dalam blok if ketika kondisi yang dievaluasi bernilai benar, yaitu pada saat variabel timeDelay memiliki nilai kurang dari atau sama dengan 100 (timeDelay <= 100).
2. Kondisi Program Masuk ke Blok else
Program mengeksekusi instruksi di dalam blok else apabila kondisi utama pada blok if tidak terpenuhi (bernilai salah). Dalam konteks kode ini, hal tersebut terjadi ketika nilai timeDelay lebih besar dari 100.
3. Fungsi Perintah delay(timeDelay)
Perintah delay(timeDelay) berfungsi untuk menghentikan sementara (menunda) eksekusi baris program selanjutnya selama durasi waktu yang tersimpan di dalam variabel timeDelay (dalam satuan milidetik). Perintah ini secara langsung mengatur durasi lamanya LED dalam keadaan menyala (HIGH) dan mati (LOW).
4. Modifikasi Alur dan Penjelasan Kode (Format README.md)Untuk mengubah alur menjadi "Cepat → Sedang → Mati", diperlukan struktur logika percabangan bertingkat (if-else if). Nilai inisialisasi awal juga harus diubah menjadi angka yang kecil (cepat).
