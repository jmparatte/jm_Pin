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

/*
	wiring_digital.c
	================
	C:\arduino-1.0.3\hardware\arduino\cores\arduino\wiring_digital.c

	*/

#include <jm_Pin.h>

//------------------------------------------------------------------------------

jm_Pin::jm_Pin(int8_t number, int8_t mode=INPUT, bool reverse=false, bool state=false) :
	_superseded(false),
	_set_up(false)
{
	setup(number, mode, reverse, state);
}

void jm_Pin::setup(int8_t number, int8_t mode=INPUT, bool reverse=false, bool state=false)
{
	_set_up = false;

	_number = number;
	_mode = mode;
	_reverse = reverse;
	_state = state;

	if (_superseded) return;
	setup1();
}

void jm_Pin::setup1()
{
	switch(_mode)
	{
	case INPUT:
		pinMode(_number, INPUT);
		break;

	case OUTPUT:
		digitalWrite(_number, _reverse ^ _state);
		pinMode(_number, OUTPUT);
		break;

	case INPUT_PULLUP:
		pinMode(_number, INPUT_PULLUP);
		break;

	case OPEN_DRAIN:
		if (_reverse ^ _state) {
			digitalWrite(_number, 0);
			pinMode(_number, OUTPUT);
		} else {
			pinMode(_number, INPUT);
			digitalWrite(_number, 0);
		}
		break;
	}

	_set_up = true;
}

//------------------------------------------------------------------------------

bool jm_Pin::input()
{
	switch(_mode)
	{
	case INPUT:
	case OUTPUT:
		return (digitalRead(_number) ^ _reverse);
		break;

	case INPUT_PULLUP:
	case OPEN_DRAIN:
		return (!digitalRead(_number) ^ _reverse);
		break;
	}
}

void jm_Pin::output(bool state)
{
	_state = state;

	if (_superseded) return;
	if (!_set_up) setup1();

	switch(_mode)
	{
	case INPUT:
		break;

	case OUTPUT:
		digitalWrite(_number, (_reverse ^ _state));
		break;

	case INPUT_PULLUP:
		break;

	case OPEN_DRAIN:
		if (_reverse ^ _state) {
			pinMode(_number, OUTPUT);
		} else {
			pinMode(_number, INPUT);
		}
		break;
	}
}

//------------------------------------------------------------------------------

void jm_Pin::on()
{
	output(true);
}

void jm_Pin::off()
{
	output(false);
}

void jm_Pin::toggle()
{
	output(!_state);
}

//------------------------------------------------------------------------------

int8_t jm_Pin::number()
{
	return _number;
}

int8_t jm_Pin::mode()
{
	return _mode;
}

bool jm_Pin::reverse()
{
	return _reverse;
}

bool jm_Pin::state()
{
	return _state;
}

//------------------------------------------------------------------------------

void jm_Pin::supersede(bool value)
{
	if (value != _superseded) {
		_superseded = value;
		if (value) {
			_set_up = false;
		} else {
			setup1();
		}
	}
}

bool jm_Pin::superseded()
{
	return _superseded;
}

//------------------------------------------------------------------------------

// END.