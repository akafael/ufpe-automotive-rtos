/**
 * 4 Process example
 *  T1,T2,T3 process print how often they were executed
 *  T4 process print how often T1,T2,T3 were executed
 */

#include "Arduino.h"
#include "tpl_os.h"

enum TASKS { T1 = 0, T2 = 1, T3 = 2, T4 = 3 };
int taskState[4] = {0, 0, 0, 0};

void setup() {
  Serial.begin(115200); // 115200 bps, 8N1
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

TASK(periodicTaskT1) {
  taskState[T1] += 1;
  Serial.print("T1: ");
  Serial.println(taskState[T1]);

  TerminateTask();
}

TASK(periodicTaskT2) {
  taskState[T2] += 1;
  Serial.print("T2: ");
  Serial.println(taskState[T2]);

  TerminateTask();
}

TASK(periodicTaskT3) {
  taskState[T3] += 1;
  Serial.print("T3: ");
  Serial.println(taskState[T3]);

  TerminateTask();
}

TASK(periodicTaskT4) {
  taskState[T4] += 1;
  Serial.print("T1: ");
  Serial.print(taskState[T1]);
  Serial.print(" , T2: ");
  Serial.print(taskState[T2]);
  Serial.print(" , T3: ");
  Serial.println(taskState[T3]);

  TerminateTask();
}
