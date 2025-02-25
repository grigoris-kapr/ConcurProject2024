#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <IRremote.hpp>

// define pins as constants for easier changes to pinout
#define DecrementPin 12
#define IncrementPin 11
#define RedPin 8
#define GreenPin 9
#define BluePin 10
#define IRPin 2 // can only be 2 or 3 on this board

// delay between button polling in ms
#define ButtonPollingDelay 30

SemaphoreHandle_t mutex;
// LED state as an int
int led_state = 0;
// IR proccess flag
bool IrReceived = false;

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
  // init IR receiver
  IrReceiver.begin(IRPin, ENABLE_LED_FEEDBACK);
  attachInterrupt(digitalPinToInterrupt(IRPin), handleIR, CHANGE);
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

// set state to same as the button pressed on the RC. ignore IR signal if not corresponding to 0-7
void handleIR(){
  if(IrReceived){
    return;
  }
  IrReceived = true;
  if(IrReceiver.decode()){
    Serial.println("IR signal received");
    if(IrReceiver.decodedIRData.command>=0 && IrReceiver.decodedIRData.command<=7){
      led_state = IrReceiver.decodedIRData.command;
      set_LED_state(led_state);
    }
    IrReceiver.resume();
    delay(50);
    IrReceived = false;
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
  /*if(IrReceiver.decode()){
    Serial.println("IR signal received");
    if(IrReceiver.decodedIRData.command>=0 && IrReceiver.decodedIRData.command<=7){
      led_state = IrReceiver.decodedIRData.command;
      set_LED_state(led_state);
    }
    IrReceiver.resume();
    delay(50);
  }
  else{
    delay(5);
  }*/
}
