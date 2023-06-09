/**
 * Small program to read serial input and print the latest byte read
 */

#include "Arduino.h"
#include "tpl_os.h"

uint8_t incomingByte = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

/**
 * Read a byte and store
 *  - turn led on to sign reading
 */
TASK(periodicTaskRead) {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    digitalWrite(LED_BUILTIN, HIGH);
  }
  TerminateTask();
}

/*
 * Send msg with latest byte read
 *  - turn led off to sign byte sent
 */
TASK(periodicTaskSendMsg) {
  Serial.print("Last byte received: ");
  Serial.println(incomingByte, DEC);
  digitalWrite(LED_BUILTIN, LOW);
  TerminateTask();
}