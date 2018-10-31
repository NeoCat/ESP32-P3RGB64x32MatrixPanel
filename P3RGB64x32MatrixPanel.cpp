#include "P3RGB64x32MatrixPanel.h"

volatile SemaphoreHandle_t P3RGB64x32MatrixPanel::timerSemaphore;
P3RGB64x32MatrixPanel* P3RGB64x32MatrixPanel ::singleton;

void IRAM_ATTR P3RGB64x32MatrixPanel::onTimer() {
  portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
  portENTER_CRITICAL_ISR(&timerMux);

  singleton->draw();

  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
}

void P3RGB64x32MatrixPanel::begin() {
  singleton = this;

  pinMode(pinR1, OUTPUT);
  pinMode(pinG1, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinR2, OUTPUT);
  pinMode(pinG2, OUTPUT);
  pinMode(pinB2, OUTPUT);

  pinMode(pinLAT, OUTPUT);
  pinMode(pinCLK, OUTPUT);
  pinMode(pinOE,  OUTPUT);

  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);

  digitalWrite(pinLAT, LOW);
  digitalWrite(pinCLK, LOW);
  digitalWrite(pinOE, HIGH);

  timerSemaphore = xSemaphoreCreateBinary();
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 30, true);
  timerAlarmEnable(timer);
}

void P3RGB64x32MatrixPanel::stop() {
  if (timer) {
    timerDetachInterrupt(timer);
    timerEnd(timer);
  }
}

uint16_t P3RGB64x32MatrixPanel::colorHSV(long hue, uint8_t sat, uint8_t val) {
  uint8_t  r, g, b, lo;
  uint16_t s1, v1;

  // Hue ( 0 - 1535 )
  hue %= 1536;
  if (hue < 0) hue += 1536;
  lo = hue & 255;          // Low byte  = primary/secondary color mix
  switch (hue >> 8) {      // High byte = sextant of colorwheel
    case 0 : r = 255     ; g =  lo     ; b =   0     ; break; // R to Y
    case 1 : r = 255 - lo; g = 255     ; b =   0     ; break; // Y to G
    case 2 : r =   0     ; g = 255     ; b =  lo     ; break; // G to C
    case 3 : r =   0     ; g = 255 - lo; b = 255     ; break; // C to B
    case 4 : r =  lo     ; g =   0     ; b = 255     ; break; // B to M
    default: r = 255     ; g =   0     ; b = 255 - lo; break; // M to R
  }

  s1 = sat + 1;
  r  = 255 - (((255 - r) * s1) >> 8);
  g  = 255 - (((255 - g) * s1) >> 8);
  b  = 255 - (((255 - b) * s1) >> 8);

  v1 = val + 1;
  r = (r * v1) >> 11;
  g = (g * v1) >> 11;
  b = (b * v1) >> 11;

  return color555(r, g, b);
}

void P3RGB64x32MatrixPanel::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (x < 0 || x >= 64 || y < 0 || y >= 32) return;
  int16_t idx = x + y * 64;
  drawBuffer()[idx] = color;
}

void IRAM_ATTR P3RGB64x32MatrixPanel::draw() {
  static byte cnt = 30;
  static byte y = 15;
  static uint32_t out = 0;
  y = (y + 1) % 16;

  if (y == 0)
    cnt = (cnt + 1) % 31; // 31 must be skipped

  byte cmp = (cnt >> 4) | ((cnt >> 2) & 0x2) | (cnt & 0x4) | ((cnt << 2) & 0x8) | ((cnt << 4) & 0x10);

  for (int x = 0; x < 64; x++) {
    bool r1, b1, g1, r2, g2, b2;
    uint16_t c = matrixbuff[x + y * 64];
    r1 = (c & 0x1f) > cmp;
    g1 = ((c >>  5) & 0x1f) > cmp;
    b1 = ((c >> 10) & 0x1f) > cmp;
    c = matrixbuff[x + (y + 16) * 64];
    r2 = (c & 0x1f) > cmp;
    g2 = ((c >>  5) & 0x1f) > cmp;
    b2 = ((c >> 10) & 0x1f) > cmp;

    REG_WRITE(GPIO_OUT_REG, out |
       ((uint32_t)r1 << pinR1) |
       ((uint32_t)g1 << pinG1) |
       ((uint32_t)b1 << pinB1) |
       ((uint32_t)r2 << pinR2) |
       ((uint32_t)g2 << pinG2) |
       ((uint32_t)b2 << pinB2));

    REG_WRITE(GPIO_OUT_W1TS_REG, (1 << pinCLK));
  }
  //REG_WRITE(GPIO_OUT_W1TC_REG, (1 << pinCLK));

  REG_WRITE(GPIO_OUT1_W1TS_REG, (1 << (pinOE - 32)) | (1 << (pinLAT - 32)));

  out = ((y & 1) << pinA) | ((y & 2) << (pinB - 1)) |
        ((y & 4) << (pinC - 2)) | ((y & 8) << (pinD - 3));
  REG_WRITE(GPIO_OUT_REG, out);

  for (int x = 0; x < 8; x++) NOP(); // to wait latch and row switch

  REG_WRITE(GPIO_OUT1_W1TC_REG, (1 << (pinOE - 32)) | (1 << (pinLAT - 32)));
}
