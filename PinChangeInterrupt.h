/*
Copyright (c) 2014-2015 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#ifndef PINCHANGEINTERRUPT_H
#define PINCHANGEINTERRUPT_H

#include "Arduino.h"

//================================================================================
// Settings
//================================================================================

/* Settings to de/activate ports/pins
This will save you flash and ram because the arrays
are managed dynamically with the definitions below.
Make sure you still have all needed ports activated.
Each deactivated port saves 3 bytes of ram.
If you deactivate the whole port,
you dont need to deactivate the pins.
Same for the port if you deactivate all 8 pins.
You dont have to deactivate pins/ports that dont exist.
That is done by the macros. */

#define PCINT_ENABLE_PORT0
#define PCINT_ENABLE_PORT1
#define PCINT_ENABLE_PORT2

#define PCINT_ENABLE_PCINT0
#define PCINT_ENABLE_PCINT1
#define PCINT_ENABLE_PCINT2
#define PCINT_ENABLE_PCINT3
#define PCINT_ENABLE_PCINT4
#define PCINT_ENABLE_PCINT5
#define PCINT_ENABLE_PCINT6
#define PCINT_ENABLE_PCINT7
#define PCINT_ENABLE_PCINT8
#define PCINT_ENABLE_PCINT9
#define PCINT_ENABLE_PCINT10
#define PCINT_ENABLE_PCINT11
#define PCINT_ENABLE_PCINT12
#define PCINT_ENABLE_PCINT13
#define PCINT_ENABLE_PCINT14
#define PCINT_ENABLE_PCINT15
#define PCINT_ENABLE_PCINT16
#define PCINT_ENABLE_PCINT17
#define PCINT_ENABLE_PCINT18
#define PCINT_ENABLE_PCINT19
#define PCINT_ENABLE_PCINT20
#define PCINT_ENABLE_PCINT21
#define PCINT_ENABLE_PCINT22
#define PCINT_ENABLE_PCINT23

//================================================================================
// General Helper Definitions and Mappings
//================================================================================

// Board definitions are seperated to get an better overview.
#include "PinChangeInterruptBoards.h"

#if !PCINT_NUM_USED_PORTS
#error Please enable at least one PCINT port!
#endif

// map the port to the array position, depending on what ports are activated. this is only usable with port 0-2, not 3
#define PCINT_ARRAY_POS(p) ((PCINT_NUM_USED_PORTS == 3) ? p : (PCINT_NUM_USED_PORTS == 1) ? 0 : \
/*(PCINT_NUM_USED_PORTS == 2)*/ (PCINT_USE_PORT2 == 0) ? p : (PCINT_USE_PORT0 == 0) ? (p - 1) : \
/*(PCINT_USE_PORT1 == 0)*/ ((p >> 1) & 0x01))

// only check enabled + physically available ports. Always choose the port if its the last one that's possible with the current configuration
#define pinChangeInterruptPortToInput(p) (((PCINT_USE_PORT0 == 1) && ((p == 0) || (PCINT_NUM_USED_PORTS == 1))) ?  PCINT_INPUT_PORT0 :\
	((PCINT_USE_PORT1 == 1) && ((p == 1) || (PCINT_USE_PORT2 == 0))) ?  PCINT_INPUT_PORT1 :\
	((PCINT_USE_PORT2 == 1) /*&& ((p == 2) || (PCINT_NUM_USED_PORTS == 1))*/) ?  PCINT_INPUT_PORT2 : 0)

// missing 1.0.6 definition workaround
#ifndef NOT_AN_INTERRUPT
#define NOT_AN_INTERRUPT -1
#endif

//================================================================================
// User Definitions
//================================================================================

// definition used by the user to create his custom PCINT functions
#define PinChangeInterruptEvent_Wrapper(n) pcint_callback_ptr_ ## n
#define PinChangeInterruptEvent(n) PinChangeInterruptEvent_Wrapper(n)

