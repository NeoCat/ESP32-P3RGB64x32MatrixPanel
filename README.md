# P3RGB64x32MatrixPanel library for ESP32

ESP32 Arduino library for P3 64x32 RGB LED Matrix Panel (available at https://www.amazon.com/dp/B079JSKF21/ or https://amzn.to/2jlqX1q (Amazon Japan) ).

[![DEMO](http://img.youtube.com/vi/5Z31kwsd-1I/0.jpg)](http://www.youtube.com/watch?v=5Z31kwsd-1I)

This software is released under the MIT License, see LICENSE.txt.

# Installation

To download, click the "Download ZIP" button, and specify the zip file
from the "Sketch > Include Library > Add .ZIP Library" menu in the Arduino IDE.

You also need to install "Adafruit_GFX_Library" from the "Library > Manage Libraries" menu.

## Patching GPIO to avoid eratta of ESP32

You should patch the `tools/sdk/ld/esp32.peripherals.ld` to avoid random pixel noise on the display as following:

```
/* PROVIDE ( GPIO = 0x3ff44000 ); */
PROVIDE ( GPIO = 0x60004000 );
```

Please refer section 3.3. in https://www.espressif.com/sites/default/files/documentation/eco_and_workarounds_for_bugs_in_esp32_en.pdf for more details.

# Connection to ESP32 and the LED Matrix Panel

The panel has HUB75 compatible pins.

```
+---------+   Panel - ESP32 pins
|  R1 G1  |    R1   - IO25      G1   - IO26
|  B1 GND |    B1   - IO27
|  R2 G2  |    R2   - IO21      R2   - IO22
|  B2 GND |    B2   - IO23
|   A B   |    A    - IO12      B    - IO16
|   C D   |    C    - IO17      D    - IO18
| CLK LAT |    CLK  - IO15      LAT  - IO32
|  OE GND |    OE   - IO33
+---------+
```

The panel must be powered by 5V AC adapter with enough current capacity.
(Current varies due to how many LED are turned on at the same time.
 To drive all the LED, you need 5V4A adapter.)

# Usage

Please check the examples directory for sample programs.

Note that this library heavily use the timer interrupt to drive LED matrix to enable software based 5bit PWM for each line.
This may conflict with WiFi setup, so you should start WiFi before `matrix.begin()` which sets up timer interrupts.

