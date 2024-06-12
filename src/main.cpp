#include <Arduino.h>

// Definir el pin del LED
const int LED_PIN = 23;

// Prototipo de funciones de las tareas
void taskEncenderLED(void * parameter);
void taskApagarLED(void * parameter);

// Definir los identificadores de las tareas
TaskHandle_t taskEncenderLEDHandle = NULL;
TaskHandle_t taskApagarLEDHandle = NULL;

void setup() {
  // Inicializar el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);

  // Crear la tarea para encender el LED
  xTaskCreate(
    taskEncenderLED,      // Función de la tarea
    "TaskEncenderLED",    // Nombre de la tarea
    1000,                 // Tamaño de la pila (bytes)
    NULL,                 // Parámetro de la tarea
    1,                    // Prioridad de la tarea
    &taskEncenderLEDHandle // Manejador de la tarea
  );

  // Crear la tarea para apagar el LED
  xTaskCreate(
    taskApagarLED,      // Función de la tarea
    "TaskApagarLED",    // Nombre de la tarea
    1000,               // Tamaño de la pila (bytes)
    NULL,               // Parámetro de la tarea
    1,                  // Prioridad de la tarea
    &taskApagarLEDHandle // Manejador de la tarea
  );
}

void loop() {
  // No se utiliza el loop en FreeRTOS
}

// Tarea para encender el LED
void taskEncenderLED(void * parameter) {
  for (;;) {
    digitalWrite(LED_PIN, HIGH); // Encender el LED
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Esperar 1 segundo
    xTaskNotifyGive(taskApagarLEDHandle); // Notificar a la tarea para apagar el LED
    vTaskSuspend(NULL); // Suspender esta tarea
  }
}

// Tarea para apagar el LED
void taskApagarLED(void * parameter) {
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Esperar la notificación de encender el LED
    digitalWrite(LED_PIN, LOW); // Apagar el LED
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Esperar 1 segundo
    vTaskResume(taskEncenderLEDHandle); // Resumir la tarea para encender el LED
  }
}
