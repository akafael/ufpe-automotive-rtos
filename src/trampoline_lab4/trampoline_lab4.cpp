// see blink.oil head of file for information about the compilation process.

#include "tpl_os.h"
#include "Arduino.h"

void setup()
{
	Serial.begin(115200); //115200 bps, 8N1
	// initialize digital pin 13 as an output.
	pinMode(13, OUTPUT);
}

TASK(periodicTaskT1)
{
	Serial.println("T1");
	TerminateTask();
}

TASK(periodicTaskT2)
{
	Serial.println("T2");
	TerminateTask();
}


TASK(periodicTaskT3)
{
	Serial.println("T3");
	TerminateTask();
}

TASK(periodicTaskT4)
{
	Serial.println("T4");
	TerminateTask();
}
