#include "ledControl.h"
#include <Arduino.h>
void ledInit() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}
void ledOn() {
  digitalWrite(LED_PIN, HIGH);
}
void ledOff() {
  digitalWrite(LED_PIN, LOW);
}