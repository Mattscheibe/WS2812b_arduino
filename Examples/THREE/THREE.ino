// initialize LED strand:
#include <WS2812b.h>
WS2812b led(1,1,1,10);    // one LED on pin D10 (one row, one column, type 1, pin D10)
#define PAUSE 30          // define how long we will wait after each update (-> cycling speed while "breathing")


// do this at the beginning:
void setup() {
  led.setR(0,0,0xFF);     // LED to red -- note: it is not shown yet, because showLEDs() is not called yet
}  


// do this repeatedly:
void loop() {
  // change brightness ("pulse"):
  // 1st up:
  unsigned int x;
  for (x=0;x<20;x++) {
    led.setMaxBrightness(x); // change brightness (color will remain red, because no new set command is being applied)
    led.showLEDs();          // show LED color with updated brightness
    delay(PAUSE);            // little pause for our lazy eyes
  }
  // 2nd down:
  for (x=20;x>0;x--) {
    led.setMaxBrightness(x);
    led.showLEDs();
    delay(PAUSE);
  }
}
