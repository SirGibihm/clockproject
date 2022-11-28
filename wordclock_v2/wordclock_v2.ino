#include <FastLED.h>
#include <Wire.h>  // Die Datums- und Zeit-Funktionen der DS3231 RTC werden ueber das I2C aufgerufen.
#include "RTClib.h"

#define DATA_PIN 3
#define LED_TYPE NEOPIXEL  // WS2812B
#define NUM_LEDS 121
#define BRIGHTNESS 100
RTC_DS3231 rtc;
#define color CRGB::White

bool syncOnFirstStart = true;
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);

  delay(3000);  // Warte auf Terminal
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  leds[0] = CRGB::White;
  FastLED.show();
  delay(200);
  if (!rtc.begin()) {
    Serial.println("Kann RTC nicht finden");
    while (1)
      ;
  }

  if (rtc.lostPower() || syncOnFirstStart) {
    Serial.println("Die RTC war vom Strom getrennt. Die Zeit wird neu synchronisiert.");
    // ueber den folgenden Befehl wird die die RTC mit dem Zeitstempel versehen, zu dem der
    // Kompilierungsvorgang gestartet wurde, beginnt aber erst mit dem vollstaendigen Upload
    // selbst mit zaehlen. Daher geht die RTC von Anfang an wenige Sekunden nach.
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println(__DATE__);
  }
}



void manipulateDisplay(int ledArray[], int sizeLEDArray) {
  for (int i = 0; i < sizeLEDArray; i++) {
    leds[ledArray[i]] = color;
  }
}

void generateDisplayDefault() {
  // turn on ES IST
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Black;
  }
  // ES IST UHR M
  int defaultDisplay[] = { 0, 1, 3, 4, 5, };
  manipulateDisplay(defaultDisplay, 5);
}


void generateDisplayMinutes(int minute) {
 
  
 
  // If FÜNF needs to be shown
  if (((4 < minute) && (minute  < 10)) || ((24 < minute) && (minute < 30)) || ((34 < minute) && (minute  < 40)) || (54 < minute)) {
    int five[] = { 7, 8, 9, 10 };
    manipulateDisplay(five, 4);
  }

  

  // If UHR needs to be shown
  if (minute < 5) {
    int five[] = { 118, 119, 120 };
    manipulateDisplay(five, 4);
  }
  // if ZEHN needs to be shown
  else if (((9 < minute) && (minute  < 15)) || ((49 < minute) && (minute  < 55))) {
    int ten[] = { 22, 23, 24, 25 };
    manipulateDisplay(ten, 4);
  }

  // if VIERTEL needs to be shown
  else if (((14 < minute) && (minute  < 20)) || ((44 < minute) && (minute  < 50))) {
    int quarter[] = { 26, 27, 28, 29, 30, 31, 32 };
    manipulateDisplay(quarter, 7);
  }
  
  // if ZWANZIG needs to be shown
  else if (((19 < minute) && (minute < 25)) || ((39 < minute) && (minute  < 45))) {
    int ten[] = { 17, 16, 15, 14, 13, 12, 11 };
    manipulateDisplay(ten, 7);
  }

  // if HALB needs to be shown
  else if ((24 < minute) && (minute < 40)) {
    int half[] = { 49, 50, 51, 52 };
    manipulateDisplay(half, 4);
  } 

  // if NACH needs to be shown
  if (((4 < minute) && (minute < 25)) || ((34 < minute) && (minute < 40))) {
    int half[] = { 44, 45, 46, 47 };
    manipulateDisplay(half, 4);
  } 

  // if VOR needs to be shown
  else if (((24 < minute) && (minute < 30)) || (39 < minute)) {
    int half[] = { 41, 40, 39 };
    manipulateDisplay(half, 3);
  } 

}

int generateDisplayHours(int input_hour, int minute) {

  int hour = input_hour;
  if (minute > 30) {
    hour++;
  }


  if (hour % 12 == 0) {
    int defaultDisplay[] = { 86, 85, 84, 83, 82};
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 1) {
    int defaultDisplay[] = { 61, 60, 59 };
    manipulateDisplay(defaultDisplay, 3);
    if (minute > 4) {
      int s_of_ones[] = { 58 };
    manipulateDisplay(s_of_ones, 1);
    }
  } else if (hour % 12 == 2) {
    int defaultDisplay[] = { 63, 62, 61, 60, };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 3) {
    int defaultDisplay[] = { 67, 68, 69, 70 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 4) {
    int defaultDisplay[] = { 72, 73, 74, 75};
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 5) {
    int defaultDisplay[] = { 113, 114, 115, 116 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 6) {
    int defaultDisplay[] = { 77, 78, 79, 80, 81 };
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 7) {
    int defaultDisplay[] = { 88, 89, 90, 91, 92, 93 };
    manipulateDisplay(defaultDisplay, 6);
  } else if (hour % 12 == 8) {
    int defaultDisplay[] = { 95, 96, 97, 98 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 9) {
    int defaultDisplay[] = { 109, 108, 107, 106 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 10) {
    int defaultDisplay[] = { 105, 104, 103, 102 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 11) {
    int defaultDisplay[] = { 111, 112, 113 };
    manipulateDisplay(defaultDisplay, 3);
  }
}


void generateDisplay(int hour, int minute) {
  generateDisplayDefault();
  generateDisplayHours(hour, minute);
  generateDisplayMinutes(minute);
  FastLED.show();
}


void loop() {
  DateTime now = rtc.now();
  generateDisplay(now.hour(), now.minute());
  Serial.println(now.hour()%12);
  Serial.println(now.minute()); 
  
  /*for (int i = 0; i < 24; i++) {
    for (int j = 0; j < 60; j++) {
      generateDisplay(i, j);
      delay(250);
    }
  }
  FastLED.show();
  delay(1000);
  */
}
