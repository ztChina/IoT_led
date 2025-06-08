#include "led.h"
#include <Arduino.h>

//*************************定义led类
led::led(uint8_t PIN) {
  Init();
}
void led::On() {
  digitalWrite(PIN, HIGH);
  ledStuatus = 1;
}
void led::Off() {
  digitalWrite(PIN, LOW);
  ledStuatus = 0;
}
void led::Init() {
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
  ledStuatus = 0;
}