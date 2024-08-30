#ifndef VOICEBALL_HPP
#define VOICEBALL_HPP

class VoiceBall : public ArcadeGame {
private:
  int ballX;         // Позиция шара по оси X
  int ballY;         // Позиция шара по оси Y
  int ballRadius;    // Радиус шара

public:
  // Конструктор
  VoiceBall() : ballX(80), ballY(50), ballRadius(5) {}

  // Функция для начальной настройки игры
  bool Setup() override {
    M5.begin();
    M5.Lcd.fillScreen(BLACK);
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    return true;
  }

  // Функция для сброса состояния игры
  void Reset() override {
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
  }

  // Основной игровой цикл
  bool MainLoop() override {
    // Чтение данных с микрофона
    int micValue = analogRead(34);

    // Обработка данных микрофона
    int threshold = 2000;  // Порог громкости
    if (micValue > threshold) {
      ballY -= (micValue - threshold) / 500;  // Чем громче звук, тем выше поднимается шар
    } else {
      ballY += 1;  // Если звук тихий, шар падает
    }

    // Ограничиваем шар рамками экрана
    if (ballY < ballRadius) ballY = ballRadius;
    if (ballY > SCREEN_HEIGHT - ballRadius) ballY = SCREEN_HEIGHT - ballRadius;

    // Отрисовка шара
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.fillCircle(ballX, ballY, ballRadius, RED);

    delay(50); // Задержка для плавного движения
    return true;
  }

  // Условие завершения игры
  bool End() override {
    // В данной простой игре нет условия завершения
    return false;
  }

  // Функция запуска игры
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