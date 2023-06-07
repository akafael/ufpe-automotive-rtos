//////////////////////////////////////////////////////////////////////////////////
// Universidade de Brasilia (UnB)
// Faculdade do Gama
// Curso: Engenharia Automotiva
// Disciplina : Engenharia de Software Automotivo
//////////////////////////////////////////////////////////////////////////////////
#include "Board.h"
#include <SPI.h>
#include <mcp_can.h>

#define PIN_CAN_CS 10
#define CAN_INT 2

// Variavel para armazenar informacoes do frame recebido
byte mDATA[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
MCP_CAN CAN1(PIN_CAN_CS);
void setup() {
  // Inicializa a interface serial : baudrate = 115200
  Serial.begin(115200);
  // Inicializa o controlador CAN : baudrate = 500K, clock=8MHz
  // Mascaras e filtros desabilitados .
  if (CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK) {
    delay(500);
  }
  Serial.println("MCP2515 can_send inicializado com sucesso!");
  // Modifica para o modo normal de operação
  CAN1.setMode(MCP_NORMAL);
}
void loop() {
  // Envia um frame de dados: ID = 0x100, Std, DLC = 8 bytes,
  // mDATA = array de dados a ser enviado
  byte sndStat = CAN1.sendMsgBuf(0x100, CAN_STDID, 8, mDATA);
  if (sndStat == CAN_OK) {
    Serial.println("Mensagem enviada com sucesso!");
  } else {
    Serial.println("Erro para enviar a mensagem...");
  }
  // Aguarda por 500ms
  delay(500);
}
