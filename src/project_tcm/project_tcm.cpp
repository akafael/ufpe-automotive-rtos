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

MCP_CAN CAN_SPI(CAN1_CS_PIN); // MCP2515 CAN controler with SPI interface

uint8_t currentGear = 0; // Current Gear number: valid numbers are [0-5]

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  // Start communication with MCP2515
  if (CAN_SPI.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("[TCM] MCP2515 Initialized Successfully!");
  else
    Serial.println("[TCM] Error Initializing MCP2515...");

  // CAN Settings
  pinMode(CAN_INT_PIN, INPUT); // Configuring pin for /INT input
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
      currentGear = eval_gear(rawSensorData);
    }
  }

  TerminateTask();
}

/**
 * Send msg with gear value
 */
TASK(periodicTaskSendGearMsg) {
  union msgSerialized {
    uint8_t rawData;
    uint8_t bytes[1];
  } gearData; // Gear Value Message Serialized

  gearData.rawData = encode_gearData(currentGear);
  CAN_SPI.sendMsgBuf(IdMsgGear, CAN_EXTID, sizeof(gearData), gearData.bytes);

  Serial.print("[TCM] Gear: ");
  Serial.println(currentGear);

  TerminateTask();
}
