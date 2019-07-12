#include "SparkFunISL29125.h"
#define SDA 21
#define SCL 22
#define VCC 19
SFE_ISL29125 RGB_sensor;
void ISL29125Init() {
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);
  if (RGB_sensor.init(SDA, SCL))
  {
    RGB_sensor.config(CFG1_MODE_RGB |  CFG1_12BIT, CFG2_IR_ADJUST_HIGH, CFG_DEFAULT);
    Serial.println("Sensor Initialization Successful\n\r");
  } else {
    Serial.println("Not Found ISL29125!");
  }
}

uint8_t *getRGB() {
  static uint8_t  ColorRGB[3];
  float r, g, b;
  r = RGB_sensor.readRed();
  g = RGB_sensor.readGreen();
  b = RGB_sensor.readBlue();
  
  ColorRGB[0] = (uint8_t)(r * 256 / 4096);
  ColorRGB[1]  = (uint8_t)(g * 256 / 4096);
  ColorRGB[2] = (uint8_t)(b * 256 / 4096);
  return ColorRGB;
}
