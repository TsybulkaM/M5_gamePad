#ifndef VOICEBALL_HPP
#define VOICEBALL_HPP

class VoiceBall : public ArcadeGame {
private:
  int ballX;
  int ballY;
  int ballRadius;

public:
  VoiceBall() : ballX(80), ballY(50), ballRadius(5) {}

  bool Setup() override {
    M5.begin();
    M5.Lcd.fillScreen(BLACK);
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    return true;
  }

  void Reset() override {
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
  }

  bool MainLoop() override {
    int micValue = analogRead(34);

    int threshold = 2000;
    if (micValue > threshold) {
      ballY -= (micValue - threshold) / 500;
    } else {
      ballY += 1;
    }

    if (ballY < ballRadius) ballY = ballRadius;
    if (ballY > SCREEN_HEIGHT - ballRadius) ballY = SCREEN_HEIGHT - ballRadius;

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.fillCircle(ballX, ballY, ballRadius, RED);

    delay(50);
    return true;
  }

  bool End() override {
    return false;
  }

  void execute() override {
    if (Setup()) {
      while (!End()) {
        MainLoop();
      }
      Reset();
    }
  }
};

#endif // VOICEBALL_HPP