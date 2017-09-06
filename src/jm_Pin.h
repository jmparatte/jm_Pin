/*
	jm_Pin.cpp - Arduino library to emulate various kinds of pins like Open-Drain
	Copyright (c) 2017 Jean-Marc Paratte.  All right reserved.

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
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
	*/

/*

	jm_Pin - Class to emulate various kinds of pins like Open-Drain
	===============================================================

	Emulated modes
	===============
	OPEN_DRAIN: emulate a N-FET Open-Drain

	*/

#ifndef jm_Pin_h
#define jm_Pin_h

#include <Arduino.h>

#if INPUT != 0 || OUTPUT != 1 || INPUT_PULLUP != 2 || defined OPEN_DRAIN
#error Incompatible Arduino board.
#endif

#define OPEN_DRAIN (3) // new emulated Open-Drain output mode

class jm_Pin
{
private:

	int8_t _number;
	int8_t _mode;
	bool _reverse;

public:

	jm_Pin(int8_t number, int8_t mode);

	jm_Pin(int8_t number, int8_t mode, bool reverse);

	void setup(int8_t number, int8_t mode);

	void setup(int8_t number, int8_t mode, bool reverse);

	bool input();

	void on();

	void off();

	void output(bool state);

	void toggle();

	int8_t number();

	int8_t mode();
};

#endif
