/*
	jm_Pin.cpp - An Arduino library to emulate various kinds of pins like Open-Drain
	Copyright (c) 2018,2017 Jean-Marc Paratte. All right reserved.

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

#ifndef jm_Pin_h
#define jm_Pin_h

#include <inttypes.h>

//----------------------------------------------------------------------

#ifndef voidfuncptr_t
#define voidfuncptr_t voidfuncptr_t
typedef void (*voidfuncptr_t)(void); // void function pointer typedef
#endif

#ifndef vfp_vpu32b_t
#define vfp_vpu32b_t vfp_vpu32b_t
typedef void (*vfp_vpu32b_t)(void*, uint32_t, bool);
#endif

/*
void jm_PCF8574::obj_pinMode(void * obj, uint8_t pin, uint8_t mode)
{
	((jm_PCF8574*)obj)->pinMode(pin, mode);
}

int jm_PCF8574::obj_digitalRead(void * obj, uint8_t pin)
{
	return ((jm_PCF8574*)obj)->digitalRead(pin);
}

void jm_PCF8574::obj_digitalWrite(void * obj, uint8_t pin, uint8_t value)
{
	((jm_PCF8574*)obj)->digitalWrite(pin, value);
}
*/

#ifndef vfp_vpu8u8_t
#define vfp_vpu8u8_t vfp_vpu8u8_t
typedef void (*vfp_vpu8u8_t)(void*, uint8_t, uint8_t);
#endif

#ifndef ifp_vpu8_t
#define ifp_vpu8_t ifp_vpu8_t
typedef int (*ifp_vpu8_t)(void*, uint8_t);
#endif

//----------------------------------------------------------------------

#ifdef assert
#else
#	ifdef NDEBUG
#		define assert(e) ((void)0)
#	else
#		define assert(e) ((e) ? (void)0 : abort())
#	endif
#endif

//----------------------------------------------------------------------

#include <Arduino.h>

#if INPUT != 0 || OUTPUT != 1 || INPUT_PULLUP != 2 || defined OPEN_DRAIN
#error Incompatible Arduino board (OPEN_DRAIN already defined).
#endif

#define OPEN_DRAIN (3) // new emulated Open-Drain output mode

struct jm_Pin_ext_pins {
	jm_Pin_ext_pins * next;
	void * obj;
	vfp_vpu8u8_t obj_pinMode;
	ifp_vpu8_t obj_digitalRead;
	vfp_vpu8u8_t obj_digitalWrite;
};

class jm_Pin
{
protected:

	bool _superseded;	// pin temporarily superseded by hardware peripherals like SPI or I2C
	bool _set_up;		// pin setup() executed

	uint8_t _pin;		// pin number (0..13, A0..A5, ...)
	uint8_t _mode;		// mode (INPUT|OUTPUT|INPUT_PULLUP|OPEN_DRAIN)
	bool _reverse;		// reverse hardware state (false|true)
	bool _state;		// logical state (false|true)

	void setup1();

	static uint8_t _npins;
	static jm_Pin_ext_pins * _first;

	void * _obj;

	void pinMode(uint8_t pin, uint8_t mode);
	int digitalRead(uint8_t pin);
	void digitalWrite(uint8_t pin, uint8_t value);

public:

	jm_Pin(uint8_t pin, uint8_t mode=INPUT, bool reverse=false, bool state=false);
	jm_Pin(void * obj, uint8_t pin, uint8_t mode=INPUT, bool reverse=false, bool state=false);

	void setup(uint8_t pin, uint8_t mode=INPUT, bool reverse=false, bool state=false);

	bool input();
	void output(bool state);

	void on();
	void off();
	void toggle();

	uint8_t pin();
	uint8_t mode();
	bool reverse();
	bool state();

	void supersede(bool value);
	bool superseded();

	static void * port_register(void * obj, vfp_vpu8u8_t obj_pinMode, ifp_vpu8_t obj_digitalRead, vfp_vpu8u8_t obj_digitalWrite);
};

#endif // jm_Pin_h
