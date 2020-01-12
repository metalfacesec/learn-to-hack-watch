#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BUTTON_LEFT_GPIO 15
#define BUTTON_RIGHT_GPIO 2
#define MENU_ITEM_SPACE 10
#define MENU_ITEM_START_ROW 16


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int lastLeftButtonRead = 0;
int lastRightButtonRead = 0;

enum GameState { main_menu, hacker_trivia };
GameState currentGameState = main_menu;

boolean wasButtonReleased(bool isRight, int reading) {
  if (lastRightButtonRead == 1 && reading == 0 && isRight) {
    return true;
  } else if (lastLeftButtonRead == 1 && reading == 0 && !isRight) {
    return true;
  }
  return false;
}

void setupDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void showSelectGameMenu() {
  String menuOption1 = "Hacker Trivia";
  String menuOption2 = "Placeholder Game";
  
  display.clearDisplay();

  display.setCursor(0, 6);
  display.println("Select A Game");

  display.setCursor(0, MENU_ITEM_START_ROW);
  display.println("Hacker Trivia");

  display.setCursor(0, MENU_ITEM_START_ROW + MENU_ITEM_SPACE);
  display.println("Placeholder Game");
  
  display.display();
}

void setup() {
  Serial.begin(115200);
  
  setupDisplay();

  switch(currentGameState) {
    case main_menu  : showSelectGameMenu(); break;
  }
}

void loop() {
  int leftButtonRead = digitalRead(BUTTON_LEFT_GPIO);
  int rightButtonRead = digitalRead(BUTTON_RIGHT_GPIO);
  
  lastRightButtonRead = rightButtonRead;
  lastLeftButtonRead = leftButtonRead;
}
