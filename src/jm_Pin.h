/*
	jm_Pin.cpp - An Arduino library to emulate various kinds of pins like Open-Drain
	Copyright (c) 2017 Jean-Marc Paratte. All right reserved.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

	*/

/*

	jm_Pin - Class to emulate various kinds of pins like Open-Drain
	===============================================================

	Emulated modes
	==============
	OPEN_DRAIN: emulate a N-FET Open-Drain

	*/

#ifndef jm_Pin_h
#define jm_Pin_h

#ifdef assert
#else
#	ifdef NDEBUG
#		define assert(e) ((void)0)
#	else
#		define assert(e) ((e) ? (void)0 : abort())
#	endif
#endif

#ifndef voidfuncptr_t
typedef void (*voidfuncptr_t)(void); // void function pointer typedef
#endif

#include <Arduino.h>

#if INPUT != 0 || OUTPUT != 1 || INPUT_PULLUP != 2 || defined OPEN_DRAIN
#error Incompatible Arduino board (OPEN_DRAIN already defined).
#endif

#define OPEN_DRAIN (3) // new emulated Open-Drain output mode

class jm_Pin
{
protected:

	bool _superseded;	// pin temporarily superseded by hardware peripherals like SPI or I2C
	bool _set_up;		// pin setup() performed

	int8_t _number;		// pin number (0..13, A0..A5, ...)
	int8_t _mode;		// mode (INPUT|OUTPUT|INPUT_PULLUP|OPEN_DRAIN)
	bool _reverse;		// reverse hardware state (false|true)
	bool _state;		// logical state (false|true)

	void setup1();

public:

	jm_Pin(int8_t number, int8_t mode=INPUT, bool reverse=false, bool state=false);

	void setup(int8_t number, int8_t mode=INPUT, bool reverse=false, bool state=false);

	bool input();
	void output(bool state);

	void on();
	void off();
	void toggle();

	int8_t number();
	int8_t mode();
	bool reverse();
	bool state();

	void supersede(bool value);
	bool superseded();
};

#endif // jm_Pin_h
