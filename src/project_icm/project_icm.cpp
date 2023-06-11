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

TASK(periodicTaskReadCANMsg) {
  // If CAN_SPI_INT pin is low, read receive buffer
  if (!digitalRead(CAN_SPI_INT)) {
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
      // currentRPM = eval_rpm(rxBuffer[0]);
    } else if (msgId == IdMsgVelocity) {
      // currentVelocity = calc_velocity(rxBuffer[0]);
    }
  }

  TerminateTask();
}

TASK(periodicTaskPrint) {
  Serial.print("Gear: ");
  Serial.println(currentGear);

  Serial.print("RPM: ");
  Serial.println(currentRPM);

  Serial.print("Velocity: ");
  Serial.println(currentVelocity);

  TerminateTask();
}
