/**
 * CAN Comunication using the MCP 2515 module
 */

#include "Arduino.h"
#include "tpl_os.h"
#include <SPI.h>
#include <mcp_can.h>

#define PIN_SPI_CS_CAN 10

MCP_CAN CAN_0(PIN_SPI_CS_CAN);

void setup() {
  Serial.begin(115200); // 115200 bps, 8N1
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

// The TASK is activated by the alarm "periodicAl":
//* The alarm "periodicAl" is configured in the .oil file to activate task
//"periodicTask" each 1000 pulses of counter SystemCounter.
//* The SystemCounter is the same as the systick used in Arduino, a tick each
// 1024us on a 16MHz arduino.
//* This task is periodic, with a period of 1024ms.
TASK(periodicTask) {
  static unsigned int nb = 0;
  nb++;
  if (nb & 1)
    digitalWrite(13, HIGH); // odd
  else
    digitalWrite(13, LOW); // even
  TerminateTask();
}

void sendCanMsg(int data) {}