// see blink.oil head of file for information about the compilation process.

#include "Arduino.h"
#include "tpl_os.h"

const int maxBufferSize = 10;
int buffer = 0;

void setup() {
  Serial.begin(115200); // 115200 bps, 8N1
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

TASK(periodicTaskProducer) {
  buffer = (buffer < maxBufferSize) ? buffer + 1 : maxBufferSize;

  Serial.print("P: ");
  Serial.println(buffer);

  TerminateTask();
}

TASK(periodicTaskConsumer) {
  buffer = (buffer > 0) ? buffer - 1 : buffer;

  Serial.print("C: ");
  Serial.println(buffer);

  TerminateTask();
}
