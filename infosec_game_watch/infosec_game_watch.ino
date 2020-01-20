#include<random>
#include "ButtonManager.h"
#include "GameStateManager.h"

#define MENU_ITEM_SPACE 10
#define MENU_ITEM_START_ROW 16

typedef struct {
  String text;
  int x;
  int y;
  int scrollable;
  int maxCharacters;
  int xCurrent;
} Text;

typedef struct {
  String question;
  std::vector<String> answers;
  int answerIndex;
} HackerTriviaQuestion;

HackerTriviaQuestion currentQuestion;
GameState currentGameState = main_menu;
std::vector<Text> textOnScreen;
std::vector<HackerTriviaQuestion> hackerTriviaQuestions;

void setupMainMenu() {
  textOnScreen.clear();
  textOnScreen.push_back({"Select A Game", 0, 6, 0, 0, 0});

  textOnScreen.push_back({"Hacker Trivia", 0, 0, 0, 0, 0});
  textOnScreen.push_back({"Firewall Hacker", 0, 0, 0, 0, 0});
}

void setupHackerTriviaQuestions() {
  std::vector<String> answers = {"Kali", "Ubuntu", "Gentoo"};
  hackerTriviaQuestions.push_back({"Which OS based on Knoppix was rewritten by Kali Linux?", answers, 0});

  answers = {"Top Layer Severity", "Transport Layer Security", "Top Load Server"};
  hackerTriviaQuestions.push_back({"What does TLS stand for?", answers, 1});

  answers = {"Virtual Inteligence Resource Under Survelance", "Victum Information Reset Under Stress", "Vital Information Resource Under Siege"};
  hackerTriviaQuestions.push_back({"What does the acronym VIRUS stand for?", answers, 2});

  changeCurrentQuestion();
}

void changeCurrentQuestion() {
  currentQuestion = hackerTriviaQuestions[random(0, hackerTriviaQuestions.size())];
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
        case 1:
        case 2:
          currentGameState = hacker_trivia; 
          changeCurrentQuestion();
          setSelectedMenuIndex(0);
          break;
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
      setSelectedMenuIndex(1);
      
      draw();
    } else if (buttonManager.leftPressed == 1) {
      buttonManager.leftPressed = 0;
      buttonManager.lastLeftButtonPressedTime = 0;

      hackerTriviaCursorMoveDown();
      
      draw();
    } else if (buttonManager.rightPressed == 1) {
      buttonManager.rightPressed = 0;
      buttonManager.lastRightButtonPressedTime = 0;

      menuCursorMoveUp();
      
      draw();
    }
  }
}

void setup() {
  Serial.begin(115200);

  setupDisplay();
  setupMainMenu();
  setupHackerTriviaQuestions();
  
  draw();
}

void loop() {
  updateButtonPresses();
  
  switch (currentGameState) {
    case main_menu: mainMenuLoop(); break;
    case hacker_trivia: hackerTriviaLoop(); break;
  }
}
