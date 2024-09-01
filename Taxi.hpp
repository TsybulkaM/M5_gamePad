#ifndef TAXI_HPP
#define TAXI_HPP

#include "GameArch.hpp"

class Taxi : public ArcadeGame {
private:
  uint8_t CARS_WIDTH, CARS_HEIGHT;
  int16_t CAR_COLOR, WINDOW_CAR_COLOR, OPPOSITE_COLOR;
  uint8_t SMOOTHING_FACTOR;
  float smoothedAccX;
  uint8_t MIN_OPPOSITE_SPEED, MAX_OPPOSITE_SPEED, LINE_SPEED;
  uint8_t LINE_POSSITIONX, LINE_POSSITIONY, LINE_LENGTH;
  uint16_t lineY1, lineY2, lineY3, lineY4, lineY5;

  uint8_t ADRESS_MAX_SCORE_TAXI;

  uint32_t resultTaxi;
  uint32_t MAX_SCORE_TAXI;

  float accX, accY, accZ;

  class TaxiObj {
  public:
    uint16_t x;
    uint16_t y;
  };

  class Opposite {
  public:
    uint16_t x;
    uint16_t y;
    uint16_t speed;
  };

  TaxiObj taxi;
  Opposite opposite1, opposite2, opposite3;

  bool checkCollision(const TaxiObj& p, const Opposite& o) {
    return (p.x < o.x + CARS_WIDTH && p.x + CARS_WIDTH > o.x && p.y < o.y + CARS_HEIGHT && p.y + CARS_HEIGHT > o.y);
  }

  void drawTheOpposite(Opposite& opposite, short line) {
    M5.Lcd.fillRect(opposite.x, opposite.y, CARS_WIDTH, CARS_HEIGHT, BLACK);

    opposite.y += opposite.speed;

    if (opposite.y + CARS_HEIGHT >= M5.Lcd.height() && random(60) < 2) {
      if (line == 1) {
        opposite.x = random(0, M5.Lcd.width() / 3 - CARS_WIDTH);
      } else if (line == 2) {
        opposite.x = random(M5.Lcd.width() / 3 + 1, 2 * M5.Lcd.width() / 3 - CARS_WIDTH - 1);
      } else if (line == 3) {
        opposite.x = random(2 * M5.Lcd.width() / 3 + 1, M5.Lcd.width() - CARS_WIDTH - 1);
      }
      opposite.y = 0;
      opposite.speed = random(MIN_OPPOSITE_SPEED, MAX_OPPOSITE_SPEED);
    }

    M5.Lcd.fillRect(opposite.x, opposite.y, CARS_WIDTH, CARS_HEIGHT, OPPOSITE_COLOR);
    M5.Lcd.fillRect(opposite.x + 2, opposite.y + 2, 22, 6, WINDOW_CAR_COLOR);
    M5.Lcd.fillRect(opposite.x + 2, opposite.y + 22, 22, 6, WINDOW_CAR_COLOR);
  }

  void drawLines() {
    singleLineDrawer(lineY1);
    singleLineDrawer(lineY2);
    singleLineDrawer(lineY3);
    singleLineDrawer(lineY4);
    singleLineDrawer(lineY5);
  }


  void singleLineDrawer(uint16_t& lineY) {
    M5.Lcd.drawLine(LINE_POSSITIONX, lineY, LINE_POSSITIONX, lineY + LINE_LENGTH, TFT_BLACK);
    M5.Lcd.drawLine(2 * LINE_POSSITIONX, lineY, 2 * LINE_POSSITIONX, lineY + LINE_LENGTH, TFT_BLACK);

    lineY += LINE_SPEED;

    M5.Lcd.drawLine(LINE_POSSITIONX, lineY, LINE_POSSITIONX, lineY + LINE_LENGTH, TFT_WHITE);
    M5.Lcd.drawLine(2 * LINE_POSSITIONX, lineY, 2 * LINE_POSSITIONX, lineY + LINE_LENGTH, TFT_WHITE);

    if (lineY > M5.Lcd.height()) {
      lineY = 0;
    }
  }

public:
  Taxi() : 
    CARS_WIDTH(26), 
    CARS_HEIGHT(34), 
    CAR_COLOR(YELLOW), 
    WINDOW_CAR_COLOR(BLUE), 
    OPPOSITE_COLOR(DARKGREEN), 
    SMOOTHING_FACTOR(0.05),
    MIN_OPPOSITE_SPEED(3),
    MAX_OPPOSITE_SPEED(5),
    LINE_SPEED(1),
    LINE_POSSITIONX(SCREEN_WIDTH/ 3),
    LINE_POSSITIONY(SCREEN_WIDTH / 4),
    LINE_LENGTH(SCREEN_WIDTH / 6),
    ADRESS_MAX_SCORE_TAXI(sizeof(int))
  {
    MAX_SCORE_TAXI = EEPROM.readInt(ADRESS_MAX_SCORE_TAXI);
  }

