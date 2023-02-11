#include <Wire.h>  // Die Datums- und Zeit-Funktionen der DS3231 RTC werden ueber das I2C aufgerufen, ebenso der Lichtsensor BH1750


// Settings for the LEDs
#include <FastLED.h>
#define DATA_PIN 3
#define LED_TYPE NEOPIXEL  // WS2812B
#define NUM_LEDS 121
#define BRIGHTNESS 100
#define color CRGB::White
CRGB leds[NUM_LEDS];


// Settings for the clock
#include "RTClib.h"
RTC_DS3231 rtc;
bool syncOnFirstStart = true;


// Settings for the light sensor
#include <BH1750.h>
BH1750 lightMeter;


//Necessary definitions for the Rotary Encoder
#include <Encoder.h>    
const int CLK = 10;       
const int DT = 9;
const int SW = 2;       // Use interrupt for switch
long oldPosition = -999;  // Initial position
Encoder meinEncoder(DT,CLK);  

void setup() {
  Serial.begin(9600);
  
  // Prepare the light meter
  Wire.begin();// Initialize the I2C bus (BH1750 library doesn't do this automatically)
  
  lightMeter.begin();

  delay(3000);  // Wait for Terminal

  // Start up LEDs
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  leds[0] = CRGB::White;
  FastLED.show();
  delay(200);

  // Needs the time to start this clock. Fails otherwise
  if (!rtc.begin()) {
    while (1);
  }

  // Setting compile time to watch time
  if (rtc.lostPower() || syncOnFirstStart) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  

  pinMode(SW, INPUT);   // Hier wird der Interrupt installiert.
  attachInterrupt(digitalPinToInterrupt(SW), Interrupt, CHANGE); // Sobald sich der Status (CHANGE) des Interrupt Pins (SW = D2) ändern, soll der Interrupt Befehl (onInterrupt)ausgeführt werden.
  encoderMovement();
}

void Interrupt() // Beginn des Interrupts. Wenn der Rotary Knopf betätigt wird, springt das Programm automatisch an diese Stelle. Nachdem...

{
  Serial.println("Switch betaetigt"); //... das Signal ausgegeben wurde, wird das Programm fortgeführt.
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
  if (minute > 24) {
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

void setBrightness() {
  int light_level = lightMeter.readLightLevel();
  if(light_level < 5) {
    FastLED.setBrightness(5);    
  }
  else if (light_level > 100) {
    FastLED.setBrightness(200);
  }
  else {
    FastLED.setBrightness(light_level*2);
  }
}

int encoderMovement() {
  // Check if the setting of the rotary encoder is changed:
  long curPosition = meinEncoder.read(); 

  if (curPosition > oldPosition +3) {
    oldPosition = curPosition;       
    return 1;
  }
  else if (curPosition < oldPosition -3) {
    oldPosition = curPosition;
    return -1;
  }
  else {
    return 0;
  }
}

void loop() {
  
  int encMove = encoderMovement();
  if(encMove != 0) {
    DateTime now = rtc.now();
    if(encMove == 1) {
      rtc.adjust(now+TimeSpan(60-now.second()));
    } 
    else if(encMove == -1) {
      rtc.adjust(now-TimeSpan(60+now.second()));
    } 
  }
  DateTime displayTime = rtc.now();

  //Get the current time and set the display accordingly
  generateDisplay(displayTime.hour(), displayTime.minute());
  setBrightness();

}






