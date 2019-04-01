# P3RGB64x32MatrixPanel library for ESP32

ESP32 Arduino library for P3 64x32 RGB LED Matrix Panel.

The panel is available at:
- https://www.amazon.com/dp/B079JSKF21/
- https://amzn.to/2jlqX1q (Amazon Japan)
etc.

[![DEMO](http://img.youtube.com/vi/5Z31kwsd-1I/0.jpg)](http://www.youtube.com/watch?v=5Z31kwsd-1I)
(Click to play the video)

This software is released under the MIT License, see LICENSE.txt.

# Installation

To download, click the "Download ZIP" button, and specify the zip file
from the "Sketch > Include Library > Add .ZIP Library" menu in the Arduino IDE.

You also need to install [Adafruit_GFX_Library](https://github.com/adafruit/Adafruit-GFX-Library) from the "Library > Manage Libraries" menu.

## Patching GPIO to avoid eratta of ESP32

You should patch the `tools/sdk/ld/esp32.peripherals.ld` to avoid random pixel noise on the display as following:

```
/* PROVIDE ( GPIO = 0x3ff44000 ); */
PROVIDE ( GPIO = 0x60004000 );
```

Please refer section 3.3. in https://www.espressif.com/sites/default/files/documentation/eco_and_workarounds_for_bugs_in_esp32_en.pdf for more details.

# Wiring ESP32 with the LED Matrix Panel

The panel has HUB75 compatible pins. The library provides two constructors.
One with default pin wiring like:

```
+---------+   Panel - ESP32 pins
|  R1 G1  |    R1   - IO25      G1   - IO26
|  B1 GND |    B1   - IO27      GND  - GND
|  R2 G2  |    R2   - IO21      G2   - IO22
|  B2 GND |    B2   - IO23      GND  - GND
|   A B   |    A    - IO12      B    - IO16
|   C D   |    C    - IO17      D    - IO18
| CLK LAT |    CLK  - IO15      LAT  - IO32
|  OE GND |    OE   - IO33      GND  - GND
+---------+
```

The second constructor takes each single pin as argument so that yo umay customize the pin wiring,
but the pins for LAT and OE must be assigned to IO32 or larger, and other pins must use IO1-31.
Note that IO only for INPUT or ocuppied by other peripheral devices cannot be used.

The panel must be powered by 5V AC adapter with enough current capacity.
(Current varies due to how many LED are turned on at the same time.
 To drive all the LEDs, you need 5V4A adapter.)

# Usage

Please check the examples directory for sample programs.

Note that this library heavily use the timer interrupt to drive LED matrix to enable software based 5bit PWM for each line.
This may conflict with WiFi setup, so you should start WiFi before `matrix.begin()` which sets up timer interrupts.

