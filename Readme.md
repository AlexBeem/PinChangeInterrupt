PinChangeInterrupt Library 1.1
==============================

![Header Picture](header.jpg)

New PinChangeInterrupt with a very resource friendly implementation.
Compared with the normal Interrupts it is even more compact and all available PCINTs can be used.
PinChangeInterrupts are a bit slower than normal Interrupts because of the pin change comparison
but this library tries to reduce this disadvantage to a minimum, even with user friendly API.


**Features:**
* PinChangeInterrupt for many pins
* Usable on every standard Arduino and Attiny as well
* Rising, Falling or Change detection for every pin separately
* Uses less ram & flash than normal Interrupts
* Implementation in fast&compact C
* Ports can be manually deactivated in the .h file

**The following pins are usable for PinChangeInterrupt on normal Arduinos:**
* On Arduino Uno you can use any pin for PinChangeInterrupt.
* Arduino Mega: 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)
* Arduino Leonardo: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI)
* With HoodLoader2 you can use pin 1-7 and some other pins which are normally not connected.

**Todo:**
* Change .cpp in .c?

**[Comment for feedback on my blog.](http://nicohood.wordpress.com/)**

Installation/How to use
=======================

Download the zip, extract and remove the "-master" of the folder.
Install the library [as described here](http://arduino.cc/en/pmwiki.php?n=Guide/Libraries).

### Basic example
The basic example shows you how the library and its syntax work.

You can attach your pins with the attach function and the digitalPinToPinChangeInterrupt(p) definition.
Make sure your attached pin is actually a PinChangeInterrupt as listed above.
Keep in mind that on Arduino Mega the definition excludes some pins like 14 (TX3) and 15 (RX3).
You can still use them by manually passing attachPinChangeInterrupt(PCINT10, CHANGE);

PinChangeInterrupts are always executed after each other. Make sure that the functions are short and have no Serial prints in it.
Lower pins on a port are executed faster. This means PCINT is faster than PCINT7. PCINT8 is on another port (port 1) and faster than PCINT15 for example.
As normal user you might wonder what I am talking about, maybe you should read how PinChangeInterrupts work. Its really important to understand what your are using.
PinChangeInterrupts trigger for pin changes on a whole port so we have to check the difference between the last interrupt.
That's why we also have in general a **delay between the interrupt call and the actual function execution**.

### IRLremote example

To test the IRLremote example install the IRLremote:
https://github.com/NicoHood/IRLremote

This demonstrates how to use the PinChangeInterrupt library together with the IRLremote library. This can be useful if you are running out if normal
interrupt pins or don't have them at all (HoodLoader2). The PinChangeInterrupt library is also smaller than the normal Interrupt library, just a tiny bit slower.


More projects + contact can be found here:
http://nicohood.wordpress.com/

How it works
============

Coding a PCINT library is kind of tricky. At least if you want to keep flash and ram size low, as I always try to do ;)
There are two main challenges: compare the cold values as fast as you can but still be able to get a RISING, FALLING, CHANGE for every port
and somehow start the function the user wants to use.

The first challenge about the comparison is solved this way: we always compare a whole port at once. This saves us a lot of cpu cycles and also ram.
We only need a value for the old port state, the rising and the falling setting. CHANGE is interpreted as rising or falling, so both settings are high.
The rest is a bit of clever bit calculation to get the triggers for the whole port.

The hardest challenge was to find a way to start the user functions. Should we save a function pointer array with up to 24 entries? That would cost 48 bytes of ram,
some overhead for 24 functions of which the user might use 1-3. It took me some time to realize a different solution. What if you simply use only one function for every
interrupt and pass the changed port? It turned out that it works perfectly. The user has to overwrite this function in the .ino file and it can only be accessed from there
but that should work for many of you. And you could also implement another function pointer solution if needed.

On top of that is a logic of #defines which dynamically enable and disable not used ports - selected by the user or as limit by the chip itself.
It automatically uses only the ram for the available and enabled ports and also only checks them. Its a bit tricky and hard to explain, you should just have a look
at the code if you are interested. You should also know that AVRs with 4 PCINT ports (0-3) are not compatible yet because this would need a huge change to the definitions and
none of the 'standard' MCU's has 4 PCINT ports. Another advantage is that we use the actual hardware numbers of the PCINTs and no pin numbers which we had to convert again.
That saves us flash and the pin mapping is done by a definition from pin -> pcint, not vice versa. Due to the inline this mapping is also very small.

That's it! I hope you like the library. I tried to make it as simple and small as possible. Keep in mind that PCINTs are not useful for every project but in most cases
the new PinChangeInterrupts may help you a lot.

Version History
===============
```
1.1 Release (06.12.2014)
* Added port deactivation
* Ram usage improvements for AVRs with <3 PCINT ports

1.0 Release (04.12.2014)
* Added general PinChangeInterrupt functions
* Added support for most Arduino boards
* Added basic example
* Added an example with IRLremote
```


Licence and Copyright
=====================
If you use this library for any cool project let me know!

```
Copyright (c) 2014 NicoHood
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
