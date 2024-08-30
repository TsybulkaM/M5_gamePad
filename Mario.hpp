/*#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135

#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 20
#define PLAYER_COLOR TFT_RED
#define GROUND_HEIGHT 20

#define GRAVITY 1
#define JUMP_FORCE 14
#define MOVE_SPEED 5

static struct Player {
  int x;
  int y;
  int velocityY;
  int velocityX;
} player;

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

void MarioSetup() {
  M5.begin();
  M5.Lcd.fillScreen(TFT_BLACK);
}

void MarioLoop() {
  M5.Lcd.setRotation(1);
  while (1) {
    static Player player = { 50, SCREEN_HEIGHT - GROUND_HEIGHT - PLAYER_HEIGHT, 0, 0 };

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
}*/
