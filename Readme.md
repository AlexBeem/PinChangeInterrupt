PinChangeInterrupt Library 1.2
==============================

![Header Picture](header.png)

New PinChangeInterrupt with a very resource friendly implementation.
PinChangeInterrupts are a bit slower than normal Pin Interrupts because of the pin change comparison
but this library tries to reduce this disadvantage to a minimum, even with an user friendly API.
Compared with the normal Interrupts it is even more compact and all available PCINTs can be used.

**Features:**
* PinChangeInterrupt for a lot of pins
* Rising, Falling or Change detection for every pin separately
* Usable on every standard Arduino and Attiny as well
* Uses less ram & flash than normal Interrupts
* Implementation is fast and compact
* Ports/Pins can be manually deactivated in the .h file
* API and LowLevel option
* .a linkage optimization

**Supported pins for PinChangeInterrupt:**
```
 Arduino Uno: All pins are usable  
 Arduino Mega: 10, 11, 12, 13, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64),
               A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)
 Arduino Leonardo: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI)
 HoodLoader2: All (broken out 1-7) pins are usable
 Attiny 24/44/84: All pins are usable  
 Attiny 25/45/85: All pins are usable 
 ```
 

| PCINT |   Uno    |   Mega   | Leonardo |   16u2    | Attiny x4 | Attiny x5 |
| ----- | -------- | -------- | -------- | --------- | --------- | --------- |
|     0 |  8 (PB0) |  0 (PB0) |  0 (PB0) |  0* (PB0) |  0 (PB0)  |  0 (PB0)  |
|     1 |  9 (PB1) |  0 (PB0) |  0 (PB0) |  0  (PB0) |  0 (PB0)  |
|     2 | 10 SS (PB2) |  0 (PB0) |  0 (PB0) |  0  (PB0) |  0 (PB0)  |
|     3 | 11 MISO (PB3) |  0 (PB0) |  0 (PB0) |  0  (PB0) |  0 (PB0)  |
|     4 | 12 MOSI (PB4) |  0 (PB0) |  0 (PB0) |  0 (PB0) | 0 (PB0) |
|     5 | 13 SCK (PB5) |  0 (PB0) |  0 (PB0) |  0 (PB0) | 0 (PB0) |
|     6 | XTAL1 (PB6) |  0 (PB0) |  0 (PB0) |  0 (PB0) | 0 (PB0) |
|     7 | XTAL2 (PB7) |  0 (PB0) |  0 (PB0) |  0 (PB0) | 0 (PB0) |
|     8 | A0 (PC0) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|     9 | A1 (PC1) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    10 | A2 (PC2) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    11 | A3 (PC3) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    12 | A4 SDA (PC4) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    13 | A5 SDC (PC5) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    14 | RST (PC6) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    15 |  NC |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    16 |  0 RX (PD0) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    17 |  1 TX (PD1) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    18 |  2 INT0 (PD2) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    19 |  3 INT1 (PD3) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    20 |  4 (PD4) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    21 |  5 (PD5) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    22 |  6 (PD6) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |
|    23 |  7 (PD7) |  0 (PB0) |          |  0 (PB0) | 0 (PB0) |

Pins with * are deactivated/not broken out by default.
You may activate them in the setting file (advanced).

**[Comment for feedback on my blog.](http://www.nicohood.de)**

Installation
============

Download the zip, extract and remove the "-master" of the folder.
Install the library [as described here](http://arduino.cc/en/pmwiki.php?n=Guide/Libraries).

#####Optional Installation
To get even more optimization you can move the library to the Arduino core folder instead.
Then the library is linked different (through .a file) and better optimized if you use less ports.
Place all files into https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/cores/arduino instead.

How to use
==========

It is important that you know at least the basic difference between **PinInterrupts** and **PinChangeInterrupts**.
I will explain the basics of PinChangeInterrupts (PCINTs) based on an Arduino Uno.

On a standard Arduino Pin 2 and 3 have **PinInterrupts**. Those are exclusively for each single pin and can detect RISING, FALLING and CHANGE.

PinChangeInterrupts instead are used for a whole port (they should have better named them PortChangeInterrupts) and can only detect CHANGE.
Each pin row (0-7, 8-13, A0-A5) represents a port. If an Interrupt occurs on one of the pins of this port
it is still unclear what pin caused this interrupt. Therefore this library saves the state of all pins per port and compares the state.
This way we can also see if it was a RISING or FALLING edge instead of only knowing the pin has changed.
A PinChangeInterrupt will only be triggered for the attached pins per port. Meaning if your input on pin 6 is changing a lot
but only a PCINT for pin 7 is set it will not disturb your program.

You should know that PinChangeInterrupts are slower and not that reliable because of that detection overhead if you need very precise interrupts (ISR).
But since it is never good to add long ISRs that should be fine. You have the same issues on normal **PinInterrupts** if your ISR takes too long.
The library is coded to get maximum speed and minimum code size. The low level without the API takes 4uS to enter the interrupt function in the worst case
which is pretty good and might be even better than the **PinInterrupt** code from the official Arduino core.

#####Examples
To see how the code works just check the Led and TickTock example.
The LowLevel example is for advanced users with more optimization and more direct access.
The HowItWorks example shows the basic PinChangeInterrupt setup and decoding routine, similar to the library.
See the notes in the examples about more details.

An useful example of the PinChangeInterrupt library can be found here:
https://github.com/NicoHood/IRLremote

#####Information about this library itself (advanced):
If a PinChangeInterrupt occurs it will determine the triggered pin(s).
The library uses weak callback functions that are called for the triggered pins(s).
This way we can easily skip not triggered pins (I looked at the assembler) and also implement a fast LowLevel version.

Also the order of the function execution is (normally) ordered from the lower pin number to the higher.
Meaning pin 8 will be checked faster as pin 13. Talking about micro seconds here! You can change the order in the settings.
For example by default pin 0-3 have a low priority order than pin 4-7. Because they are used for Serial and normal PinChangeInterrupts.
I don't expect anyone to use those pins at all with PCINT but at least the priority is lowered compared to the other pins.

The API takes those functions and just overwrites all of them and call the function pointers of the attached functions instead.
This way the function can be changed at runtime and its also easier to integrate into other libraries.
The function pointers take a bit flash though.

You can get better performance and less code size if you deactivate the not used pins/ports manually.
This way only the needed pins get compiled and the code is optimized by the preprocessor.
For a bit more comfortable/automatic optimization you can [install the library into the core]()
to get use of the .a linkage. This way only the used ports get compiled.
So if you only use pins on a single port (eg 8-13) then only this port get compiled. This only works with the core installation.


That's it! I hope you like the library. I tried to make it as simple and small as possible.
Keep in mind that PCINTs are not useful for every project but in most cases
the new PinChangeInterrupts may help you a lot.


Version History
===============
```
1.2 Release (xx.02.2015)
* Added weak interrupt function
* Improved interrupt function calls
* Changed the digitalPinToPinChangeInterrupt(p) macro
* Fixed attach/detach array position when ports are deactivated
* Improved manual PCINT deactivation by user
* Improved definitions for different boards
* HoodLoader2 definition fixes
* Improved speed
* Improved specific boards
* Moved attach function to .cpp file
* Updated examples
* Added API and LowLevel

1.1 Release (06.12.2014)
* Added port deactivation
* Ram usage improvements for AVRs with <3 PCINT ports

1.0 Release (04.12.2014)
* Added general PinChangeInterrupt functions
* Added support for most Arduino boards
* Added basic example
* Added an example with IRLremote
```


License and Copyright
=====================
If you use this library for any cool project let me know!

```
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
```
