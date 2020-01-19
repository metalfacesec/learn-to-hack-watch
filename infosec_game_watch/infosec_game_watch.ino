#include<random>
#include "ButtonManager.h"
#include "GameStateManager.h"

#define MENU_ITEM_SPACE 10
#define MENU_ITEM_START_ROW 16

GameState currentGameState = main_menu;

typedef struct {
  String text;
  int x;
  int y;
  int scrollable;
  int maxCharacters;
  int xCurrent;
} Text;

std::vector<Text> textOnScreen;

void setupMainMenu() {
  textOnScreen.clear();
  textOnScreen.push_back({"Select A Game", 0, 6, 0, 0, 0});

  textOnScreen.push_back({"Hacker Trivia", 0, 0, 0, 0, 0});
  textOnScreen.push_back({"Firewall Hacker", 0, 0, 0, 0, 0});
}

void mainMenuLoop() {
  if (millis() - lastButtonPressCheck > 50) {
    lastButtonPressCheck = millis();
    if (buttonManager.leftPressed == 1 && buttonManager.rightPressed == 1) {
      buttonManager.leftPressed = 0;
      buttonManager.rightPressed = 0;
      buttonManager.lastRightButtonPressedTime = 0;
      buttonManager.lastLeftButtonPressedTime = 0;

      switch(getSelectedMenuIndex()) {
        case 1: currentGameState = hacker_trivia; break;
        case 2: currentGameState = hacker_trivia; break;
      }
      
      draw();
    } else if (buttonManager.leftPressed == 1) {
      menuCursorMoveDown();
      buttonManager.leftPressed = 0;
      buttonManager.lastLeftButtonPressedTime = 0;
      draw();
    } else if (buttonManager.rightPressed == 1) {
      menuCursorMoveUp();
      buttonManager.rightPressed = 0;
      buttonManager.lastRightButtonPressedTime = 0;
      draw();
    }
  }
}

void hackerTriviaLoop() {
  if (millis() - lastButtonPressCheck > 50) {
    lastButtonPressCheck = millis();
    if (buttonManager.leftPressed == 1 && buttonManager.rightPressed == 1) {
      buttonManager.leftPressed = 0;
      buttonManager.rightPressed = 0;
      buttonManager.lastRightButtonPressedTime = 0;
      buttonManager.lastLeftButtonPressedTime = 0;

      currentGameState = main_menu;
      
      draw();
    } else if (buttonManager.leftPressed == 1) {
      currentGameState = main_menu;
      buttonManager.leftPressed = 0;
      buttonManager.lastLeftButtonPressedTime = 0;
      draw();
    } else if (buttonManager.rightPressed == 1) {
      currentGameState = main_menu;
      buttonManager.rightPressed = 0;
      buttonManager.lastRightButtonPressedTime = 0;
      draw();
    }
  }
}

void setup() {
  Serial.begin(115200);

  setupDisplay();
  setupMainMenu();
  
  draw();
}

void loop() {
  updateButtonPresses();
  
  switch (currentGameState) {
    case main_menu: mainMenuLoop(); break;
    case hacker_trivia: hackerTriviaLoop(); break;
  }
}
