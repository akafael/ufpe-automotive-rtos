/**
 * CAN Comunication using the MCP 2515 module
 */

#include "Arduino.h"
#include "tpl_os.h"
#include <SPI.h>
#include <mcp_can.h>

#define CAN_SPI_INT 2 // Set INT to pin 2
MCP_CAN CAN_SPI(10);  // Set CS to pin 10

void setup() {
  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the
  // masks and filters disabled.
  if (CAN_SPI.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN_SPI.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515
                               // sends acks to received data.

  pinMode(CAN_SPI_INT, INPUT); // Configuring pin for /INT input

  Serial.println("MCP2515 Library Receive Example...");
}

/**
 * Send msg through CAN
 */
TASK(periodicTask) {
  // If CAN_SPI_INT pin is low, read receive buffer
  if (!digitalRead(CAN_SPI_INT)) {
    long unsigned int rxId;
    unsigned char len = 0;
    unsigned char rxBuf[8];

    // Read data: len = data length, buf = data byte(s)
    CAN_SPI.readMsgBuf(&rxId, &len, rxBuf);

    char msgString[128]; // Array to store serial string

    if ((rxId & 0x80000000) == 0x80000000) // Determine if ID is standard (11
                                           // bits) or extended (29 bits)
      sprintf(msgString,
              "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF),
              len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId,
              len);

    Serial.print(msgString);

    if ((rxId & 0x40000000) ==
        0x40000000) { // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for (byte i = 0; i < len; i++) {
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }

    Serial.println();
  }

  TerminateTask();
}