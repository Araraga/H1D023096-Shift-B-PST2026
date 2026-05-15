# Jawaban Pertanyaan Praktikum Modul 5: RTOS Multitasking dan Komunikasi Task

---

## A. Jawaban Pertanyaan Praktikum 5.5.4 (Percobaan 5A: Multitasking)

### 1. Mekanisme Eksekusi Task (Bersamaan vs Bergantian)

Secara harfiah pada arsitektur perangkat keras, mikrokontroler Arduino Uno menggunakan prosesor _single-core_ (satu inti), yang berarti prosesor tersebut hanya dapat mengeksekusi satu baris instruksi pada satu waktu. Oleh karena itu, ketiga _task_ tersebut sebenarnya berjalan secara **bergantian** (_time-slicing_).

Namun, _kernel scheduler_ dari FreeRTOS melakukan perpindahan alih daya pemrosesan (_context switching_) antar ketiga _task_ tersebut dengan sangat cepat (biasanya dalam rentang milidetik berdasarkan _system tick_). Perpindahan ini sangat presisi sehingga di mata pengguna, _task-task_ tersebut seolah-olah dieksekusi secara **bersamaan** (_concurrent_).

### 2. Langkah Menambahkan Task Keempat

Untuk menambahkan _task_ keempat, diperlukan langkah-langkah prosedural berikut pada kode program:

1.  **Deklarasi Fungsi:** Tambahkan prototipe fungsi untuk _task_ tersebut sebelum blok `setup()`, misalnya: `void TaskBlink3(void *pvParameters);`.
2.  **Inisialisasi Task:** Panggil fungsi `xTaskCreate()` di dalam blok `setup()` sebelum perintah `vTaskStartScheduler()`. Isikan argumen untuk nama fungsi, label _string_ untuk _debugging_, ukuran memori _stack_ (misal: 128 byte), parameter (NULL), tingkat prioritas (misal: 1), dan _handle_ (NULL).
3.  **Definisi Logika Task:** Buat blok fungsi `void TaskBlink3(void *pvParameters)` di luar fungsi `loop()`. Fungsi ini wajib berisi perulangan tak terbatas (`while(1)`) dan menyertakan fungsi tundaan RTOS non-pemblokiran (seperti `vTaskDelay`) agar tidak memonopoli sumber daya prosesor.

### 3. Modifikasi Program (Potensiometer Pengontrol Kecepatan Kedipan LED RTOS)

```cpp
/* Program Modifikasi RTOS (Kontrol Kecepatan Blink via Potensiometer) */

#include <Arduino_FreeRTOS.h>

const int PIN_POT = A0;
const int PIN_LED = 8;

// Prototipe Task
void TaskControlBlink(void *pvParameters);
void TaskMonitorADC(void *pvParameters);

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);

  // Membuat dua task independen
  xTaskCreate(TaskControlBlink, "ControlBlink", 128, NULL, 1, NULL);
  xTaskCreate(TaskMonitorADC, "MonitorADC", 128, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // Kosong
}

void TaskControlBlink(void *pvParameters) {
  while(1) {
    // Membaca ADC dan memetakannya menjadi delay (100ms - 1000ms)
    int rawADC = analogRead(PIN_POT);
    int intervalKedip = map(rawADC, 0, 1023, 100, 1000);

    digitalWrite(PIN_LED, HIGH);
    vTaskDelay(intervalKedip / portTICK_PERIOD_MS);

    digitalWrite(PIN_LED, LOW);
    vTaskDelay(intervalKedip / portTICK_PERIOD_MS);
  }
}

void TaskMonitorADC(void *pvParameters) {
  while(1) {
    int logADC = analogRead(PIN_POT);
    Serial.print("Nilai ADC Potensio: ");
    Serial.println(logADC);

    // Memberikan jeda 500ms agar serial monitor tidak terlalu dibanjiri data
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
```
