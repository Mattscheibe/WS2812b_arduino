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
*/
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
#include "WS2812b.h"
WS2812b::WS2812b(uint16_t matrixWidth, uint16_t matrixHeight, uint8_t matrixType, uint8_t ws2812b_Pin) {
  ws2812b_port = portOutputRegister(digitalPinToPort(ws2812b_Pin)); // which port data pin belongs to
  ws2812b_ddr = portModeRegister(digitalPinToPort(ws2812b_Pin));    // corresponding ddr register
  maskhi = digitalPinToBitMask(ws2812b_Pin);
  *ws2812b_ddr |= maskhi; // set data pin as output  
  masklo = ~maskhi & *(uint8_t*)ws2812b_port;												// bit position
  maskhi |= *(uint8_t*)ws2812b_port;																// bit position
  pixel = (cRGB*)malloc(matrixWidth*matrixHeight*3);
  _width = matrixWidth;   // internal representation
  _height = matrixHeight; // dito
  maxBrightnessLEDs=255;
  TYPE = matrixType;
  clearLEDs();
}
void WS2812b::setRGB(uint16_t i, uint16_t j, uint32_t rgb) {
  uint8_t r,g,b;
  r = rgb >> 16;
  g = (rgb >> 8) & 0xFF;
  b = rgb & 0xFF;
  uint16_t x = idx(i,j);
  pixel[x].r=r;
  pixel[x].g=g;
  pixel[x].b=b;
}
void WS2812b::setR(uint16_t i, uint16_t j, uint8_t red) {
  uint16_t x = idx(i,j);
  pixel[x].r=red;
}
void WS2812b::setG(uint16_t i, uint16_t j, uint8_t green) {
  uint16_t x = idx(i,j);
  pixel[x].g=green;
}
void WS2812b::setB(uint16_t i, uint16_t j, uint8_t blue) {
  uint16_t x = idx(i,j);
  pixel[x].b=blue;
}
uint32_t WS2812b::getRGB(uint16_t i, uint16_t j) {
  uint32_t rgb;
  uint16_t x = idx(i,j);
  rgb = ((uint32_t)pixel[x].r << 16) + (pixel[x].g << 8) + pixel[x].b;
  return rgb;
}
uint8_t WS2812b::getR(uint16_t i, uint16_t j) {
  uint16_t x = idx(i,j);
  return pixel[x].r;
}
uint8_t WS2812b::getG(uint16_t i, uint16_t j) {
  uint16_t x = idx(i,j);
  return pixel[x].g;
}
uint8_t WS2812b::getB(uint16_t i, uint16_t j) {
  uint16_t x = idx(i,j);
  return pixel[x].b;
}
void WS2812b::setMaxBrightness(uint8_t value) {
  maxBrightnessLEDs = value;
}
void WS2812b::clearLEDs() {
  // all LEDs off
  uint16_t x;
  for (x=0;x<_width*_height;x++) {
    pixel[x].r=0;
    pixel[x].g=0;
    pixel[x].b=0;
  }
  showLEDs();
}
uint16_t WS2812b::idx(uint16_t i, uint16_t j) {
	// calculates offset for chosen wiring scheme:
  uint16_t x;
  if (TYPE ==  0) {
    if (j%2 == 0) {
      x = j * _height + i ;
    } else {
      x = j * _height + ( _height - 1 - i ) ;
    }
  } else if (TYPE ==  1) {
    x = j * _height + i ;
  } else if (TYPE ==  2) {
    if ((_width-j)%2 == 0) {
      x = ( _width - 1 - j ) * _height + _height - 1 - i ;
    } else {
      x = ( _width - 1 - j ) * _height + i ;
    }
  } else if (TYPE ==  3) {
    x = ( _width - 1 - j ) * _height + i ;
  } else if (TYPE ==  4) {
    if (i%2 == 0) {
      x = i * _width + j ;
    } else {
      x = i * _width + ( _width - 1 - j ) ;
    }
  } else if (TYPE ==  5) {
    x = i * _width + j;
  } else if (TYPE ==  6) {
    if (i%2 == 0) {
      x = i * _width + ( _width - 1 - j ) ;
    } else {
      x = i * _width + j ;
    }
  } else if (TYPE ==  7) {
    x = i * _width + ( _width - 1 - j ) ;
  } else if (TYPE ==  8) {
    if (j%2 == 0) {
      x = j * _height + ( _height - 1 - i ) ;
    } else {
      x = j * _height + i ;
    }
  } else if (TYPE ==  9) {
    x = j * _height + ( _height - 1 - i ) ;
  } else if (TYPE == 10) {
    if ((_width-j)%2 == 0) {
      x = ( _width - 1 - j ) * _height + i ;
    } else {
      x = ( _width - 1 - j ) * _height + ( _height - 1 - i ) ;
    }
  } else if (TYPE == 11) {
    x = ( _width - 1 - j ) * _height + ( _height - 1 - i ) ;
  } else if (TYPE == 12) {
    if ((_height-i)%2 == 0) {
      x = ( _height - 1 - i ) * _width + ( _width - 1 - j ) ;
    } else {
      x = ( _height - 1 - i ) * _width + j ;
    }
  } else if (TYPE == 13) {
    x = ( _height - 1 - i ) * _width + j ;
  } else if (TYPE == 14) {
    if ((_height-i)%2 == 0) {
      x = ( _height - 1 - i ) * _width + j ;
    } else {
      x = ( _height - 1 - i ) * _width + ( _width - 1 - j ) ;
    }
  } else if (TYPE == 15) {
    x = ( _height - 1 - i ) * _width + ( _width - 1 - j ) ;
  }
  return x;
}
// Timing in ns:
#define w_zeropulse   350
#define w_onepulse    900
#define w_totalperiod 1250
// Fixed cycles used by the inner loop:
#define w_fixedlow    3
#define w_fixedhigh   6
#define w_fixedtotal  10   
// Insert NOPs to match the timing, if possible:
#define w_zerocycles    (((F_CPU/1000)*w_zeropulse          )/1000000)
#define w_onecycles     (((F_CPU/1000)*w_onepulse    +500000)/1000000)
#define w_totalcycles   (((F_CPU/1000)*w_totalperiod +500000)/1000000)
// w1 - nops between rising edge and falling edge - low:
#define w1 (w_zerocycles-w_fixedlow)
// w2   nops between fe low and fe high
#define w2 (w_onecycles-w_fixedhigh-w1)
// w3   nops to complete loop
#define w3 (w_totalcycles-w_fixedtotal-w1-w2)
#if w1>0
  #define w1_nops w1
