#include "GameHub.hpp"
#include "Menu.hpp"


MenuApp* menu;

void setup() {
    Serial.begin(9600);
    M5.begin();
    menu = new MenuApp();
}

void loop() {
  menu->main();
}
