// initialize LED strand:
#include "WS2812b.h"
WS2812b led(1,3,1,10); // one row, three columns (i.e. one strand of length 3 --> type 1), pin D10

// do this at the beginning:
void setup() {
  led.setRGB(0,0,0xFF0000); // pixel 0 as red
  led.setRGB(1,0,0x00FF00); // pixel 1 as green
  led.setRGB(2,0,0x0000FF); // pixel 2 as blue
  led.setMaxBrightness(5); // dim down to 5 = low brightness
  led.showLEDs(); // display all color values now
}

// repeat this:
void loop() {
  // well, nothing...
}
