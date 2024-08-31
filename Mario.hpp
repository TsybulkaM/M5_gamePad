class Mario : public ArcadeGame {
public:

  Mario() : 
    PLAYER_WIDTH(20),
    PLAYER_HEIGHT(20),
    PLAYER_COLOR(TFT_RED),
    GROUND_HEIGHT(20),
    GRAVITY(1),
    JUMP_FORCE(14),
    MOVE_SPEED(5)
  {

  }

  void execute() {
    while (this->Setup()) {
        this->Reset();
        this->MainLoop();
        this->End();
    }
  }

private:
  static struct Player {
    int x;
    int y;
    int velocityY;
    int velocityX;
  } player;

  uint8_t PLAYER_WIDTH, PLAYER_HEIGHT, GROUND_HEIGHT;
  uint16_t PLAYER_COLOR;
  uint8_t GRAVITY, JUMP_FORCE, MOVE_SPEED;

  bool Setup() {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setRotation(1);
    M5.Lcd.printf("Hi, Ken!");
    return this->choice();
  }

  void Reset() {
    player = { 50, SCREEN_HEIGHT - GROUND_HEIGHT - PLAYER_HEIGHT, 0, 0 };
  }

  bool MainLoop() {
    while (1) {
      M5.update();

      // Handle player input
      if (M5.BtnA.isPressed()) {
        jump();
      }

      if (M5.BtnB.isPressed()) {
        player.velocityX = MOVE_SPEED;
      }

      // Update game logic
      M5.Lcd.fillScreen(TFT_BLACK);
      updatePlayer();

      // Draw game elements
      drawPlayer();

      // Delay to control the frame rate
      delay(16);
    }
  }

  bool End() {

  }

  void drawPlayer() {
    M5.Lcd.fillRect(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_COLOR);
  }

  void updatePlayer() {
    player.velocityY += GRAVITY;

    player.y += player.velocityY;
    player.x += player.velocityX;

    if (player.y > SCREEN_HEIGHT - GROUND_HEIGHT - PLAYER_HEIGHT) {
      player.y = SCREEN_HEIGHT - GROUND_HEIGHT - PLAYER_HEIGHT;
      player.velocityY = 0;
    }

    if (player.x < 0) {
      player.x = 0;
    }
    if (player.x > SCREEN_WIDTH - PLAYER_WIDTH) {
      player.x = SCREEN_WIDTH - PLAYER_WIDTH;
    }
  }

  void jump() {
    if (player.y == SCREEN_HEIGHT - GROUND_HEIGHT - PLAYER_HEIGHT) {
      player.velocityY = -JUMP_FORCE;
    }
  }
};
