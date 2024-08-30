#ifndef PONG_HPP
#define PONG_HPP

#include "GameArch.hpp"

class Pong : public ArcadeGame {
private:
  uint8_t PADDLE_WIDTH, PADDLE_HEIGHT, BALL_SIZE, PADDLE_SPEED, BALL_SPEED;

  int16_t leftPaddleY, rightPaddleY, pongBallX, pongBallY, ballVelocityX, ballVelocityY;
  uint8_t prevBallX, prevBallY, prevLeftPaddleY, prevRightPaddleY;

  uint8_t leftScore, rightScore;

  void drawPaddles() {
    M5.Lcd.fillRect(0, leftPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
    M5.Lcd.fillRect(SCREEN_WIDTH - PADDLE_WIDTH, rightPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
  }


  void drawBall() {
    M5.Lcd.fillRect(pongBallX, pongBallY, BALL_SIZE, BALL_SIZE, WHITE);
  }


  void drawScores() {
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(50, 0);
    M5.Lcd.print(leftScore);
    M5.Lcd.setCursor(SCREEN_WIDTH - 70, 0);
    M5.Lcd.print(rightScore);
  }


  void resetBall() {
    M5.Lcd.fillScreen(BLACK);

    pongBallX = SCREEN_WIDTH / 2;
    pongBallY = SCREEN_HEIGHT / 2;
    ballVelocityX = -ballVelocityX;
    ballVelocityY = BALL_SPEED;
  }


  void updateScreen() {
    // Стирание старых позиций
    M5.Lcd.fillRect(0, prevLeftPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT, BLACK);
    M5.Lcd.fillRect(SCREEN_WIDTH - PADDLE_WIDTH, prevRightPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT, BLACK);
    M5.Lcd.fillRect(prevBallX, prevBallY, BALL_SIZE, BALL_SIZE, BLACK);

    // Рисование новых позиций
    drawPaddles();
    drawBall();
    drawScores();

    // Обновление предыдущих значений
    prevLeftPaddleY = leftPaddleY;
    prevRightPaddleY = rightPaddleY;
    prevBallX = pongBallX;
    prevBallY = pongBallY;
  }

public:
  Pong()
    : PADDLE_WIDTH(4),
      PADDLE_HEIGHT(20),
      BALL_SIZE(4),
      PADDLE_SPEED(3),
      BALL_SPEED(2) {
  }


  void execute() {
    while (this->Setup()) {
        this->Reset();
        this->MainLoop();
        this->End();
    }
  }


  bool Setup() {
    M5.Lcd.setRotation(1);
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.setCursor(20, M5.Lcd.height() / 2 - 60);
    M5.Lcd.setTextSize(3);
    M5.Lcd.print("oy yoy");
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(10, M5.Lcd.height() / 2 - 30);
    M5.Lcd.print("Play with a master of pong");
    M5.Lcd.setTextSize(2);

    while (true) {
      M5.update();
      if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillScreen(BLACK);
        return true;
      } else if (M5.BtnB.wasPressed()) {
        return false;
      }
    }
  }


  void Reset() {
    prevBallX = pongBallX;
    prevBallY = pongBallY;
    prevLeftPaddleY = leftPaddleY;
    prevRightPaddleY = rightPaddleY;

    leftPaddleY = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
    rightPaddleY = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
    pongBallX = SCREEN_WIDTH / 2;
    pongBallY = SCREEN_HEIGHT / 2;
    ballVelocityX = BALL_SPEED;
    ballVelocityY = BALL_SPEED;

    leftScore = 0;
    rightScore = 0;

    drawPaddles();
    drawBall();
    drawScores();
  }


  bool MainLoop() {
    while (true) {
      M5.update();

      // Управление левым паддлом
      if (M5.BtnA.isPressed()) {
        leftPaddleY -= PADDLE_SPEED;
        if (leftPaddleY < 0) leftPaddleY = 0;
      } else if (M5.BtnB.isPressed()) {
        leftPaddleY += PADDLE_SPEED;
        if (leftPaddleY > SCREEN_HEIGHT - PADDLE_HEIGHT) leftPaddleY = SCREEN_HEIGHT - PADDLE_HEIGHT;
      }

      // Прерывание игры по длительному нажатию кнопки B
      if (M5.BtnB.wasReleasefor(4000)) {
        leftScore = 0;
        return false;
      }

      // Автоматическое управление правым паддлом с случайными промахами
      if (random(10) > 3) {  // 30% шанс промаха
        if (pongBallY < rightPaddleY + PADDLE_HEIGHT / 2) {
          rightPaddleY -= PADDLE_SPEED;
          if (rightPaddleY < 0) rightPaddleY = 0;
        } else {
          rightPaddleY += PADDLE_SPEED;
          if (rightPaddleY > SCREEN_HEIGHT - PADDLE_HEIGHT) rightPaddleY = SCREEN_HEIGHT - PADDLE_HEIGHT;
        }
      }

      // Обновление позиции мяча
      pongBallX += ballVelocityX;
      pongBallY += ballVelocityY;

      // Столкновение с верхней и нижней границами
      if (pongBallY <= 0 || pongBallY >= SCREEN_HEIGHT - BALL_SIZE) {
        ballVelocityY = -ballVelocityY;
      }

      // Столкновение с паддлами
      if (pongBallX <= PADDLE_WIDTH && pongBallY >= leftPaddleY && pongBallY <= leftPaddleY + PADDLE_HEIGHT) {
        ballVelocityX = -ballVelocityX;
      } else if (pongBallX >= SCREEN_WIDTH - PADDLE_WIDTH - BALL_SIZE && pongBallY >= rightPaddleY && pongBallY <= rightPaddleY + PADDLE_HEIGHT) {
        ballVelocityX = -ballVelocityX;
      }

      // Проверка на гол
      if (pongBallX <= 0) {
        rightScore++;
        resetBall();
      } else if (pongBallX >= SCREEN_WIDTH - BALL_SIZE) {
        leftScore++;
        resetBall();
      }

      // Перерисовка экрана
      updateScreen();

      delay(30);
    }
  }


  bool End() {
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.printf("%d : %d", leftScore, rightScore);

    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(30, 40);
    if (leftScore > rightScore) {
      M5.Lcd.print("You won!");
    } else if (leftScore < rightScore) {
      M5.Lcd.print("You lose!");
    }

    while (true) {
      M5.update();
      if (M5.BtnA.wasPressed()) {
        M5.Lcd.fillScreen(BLACK);
        return true;
      } else if (M5.BtnB.wasPressed()) {
        return false;
      }
    }
  }
};

#endif  // PONG_HPP