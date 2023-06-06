/**
 *
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
 */
TASK(periodicTaskRead) {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
  }
}

/*
 * Send msg with latest byte read
 */
TASK(periodicTaskSendMsg) {
  Serial.print("Last byte received: ");
  Serial.println(incomingByte, DEC);
}