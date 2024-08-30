#ifndef BALL_HPP
#define BALL_HPP

#include "GameArch.hpp"

class Ball : public ArcadeGame {
private:
    uint16_t ballX;
    uint16_t ballY;

    int8_t speedX;
    int8_t speedY;

    const uint8_t ballSize;

    int gols, recordBalls, antirecordBalls;

    static const int ADRESS_MAX_SCORE_BALLS = 2 * sizeof(int);
    static const int ADRESS_ANTI_MAX_SCORE_BALLS = 3 * sizeof(int);

public:
    Ball() : ballSize(10) {}

    void execute() {
      while(this->Setup()) {
        while(this->MainLoop()) {
          this->Reset();
        }
      }
    }

    bool Setup() override {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setRotation(1);

      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(M5.Lcd.width() / 2 - 30, M5.Lcd.height() / 2 - 50);
      M5.Lcd.print("Do it");
      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(10, M5.Lcd.height() / 2 - 30);
      M5.Lcd.print("Catch the ball in the center of screen");
      M5.Lcd.setTextSize(2);

      gols = 0;

      while (true) {
          M5.update();
          if (M5.BtnA.wasPressed()) {
              M5.Lcd.fillScreen(BLACK);
              this->Reset();
              return true;
          } else if (M5.BtnB.wasPressed()) {
              return false;
          }
      }
    }

    void Reset() override {
      M5.Lcd.fillScreen(BLACK);

      ballX = random(ballSize, M5.Lcd.width() - ballSize);
      ballY = random(ballSize, M5.Lcd.height() - ballSize);

      speedX = random(2, 5);
      speedY = random(2, 5);
    }

    bool MainLoop() override {
      while (true) {
        M5.update();

        M5.Lcd.fillCircle(ballX, ballY, ballSize, BLACK);

        ballX += speedX;
        ballY += speedY;

        if (ballX <= ballSize || ballX >= M5.Lcd.width() - ballSize) {
            speedX = -speedX;
        }
        if (ballY <= ballSize || ballY >= M5.Lcd.height() - ballSize) {
            speedY = -speedY;
        }

        M5.Lcd.fillCircle(ballX, ballY, ballSize, RED);

        if (M5.BtnA.wasPressed()) {
            if (ballX >= (M5.Lcd.width() / 2) - ballSize && ballX <= (M5.Lcd.width() / 2) + ballSize && ballY >= (M5.Lcd.height() / 2) - ballSize && ballY <= (M5.Lcd.height() / 2) + ballSize) {
                gols++;
            } else {
                gols--;
            }

            recordBalls = EEPROM.readInt(ADRESS_MAX_SCORE_BALLS);
            antirecordBalls = EEPROM.readInt(ADRESS_ANTI_MAX_SCORE_BALLS);

            if (gols > recordBalls) {
                EEPROM.writeInt(ADRESS_MAX_SCORE_BALLS, gols);
                EEPROM.commit();
                recordBalls = gols;
            }

            if (gols < antirecordBalls) {
                EEPROM.writeInt(ADRESS_ANTI_MAX_SCORE_BALLS, gols);
                EEPROM.commit();
                antirecordBalls = gols;
            }

            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setCursor(20, 20);
            M5.Lcd.printf("Gols: %d", gols);

            M5.Lcd.setCursor(20, 50);
            M5.Lcd.printf("Record: %d", recordBalls);

            M5.Lcd.setCursor(20, 80);
            M5.Lcd.printf("Antirecord: %d", antirecordBalls);

            delay(1300);
            return true;
        } else if (M5.BtnB.wasPressed()) {
            return false;
        }

        delay(30);
      }
    }

    bool End() override {
      Serial.println("End!");
    }
};

#endif // BALL_HPP
