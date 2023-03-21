#include <FastLED.h>


#define DATA_PIN 3
#define LED_TYPE NEOPIXEL  // WS2812B
#define NUM_LEDS 121
#define BRIGHTNESS 100

CRGB leds[NUM_LEDS];


void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  leds[0] = CRGB::White;
  FastLED.show();
  delay(200);
}

void loop() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::White;  //.setRGB( 255, 255, 255);
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(200);
  }
}
