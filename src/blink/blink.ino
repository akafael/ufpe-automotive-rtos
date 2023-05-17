////////////////////////////////////////////////////////////////////////////////
// Automotive Network Evaluation Board v1.0 (Arquivo Blink.h)
// Pisca um led a cada 1 segundo
// Exemplo de domínio público
// https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
////////////////////////////////////////////////////////////////////////////////
#include "Board.h"
// Executa somente uma vez após pressionar o botão reset ou ligar

int stateLed = HIGH;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

//Executa continuamente
void loop() {
  stateLed = !stateLed;

  digitalWrite (LED_BUILTIN, stateLed);
  delay(150);
}

