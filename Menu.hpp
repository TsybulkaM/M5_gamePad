#ifndef MENU_HPP
#define MENU_HPP

#include "GameHub.hpp"
#include "Services.hpp"

class MenuApp {
public:
  MenuApp() 
  : menuOption(-1),
   COLOR_BACKGROUND(0xb1ad80),
   COLOR_CIRCLE(0xa7c744),
   COLOR_TEXT_MAIN(0x3f4b3d),
   COLOR_TEXT_SUB(0x272c1a),
   COLOR_LABEL_BG1(0xbfbc93),
   COLOR_LABEL_BG2(0xdad7b1),

   circle1X(52),
   circle1Y(10),
   circle1Radius(75),
   
   circle2X(113),
   circle2Y(252),
   circle2Radius(39)
  {
    menu_battery =  new Battery();
  }

  void main() {
    menu_battery->updateAndDisplay(7, 28, 2);
    if (menuOption == -1) {
      M5.Lcd.setRotation(4);
      menuOption = 0;
      menu_battery->display(10, 10, 2);
      updateMenuDisplay();
    }
    waitForMenuSelection();
  }

private:
  uint16_t COLOR_BACKGROUND, COLOR_CIRCLE, COLOR_TEXT_MAIN, COLOR_TEXT_SUB, COLOR_LABEL_BG1, COLOR_LABEL_BG2;
  int circle1X,circle1Y,circle1Radius,circle2X,circle2Y,circle2Radius;

  int menuOption;
  Battery* menu_battery;

  void displayTital(uint8_t x, uint8_t y, uint8_t front_size) {
    M5.Lcd.fillScreen(COLOR_BACKGROUND); 
    M5.Lcd.fillCircle(circle1X, circle1Y, circle1Radius, COLOR_CIRCLE);
    M5.Lcd.fillCircle(circle2X, circle2Y, circle2Radius, COLOR_CIRCLE);
    M5.Lcd.setTextColor(COLOR_TEXT_MAIN, COLOR_CIRCLE);
    M5.Lcd.setTextFont(front_size);
    M5.Lcd.drawString("GamePad", x, y);
  }

  void darkMode() {
    M5.Axp.ScreenBreath(1);
  }

  void lightMode() {
    M5.Axp.ScreenBreath(100);
  }

  /*void checkSleepMode() {
    M5.update();
    if (M5.BtnA.wasReleased() || M5.BtnB.wasReleased()) {
      sleepTimer.setInterval(sleepTime);
      isActive = true;
    }
  }*/

  void waitForMenuSelection() {
    M5.update();
    if (M5.BtnB.wasPressed()) {
      if (menuOption == GameHub_size-1) menuOption = -1;
      menuOption++;
      updateMenuDisplay();
    }

    if (M5.BtnA.wasPressed()) {
      handleMenuSelection(menuOption);
    }
  }

  void updateMenuDisplay() {
    displayTital(5, 40, 3);
    M5.Lcd.fillScreen(COLOR_BACKGROUND);
    M5.Lcd.setTextColor(COLOR_TEXT_SUB, COLOR_BACKGROUND);

    for (int i = 0; i < GameHub_size; ++i) {
      M5.Lcd.setCursor(10, 70 + i * 30);

      if (i == menuOption) {
        M5.Lcd.setTextColor(COLOR_TEXT_SUB, COLOR_BACKGROUND);
      }

      M5.Lcd.printf(GameHub[i]->name);
    }
  }

  void handleMenuSelection(int option) {
    M5.Lcd.setTextColor();
    GameHub[option]->engine->execute();
    menuOption = -1;
  }
};


#endif // MENU_HPP