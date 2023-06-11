/**
 * TCM - Emulate an Gearbox transmision ECU
 *  - Read gear value from Serial Input
 *  - Pack valid gear value inside CAN frame
 *  - Send CAN msg with gear value using MCP-2515 connected to SPI
 */

#include "Arduino.h"
#include "tpl_os.h"
#include <SPI.h>
#include <mcp_can.h>

#include "../project_lib/Board.h"
#include "../project_lib/Utils.hpp"

#define PIN_CAN_SPI_INT 2 // PIN used by MCP2515 to for msg available
#define PIN_CAN_CS 10     // PIN used as chip selector by MCP2515

MCP_CAN CAN_SPI(10); // MCP2515 CAN controler with SPI interface

uint8_t currentGear = 0; // Current Gear number: valid numbers are [0-5]

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  // Start communication with MCP2515
  if (CAN_SPI.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  // CAN Settings
  pinMode(PIN_CAN_SPI_INT, INPUT); // Configuring pin for /INT input
  CAN_SPI.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515
                               // sends acks to received msgSerialized.
}

/**
 * Read a byte and store
 *  - turn led on to sign reading
 */
TASK(periodicTaskReadSerial) {
  if (Serial.available() > 0) {
    // Raw input from user simulating a sensor read
    const int8_t rawSensorData = Serial.parseInt();

    // Store new gear data
    if (rawSensorData != 0) {
      currentGear = rawSensorData;
    }
  }

  TerminateTask();
}

/**
 * Send msg with gear value
 */
TASK(periodicTaskSendGearMsg) {
  union msgSerialized {
    uint16_t rawData;
    uint8_t bytes[2];
  } gearData; // Gear Value Message Serialized

  gearData.rawData = currentGear;
  CAN_SPI.sendMsgBuf(IdMsgRPM, 0, sizeof(gearData), gearData.bytes);

  Serial.print("Gear: ");
  Serial.println(currentGear);

  TerminateTask();
}
