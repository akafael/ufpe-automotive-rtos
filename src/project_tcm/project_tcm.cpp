/**
 * Small program to read serial input and print the latest byte read - cpp
 */

#include "Arduino.h"
#include "tpl_os.h"
#include <SPI.h>
#include <mcp_can.h>

#include "project_ecm.hpp"

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
    int8_t gear = Serial.parseInt();
  }

  TerminateTask();
}
/**
 * Send msg with gear value
 */
TASK(periodicTaskSendRPMMsg) {
  union {
    uint16_t value;
    uint8_t bytes[2];
  } data;

  data.value = currentRPM;
  CAN_SPI.sendMsgBuf(IdMsgRPM, 0, 2, data.bytes);

  Serial.print("RPM: ");
  Serial.println(currentRPM);

  TerminateTask();
}

