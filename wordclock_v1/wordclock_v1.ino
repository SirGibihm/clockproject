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
  // turn on UHR
  // turn on M
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Black;
  }
  // ES IST UHR M
  int defaultDisplay[] = { 0, 1, 3, 4, 5, 58, 57, 56 };  //, 120};
  manipulateDisplay(defaultDisplay, 8);
}

int generateDisplayHour(int hour) {

  Serial.println("Reached 2: generateDisplayHour");

  Serial.print("Hour: ");
  Serial.println(hour);


  if (hour % 12 == 0) {
    int defaultDisplay[] = { 64, 63, 62, 61, 60 };
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 1) {
    int defaultDisplay[] = { 19, 18, 17 };
    manipulateDisplay(defaultDisplay, 3);
  } else if (hour % 12 == 2) {
    int defaultDisplay[] = { 21, 20, 19, 18 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 3) {
    int defaultDisplay[] = { 7, 8, 9, 10 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 4) {
    int defaultDisplay[] = { 14, 13, 12, 11 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 5) {
    int defaultDisplay[] = { 23, 24, 25, 26 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 6) {
    int defaultDisplay[] = { 28, 29, 30, 31, 32 };
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 7) {
    int defaultDisplay[] = { 43, 42, 41, 40, 39, 38 };
    manipulateDisplay(defaultDisplay, 6);
  } else if (hour % 12 == 8) {
    int defaultDisplay[] = { 37, 36, 35, 34 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 9) {
    int defaultDisplay[] = { 44, 45, 46, 47 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 10) {
    int defaultDisplay[] = { 48, 49, 50, 51 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 11) {
    int defaultDisplay[] = { 52, 53, 54 };
    manipulateDisplay(defaultDisplay, 3);
  }
}

void generateDisplayMinute(int minute) {
  if (minute % 10 > 4) {
    int defaultDisplay[] = { 66, 67, 68, 69 };
    manipulateDisplay(defaultDisplay, 4);
    if (minute > 24) {
      int defaultDisplay[] = { 70, 71, 72 };
      manipulateDisplay(defaultDisplay, 4);
    }
  }
  if (minute / 10 == 1) {
    int defaultDisplay[] = { 73, 74, 75, 76 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (minute / 10 == 2) {
    int defaultDisplay[] = {
      84,
      83,
      82,
      81,
      80,
      79,
      78,
    };
    manipulateDisplay(defaultDisplay, 7);
  } else if (minute / 10 == 3) {
    int defaultDisplay[] = { 88, 89, 90, 91, 92, 93, 94 };
    manipulateDisplay(defaultDisplay, 7);
  } else if (minute / 10 == 4) {
    int defaultDisplay[] = { 107, 106, 105, 104, 103, 102, 101 };
    manipulateDisplay(defaultDisplay, 7);
  } else if (minute / 10 == 5) {
    int defaultDisplay[] = { 111, 112, 113, 114, 115, 116, 117 };
    manipulateDisplay(defaultDisplay, 7);
  }
}

void generateDisplayAMPM(int hour) {
  if (hour > 11) {
    int defaultDisplay[] = { 119 };
    manipulateDisplay(defaultDisplay, 1);
  } else {
    int defaultDisplay[] = { 99 };
    manipulateDisplay(defaultDisplay, 1);
  }
}


void generateDisplay(int hour, int minute) {
  //bool isAM = hour < 12;
  generateDisplayDefault();
  generateDisplayHour(hour);
  generateDisplayMinute(minute);
  //generateDisplayAMPM(isAM);
  FastLED.show();
}


void loop() {
  DateTime now = rtc.now();
  generateDisplay(now.hour(), now.minute());
  FastLED.show();
  delay(100);
}
