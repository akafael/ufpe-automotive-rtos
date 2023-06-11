/**
 * Small program to read serial input and print the latest byte read
 */

#include "Arduino.h"
#include "tpl_os.h"
#include <SPI.h>
#include <mcp_can.h>

#include "../project_lib/Board.h"
#include "../project_lib/Utils.hpp"

#define CAN_SPI_INT 2 // Set INT to pin 2
MCP_CAN CAN_SPI(10);  // Set CS to pin 10

uint8_t currentGear = 0;
uint16_t currentRPM = 0;
float currentVelocity = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  // Start CAN Device
  if (CAN_SPI.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  // CAN Settings
  pinMode(CAN_SPI_INT, INPUT); // Configuring pin for /INT input
  CAN_SPI.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515
                               // sends acks to received data.
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
 * Read msg with Gear value
 */
TASK(periodicTaskReadGearMsg) {
  // If CAN_SPI_INT pin is low, read receive buffer
  if (!digitalRead(CAN_SPI_INT)) {
    uint32_t rxId;
    uint8_t dataLenght = 0;
    uint8_t rxBuffer[8];

    // Read Data from CAN
    CAN_SPI.readMsgBuf(&rxId, &dataLenght, rxBuffer);

    // const uint32_t msgId = (rxId & 0x1FFFFFFF);
    if (rxId == IdMsgGear) {
      currentGear = eval_gear(rxBuffer[0]);
    }
  }

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
  union {
    float value;
    uint8_t bytes[sizeof(float)];
  } data;

  data.value = currentRPM;
  CAN_SPI.sendMsgBuf(IdMsgVelocity, 0, sizeof(data), data.bytes);

  Serial.print("Velocity: ");
  Serial.println(currentVelocity);

  TerminateTask();
}

/**
 * Send msg with RPM value
 */
TASK(periodicTaskSendRPMMsg) {
  union {
    uint16_t value;
    uint8_t bytes[2];
  } data;

  data.value = currentRPM;
  CAN_SPI.sendMsgBuf(IdMsgRPM, 0, sizeof(data), data.bytes);

  Serial.print("RPM: ");
  Serial.println(currentRPM);

  TerminateTask();
}