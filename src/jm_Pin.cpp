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
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
	*/

/*

	jm_Pin - Class to emulate various kinds of pins like Open-Drain
	===============================================================

	Emulated modes
	===============
	OPEN_DRAIN: emulate a N-FET Open-Drain

	*/

/*
	wiring_digital.c
	================
	C:\arduino-1.0.3\hardware\arduino\cores\arduino\wiring_digital.c

	*/

#include <jm_Pin.h>

jm_Pin::jm_Pin(int8_t number, int8_t mode)
{
	setup(number, mode, false);
}

jm_Pin::jm_Pin(int8_t number, int8_t mode, bool reverse)
{
	setup(number, mode, reverse);
}

void jm_Pin::setup(int8_t number, int8_t mode)
{
	setup(number, mode, false);
}

void jm_Pin::setup(int8_t number, int8_t mode, bool reverse)
{
	_number = number;
	_mode = mode;
	_reverse = reverse;

	switch(_mode)
	{
	case INPUT:
		pinMode(_number, _mode);
		break;

	case OUTPUT:
		digitalWrite(_number, _reverse);
		pinMode(_number, _mode);
		break;

	case INPUT_PULLUP:
		pinMode(_number, _mode);
		break;

	case OPEN_DRAIN:
		digitalWrite(_number, 0);
		if (_reverse) {
			pinMode(_number, OUTPUT);
		} else {
			pinMode(_number, INPUT);
		}
		break;
	}
}

bool jm_Pin::input()
{
	switch(_mode)
	{
	case INPUT:
	case OUTPUT:
//	case INPUT_PULLUP:
		return (digitalRead(_number) ^ _reverse);
		break;

	case INPUT_PULLUP:
	case OPEN_DRAIN:
		return (!digitalRead(_number) ^ _reverse);
		break;
	}
}

void jm_Pin::on()
{
	switch(_mode)
	{
	case INPUT:
		break;

	case OUTPUT:
		digitalWrite(_number, !_reverse);
		break;

	case INPUT_PULLUP:
		break;

	case OPEN_DRAIN:
		if (!_reverse) {
			pinMode(_number, OUTPUT);
		} else {
			pinMode(_number, INPUT);
		}
		break;
	}
}

void jm_Pin::off()
{
	switch(_mode)
	{
	case INPUT:
		break;

	case OUTPUT:
		digitalWrite(_number, _reverse);
		break;

	case INPUT_PULLUP:
		break;

	case OPEN_DRAIN:
		if (_reverse) {
			pinMode(_number, OUTPUT);
		} else {
			pinMode(_number, INPUT);
		}
		break;
	}
}

void jm_Pin::output(bool state)
{
	if(state) on(); else off();
}

void jm_Pin::toggle()
{
	output(!input());
}

int8_t jm_Pin::number()
{
	return _number;
}

int8_t jm_Pin::mode()
{
	return _mode;
}
