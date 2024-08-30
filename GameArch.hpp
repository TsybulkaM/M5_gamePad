#ifndef GAMEARCH_HPP
#define GAMEARCH_HPP

#include <EEPROM.h>
#include <M5StickCPlus.h>
#include <GyverTimer.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

class ArcadeGame {
public:
  virtual void execute() = 0;
  virtual bool Setup() = 0;
  virtual void Reset() = 0;
  virtual bool MainLoop() = 0;
  virtual bool End() = 0;
};

#endif // GAMEARCH_HPP