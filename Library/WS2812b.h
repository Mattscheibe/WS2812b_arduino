/*
* ws2812b lib V1.0a: Programming interface for controlling WS2811/WS2812/WS2812B RGB-LEDs.
*
* assembler routine derived from light weight WS2812 lib V2.1 by Tim (cpldcpu@gmail.com).
*
* Author: Chris (Mattscheibenpost@gmail.com)
*
* January 9th, 2016 V1.0a Initial Version
*
* License: GNU GPL v2 (see License.txt)
*
* Refer to information in this file for setting up wiring scheme and data output pin when
* initialising driver.
*/
#ifndef _ws2812b_h_
#define _ws2812b_h_
/////////////////////////////////////////////////////////////////////////ç
/// C A L L  WS2812B  W I T H  C O R R E C T  W I R I N G  S C H E M E  //
// ==================================================================== //
// at the beginning of your program write something like:               //
//                                                                      //
// WS2812B led(matrixWidth, matrixHeight, matrixType, dataPin);         //
//                                                                      //
// for single strands (no matrix) set matrixWidth to 1                  //
// for single strands (no matrix) set matrixHeight to length of strand  //
// for single strands (no matrix) set to matrixType to 1                //
//                                                                      //
// for a real matrix please set type according to wiring scheme chosen: //
//                                                                      //
//          o            o            ^         o            o          //
// Type  0: ||_||¯||  1: ||/||/||  2: ||_||¯||_||  3: ||\||\||          //
//                 v            v                     v                 //
//                                                                      //
// Type  4: o=        5: o=        6:  =o          7: =o                //
//            |           /           |               \                 //
//           =            =            =              =                 //
//          |             /             |             \                 //
//           =>           =>          <=             <=                 //
//                                                                      //
//                 ^            ^     ^               ^                 //
// Type  8: ||¯||_||  9: ||\||\|| 10: ||_||¯||    11: ||/||/||          //
//          o            o                   o               o          //
//                                                                      //
// Type 12:  =>      13: =>       14: <=          15: <=                //
//          |            \              |              /                //
//           =           =             =               =                //
//            |          \            |                /                //
//          o=          o=             =o              =o               //
//                                                                      //
//                                                                      //
// ( double lines symbolize strands, single lines interconnections )    //
// ( o mark positions of input to strand = Din )                        //
// ( arrows indicate strands' directions => towards Dout )              //
// (despite schemes being illustrated with 3 or 4 elements here, they   //
//  may actually be of any length and number)                           //
//                                                                      //
/////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifdef ARDUINO
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif
#endif
struct cRGB {
  uint8_t g;
  uint8_t r;
  uint8_t b;
}; // = Struct for color values of one pixel
class WS2812b {
  public:
    // constructor (initializes according to wiring scheme: width / height / type , sets assigned data pin as output)
    WS2812b(uint16_t matrixWidth, uint16_t matrixHeight, uint8_t matrixType, uint8_t ws2812b_Pin);
    // functions:
    void setRGB(uint16_t row, uint16_t column, uint32_t color);
    void setR(uint16_t row, uint16_t column, uint8_t red);
    void setG(uint16_t row, uint16_t column, uint8_t green);
    void setB(uint16_t row, uint16_t column, uint8_t blue);
    uint32_t getRGB(uint16_t row, uint16_t column);
    uint8_t getR(uint16_t row, uint16_t column);
    uint8_t getG(uint16_t row, uint16_t column);
    uint8_t getB(uint16_t row, uint16_t column);
    void setMaxBrightness(uint8_t value);
    void clearLEDs(); // all LEDs off
    void showLEDs();  // lights up strand with stored color value ( => displays values)
    // variables:
    cRGB* pixel;// = array with all pixel color values, will be allocated at runtime
    uint8_t maxBrightnessLEDs; // either change with setMaxBrightness(0...255) or by setting it directly
  private:
    // functions:
    uint16_t idx(uint16_t row, uint16_t column); // gets index (offset) in a (linearily wired) strand
    // variables:
    unsigned char TYPE; // TYPE ranges from 0 to 15, see above, it will be set by calling WS2812b ( = init ) with a specific value
    const volatile uint8_t *ws2812b_port; // port of data pin
    volatile uint8_t *ws2812b_ddr;        // ddr  of data pin
    uint8_t maskhi, masklo; // for choosing corresponding bit
    uint16_t _width;  // matrix width of strand(s) - set to 1 if just one strand
    uint16_t _height; // matrix height of strands or lengtj of (one) strand
};
#endif
