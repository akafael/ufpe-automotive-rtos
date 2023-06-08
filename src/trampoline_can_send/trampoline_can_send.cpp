/**
 * CAN Comunication using the MCP 2515 module
 *  ref:
 * https://ww1.microchip.com/downloads/en/DeviceDoc/MCP2515-Stand-Alone-CAN-Controller-with-SPI-20001801J.pdf
 */

#include "Arduino.h"
#include "tpl_os.h"
#include <SPI.h>
#include <mcp_can.h>

MCP_CAN CAN_SPI(10); // Set CS to pin 10

void setup() {
  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the
  // masks and filters disabled.
  if (CAN_SPI.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN_SPI.setMode(MCP_NORMAL);
}

/**
 * Initialize comunication with MCP 2515 module
 */
TASK(taskStartMCP) {
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the
  // masks and filters disabled.
  if (CAN_SPI.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  TerminateTask();
}

/**
 * Send msg through CAN
 */
TASK(periodicTask) {
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' =
  // array of data bytes to send
  static byte data[8] = {0x00, 0x01, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07};
  byte sndStat = CAN_SPI.sendMsgBuf(0x100, 0, 8, data);
  if (sndStat == CAN_OK) {
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }

  TerminateTask();
}