// convert a normal pin to its PCINT number (0 - max 23), used by the user
// newer version, to work with the event definition above.
#define digitalPinToPinChangeInterruptWrapper(p) PIN_TO_PCINT_ ## p
#define digitalPinToPinChangeInterrupt(p) digitalPinToPinChangeInterruptWrapper(p)
// old version, calculates the pin by the Arduino definitions
#define digitalPinToPinChangeInterruptOld(p) (digitalPinToPCICR(p) ? ((8 * digitalPinToPCICRbit(p)) + digitalPinToPCMSKbit(p)) : NOT_AN_INTERRUPT)

//================================================================================
// PinChangeInterrupt User Functions
//================================================================================

// variables to save the last port states and the interrupt settings
extern uint8_t oldPorts[PCINT_NUM_USED_PORTS];
extern uint8_t fallingPorts[PCINT_NUM_USED_PORTS];
extern uint8_t risingPorts[PCINT_NUM_USED_PORTS];

inline void attachPinChangeInterrupt(const uint8_t pcintNum, const uint8_t mode) {
	// get PCINT registers
	uint8_t pcintPort = pcintNum / 8;

	//TODO rework this check
	// check if pcint is a valid pcint, exclude deactivated ports
	if (!(pcintNum < (PCINT_NUM_PORTS * 8)))
		return;
	// only do the check if they are physically available
#if (PCINT_HAS_PORT0 == true) && (PCINT_USE_PORT0 == false)
	if (pcintPort == 0) return;
#endif
#if (PCINT_HAS_PORT1 == true) && (PCINT_USE_PORT1 == false)
	if (pcintPort == 1) return;
#endif
#if (PCINT_HAS_PORT2 == true) && (PCINT_USE_PORT2 == false)
	if (pcintPort == 2) return;
#endif

	// get bitmask and array position
	uint8_t pcintMask = (1 << (pcintNum % 8));
	uint8_t arrayPos = PCINT_ARRAY_POS(pcintPort);

	// save settings related to mode and registers
	if (mode == CHANGE || mode == RISING)
		risingPorts[arrayPos] |= pcintMask;
	if (mode == CHANGE || mode == FALLING)
		fallingPorts[arrayPos] |= pcintMask;

	// update the old state to the actual state
	oldPorts[arrayPos] = *portInputRegister(digitalPinToPort(pcintNum));

	// pin change mask registers decide which pins are ENABLE as triggers
	*(&PCMSK0 + pcintPort) |= pcintMask;

	// PCICR: Pin Change Interrupt Control Register - enables interrupt vectors
	PCICR |= (1 << pcintPort);
}

inline void detachPinChangeInterrupt(const uint8_t pcintNum) {
	// get PCINT registers
	uint8_t pcintPort = pcintNum / 8;

	// check if pcint is a valid pcint, exclude deactivated ports
	if (!(pcintNum < (PCINT_NUM_PORTS * 8)))
		return;
	// only do the check if they are physically available
#if (PCINT_HAS_PORT0 == true) && (PCINT_USE_PORT0 == false)
	if (pcintPort == 0) return;
#endif
#if (PCINT_HAS_PORT1 == true) && (PCINT_USE_PORT1 == false)
	if (pcintPort == 1) return;
#endif
#if (PCINT_HAS_PORT2 == true) && (PCINT_USE_PORT2 == false)
	if (pcintPort == 2) return;
#endif

	// get bitmask and array position
	uint8_t pcintMask = (1 << (pcintNum % 8));
	uint8_t arrayPos = PCINT_ARRAY_POS(pcintPort);

	// delete setting
	risingPorts[arrayPos] &= ~pcintMask;
	fallingPorts[arrayPos] &= ~pcintMask;

	// disable the mask.
	*(&PCMSK0 + pcintPort) &= ~pcintMask;

	// if that's the last one, disable the interrupt.
	if (*(&PCMSK0 + pcintPort) == 0)
		PCICR &= ~(1 << pcintPort);
}

#endif // include guard
