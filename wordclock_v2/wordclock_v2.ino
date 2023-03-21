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
const byte CLK = 10;       
const byte DT = 9;
const byte SW = 2;       // Use interrupt for switch
const byte digitLength[] = {12, 5, 11, 10, 9, 11, 12, 7, 13, 12};
int oldPosition = -999;  // Initial position

byte mode = 0; //(0 default, 1 digital, 2 isa+david)
byte push_down=1;
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

  // Setting compile time to watch time
  // if (rtc.lostPower() || syncOnFirstStart) {
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //}
  

  pinMode(SW, INPUT);   // Hier wird der Interrupt installiert.
  attachInterrupt(digitalPinToInterrupt(SW), Interrupt, CHANGE); // Sobald sich der Status (CHANGE) des Interrupt Pins (SW = D2) ändern, soll der Interrupt Befehl (onInterrupt)ausgeführt werden.
  encoderMovement();
}

void Interrupt() { // Beginn des Interrupts. Wenn der Rotary Knopf betätigt wird, springt das Programm automatisch an diese Stelle. Nachdem...

  push_down = (push_down + 1) % 2;
  if (push_down==1) {
    mode = (mode + 1) % 3;
  }
  delay(300);
}

void manipulateDisplay(byte ledArray[], byte sizeLEDArray) {
  for (int i = 0; i < sizeLEDArray; i++) {
    leds[ledArray[i]] = color;
  }
}

void resetDisplay() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Black;
  }
}

void generateAnalogueDefault() {
  // ES IST UHR M
  byte defaultDisplay[] = { 0, 1, 3, 4, 5, };
  manipulateDisplay(defaultDisplay, 5);
}

void generateAnalogueDefaultFlipped() {
  // ES IST UHR M
  byte defaultDisplay[] = { 10, 9, 7, 6, 5 };
  manipulateDisplay(defaultDisplay, 5);
}

// Use this if the LEDs start with 0 in the top left corner
void generateAnalogueMinutes(int minute) {
 
  // If FÜNF needs to be shown
  if (((4 < minute) && (minute  < 10)) || ((24 < minute) && (minute < 30)) || ((34 < minute) && (minute  < 40)) || (54 < minute)) {
    byte five[] = { 7, 8, 9, 10 };
    manipulateDisplay(five, 4);
  }

  // If UHR needs to be shown
  if (minute < 5) {
    byte five[] = { 118, 119, 120 };
    manipulateDisplay(five, 4);
  }
  // if ZEHN needs to be shown
  else if (((9 < minute) && (minute  < 15)) || ((49 < minute) && (minute  < 55))) {
    byte ten[] = { 22, 23, 24, 25 };
    manipulateDisplay(ten, 4);
  }

  // if VIERTEL needs to be shown
  else if (((14 < minute) && (minute  < 20)) || ((44 < minute) && (minute  < 50))) {
    byte quarter[] = { 26, 27, 28, 29, 30, 31, 32 };
    manipulateDisplay(quarter, 7);
  }
  
  // if ZWANZIG needs to be shown
  else if (((19 < minute) && (minute < 25)) || ((39 < minute) && (minute  < 45))) {
    byte ten[] = { 17, 16, 15, 14, 13, 12, 11 };
    manipulateDisplay(ten, 7);
  }

  // if HALB needs to be shown
  else if ((24 < minute) && (minute < 40)) {
    byte half[] = { 49, 50, 51, 52 };
    manipulateDisplay(half, 4);
  } 

  // if NACH needs to be shown
  if (((4 < minute) && (minute < 25)) || ((34 < minute) && (minute < 40))) {
    byte half[] = { 44, 45, 46, 47 };
    manipulateDisplay(half, 4);
  } 

  // if VOR needs to be shown
  else if (((24 < minute) && (minute < 30)) || (39 < minute)) {
    byte half[] = { 41, 40, 39 };
    manipulateDisplay(half, 3);
  } 

}

