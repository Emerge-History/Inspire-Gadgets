#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "font.h"
#include "Bitmap.h"

#define TFT_RST    -1
#define TFT_CS   17
#define TFT_DC   5
#define TFT_WIDTH 80
#define TFT_HEIGHT 160
#define BRIGHTNESS 27
extern uint8_t ColorList[256][3];

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

uint16_t colorRGB(uint8_t blue, uint8_t green, uint8_t red) {
  return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}
void drawLogo(int x, int y) { //67 151
  tft.drawBitmap(x, y, eLogo, logoWidth, logoHeight, ST77XX_WHITE);
  tft.drawRect(x, y + 2, 3, 1, tft.color565(136, 45, 236));
}
int bootRect[4] = {80, 60, 36, 13};
uint16_t bootRectColor[4] = {colorRGB(236, 45, 136), colorRGB(255, 255, 255), colorRGB(0, 0, 255), colorRGB(255, 255, 0)};

void drawBootScreen(boolean f, int c) {
  if (f) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setFont(&kampen_pixel5pt7b);
    tft.setCursor(15, 12);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextWrap(true);
    tft.print("Emerge");
    tft.setFont(&kampen_pixel8pt7b);
    tft.setCursor(0, 50);
    tft.print("COLOR");
    tft.setCursor(0, 72);
    tft.print("PEN");
    tft.drawBitmap(45, 90, pen, penWidth, penHeight, ST77XX_WHITE);
    drawLogo(67, 151);
  }
  tft.drawRect(0, 137, TFT_WIDTH, 2, ST77XX_BLACK);
  for (int j = 0; j < 4; j++) {
    tft.drawRect(c > bootRect[j] ? c - 13 - bootRect[j] : c + 80 - bootRect[j], 137, 13, 2, bootRectColor[j]);
  }
}

void setBrightness(int b) {
  ledcWrite(1, b);
}

void drawSlurping(boolean f, int g) {
  if (f) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setFont(&kampen_pixel5pt7b);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextWrap(true);
    tft.setCursor(5, 12);
    tft.print("Slurping");
    tft.drawBitmap(23, 25, slurping, slurpingWidth, slurpingHeight, ST77XX_WHITE);
    drawLogo(67, 151);
  }
  tft.setFont(&kampen_num);
  tft.setCursor(29, 146);
  tft.fillRect(29, 141, 20, 7, ST77XX_BLACK);
  char tmpS[5];
  sprintf(tmpS, "%d%%", g);
  tft.print(tmpS);
  for (int i = 0; i < g + 1; i++) {
    tft.drawLine(35, 110 - (g / 4) * 2, 44, 110 - (g / 4) * 2, ST77XX_WHITE);
  }
}
void drawSelecting(boolean f, int t, int s) {
  int colorChange[4];
  char *colorType[4] = {"RGB", "CMYK", "HSB"};
  if (f) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setFont(&kampen_pixel5pt7b);
    tft.drawBitmap(51, 5, COLOR_TYPE[t], 24, 7, ST77XX_WHITE);
    for (int i = 0; i < 4; i++) {
      tft.setCursor(44, 66 + i * 11);
      tft.print(colorType[t][i]);
    }
    drawLogo(67, 151);
  }
  for (int i = 0; i < 256; i++) {
    tft.fillRect(17, (i - s) * 3 + 116, 14, 3, colorRGB(ColorList[i][0], ColorList[i][1], ColorList[i][2]));
  }
  tft.drawLine(35, 120, 35, 113, ST77XX_WHITE);
  tft.drawLine(0, 117, 35, 117, ST77XX_WHITE);
  tft.setFont(&kampen_num);
  if (t == 1) {
    rgbToCmyk(ColorList[s], colorChange);
  } else if (t == 2) {
    rgbToHsv(ColorList[s], colorChange);
  } else {
    for (int i = 0; i < 3; i++) {
      colorChange[i] = ColorList[s][i];
    }
  }
  for (int i = 0; i < (t == 1 ? 4 : 3); i++) {
    tft.fillRect(56, 60 + i * 11, 20, 7, ST77XX_BLACK);
    tft.setCursor(56, 65 + i * 11);
    tft.print(colorChange[i]);
  }
  tft.drawBitmap(44, 34, COLOR, 29, 12, colorRGB(ColorList[s][0], ColorList[s][1], ColorList[s][2]));
}
void drawBattery(boolean v) {
  if (v) {
    tft.drawBitmap(3, 151, batteryBitmap, 6, 8, ST77XX_WHITE);
  } else {
    tft.fillRect(3, 151, 6, 8, ST77XX_BLACK);
  }
}
void displayInit() {
  ledcSetup(1, 5000, 8);
  ledcAttachPin(BRIGHTNESS, 1);
  setBrightness(255);
  tft.initR(INITR_MINI160x80);
  tft.setRotation(2);
  tft.invertDisplay(true);
  tft.fillScreen(ST77XX_BLACK);
  for (int i = 0; i < 256; i++) {
    ColorList[i][0] = 0;
    ColorList[i][1] = 0;
    ColorList[i][2] = 0;
  };
  delay(100);
  Serial.println("Initialized");
}