  void execute() {
    while(this->Setup()) {
      this->Reset();
      this->MainLoop();
      if (!this->End()) return;
    }
  }

  bool Setup() override {
    M5.Imu.Init();

    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(30, M5.Lcd.height() / 2 - 50);
    M5.Lcd.print("Ready?");
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(10, M5.Lcd.height() / 2 - 30);
    M5.Lcd.print("Place the device horizontal");

    return choice();
  }

  void Reset() override {
    smoothedAccX = 0;

    taxi.x = M5.Lcd.width() / 2 - CARS_WIDTH / 2;
    taxi.y = M5.Lcd.height() / 2 + CARS_HEIGHT / 2;

    lineY1 = 0;
    lineY2 = LINE_POSSITIONY;
    lineY3 = 2 * LINE_POSSITIONY;
    lineY4 = 3 * LINE_POSSITIONY;
    lineY5 = 4 * LINE_POSSITIONY;

    opposite1.y = M5.Lcd.height();
    opposite2.y = M5.Lcd.height();
    opposite3.y = M5.Lcd.height();
  }

  bool MainLoop() override {
    while (true) {
      drawLines();

      M5.IMU.getGyroData(&accX, &accY, &accZ);
      Serial.println(accX);
      smoothedAccX = SMOOTHING_FACTOR * accX + (1.0 - SMOOTHING_FACTOR) * smoothedAccX;

      M5.Lcd.fillRect(taxi.x, taxi.y, CARS_WIDTH, CARS_HEIGHT, BLACK);

      if (0 <= taxi.x && taxi.x + CARS_WIDTH <= M5.Lcd.width()) {
        taxi.x += static_cast<int>(3 * smoothedAccX);
        taxi.x = constrain(taxi.x, 0, M5.Lcd.width() - CARS_WIDTH);
      }

      M5.Lcd.fillRect(taxi.x, taxi.y, CARS_WIDTH, CARS_HEIGHT, CAR_COLOR);
      M5.Lcd.fillRect(taxi.x + 2, taxi.y + 6, 22, 6, WINDOW_CAR_COLOR);
      M5.Lcd.fillRect(taxi.x + 2, taxi.y + 26, 22, 6, WINDOW_CAR_COLOR);

      drawTheOpposite(opposite1, 1);
      drawTheOpposite(opposite2, 2);
      drawTheOpposite(opposite3, 3);

      if (checkCollision(taxi, opposite1) || checkCollision(taxi, opposite2) || checkCollision(taxi, opposite3)) {
        return true;
      }

      delay(50);
    }
  }

  bool End() override {
    M5.Lcd.setTextSize(2);
    MAX_SCORE_TAXI = EEPROM.readInt(ADRESS_MAX_SCORE_TAXI);

    resultTaxi = millis() - resultTaxi;

    if(resultTaxi > MAX_SCORE_TAXI) {
      EEPROM.writeInt(ADRESS_MAX_SCORE_TAXI, MAX_SCORE_TAXI);
      EEPROM.commit();
      MAX_SCORE_TAXI = resultTaxi;

    M5.Lcd.setTextColor(RED);
    }

    M5.Lcd.setCursor(5, M5.Lcd.height() / 2);
    M5.Lcd.printf("Max Reslut: %dms", MAX_SCORE_TAXI);

    M5.Lcd.setTextColor(GREEN);

    M5.Lcd.setCursor(20, M5.Lcd.height() / 2 - 20);
    M5.Lcd.printf("Reslut: %dms", resultTaxi);
    M5.Lcd.setCursor(20, M5.Lcd.height() / 2 - 50);
    M5.Lcd.print("Game Over!");

    while (true) {
      M5.update();
      if (M5.BtnA.wasPressed()) {
        return true;
      } else if (M5.BtnB.wasPressed()) {
        return false;  // exit
      }
    }
  }
};

#endif // TAXI_HPP
