//////////////////////////////////////////////////////////////////////////////////
// Universidade de Brasilia (UnB)
// Faculdade do Gama
// Curso: Engenharia Automotiva
// Disciplina : Engenharia de Software Automotivo
//////////////////////////////////////////////////////////////////////////////////
#include <mcp_can.h>
#include <SPI.h>
#include "Board.h"
// Variavel para armazenar informacoes do frame recebido
unsigned char mDLC = 0;
unsigned char mDATA[8];
long unsigned int mID;
char msgString[128];
MCP_CAN CAN1(CAN1_CS);

void setup()
{
    // Inicializa a interface serial : baudrate = 115200
    Serial.begin(115200);
    // Inicializa o controlador CAN : baudrate = 500K, clock=8MHz
    // Mascaras e filtros desabilitados .
    if (CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) != CAN_OK)
    {
        delay(200);
    }
    Serial.println("Modulo CAN inicializado!");
    // Configura o controlador MCP2515 no modo normal.
    CAN1.setMode(MCP_NORMAL);
    // Configura o pino de interrupção para recepção
    pinMode(CAN_INT, INPUT);
    Serial.println("MCP2515 exemplo can_receive...");
    Serial.print("ID\t\tType\tDLC\tByte0\tByte1\tByte2");
    Serial.println("\tByte3\tByte4\tByte5\tByte6\tByte7");
}
void loop()
{
    // Se uma interrupção ocorreu interrupção (CAN_INT pino = 0), lê o buffer de recepção
    if (!digitalRead(CAN_INT))
    {
        // Lê os dados: mID = identificador, mDLC = comprimento, mDATA = dados do frame
        CAN1.readMsgBuf(&mID, &mDLC, mDATA);
        // Determina se o frame é do tipo standard (11 bits) ou estendido (29 bits)
        if ((mID & CAN_IS_EXTENDED) == CAN_IS_EXTENDED)
        {
            sprintf(msgString, "0x%.8lX\t\tExt\t[%1d]\t", (mID & CAN_EXTENDED_ID),
                    mDLC);
        }
        else
        {
            sprintf(msgString, "0x%.3lX\t\tStd\t[%1d]\t", mID, mDLC);
        }
        Serial.print(msgString);
        // Verifica se a mensagem é do tipo de requisição remota.
        if ((mID & CAN_IS_REMOTE_REQUEST) == CAN_IS_REMOTE_REQUEST)
        {
            sprintf(msgString, "rrf");
            Serial.print(msgString);
        }
        else
        {
            for (byte i = 0; i < mDLC; i++)
            {
                sprintf(msgString, "0x%.2X\t", mDATA[i]);
                Serial.print(msgString);
            }
        }
        Serial.println();
    }
}