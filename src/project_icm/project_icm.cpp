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

uint8_t currentGear = 3;
uint16_t currentRPM = 0;
float currentVelocity = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  // Start CAN Device
  if (CAN_SPI.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("[ICM] MCP2515 Initialized Successfully!");
  else
    Serial.println("[ICM] Error Initializing MCP2515...");

  // CAN Settings
  pinMode(CAN_INT_PIN, INPUT); // Configuring pin for /INT input
  CAN_SPI.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515
                               // sends acks to received data.
}

TASK(periodicTaskReadCANMsg) {
  // If CAN_SPI_INT pin is low, read receive buffer
  if (!digitalRead(CAN_INT_PIN)) {
    uint32_t rxId;          // CAN Frame ID
    uint8_t dataLenght = 0; // Payload data length
    uint8_t rxBuffer[8];    // Payload data

    // Read Data from CAN
    CAN_SPI.readMsgBuf(&rxId, &dataLenght, rxBuffer);

    // Decode msg payload considering msgId
    const uint32_t msgId = (rxId & 0x1FFFFFFF);
    if (msgId == IdMsgGear) {
      const uint8_t gear = decode_gearData(rxBuffer[0]);
      currentGear = eval_gear(gear);
    } else if (msgId == IdMsgRPM) {
      union {
        uint16_t value;
        uint8_t bytes[2];
      } data;

      data.bytes[1] = rxBuffer[1];
      data.bytes[0] = rxBuffer[0];

      const uint16_t rpm = decode_rpmData(data.value);
      currentRPM = eval_rpm(currentGear, rpm);
    } else if (msgId == IdMsgVelocity) {
      union {
        uint16_t value;
        uint8_t bytes[2];
      } data;

      data.bytes[1] = rxBuffer[1];
      data.bytes[0] = rxBuffer[0];

      const float velocity = decode_velocityData(data.value);
      currentVelocity = velocity;
    }
  }

  TerminateTask();
}

TASK(periodicTaskPrint) {
  Serial.print("[ICM] Gear: ");
  Serial.println(currentGear);

  Serial.print("[ICM] RPM: ");
  Serial.println(currentRPM);

  Serial.print("[ICM] Velocity: ");
  Serial.println(currentVelocity);

  TerminateTask();
}
