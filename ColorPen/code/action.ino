#include <Bounce2.h>

#define TOP_BUTTON 14
#define UP_BUTTON 32
#define CLICK_BUTTON 33
#define DOWN_BUTTON 25
#define PING_GND 12

#define SLEEP 30
#define DEEPSLEEP 30

extern int state;
extern int colorStyle;
extern int colorSelect;
float floatSelect = 100;
extern int rgbIndex;
extern boolean f1, f2, f3;
int upDownButtonState;
boolean resetFlag = false;
boolean resetB1 = false;
boolean resetB2 = false;
boolean clickFlag = false;

Bounce topButton = Bounce();
Bounce upButton = Bounce();
Bounce clickButton = Bounce();
Bounce downButton = Bounce();

void clickButtonInit() {
  //pinMode(PING_GND, OUTPUT);
  //digitalWrite(PING_GND, LOW);

  pinMode(TOP_BUTTON, INPUT_PULLUP);
  topButton.attach(TOP_BUTTON);
  topButton.interval(50);

  pinMode(UP_BUTTON, INPUT_PULLUP);
  upButton.attach(UP_BUTTON);
  upButton.interval(50);

  pinMode(CLICK_BUTTON, INPUT_PULLUP);
  clickButton.attach(CLICK_BUTTON);
  clickButton.interval(50);

  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  downButton.attach(DOWN_BUTTON);
  downButton.interval(50);

}
void buttonUpdate() {
  topButton.update();
  upButton.update();
  clickButton.update();
  downButton.update();
  if (!resetFlag)
  {
    if (topButton.fell()) {
      state = 1;
      f1 = true;
      resetB1 = true;
      Serial.println("topPush");
      sleepInit();
    }
    if (topButton.rose()) {
      resetB1 = false;
      if (rgbIndex) {
        state = 0;
        f0 = true;
        rgbIndex = 0;
      } else {
        state = 2;
        f1 = true;
      }
      sleepInit();
      Serial.println("topPop");
    }

    if (clickButton.fell()) {
      resetB2 = true;
      clickFlag = true;
      sleepInit();
    }

    if (clickFlag) {
      if (clickButton.duration() > 2000) {
        Serial.println("send....");
        state = 3;
        clickFlag = false;
      }
    }

    if (clickButton.rose()) {
      resetB2 = false;
      if (clickFlag) {
        colorStyle = colorStyle < 2 ? colorStyle + 1 : 0;
        Serial.println(colorStyle);
      }
      clickFlag  = false;
      sleepInit();
    }

    if (resetB1 && resetB2) {
      if (clickButton.duration() > 5000 && topButton.duration() > 5000) {
        state = 4;
        resetFlag = true;
        Serial.println("reset");
      }
    }

    if (upButton.fell()) {
      floatSelect = floatSelect < 255 ? floatSelect + 1 : 255;
      colorSelect = (int)floatSelect;
      upDownButtonState = 1;
      Serial.println(colorSelect);
      sleepInit();
    }
    if (upButton.rose()) {
      upDownButtonState = 0;
      sleepInit();
    }
    if (downButton.fell()) {
      upDownButtonState = 2;
      floatSelect = floatSelect > 0 ? floatSelect - 1 : 0;
      colorSelect = (int)floatSelect;
      Serial.println(colorSelect);
      sleepInit();
    }
    if (downButton.rose()) {
      upDownButtonState = 0;
      sleepInit();
    }
    if (upDownButtonState == 1) {
      if (upButton.duration() > 1000) {
        floatSelect = floatSelect < 255 ? floatSelect + 0.5 : 255;
        colorSelect = (int)floatSelect;
        Serial.println(colorSelect);
      }
    }
    if (upDownButtonState == 2) {
      if (downButton.duration() > 1000) {
        floatSelect = floatSelect > 0 ? floatSelect - 0.5 : 0;
        colorSelect = (int)floatSelect;
        Serial.println(colorSelect);
      }
    }
  }
}
