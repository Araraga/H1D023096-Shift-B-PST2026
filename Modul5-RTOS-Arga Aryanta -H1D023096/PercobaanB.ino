#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Definisi struktur data untuk dikirim melalui antrean
struct readings {
  int temp;
  int h;
};

// Handle untuk antrean (Queue)
QueueHandle_t my_queue;

// Prototype fungsi Task
void read_data(void *pvParameters);
void display(void *pvParameters);

void setup() {
  Serial.begin(9600);
  
  // Membuat Queue untuk menampung 1 item bertipe struct readings
  my_queue = xQueueCreate(1, sizeof(struct readings));
  
  // Membuat task untuk membaca data dan menampilkannya
  xTaskCreate(read_data, "read", 128, NULL, 1, NULL);
  xTaskCreate(display, "display", 128, NULL, 1, NULL);

  // Scheduler akan otomatis berjalan setelah setup selesai dalam Arduino_FreeRTOS
}

// WAJIB ADA: Meskipun kosong, linker Arduino tetap mencari fungsi ini
void loop() {
  // Tidak ada instruksi di sini karena kontrol sudah diambil alih RTOS
}

void read_data(void *pvParameters) {
  struct readings x;
  for(;;) {
    x.temp = 54; // Simulasi data sensor statis
    x.h = 30;
    
    // Mengirim data ke antrean (Queue)
    // portMAX_DELAY membuat task menunggu jika antrean penuh
    xQueueSend(my_queue, &x, portMAX_DELAY);
    
    vTaskDelay(100 / portTICK_PERIOD_MS); // Jeda 100ms
  }
}

void display(void *pvParameters) {
  struct readings x;
  for(;;) {
    // Menerima data dari antrean (Queue)
    // Jika berhasil (pdPASS), cetak ke Serial Monitor
    if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.print("temp = ");
      Serial.println(x.temp);
      Serial.print("humidity = ");
      Serial.println(x.h);
    }
  }
}