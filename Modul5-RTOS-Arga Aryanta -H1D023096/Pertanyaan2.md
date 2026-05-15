# Jawaban Pertanyaan Praktikum Modul 5: Percobaan 5B (Komunikasi Task)

---

## 1. Mekanisme Komunikasi Antar Task (Bersamaan vs Bergantian)

Pada arsitektur mikrokontroler dengan inti prosesor tunggal (_single-core_) seperti Arduino Uno, kedua _task_ (`read_data` dan `display`) beroperasi secara **bergantian**, bukan bersamaan secara absolut. Mekanisme ini diatur oleh _kernel scheduler_ dari FreeRTOS melalui metode _time-slicing_.

Mekanisme kerjanya adalah sebagai berikut: _task_ `read_data` memperoleh siklus pemrosesan (_CPU time_) untuk mengeksekusi instruksi pembacaan data dan mengirimkannya ke dalam antrean (_queue_). Saat fungsi tersebut mengeksekusi instruksi _blocking_ seperti `vTaskDelay` atau saat antrean penuh, _scheduler_ menghentikan sementara eksekusi _task_ tersebut dan mengalihkan siklus pemrosesan kepada _task_ `display`. Proses peralihan antartugas (_context switching_) ini terjadi dalam rentang waktu yang sangat singkat (satuan _tick_ atau milidetik), sehingga ilusi pemrosesan secara **bersamaan** (_concurrent_) tercipta bagi pengguna.

---

## 2. Analisis Potensi Terjadinya _Race Condition_

Program tersebut **sangat kecil potensinya atau hampir tidak berpotensi** mengalami _race condition_. _Race condition_ umumnya terjadi ketika beberapa urutan proses (_thread_/_task_) mencoba mengakses dan memodifikasi satu ruang memori global yang sama secara bersamaan tanpa mekanisme penguncian (_locking_).

Dalam program percobaan ini, komunikasi data difasilitasi sepenuhnya oleh struktur antrean bawaan FreeRTOS (`xQueueSend` dan `xQueueReceive`). Implementasi _queue_ pada FreeRTOS pada dasarnya bersifat _thread-safe_. _Kernel_ secara otomatis menangani perlindungan penulisan dan pembacaan memori. Ketika sebuah _task_ sedang menyisipkan paket data ke dalam antrean, _task_ lain tidak diizinkan untuk menarik paket tersebut hingga proses penulisan terkonfirmasi selesai. Hal ini mengeliminasi risiko bentrokan memori yang merusak integritas data.

---

## 3. Modifikasi Program Menggunakan Sensor DHT Dinamis

Berikut adalah kode modifikasi yang mengintegrasikan pembacaan dinamis menggunakan modul sensor DHT sesungguhnya, beserta dokumentasi teknisnya untuk _README.md_.

```cpp
/* Program Modifikasi Komunikasi Queue RTOS Dinamis dengan Sensor DHT */

#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

// Definisi pin dan tipe sensor DHT
#define DHTPIN 2
#define DHTTYPE DHT11

// Instansiasi objek sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Struktur paket data untuk dikirim melalui Queue
struct readings {
  float temp;
  float h;
};

// Deklarasi global handle Queue
QueueHandle_t my_queue;

// Prototipe Task
void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Pembuatan Queue dengan kapasitas 1 elemen seukuran struct readings
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Registrasi Task ke Scheduler
  xTaskCreate(read_data, "ReadSensorTask", 128, NULL, 1, NULL);
  xTaskCreate(display, "DisplayTask", 128, NULL, 1, NULL);

  // Menjalankan Scheduler
  vTaskStartScheduler();
}

void loop() {
  // Dibiarkan kosong, kendali berada pada kernel RTOS
}

void read_data(void *pvParameters) {
  struct readings x;
  for(;;) {
    // Akuisisi data presisi dari sensor
    x.temp = dht.readTemperature();
    x.h = dht.readHumidity();

    // Validasi pembacaan data (menghindari hasil NaN)
    if (!isnan(x.temp) && !isnan(x.h)) {
      // Mengirim paket struct ke Queue
      xQueueSend(my_queue, &x, portMAX_DELAY);
    }

    // Delay wajib 2 detik sesuai standar siklus pembacaan hardware DHT11
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void display(void *pvParameters) {
  struct readings x;
  for(;;) {
    // Mengekstraksi data jika status pengiriman pada Queue valid (pdPASS)
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.println("=== Pembaruan Data Sensor ===");
      Serial.print("Temperatur : ");
      Serial.print(x.temp);
      Serial.println(" °C");

      Serial.print("Kelembaban : ");
      Serial.print(x.h);
      Serial.println(" %");
      Serial.println("=============================\n");
    }
  }
}
```
