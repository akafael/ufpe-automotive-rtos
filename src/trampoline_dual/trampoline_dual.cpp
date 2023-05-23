/**
 * Dual Process Blink
 *  one process turn the light on and other turn the light off.
 *  see: oil for time schedule specs
 */

#include "tpl_os.h"
#include "Arduino.h"

#define PIN_LED 13

void setup()
{
	// initialize digital pin 13 as an output.
	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, LOW);
}


// Turn ON the LED
TASK(periodicTaskPing)
{
	digitalWrite(PIN_LED, HIGH);
	TerminateTask();
}

// Turn Off the LED
TASK(periodicTaskPong)
{
	digitalWrite(PIN_LED, LOW);
	TerminateTask();
}

