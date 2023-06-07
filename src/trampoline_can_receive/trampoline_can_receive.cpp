/**
 * CAN Comunication using the MCP 2515 module
 */

#include "Arduino.h"
#include "tpl_os.h"
#include <SPI.h>
#include <mcp_can.h>

#define PIN_CAN_CS 10
#define PIN_CAN_INT 2

// Variavel para armazenar informacoes do frame recebido
unsigned char mDLC = 0;
unsigned char mDATA[8];
long unsigned int mID;
char msgString[128];

MCP_CAN CAN_DEV(PIN_CAN_CS);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  if (CAN_DEV.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    delay(200);
  }
  Serial.println("Modulo CAN inicializado!");

  CAN_DEV.setMode(MCP_NORMAL);

  pinMode(PIN_CAN_INT, INPUT);

  Serial.println("MCP2515 exemplo can_receive...");
  Serial.print("ID\t\tType\tDLC\tByte0\tByte1\tByte2");
  Serial.println("\tByte3\tByte4\tByte5\tByte6\tByte7");
}

/**
 * Send msg through CAN
 */
TASK(periodicTask) {
  // Se uma interrupção ocorreu interrupção (PIN_CAN_INT pino = 0), lê o buffer
  // de recepção
  if (!digitalRead(PIN_CAN_INT)) {
    // Lê os dados: mID = identificador, mDLC = comprimento, mDATA = dados do
    // frame
    CAN_DEV.readMsgBuf(&mID, &mDLC, mDATA);
    // Determina se o frame é do tipo standard (11 bits) ou estendido (29 bits)
    if ((mID & CAN_IS_EXTENDED) == CAN_IS_EXTENDED) {
      sprintf(msgString, "0x%.8lX\t\tExt\t[%1d]\t", (mID & CAN_EXTENDED_ID),
              mDLC);
    } else {
      sprintf(msgString, "0x%.3lX\t\tStd\t[%1d]\t", mID, mDLC);
    }
    Serial.print(msgString);
    // Verifica se a mensagem é do tipo de requisição remota.
    if ((mID & CAN_IS_REMOTE_REQUEST) == CAN_IS_REMOTE_REQUEST) {
      sprintf(msgString, "rrf");
      Serial.print(msgString);
    } else {
      for (byte i = 0; i < mDLC; i++) {
        sprintf(msgString, "0x%.2X\t", mDATA[i]);
        Serial.print(msgString);
      }
    }
    Serial.println();
  }

  TerminateTask();
}