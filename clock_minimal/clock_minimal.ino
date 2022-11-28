#include <Wire.h> // Die Datums- und Zeit-Funktionen der DS3231 RTC werden ueber das I2C aufgerufen.
#include "RTClib.h"

RTC_DS3231 rtc;

bool syncOnFirstStart = true; // true, falls die Zeitinformationen der RTC mit dem PC synchronisiert werden sollen.
                               // sollte standardmaeig auf false stehen

void setup () {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  delay(3000); // Warte auf Terminal

  if (! rtc.begin()) {
    Serial.println("Kann RTC nicht finden");
    while (1);
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

void loop () {
  DateTime now = rtc.now();

  // immer, wenn die RTC auf 0, 10, 20, 30, 40 oder 50 volle Sekunden hochgezaehlt hat, sollen datum und Zeit
  // angegeben werden und die boardinterne LED fuer eine Sekunde leuchten.
  if (now.second() % 10 == 0) { 
    
    digitalWrite(LED_BUILTIN, HIGH);
    
    // Serial.print(now.year(), DEC);
    // Serial.print('/');
    // Serial.print(now.month(), DEC);
    // Serial.print('/');
    // Serial.print(now.day(), DEC);
    // Serial.print(" (");
    // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    // Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    // Serial.print(':');
    // Serial.print(now.second(), DEC);
    Serial.println("What");
    
    delay(4000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(5900);  // Unterbrechung der Zeitabfrage bis kurz vor dem naechsten 10s-Wert
  }
}
