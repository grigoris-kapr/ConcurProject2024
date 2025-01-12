// from workshop in December

// THIRD PROJECT

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
long debouncing_time = 150;
volatile unsigned long last_micros;
SemaphoreHandle_t interruptSemaphore;
void setup() {
pinMode(2, INPUT_PULLUP);
xTaskCreate(TaskLed, "Led", 128, NULL, 0,
NULL );
xTaskCreate(TaskBlink, "LedBlink", 128,
NULL, 0, NULL );
interruptSemaphore = xSemaphoreCreateBinary();
if (interruptSemaphore != NULL) {
attachInterrupt(digitalPinToInterrupt(2),
debounceInterrupt, LOW);
}
}
void loop() {}
void interruptHandler() {
xSemaphoreGiveFromISR(interruptSemaphore, NULL);
}
void TaskLed(void *pvParameters)
{
(void) pvParameters;
pinMode(8, OUTPUT);
for (;;) {
if (xSemaphoreTake(interruptSemaphore,
portMAX_DELAY) == pdPASS) {
digitalWrite(12, HIGH);
}
}
}
void TaskBlink(void *pvParameters)
{
(void) pvParameters;
pinMode(7, OUTPUT);
for (;;) {
digitalWrite(12, HIGH);
vTaskDelay(200 / portTICK_PERIOD_MS);
digitalWrite(7, LOW);
vTaskDelay(200 / portTICK_PERIOD_MS);
}
}
void debounceInterrupt() {
if((long)(micros() - last_micros) >=
debouncing_time * 1000) {
interruptHandler();
last_micros = micros();
}
}



// SECOND PROJECT
// #include <Arduino
_FreeRTOS.h>
// #include <semphr.h>

// SemaphoreHandle_t mutex_v;

// void setup() {
//   Serial.begin(9600);
//   mutex_v = xSemaphoreCreateMutex();
//   if (mutex_v == NULL) {
//     Serial.println("Mutex can not be created");
//   }

//   xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL);
//   xTaskCreate(Task2, "Task2", 128, NULL, 1, NULL);
// }

// void Task1(void *pvParameters) {
//   while(1) {
//   xSemaphoreTake(mutex_v, portMAX_DELAY);
//   Serial.println("Hi from Task1");
//   xSemaphoreGive(mutex_v);
//   vTaskDelay(pdMS_TO_TICKS(1000));
//   }
// }
// void Task2(void *pvParameters) {
//   while(1) {
//     xSemaphoreTake(mutex_v, portMAX_DELAY);
//     Serial.println("Hi from Task2");
//     xSemaphoreGive(mutex_v);
//     vTaskDelay(pdMS_TO_TICKS(500));
//   }
// }

// void loop() {

// }


// FIRST PROJECT
// const byte ledPin = 13;
// const byte interruptPin = 2;
// volatile byte state = LOW;

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(ledPin, OUTPUT);
//   pinMode(interruptPin, INPUT_PULLUP);
//   attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   digitalWrite(ledPin, state);
// }

// void blink() {
//   state = !state;
// }