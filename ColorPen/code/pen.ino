#define LEDPIN 26
int bootCount = 0;
int state = 0;
int preState = 0;
int colorStyle = 0;
int preColorStyle = 0;
int colorSelect = 100;
uint8_t  ColorList[256][3];
boolean f0, f1, f2, f3;
void setup() {
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  batteryInit();
  ledcSetup(0, 5000, 8);
  ledcAttachPin(LEDPIN, 0);
  displayInit();
  drawBootScreen(true, bootCount++);
  clickButtonInit();
  drawBootScreen(false, bootCount++);
  WiFiInit();
  drawBootScreen(false, bootCount++);
  serverInit();
  drawBootScreen(false, bootCount++);
  BLEHIDInit();
  drawBootScreen(false, bootCount++);
  ISL29125Init();
  drawBootScreen(false, bootCount);
  for (bootCount; bootCount < 40; bootCount++) {
    drawBootScreen(false, bootCount);
    delay(30);
  }
  Serial.println("Server started");
  f0 = true;
  f2 = true;
  f3 = true;
}

unsigned long getRgbInterval;
int rgbIndex, preRgbIndex;
long RGBHEX;
String strHex = "000000";
char strArray[6];

unsigned long sleepInterval;
unsigned long bootInterval;
unsigned long batteryInterval;
boolean batteryBooelan;
void sleepInit() {
  sleepInterval = millis();
  f3 = true;
}
void loop() {
  buttonUpdate();
  if (batteryRead())
  {
    if (millis() - batteryInterval > 500) {
      batteryInterval = millis();
      batteryBooelan = !batteryBooelan;
      drawBattery(batteryBooelan);
    }
  }
  if (millis() - sleepInterval > 15000) {
    state = 5;
    if (millis() - sleepInterval > 30000) {
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_14, 0);
      esp_deep_sleep_start();
    }
  }
  switch (state) {
    case 0:
      ledcWrite(0, 0);
      if (f0) {
        drawSlurping(f0, 0);
        f0 = false;
      }
      break;
    case 1:
      if (millis() - getRgbInterval > 10) {
        getRgbInterval = millis();
        if (rgbIndex < 256) {
          ledcWrite(0, rgbIndex / 4);
          uint8_t *tmpRgb = getRGB();
          for (int i = 0; i < 3; i++) {
            ColorList[rgbIndex][i] = tmpRgb[i];
          }
          rgbIndex++;
          if (f1) {
            drawSlurping(f1, map(rgbIndex, 0, 255, 0, 100));
            f1 = false;
          } else {
            if (preRgbIndex != rgbIndex) {
              preRgbIndex = rgbIndex;
              drawSlurping(f1, map(rgbIndex, 0, 255, 0, 100));
            }
          }
        } else {
          Serial.println(rgbIndex);
          for (int i = 0; i < 3; i++) {
            ColorList[0][i] = 0;
            ColorList[255][i] = 0;
          }
          rgbIndex = 0;
          state = 2;
          f2 = true;
        }
      }
      break;
    case 2:
      ledcWrite(0, 0);
      rgbIndex = 0;
      if (preColorStyle != colorStyle) {
        preColorStyle = colorStyle;
        f2 = true;
      }
      drawSelecting(f2, colorStyle, colorSelect);
      f2 = false;
      //Serial.println("selecting");
      break;
    case 3:
      RGBHEX = (long(ColorList[colorSelect][0])  << 16L) | (long(ColorList[colorSelect][1])   << 8L) | (long(ColorList[colorSelect][2]));
      strHex = String(RGBHEX, HEX);
      strHex.toUpperCase();
      strHex.toCharArray(strArray, 7);
      sendCharacter(strArray);
      state = 2;
      break;
    case 4:
      Serial.println("updateing");
      serverHandle();
      break;
    case 5:
      if (millis() - bootInterval > 80) {
        bootInterval = millis();
        drawBootScreen(f3, bootCount++);
        f3 = false;
        if (bootCount > 93) {
          bootCount = 0;
        }
      }
      break;
    default:
      Serial.println("deepSleeping");
  }
}
