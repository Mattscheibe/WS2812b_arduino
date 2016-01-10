// initialize LED strand:
#include <WS2812b.h>
WS2812b led(1,1,1,10);      // one row, one column (i.e. one LED) --> type 1, Arduino pin D10

// use internal LED, too:
void setup() {
  pinMode(13, OUTPUT);
}

// blink both, internal LED and 
void loop() {
  led.setRGB(0,0,0x030200); // set pixel to a little bit of red (0x03) and an even smaller bit of green (0x02) and no blue (0x00)
  led.showLEDs();						// set WS2812b LED(s) to color(s) set before
  digitalWrite(13, HIGH);   // internal LED on -- note that this needs to be done after calling led.showLEDs() if the LED strand is attached to the same PORT as the internal LED
  delay(50);							  // short pause
  led.clearLEDs(); 					// set WS2812b LED(s) off
  digitalWrite(13, LOW );   // internal LED off
  delay(500);							  // longer pause
}