#else
  #define w1_nops  0
#endif
// The only critical timing parameter is the minimum pulse length of the "0"
// Warn or throw error if this timing can not be met with current F_CPU settings.
#define w_lowtime ((w1_nops+w_fixedlow)*1000000)/(F_CPU/1000)
#if w_lowtime>550
   #error "Light_ws2812: Sorry, the clock speed is too low. Did you set F_CPU correctly?"
#elif w_lowtime>450
   #warning "Light_ws2812: The timing is critical and may only work on WS2812B, not on WS2812(S)."
   #warning "Please consider a higher clockspeed, if possible"
#endif   
#if w2>0
#define w2_nops w2
#else
#define w2_nops  0
#endif
#if w3>0
#define w3_nops w3
#else
#define w3_nops  0
#endif
#define w_nop1  "nop      \n\t"
#define w_nop2  "rjmp .+0 \n\t"
#define w_nop4  w_nop2 w_nop2
#define w_nop8  w_nop4 w_nop4
#define w_nop16 w_nop8 w_nop8
void WS2812b::showLEDs() {
  // display pixel array content to LEDs
  float currentBrightness = (float)maxBrightnessLEDs / 255.0 ;
  uint8_t curbyte,ctr;
  uint8_t sreg_prev;
  uint16_t datlen=_width*_height*3;
  uint8_t *data;
  if (maxBrightnessLEDs == 255) {
    data = (uint8_t *)pixel;
  } else {
    struct cRGB newpixel[_width*_height];
    uint16_t x;
    for (x=0;x<_width*_height;x++) {
      newpixel[x].r=(uint8_t)((float)pixel[x].r*currentBrightness);
      newpixel[x].g=(uint8_t)((float)pixel[x].g*currentBrightness);
      newpixel[x].b=(uint8_t)((float)pixel[x].b*currentBrightness);
    }
    data = (uint8_t *)newpixel;
  }
  sreg_prev=SREG;
  cli();  
  while (datlen--) {
    curbyte = *data++;
    asm volatile(
    "       ldi   %0,8  \n\t"
    "loop%=:            \n\t"
    "       st    X,%3 \n\t"    //  '1' [02] '0' [02] - re
#if (w1_nops&1)
w_nop1
#endif
#if (w1_nops&2)
w_nop2
#endif
#if (w1_nops&4)
w_nop4
#endif
#if (w1_nops&8)
w_nop8
#endif
#if (w1_nops&16)
w_nop16
#endif
    "       sbrs  %1,7  \n\t"    //  '1' [04] '0' [03]
    "       st    X,%4 \n\t"     //  '1' [--] '0' [05] - fe-low
    "       lsl   %1    \n\t"    //  '1' [05] '0' [06]
#if (w2_nops&1)
  w_nop1
#endif
#if (w2_nops&2)
  w_nop2
#endif
#if (w2_nops&4)
  w_nop4
#endif
#if (w2_nops&8)
  w_nop8
#endif
#if (w2_nops&16)
  w_nop16
#endif
    "       brcc skipone%= \n\t"    //  '1' [+1] '0' [+2] - 
    "       st   X,%4      \n\t"    //  '1' [+3] '0' [--] - fe-high
    "skipone%=:               "     //  '1' [+3] '0' [+2] - 
#if (w3_nops&1)
w_nop1
#endif
#if (w3_nops&2)
w_nop2
#endif
#if (w3_nops&4)
w_nop4
#endif
#if (w3_nops&8)
w_nop8
#endif
#if (w3_nops&16)
w_nop16
#endif
    "       dec   %0    \n\t"    //  '1' [+4] '0' [+3]
    "       brne  loop%=\n\t"    //  '1' [+5] '0' [+4]
    :	"=&d" (ctr)
    :	"r" (curbyte), "x" (ws2812b_port), "r" (maskhi), "r" (masklo)
    );
  }
  SREG=sreg_prev;
}
