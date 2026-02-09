#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const int buttonLeft = 2;
const int buttonRight = 3;
const int buttonShoot = 4;
const int buzzer = 8;

int rocketX = 60, rocketY = 55;
int rocketSpeed = 3;
int bulletX = -1, bulletY = -1;
int bulletSpeed = 4;
bool bulletActive = false;
int asteroidX[7], asteroidY[7];
int asteroidSpeed = 1;
int score = 0;

void setup() {
  pinMode(buttonLeft, INPUT_PULLUP);
  pinMode(buttonRight, INPUT_PULLUP);
  pinMode(buttonShoot, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  u8g2.begin();

  for (int i = 0; i < 7; i++) {
    asteroidX[i] = random(0, 128);
    asteroidY[i] = random(-64, -10);
  }
}

void loop() {
  if (digitalRead(buttonLeft) == LOW && rocketX > 0) {
    rocketX -= rocketSpeed;
  }
  if (digitalRead(buttonRight) == LOW && rocketX < 120) {
    rocketX += rocketSpeed;
  }

  if (digitalRead(buttonShoot) == LOW && !bulletActive) {
    bulletX = rocketX;
    bulletY = rocketY - 5;
    bulletActive = true;
    tone(buzzer, 500, 100);
  }

  if (bulletActive) {
    bulletY -= bulletSpeed;
    if (bulletY < 0) {
      bulletActive = false;
    }
  }

  for (int i = 0; i < 7; i++) {
    asteroidY[i] += asteroidSpeed;
    if (asteroidY[i] > 64) {
      asteroidY[i] = random(-64, -10);
      asteroidX[i] = random(0, 128);
    }

    if (asteroidX[i] > rocketX - 8 && asteroidX[i] < rocketX + 8 &&
        asteroidY[i] > rocketY - 8 && asteroidY[i] < rocketY + 8) {
      tone(buzzer, 1000, 200);
      gameOver();
    }

    if (bulletActive &&
        asteroidX[i] > bulletX - 4 && asteroidX[i] < bulletX + 4 &&
        asteroidY[i] > bulletY - 4 && asteroidY[i] < bulletY + 4) {
      asteroidY[i] = random(-64, -10);
      asteroidX[i] = random(0, 128);
      bulletActive = false;
      score += 10;
      tone(buzzer, 1500, 100);
    }
  }

  u8g2.clearBuffer();

  u8g2.drawTriangle(rocketX - 4, rocketY, rocketX + 4, rocketY, rocketX, rocketY - 8);
  u8g2.drawLine(rocketX - 4, rocketY, rocketX - 8, rocketY + 4);
  u8g2.drawLine(rocketX + 4, rocketY, rocketX + 8, rocketY + 4);

  if (bulletActive) {
    u8g2.drawBox(bulletX - 1, bulletY, 2, 5);
  }

  for (int i = 0; i < 7; i++) {
    u8g2.drawCircle(asteroidX[i], asteroidY[i], 4);
  }

  u8g2.setCursor(5, 10);
  u8g2.print("Score: ");
  u8g2.print(score);

  u8g2.sendBuffer();
  delay(30);
}

void gameOver() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB24_tr);
  u8g2.setCursor(20, 30);
  u8g2.print("GAME OVER");

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(30, 50);
  u8g2.print("Score: ");
  u8g2.print(score);

  u8g2.sendBuffer();
  delay(2000);
  resetGame();
}

void resetGame() {
  rocketX = 60;
  rocketY = 55;
  score = 0;

  for (int i = 0; i < 7; i++) {
    asteroidX[i] = random(0, 128);
    asteroidY[i] = random(-64, -10);
  }
}
