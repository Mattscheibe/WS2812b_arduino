# WS2812b_arduino
A library for driving WS2812b based (aka Neopixel) LED strands on an Arduino-like board. Allows for convenient setup of matrix-like displays. Uses standard addressing and standard color coding like in image processing. For code interchange, this library also exists for standard AVR and for Raspberry Pi.

---

Data transfer routine is taken from library https://github.com/cpldcpu/light_ws2812.
User interface is simplified and unified for all platforms.

---

## Hardware Setup
* Supports several output pins (by appropriately calling a constructor for each one used).
* A resistor of 220...470 ohms should be used for connecting Arduino pins and WS2812B LED Din pins.
* For longer strands, a capacitor of 100uF should be added to +5V and GND of strand.
* Find all possible wiring schemes in file WS2812b.h or in WIRING.txt.
* May either be used with Arduino IDE or with an eidtor of choice (an appropriate Makefile is included).

---

## Commands

**WS2812b led(width,height,type,arduino_pin);**

* initialize strand
* for single linear strands set width to length of strand and height to 1
* for type cf. WS2812b.h or WIRING.txt
* arduino_pin is e.g. "12" for pin "D12"
* for more than one strand repeat command while changing "led" to a different name

*Example:*

**WS2812b l1(10,12,6,12);** // 10 columns, 12 rows, type 6 (starting point upper right side, short wiring), pin "D12" on arduino


**l1.setR(row,column,color);**

* *color* from 0x00 to 0xFF, for red

**l1.setG(row,column,color);**

* dito for green

**l1.setB(row,column,color);**

* dito for blue

**l1.setRGB(row,column,fullcolor);**

* set all three color values at once
* full color ranges from 0x000000 to 0xFFFFFF
* values can be taken from image processing or color picker tools

**l1.getR(row,column);**

* return color value for red channel of pixel addressed
* possible range from 0x00...0xFF

**l1.getG(row,column);**

* dito for green

**l1.getB(row,column);**

* dito for blue

**l1.getRGB(row,column);**

* read-out function for full color value of addressed pixel
* possible range from 0x000000 to 0xFFFFFF

**l1.showLEDs();**

* will display all pixels of strand l1 (with the colors that have been set before)

**l1.clearLEDs();**

* reset all color values or strand l1 and display a "blank screen"

**l1.setMaxBrightness(maxValue);**

* used for dimming (good to the eyes!)
* maxValue ranges from 0x00 (full dimming) to 0xFF (no dimming)
* will usually be called once at the beginning of the program
* but can also be called several times for special effects
* good values for maxValue while prototyping are 3...10
* will change intensity of display by limiting color resolution
* will effect all subsequent calls to l1.showLEDs()
