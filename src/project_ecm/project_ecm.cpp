/**
 * Small program to read serial input and print the latest byte read
 */

#include "Arduino.h"
#include "tpl_os.h"

#include "project_ecm.hpp"

uint8_t currentGear = 0;
uint16_t currentRPM = 0;
float currentVelocity = 0;

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
    int32_t rawRPM = Serial.parseInt();

    if (rawRPM > 0) {
      currentRPM = eval_rpm(currentGear, (uint16_t)rawRPM);
    }
  }

  TerminateTask();
}

/**
 * REad msg with Gear value
 */
TASK(periodicTaskReadGearMsg) {
  currentGear = 1;

  Serial.print("Gear: ");
  Serial.println(currentGear);

  TerminateTask();
}

/**
 * Run task to calculate velocity and validate rpm and gear values
 */
TASK(periodicTaskCalcVelocity) {
  currentVelocity = calc_velocity(currentGear, currentRPM);

  TerminateTask();
}

/**
 * Send msg with Velocity value
 */
TASK(periodicTaskSendVelocityMsg) {
  Serial.print("Velocity: ");
  Serial.println(currentVelocity);

  TerminateTask();
}

/**
 * Send msg with RPM value
 */
TASK(periodicTaskSendRPMMsg) {
  Serial.print("RPM: ");
  Serial.println(currentRPM);

  TerminateTask();
}