/**
 * Small program to read serial input and print the latest byte read
 */

#include "Arduino.h"
#include "tpl_os.h"
#include <SPI.h>
#include <mcp_can.h>

#include "../project_lib/Board.h"
#include "../project_lib/Utils.hpp"

MCP_CAN CAN_SPI(CAN1_CS_PIN); // MCP2515 CAN controler with SPI interface

uint8_t currentGear = 1;
uint16_t currentRPM = 0;
float currentVelocity = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  // Start CAN Device
  if (CAN_SPI.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("[ECM] MCP2515 Initialized Successfully!");
  else
    Serial.println("[ECM] Error Initializing MCP2515...");

  // CAN Settings
  pinMode(CAN_INT_PIN, INPUT); // Configuring pin for /INT input
  CAN_SPI.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515
                               // sends acks to received data.
}

/**
 * Read a byte and store
 *  - turn led on to sign reading
 */
TASK(periodicTaskRead) {
  if (Serial.available() > 0) {
    const int32_t rawRPM = Serial.parseInt();

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
  if (!digitalRead(CAN_INT_PIN)) {
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

  Serial.print("[ECM] Gear: ");
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
    uint16_t value;
    uint8_t bytes[2];
  } data;

  data.value = encode_velocityData(currentVelocity);
  CAN_SPI.sendMsgBuf(IdMsgVelocity, CAN_EXTID, sizeof(data), data.bytes);

  Serial.print("[ECM] Velocity: ");
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

  data.value = encode_rpmData(currentRPM);
  CAN_SPI.sendMsgBuf(IdMsgRPM, CAN_EXTID, sizeof(data), data.bytes);

  Serial.print("[ECM] RPM: ");
  Serial.println(currentRPM);

  TerminateTask();
}