#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// define pins as constants for easier changes to pinout
#define DecrementPin 13
#define IncrementPin 12
#define RedPin 8
#define GreenPin 9
#define BluePin 10

// delay between button polling in ms
#define ButtonPollingDelay 30


SemaphoreHandle_t mutex;
// LED state as an int
int led_state = 0;

void setup() {
  // init pins & Serial
  Serial.begin(9600);
  pinMode(DecrementPin, INPUT);
  pinMode(IncrementPin, INPUT);
  pinMode(RedPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  set_LED_state(led_state);
  // mutex semaphore
  mutex = xSemaphoreCreateMutex();
  if(mutex == NULL) {
    Serial.println("Mutex can not be created");
  }
  // init button controlers
  xTaskCreate(taskDecrement, "taskDecrement", 128, NULL, 1, NULL);
  xTaskCreate(taskIncrement, "taskIncrement", 128, NULL, 1, NULL);

}

// on every button press, decrease led_state by 1. Do nothing on button hold / release
void taskDecrement(void *pvParameters) {
  // use last_pin_state to avoid long button presses from trigering decrement twice
  int last_pin_state = 1;
  int current_pin_state = 1;
  while(1) {
    current_pin_state = digitalRead(DecrementPin);
    if(current_pin_state == 0 && last_pin_state == 1) {
      last_pin_state = 0;
      xSemaphoreTake(mutex, portMAX_DELAY);
      Serial.println("Decrementing LED state");
      led_state = (led_state - 1 +8) % 8; // +8 to use mod properly
      set_LED_state(led_state);
      xSemaphoreGive(mutex);
    }
    else if (current_pin_state == 1) { // state 0 -> 0 can be safely ignored (do nothing)
      last_pin_state = 1;
    }
    vTaskDelay(pdMS_TO_TICKS(ButtonPollingDelay));
  }
}

// on every button press, increase led_state by 1. Do nothing on button hold / release
void taskIncrement(void *pvParameters) {
  // use last_pin_state to avoid long button presses from trigering decrement twice
  int last_pin_state = 1;
  int current_pin_state = 1;
  while(1) {
    current_pin_state = digitalRead(IncrementPin);
    if(current_pin_state == 0 && last_pin_state == 1) {
      last_pin_state = 0;
      xSemaphoreTake(mutex, portMAX_DELAY);
      Serial.println("Incrementing LED state");
      led_state = (led_state + 1 +8) % 8; // +8 to use mod properly
      set_LED_state(led_state);
      xSemaphoreGive(mutex);
    }
    else if (current_pin_state == 1) { // state 0 -> 0 can be safely ignored (do nothing)
      last_pin_state = 1;
    }
    vTaskDelay(pdMS_TO_TICKS(ButtonPollingDelay));
  }
}

void set_LED_state(int state){
  Serial.print("New state:");
  Serial.println(state);
  digitalWrite(BluePin, state%2);
  state = state / 2;
  digitalWrite(GreenPin, state%2);
  state = state / 2;
  digitalWrite(RedPin, state%2); // mod should be redundant here but written for consistency
}

void loop() {

}
