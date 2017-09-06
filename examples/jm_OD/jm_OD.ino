/*

	jm_OD demonstrates the capability of emulating Open-Drain on Arduino pins.
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

	Some delay(1) [ms] are inserted before pin4.input(), just after pin4.on(),
	pin4.off() or pin4.toggle(). It's because the pin capacitance after power
	OFF the LED is not immediatly discharged. The voltage decreases with an
	exponentiel law. A delay of 1 [ms] if too much, 1 or 2 [us] could be
	suffisant. Remark: the delay is unnecessary after power ON because the
	pin capacitance is charged in less than a fraction of an [us].

	DIRECT CONNECTION OF A BUTTON ONTO AN OUTPUT PIN IS PROHIBITED
	WITHOUT TAKING APPROPRIATE SOFTWARE OR ELECTRONIC PRECAUTIONS.
	THE WHOLE IC CAN BE PERMANENTLY DAMAGED BY A SHORT-CIRCUIT.
	BE SURE TO UNDERSTAND THE SCHEMA AND THE PROGRAM BEFORE TEST IT.

	Schema: http://

	*/

#include <jm_Pin.h>

jm_Pin pin4(4, OPEN_DRAIN);

bool state = false;
word time0 = 0;

void setup()
{
	pinMode(13, OUTPUT);

	time0 = millis();
}

void loop()
{
	state = !state;
	pin4.output(state);

	do
		digitalWrite(13, pin4.input());
	while ((word)millis() - time0 <= 500);

	time0 += 500;
}
