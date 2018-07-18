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

	2018-07-18
	===============================================================

	Add port_register() to expand digital pins with various extra ports like I2C PCF8574

	*/

/*
	wiring_digital.c
	================
	C:\Arduino15\hardware\arduino\avr\cores\arduino\wiring_digital.c
	C:\Arduino15\hardware\arduino\avr\variants\standard\pins_arduino.h

	*/

#include <jm_Pin.h>

//------------------------------------------------------------------------------

uint8_t jm_Pin::_npins = NUM_DIGITAL_PINS;
jm_Pin_ext_pins * jm_Pin::_first = NULL;

void jm_Pin::pinMode(uint8_t pin, uint8_t mode)
{
	if (_obj == NULL)
		::pinMode(pin, mode);
	else {
		jm_Pin_ext_pins * p = jm_Pin::_first;
		while (p && p->obj != _obj) p = p->next;
		if (p) p->obj_pinMode(_obj, pin, mode);
	}
}

int jm_Pin::digitalRead(uint8_t pin)
{
	if (_obj == NULL)
		return ::digitalRead(pin);
	else {
		jm_Pin_ext_pins * p = jm_Pin::_first;
		while (p && p->obj != _obj) p = p->next;
		if (p) return p->obj_digitalRead(_obj, pin); else return -1;
	}
}

void jm_Pin::digitalWrite(uint8_t pin, uint8_t value)
{
	if (_obj == NULL)
		::digitalWrite(pin, value);
	else {
		jm_Pin_ext_pins * p = jm_Pin::_first;
		while (p && p->obj != _obj) p = p->next;
		if (p) p->obj_digitalWrite(_obj, pin, value);
	}
}

//------------------------------------------------------------------------------

jm_Pin::jm_Pin(uint8_t pin, uint8_t mode, bool reverse, bool state) :
	_superseded(false),
	_set_up(false),
	_obj(NULL)
{
	setup(pin, mode, reverse, state);
}

jm_Pin::jm_Pin(void * obj, uint8_t pin, uint8_t mode, bool reverse, bool state) :
	_superseded(false),
	_set_up(false),
	_obj(obj)
{
	setup(pin, mode, reverse, state);
}

void jm_Pin::setup(uint8_t pin, uint8_t mode, bool reverse, bool state)
{
	_set_up = false;

	_pin = pin;
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
		this->pinMode(_pin, INPUT);
		break;

	case OUTPUT:
		this->digitalWrite(_pin, _reverse ^ _state);
		this->pinMode(_pin, OUTPUT);
		break;

	case INPUT_PULLUP:
		this->pinMode(_pin, INPUT_PULLUP);
		break;

	case OPEN_DRAIN:
		if (_reverse ^ _state) {
			this->digitalWrite(_pin, 0);
			this->pinMode(_pin, OUTPUT);
		} else {
			this->pinMode(_pin, INPUT);
			this->digitalWrite(_pin, 0);
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
		return (this->digitalRead(_pin) ^ _reverse);

	case INPUT_PULLUP:
	case OPEN_DRAIN:
		return (!this->digitalRead(_pin) ^ _reverse);

	default:
		return false;
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
		this->digitalWrite(_pin, (_reverse ^ _state));
		break;

	case INPUT_PULLUP:
		break;

	case OPEN_DRAIN:
		if (_reverse ^ _state) {
			this->pinMode(_pin, OUTPUT);
		} else {
			this->pinMode(_pin, INPUT);
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

uint8_t jm_Pin::pin()
{
	return _pin;
}

uint8_t jm_Pin::mode()
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

void * jm_Pin::port_register(void * obj, vfp_vpu8u8_t obj_pinMode, ifp_vpu8_t obj_digitalRead, vfp_vpu8u8_t obj_digitalWrite)
{
#if 0
	jm_Pin_ext_pins * p = (jm_Pin_ext_pins *) malloc(sizeof(jm_Pin_ext_pins));

	p->next = NULL;
	p->obj = obj;
	p->obj_pinMode = obj_pinMode;
	p->obj_digitalRead = obj_digitalRead;
	p->obj_digitalWrite = obj_digitalWrite;

	jm_Pin_ext_pins * p1 = jm_Pin::_first;
	jm_Pin_ext_pins * p2 = NULL;
	while (p1) {
		p2 = p1;
		p1 = p1->next;
	}
	if (jm_Pin::_first)
		p2->next = p;
	else
		jm_Pin::_first = p;
#else
	jm_Pin_ext_pins * p1 = jm_Pin::_first;
	jm_Pin_ext_pins * p2 = NULL;
	while (p1 && p1->obj != obj) {
		p2 = p1;
		p1 = p1->next;
	}
	if (p1 == NULL) {
		jm_Pin_ext_pins * p = (jm_Pin_ext_pins *) malloc(sizeof(jm_Pin_ext_pins));

		p->next = NULL;
		p->obj = obj;
		p->obj_pinMode = obj_pinMode;
		p->obj_digitalRead = obj_digitalRead;
		p->obj_digitalWrite = obj_digitalWrite;

		if (jm_Pin::_first == NULL)
			jm_Pin::_first = p;
		else
			p2->next = p;

		p1 = p;
	}
#endif

	return obj;
}

//------------------------------------------------------------------------------

// END.