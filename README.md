# clockproject



## Used Hardware:
- [Lichtsensor BH1750](https://www.az-delivery.de/en/products/gy-302-bh1750-lichtsensor-lichtstaerke-modul-fuer-arduino-und-raspberry-pi)
- [Rotary Encoder KY-040 Module](https://www.az-delivery.de/en/products/drehimpulsgeber-modul)
- [Real Time Clock DS3231](https://www.az-delivery.de/en/products/ds3231-real-time-clock)


# Set up:

1. Install the driver for the used Arduino Nano (most likely this (here)[https://www.az-delivery.de/en/products/nano-v3-mit-ch340-arduino-kompatibel] )
2. Chose the correct `COM` Port in the Arduino IDE and chose Arduino Nano
3. Chose the correct bootloader (Tools>Processor>ATMega328P (Old Bootloader)). At this point you should be able to flash the basic `Blink` example
4. Install the necessary libraries through the Arduino IDE library manager
  * For controlling the LEDs: [fastLED](https://github.com/FastLED/FastLED/)
  * For controlling the Clock: [Adafruit RTCLib](https://github.com/adafruit/RTClib)
  * For controlling the Light Sensor: [BH1750.h](https://github.com/claws/BH1750)
