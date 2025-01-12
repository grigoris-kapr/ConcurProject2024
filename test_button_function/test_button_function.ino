// Yellow and red buttons are set as inputs on pins 13 and 12 respectively. This script was created to
// verify the way in which they function and are read. Based on: https://www.instructables.com/How-to-use-a-push-button-switch-with-arduino/

void setup() {
  Serial.begin(9600);
  pinMode(13,INPUT);
  pinMode(12,INPUT);
}

void loop() {
  delay(500);
  Serial.print("13: ");
  Serial.print(digitalRead(13));
  Serial.print(" 12: ");
  Serial.println(digitalRead(12));
}
