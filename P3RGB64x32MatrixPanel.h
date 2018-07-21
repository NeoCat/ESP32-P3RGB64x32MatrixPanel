#ifndef _ESP32_P3_RGB_64_32_MATRIX_PANEL
#define _ESP32_P3_RGB_64_32_MATRIX_PANEL

#include <vector>
#include <array>
#include "Adafruit_GFX.h"

class P3RGB64x32MatrixPanel : public Adafruit_GFX {
  public:
    P3RGB64x32MatrixPanel(bool _doubleBuffer = false)
      : Adafruit_GFX(64, 32), doubleBuffer(_doubleBuffer) {
      _matrixbuff.resize(doubleBuffer ? 2 : 1);
      matrixbuff = _matrixbuff[0].data();
    }
    void begin(void);
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

    uint16_t color444(uint8_t r, uint8_t g, uint8_t b) { return ((r & 0xf) << 1) | ((uint16_t)(g & 0xf) << 6) | ((uint16_t)(b & 0xf) << 11); }
    uint16_t color555(uint8_t r, uint8_t g, uint8_t b) { return (r&0x1f) | ((uint16_t)(g & 0x1f) << 5) | ((uint16_t)(b & 0x1f) << 10); }
    uint16_t colorHSV(long hue, uint8_t sat, uint8_t val);

    void swapBuffer() {
      matrixbuff = drawBuffer();
    }

    uint16_t* matrixbuff;
    std::vector<std::array<uint16_t, 64*32>> _matrixbuff;

  private:

    static void IRAM_ATTR onTimer(void);
    void draw();

    uint16_t* drawBuffer() {
      if (!doubleBuffer) return _matrixbuff[0].data();
      if (matrixbuff == _matrixbuff[0].data())
        return _matrixbuff[1].data();
      else
        return _matrixbuff[0].data();
    }

    bool doubleBuffer;

    const int pinR1 = 25;
    const int pinG1 = 26;
    const int pinB1 = 27;
    const int pinR2 = 21;
    const int pinG2 = 22;
    const int pinB2 = 23;

    const int pinCLK = 15;
    const int pinLAT = 32;
    const int pinOE = 33;

    const int pinA = 12;
    const int pinB = 16;
    const int pinC = 17;
    const int pinD = 18;

    static volatile SemaphoreHandle_t timerSemaphore;
    static P3RGB64x32MatrixPanel *singleton;
};

#endif
