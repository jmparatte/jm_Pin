/*
	jm_Pin_OpenDrain.ino demonstrates the capability of emulating Open-Drain on Arduino pins.
	Copyright (c) 2017 Jean-Marc Paratte.  All right reserved.

	A LED is connected between the "emulated OD pin" and the +5V. The LED is
	controlled with a negative logic level: a GND signal lights the LED, a
	+5V signal clears the LED.

	A push button is connected between the "emulated OD pin" and the GND
	to demonstrate the OD capability without damaging the microcontroller.

	So the LED is controlled by 2 ways: the OD pin or the push button.
	It's a logical OR-wired circuit operating with negative logic level.
	To clear the LED, both pin and button must be OFF.

	For verification purpose, the state ON or OFF of the "emulated OD pin" is
	copied onto the pin 13 LED, which is controlled with positive logic level.

	DIRECT CONNECTION OF A BUTTON ONTO AN OUTPUT PIN IS PROHIBITED
	WITHOUT TAKING APPROPRIATE SOFTWARE OR ELECTRONIC PRECAUTIONS.
	THE WHOLE IC CAN BE PERMANENTLY DAMAGED BY A SHORT-CIRCUIT.
	BE SURE TO UNDERSTAND THE SCHEMA AND THE PROGRAM BEFORE TEST IT.

	Schema: https://github.com/jmparatte/jm_Pin/blob/master/examples/jm_OD/jm_OD.pdf

	*/

#include <jm_Pin.h>

jm_Pin Pin4(4, OPEN_DRAIN);

word time0;
#define time1 1000 // milliseconds

void setup()
{
	pinMode(13, OUTPUT);

	time0 = (word)millis();
}

void loop()
{
	Pin4.toggle(); // toggle the Pin4 logical output state
	// The push button has not effect on the Pin4 logical output state

	do
#if 1
		digitalWrite(13, Pin4.state()); // show the Pin4 logical output state
		// The push button has not effect on the Pin4 logical output state.
		// The LED13 blinks at 0.5Hz.
		// The Pin4 LED displays the OR-Wired logical state.
#else
		digitalWrite(13, Pin4.input()); // show the Pin4 Or-Wired state (Pin4 OR button)
		// The push button is OR-Wired with the Pin4 logical output state.
		// The LED13 and Pin4 LED display together the OR-Wired logical state.
#endif
	while (((word)millis() - time0) < time1);

	time0 += time1;
}
