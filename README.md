# clockproject

<img width="707" alt="Foto of the clock" src="https://github.com/user-attachments/assets/606907e8-5f1c-4e4a-befc-bf529965d08f">

These are high level instructions to build a clock showing the time of day in 5 minute increments. You can however set the time to the minute (but the display won't allow for showing that). It also features an automatic diming function that dims the light of the clock depending on the surrounding lighting conditions. Everything is based on a simple Arduino board but basically any small microcontroller should work. The schematics are provided as a draw.io and a jpg file. 

## Features
* A small rotary encoder on the back that allows for easy seeting of the time. One click of the encoder equals to one minute of time. When setting the time, seconds are always set to 0
* A quartz clock including a button cell battery so the clock keeps time even if it loses power
* A photo diode that checks the ambient light and increases the light intensity of the LEDs in bright environments but dimms the LEDs down in darker condition (you don't want them to work full power in the middle of the night on your way to the fridge, do you?)
* A small easter egg by the creators in V3 which is activated if you push the rotary encoder.

# Building it

## What you need (Clock Materials)
I constructed my clock using four layers with an 11 by 11 LED grid (121 LEDs in total). You will need a bit of woodworking experience, a drill with a 24mm forstener bit, and a router. I added some photos below. The letters used for the front of the watch have been completely custom designed by me, including the font. It is a German version but I am sure you can adapt this to any language you want.
1) **Bottom Layer** - the bottom layer (the one facing the wall) consists of a 9mm plywood that is used to hold the LED strips. I used a router to cut a groove of around 2mm to completly flush-sink the light strips. I chose 55cm by 55cm for my watch. The LEDs are glued on in an S-like pattern starting with the first LED (index=0) in the top left when looking at it from the front. So the 11th LED (Index = 10) is at the very top right. I added a table below that tries to show you how they are indexed. 
2) **Middle Layer** - it consists of a layer with 11x11 holes drilled into it, each 3.33cm center to cetner. The holes have a diameter of 2.4cm which perfectly fit my watch face. I used the router again to also cut multiple "chambers" into wall-facing side of this layer to create space for the Arduino and cables. It was a tight fit though. Not only does the layer house the electronics, it also allows the light from the LEDs to beam out ensuring even lighting of the letters.
3) **Diffusion Layer** - To ensure the light is diffused correctly, I chose a 60cm by 60cm semi-translucent plate of acrylic glas. While I am not sure what exactly I ordered back in the day, I know it was from https://www.plexiglas.de/de/
4) **Top Layer** - This has the letters cut into it and will pull everything together.

I attached the bottom layer to the middle layer using screws, the middle layer to the diffusion layer using glue and the diffusion layer to the top layer also using glue.
I also printed a little holder for the photoelectric sensor and a dial for the rotary encoder. You can fit those any way you like though!

## What you need (Electronics):
- [Lichtsensor BH1750](https://www.az-delivery.de/en/products/gy-302-bh1750-lichtsensor-lichtstaerke-modul-fuer-arduino-und-raspberry-pi)
- [Rotary Encoder KY-040 Module](https://www.az-delivery.de/en/products/drehimpulsgeber-modul)
- [Real Time Clock DS3231](https://www.az-delivery.de/en/products/ds3231-real-time-clock)
- [5V WS2812B LED LightWS2812B Strip](https://www.btf-lighting.com/products/ws2812b-led-pixel-strip-30-60-74-96-100-144-pixels-leds-m?srsltid=AfmBOooGJsxJuNZ5xVatfi5f-yggzJujvBD4NljK_5oVEBL03-9eInbE) - the 5m strip should be plenty for this application, I chose the 30 LED/m version which allowed for a nice 60cm by 60cm clock but you can scale things however you want
- Any old Power Supply that delivers 5V. 15 Watts is more than enough.
- [Arduino Nano](https://store.arduino.cc/en-de/products/arduino-nano?srsltid=AfmBOoqwJg_YfYpz5xCnbxkUurWxllIExESOIfMVZlp91QfUAvdTk8tM) or a much cheaper but fully compatible alternative from shops like [AZ-Delivery](https://www.az-delivery.de/products/nano-v3-mit-ch340-arduino-kompatibel)


# Set up the Arduino code:
1. Install the driver for the used Arduino Nano (most likely this [here](https://www.az-delivery.de/en/products/nano-v3-mit-ch340-arduino-kompatibel]) when you are using the budget friendly Arduino from AZ-delivery. When using an original Arduino Nano, I don't think you need additional drivers.
2. Download and Install the Arduino IDE, there are multiple tutorials on YouTube to get you started. Chose the correct `COM` Port in the Arduino IDE and chose Arduino Nano
3. Chose the correct bootloader (Tools>Processor>ATMega328P (Old Bootloader)). At this point you should be able to flash the basic `Blink` example
4. Install the necessary libraries through the Arduino IDE library manager
  * For controlling the LEDs: [fastLED](https://github.com/FastLED/FastLED/)
  * For controlling the Clock: [Adafruit RTCLib](https://github.com/adafruit/RTClib)
  * For controlling the Light Sensor: [BH1750.h](https://github.com/claws/BH1750)
  * For using the encoder: [Encoder](https://github.com/PaulStoffregen/Encoder)
5. Flash the Arduino sketch for the watch. Versions 1 and 2 are not at all debugged in any way, shape, or form and will most likely not work. They are here for reference only. Version 3 has a little easter egg which will not be applicable to you at all. Remove if you want or create something for yourself.

### LED Index Layout

I cut the LED strip into 11 pieces consisting of 11 LEDs each. Doing so allowed me to glue them onto the back plate flat. I then used some wire and a soldering iron to connect them back together as you can see in the photo below. The indices of the LEDs therefore are as follows when looking at the watch from the front:
||A|B|C|D|E|F|G|H|I|J|K|
|---|---|---|---|---|---|---|---|---|---|---|---|
|**A**|0|1|2|3|4|5|6|7|8|9|10|
|**B**|21|20|19|18|17|16|15|14|13|12|11|
|**C**|22|23|24|25|26|27|28|29|30|31|32|
|**...**|...|...|...|...|...|...|...|...|...|...|...|
|**J**|109|108|107|106|105|104|103|102|101|100|99|
|**K**|110|111|112|113|114|115|116|117|118|119|120|


### Photos

**Backplate Front**
<img width="453" alt="Backplate front" src="https://github.com/user-attachments/assets/4e1f236b-e342-4ac0-9f21-460ed1b262c0">

**Backplate back**
<img width="468" alt="Backplate back" src="https://github.com/user-attachments/assets/5e44e4b1-ae7e-49ad-9b21-c2d321eb8eab">

**Middle Plate**
<img width="571" alt="Middle plate" src="https://github.com/user-attachments/assets/4fa2185f-54d1-4400-94df-29945f70d3cb">

