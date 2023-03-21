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


// Settings for the light sensor
#include <BH1750.h>
BH1750 lightMeter;


//Necessary definitions for the Rotary Encoder
#include <Encoder.h>    
const int CLK = 10;       
const int DT = 9;
const int SW = 2;       // Use interrupt for switch
int oldPosition = -999;  // Initial position

int mode=0; //(0 default, 1 isa+david)
int push_down=1;
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

  // Needs the time to start this clock. Fails otherwise
  if (!rtc.begin()) {
    while (1);
  }

  pinMode(SW, INPUT);   // Hier wird der Interrupt installiert.
  attachInterrupt(digitalPinToInterrupt(SW), Interrupt, CHANGE); // Sobald sich der Status (CHANGE) des Interrupt Pins (SW = D2) ändern, soll der Interrupt Befehl (onInterrupt)ausgeführt werden.
  encoderMovement();
}

void Interrupt() { // Beginn des Interrupts. Wenn der Rotary Knopf betätigt wird, springt das Programm automatisch an diese Stelle. Nachdem...

  push_down = (push_down + 1) % 2;
  if (push_down==1) {
    mode = (mode + 1) % 2;
  }
  delay(300);
  Serial.print(mode);
}

void manipulateDisplay(int ledArray[], int sizeLEDArray) {
  for (int i = 0; i < sizeLEDArray; i++) {
    leds[ledArray[i]] = color;
  }
}

void resetDisplay() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Black;
  }
}

void generateAnalogueDefaultFlipped() {
  // ES IST UHR M
  int defaultDisplay[] = { 10, 9, 7, 6, 5 };
  manipulateDisplay(defaultDisplay, 5);
}

// Use this if the LEDs start with 0 in the top right corner
void generateAnalogueHoursFlipped(int input_hour, int minute) {

  int hour = input_hour;
  if (minute > 24) {
    hour++;
  }

  if (hour % 12 == 0) {
    int defaultDisplay[] = { 78, 79, 80, 81, 82};
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 1) {
    int defaultDisplay[] = { 59, 60, 61 };
    manipulateDisplay(defaultDisplay, 3);
    if (minute > 4) {
      int s_of_ones[] = { 62 };
    manipulateDisplay(s_of_ones, 1);
    }
  } else if (hour % 12 == 2) {
    int defaultDisplay[] = { 57, 58, 59, 60, };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 3) {
    int defaultDisplay[] = { 75, 74, 73, 72};
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 4) {
    int defaultDisplay[] = { 70, 69, 68, 67};
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 5) {
    int defaultDisplay[] = { 117, 116, 115, 114 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 6) {
    int defaultDisplay[] = { 83, 84, 85, 86, 87 };
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 7) {
    int defaultDisplay[] = { 98, 97, 96, 95, 94, 93 };
    manipulateDisplay(defaultDisplay, 6);
  } else if (hour % 12 == 8) {
    int defaultDisplay[] = { 91, 90, 89, 88 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 9) {
    int defaultDisplay[] = { 99, 100, 101, 102 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 10) {
    int defaultDisplay[] = { 103, 104, 105, 106 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 11) {
    int defaultDisplay[] = { 119, 118, 117 };
    manipulateDisplay(defaultDisplay, 3);
  }
}

// Use this if the LEDs start with 0 in the top right corner
void generateAnalogueMinutesFlipped(int minute) {
 
  // If FÜNF needs to be shown
  if (((4 < minute) && (minute  < 10)) || ((24 < minute) && (minute < 30)) || ((34 < minute) && (minute  < 40)) || (54 < minute)) {
    int five[] = { 3, 2, 1, 0 };
    manipulateDisplay(five, 4);
  }

  // If UHR needs to be shown
  if (minute < 5) {
    int five[] = { 112, 111, 110 };
    manipulateDisplay(five, 3);
  }
  // if ZEHN needs to be shown
  else if (((9 < minute) && (minute  < 15)) || ((49 < minute) && (minute  < 55))) {
    int ten[] = { 32, 31, 30, 29 };
    manipulateDisplay(ten, 4);
  }

  // if VIERTEL needs to be shown
  else if (((14 < minute) && (minute  < 20)) || ((44 < minute) && (minute  < 50))) {
    int quarter[] = { 28, 27, 26, 25, 24, 23, 22 };
    manipulateDisplay(quarter, 7);
  }
  
  // if ZWANZIG needs to be shown
  else if (((19 < minute) && (minute < 25)) || ((39 < minute) && (minute  < 45))) {
    int ten[] = { 15, 16, 17, 18, 19, 20, 21 };
    manipulateDisplay(ten, 7);
  }

  // if HALB needs to be shown
  else if ((24 < minute) && (minute < 40)) {
    int half[] = { 49, 48, 47, 46};
    manipulateDisplay(half, 4);
  } 

  // if NACH needs to be shown
  if (((4 < minute) && (minute < 25)) || ((34 < minute) && (minute < 40))) {
    int half[] = { 54, 53, 52, 51 };
    manipulateDisplay(half, 4);
  } 

  // if VOR needs to be shown
  else if (((24 < minute) && (minute < 30)) || (39 < minute)) {
    int half[] = { 35, 36, 37};
    manipulateDisplay(half, 3);
  } 
}

void generateAnalogueFlipped(int hour, int minute) {
  resetDisplay();
  generateAnalogueDefaultFlipped();
  generateAnalogueHoursFlipped(hour, minute);
  generateAnalogueMinutesFlipped(minute);
  FastLED.show();
}

void generateIsaDavid(){
  resetDisplay();
  int isa_und_david[] = { 7, 6, 4, 39, 40, 41, 66, 69, 70, 71, 75};
  manipulateDisplay(isa_und_david, 11);
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
  int curPosition = meinEncoder.read(); 

  if (curPosition > oldPosition +3) {
    oldPosition = curPosition;       
    return -1;
  }
  else if (curPosition < oldPosition -3) {
    oldPosition = curPosition;
    return 1;
  }
  return 0;
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
  setBrightness();
  if(mode==0) {
    generateAnalogueFlipped(displayTime.hour(), displayTime.minute());
  }
  else if(mode==1) {
   generateIsaDavid();
  }
  
}






