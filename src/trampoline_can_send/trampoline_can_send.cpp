/**
 * CAN Comunication using the MCP 2515 module
 */

#include "Arduino.h"
#include "tpl_os.h"
#include <SPI.h>
#include <mcp_can.h>

#define PIN_CAN_CS 10

byte mDATA[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
MCP_CAN CAN_DEV(PIN_CAN_CS);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (CAN_DEV.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    delay(500);
    Serial.println("Erro ao inicializar o MCP2515...");
  }
  Serial.println("MCP2515 can_send inicializado com sucesso!");
  CAN_DEV.setMode(MCP_NORMAL);
}

/**
 * Send msg through CAN
 */
TASK(periodicTask) {
  byte sndStat = CAN_DEV.sendMsgBuf(0x100, CAN_STDID, 8, mDATA);

  if (sndStat == CAN_OK) {
    Serial.println("Mensagem enviada com sucesso!");
  } else {
    Serial.println("Erro para enviar a mensagem...");
  }

  TerminateTask();
}