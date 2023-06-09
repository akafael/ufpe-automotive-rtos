// CAN Send Example
//

#include <SPI.h>
#include <mcp_can.h>

MCP_CAN CAN0(10); // Set CS to pin 10

void setup() {
  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the
  // masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);
}

void loop() {
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' =
  // array of data bytes to send
  static byte data[8] = {0x00, 0x03, 0x03, 0x03, 0x04, 0x05, 0x06, 0x07};
  byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
  if (sndStat == CAN_OK) {
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(100); // send data per 100ms
}