// Use this if the LEDs start with 0 in the top left corner
void generateAnalogueHours(int input_hour, int minute) {

  byte hour = input_hour;
  if (minute > 24) {
    hour++;
  }

  if (hour % 12 == 0) {
    byte defaultDisplay[] = { 86, 85, 84, 83, 82};
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 1) {
    byte defaultDisplay[] = { 61, 60, 59 };
    manipulateDisplay(defaultDisplay, 3);
    if (minute > 4) {
      byte s_of_ones[] = { 58 };
    manipulateDisplay(s_of_ones, 1);
    }
  } else if (hour % 12 == 2) {
    byte defaultDisplay[] = { 63, 62, 61, 60, };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 3) {
    byte defaultDisplay[] = { 67, 68, 69, 70 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 4) {
    byte defaultDisplay[] = { 72, 73, 74, 75};
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 5) {
    byte defaultDisplay[] = { 113, 114, 115, 116 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 6) {
    byte defaultDisplay[] = { 77, 78, 79, 80, 81 };
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 7) {
    byte defaultDisplay[] = { 88, 89, 90, 91, 92, 93 };
    manipulateDisplay(defaultDisplay, 6);
  } else if (hour % 12 == 8) {
    byte defaultDisplay[] = { 95, 96, 97, 98 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 9) {
    byte defaultDisplay[] = { 109, 108, 107, 106 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 10) {
    byte defaultDisplay[] = { 105, 104, 103, 102 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 11) {
    byte defaultDisplay[] = { 111, 112, 113 };
    manipulateDisplay(defaultDisplay, 3);
  }
}

// Use this if the LEDs start with 0 in the top right corner
void generateAnalogueMinutesFlipped(int minute) {
 
  // If FÜNF needs to be shown
  if (((4 < minute) && (minute  < 10)) || ((24 < minute) && (minute < 30)) || ((34 < minute) && (minute  < 40)) || (54 < minute)) {
    byte five[] = { 3, 2, 1, 0 };
    manipulateDisplay(five, 4);
  }

  // If UHR needs to be shown
  if (minute < 5) {
    byte five[] = { 112, 111, 110 };
    manipulateDisplay(five, 4);
  }
  // if ZEHN needs to be shown
  else if (((9 < minute) && (minute  < 15)) || ((49 < minute) && (minute  < 55))) {
    byte ten[] = { 32, 31, 30, 29 };
    manipulateDisplay(ten, 4);
  }

  // if VIERTEL needs to be shown
  else if (((14 < minute) && (minute  < 20)) || ((44 < minute) && (minute  < 50))) {
    byte quarter[] = { 28, 27, 26, 25, 24, 23, 22 };
    manipulateDisplay(quarter, 7);
  }
  
  // if ZWANZIG needs to be shown
  else if (((19 < minute) && (minute < 25)) || ((39 < minute) && (minute  < 45))) {
    byte ten[] = { 15, 16, 17, 18, 19, 20, 21 };
    manipulateDisplay(ten, 7);
  }

  // if HALB needs to be shown
  else if ((24 < minute) && (minute < 40)) {
    byte half[] = { 49, 48, 47, 46};
    manipulateDisplay(half, 4);
  } 

  // if NACH needs to be shown
  if (((4 < minute) && (minute < 25)) || ((34 < minute) && (minute < 40))) {
    byte half[] = { 54, 53, 52, 51 };
    manipulateDisplay(half, 4);
  } 

  // if VOR needs to be shown
  else if (((24 < minute) && (minute < 30)) || (39 < minute)) {
    byte half[] = { 35, 36, 37};
    manipulateDisplay(half, 3);
  } 

}

// Use this if the LEDs start with 0 in the top right corner
void generateAnalogueHoursFlipped(int input_hour, int minute) {

  byte hour = input_hour;
  if (minute > 24) {
    hour++;
  }

  if (hour % 12 == 0) {
    byte defaultDisplay[] = { 78, 79, 80, 81, 82};
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 1) {
    byte defaultDisplay[] = { 59, 60, 61 };
    manipulateDisplay(defaultDisplay, 3);
    if (minute > 4) {
      byte s_of_ones[] = { 62 };
    manipulateDisplay(s_of_ones, 1);
    }
  } else if (hour % 12 == 2) {
    byte defaultDisplay[] = { 57, 58, 59, 60, };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 3) {
    byte defaultDisplay[] = { 75, 74, 73, 72};
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 4) {
    byte defaultDisplay[] = { 70, 69, 68, 67};
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 5) {
    byte defaultDisplay[] = { 117, 116, 115, 114 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 6) {
    byte defaultDisplay[] = { 83, 84, 85, 86, 87 };
    manipulateDisplay(defaultDisplay, 5);
  } else if (hour % 12 == 7) {
    byte defaultDisplay[] = { 98, 97, 96, 95, 94, 93 };
    manipulateDisplay(defaultDisplay, 6);
  } else if (hour % 12 == 8) {
    byte defaultDisplay[] = { 91, 90, 89, 88 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 9) {
    byte defaultDisplay[] = { 99, 100, 101, 102 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 10) {
    byte defaultDisplay[] = { 103, 104, 105, 106 };
    manipulateDisplay(defaultDisplay, 4);
  } else if (hour % 12 == 11) {
    byte defaultDisplay[] = { 119, 118, 117 };
    manipulateDisplay(defaultDisplay, 3);
  }
}

void generateAnalogueFlipped(int hour, int minute) {
  resetDisplay();
  generateAnalogueDefaultFlipped();
  generateAnalogueHoursFlipped(hour, minute);
  generateAnalogueMinutesFlipped(minute);
  FastLED.show();
}

void generateDigitalTopLeft(int hour) {
  const byte topLeft[3][12] = { 
    {10, 9, 8, 11, 13, 32, 30, 54, 53, 52, 33, 35}, //0
    {8, 13, 30, 35, 52}, //1
    {10, 9, 8, 13, 30, 31, 32, 33, 54, 53, 52}}; //2
    
  manipulateDisplay(topLeft[hour/10], digitLength[hour/10]);
}

void generateDigitalTopRight(int hour) {
  const byte topRight[10][13] = { 
    {6, 5, 4, 15, 17, 28, 26, 37, 39, 50, 49, 48}, //0
    {4, 17, 26, 39, 48}, //1 
    {6, 5, 4, 17, 37, 50, 49, 48, 28, 27, 26}, //2 
    {6, 5, 4, 17, 26, 27, 39, 50, 49, 48}, //3
    {6, 4, 15, 17, 28, 27, 26, 39, 48}, //4
    {6, 5, 4, 15, 39, 50, 49, 48, 28, 27, 26}, //5
    {6, 5, 4, 15, 37, 39, 50, 49, 48, 28, 27, 26}, //6 
    {6, 5, 4, 17, 28, 39, 48, }, //7
    {6, 5, 4, 50, 49, 48, 28, 27, 26, 15, 17, 37, 39}, //8 
    {6, 5, 4, 50, 49, 48, 28, 27, 26, 15, 17, 39} //9
  };
  manipulateDisplay(topRight[hour%10], digitLength[hour%10]);
}

void generateDigitalBottomLeft(int minute) {
  const byte bottomLeft[6][12] = { 
    {76, 75, 74, 77, 79, 98, 96, 99, 101, 120, 119, 118}, //0
    {74, 79, 96, 101, 118}, //1
    {76, 75, 74, 120, 119, 118, 98, 97, 96, 79, 99}, //2
    {76, 75, 74, 79, 120, 119, 118, 101, 97, 96 }, //3
    {74, 79, 96, 101, 118, 76, 77, 98, 97}, //4
    {76, 75, 74, 120, 119, 118, 98, 97, 96, 77, 101} //5
    };
  manipulateDisplay(bottomLeft[minute/10], digitLength[minute/10]);
}

void generateDigitalBottomRight(int minute) {
   const byte bottomRight[10][13] = { 
    {72, 71, 70, 81, 83, 94, 92, 105, 103, 116, 115, 114}, //0 -> 12 
    {70, 83, 92, 105, 114}, //1 -> 5
    {72, 71, 70, 94, 93, 92, 103, 83, 116, 115, 114}, //2 -> 11
    {70, 72, 71, 83, 93, 92, 105, 114, 115, 116}, //3 -> 10
    {72, 81, 70, 94, 93, 92, 83, 105, 114, 105}, //4 -> 9
    {72, 71, 70, 81, 94, 93, 92, 105, 116, 115, 114}, //5 -> 11
    {72, 71, 70, 81, 94, 93, 92, 105, 103, 116, 115, 114}, //6 -> 12
    {72, 71, 70, 83, 92, 105, 114}, //7 -> 7
    {72, 71, 70, 81, 83, 94, 93, 92, 116, 103, 115, 114, 105}, //8 -> 13
    {72, 71, 70, 81, 83, 94, 93, 92, 116, 115, 114, 105} //9 -> 12
  };
  manipulateDisplay(bottomRight[minute%10], digitLength[minute%10]);
}

void generateDigital(int hour, int minute) {
  byte defaultDisplay[] = { 63, 64, 65};
  resetDisplay();
  manipulateDisplay(defaultDisplay, 3);
  generateDigitalTopLeft(hour);
  generateDigitalTopRight(hour);
  generateDigitalBottomLeft(hour);
  generateDigitalBottomRight(hour);
  FastLED.show();

}

void generateIsaDavid(){
  resetDisplay();

  byte isa_und_david[] = { 7, 6, 4, 39, 40, 41, 66, 69, 70, 71, 75};
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
  setBrightness();
  if(mode==0) {
    generateAnalogueFlipped(displayTime.hour(), displayTime.minute());
  }
  else if(mode==1) {
    generateDigital(displayTime.hour(), displayTime.minute());
  }
  else if(mode==2) {
   generateIsaDavid();
  }

}






