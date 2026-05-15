#include <Arduino_FreeRTOS.h>

// Prototipe Task
void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);

void setup() {
  // Inisialisasi komunikasi serial pada kecepatan 9600 bps
  Serial.begin(9600);

  // Pembuatan Task (Nama Fungsi, Label, Ukuran Stack, Parameter, Prioritas, Handle)
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL);

  // Memulai penjadwal RTOS
  vTaskStartScheduler();
}

void loop() {
  // Fungsi loop() dibiarkan kosong karena eksekusi diatur oleh Scheduler RTOS
}

void TaskBlink1(void *pvParameters) {
  pinMode(8, OUTPUT); // Konfigurasi Pin 8 sebagai OUTPUT
  
  while(1) {
    Serial.println("Task1");
    digitalWrite(8, HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS); // Penundaan 200ms non-blocking
    digitalWrite(8, LOW);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void TaskBlink2(void *pvParameters) {
  pinMode(7, OUTPUT); // Konfigurasi Pin 7 sebagai OUTPUT
  
  while(1) {
    Serial.println("Task2");
    digitalWrite(7, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS); // Penundaan 300ms non-blocking
    digitalWrite(7, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;
  
  while(1) {
    counter++;
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS); // Penundaan 500ms non-blocking
  }
}