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
#define BUTTON_LONG_PRESS 400

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

std::vector<String> menuOptions;
int selectedMenuItemIndex = 0;

int lastLeftButtonRead = 0;
int lastRightButtonRead = 0;

enum GameState { main_menu, hacker_trivia };
GameState currentGameState = main_menu;

typedef struct {
  int lastLeftRead;
  int lastRightRead;
  float lastLeftPressTime;
  float lastRightPressTime;
  float lastLeftLongPressRelease;
  float lastRightLongPressRelease;
} ButtonPressManager;

ButtonPressManager buttonPressManager = { 0, 0, millis(), millis() };

boolean wasButtonReleased(bool isRight, int reading) {
  if (lastRightButtonRead == 1 && reading == 0 && isRight) {
    return true;
  } else if (lastLeftButtonRead == 1 && reading == 0 && !isRight) {
    return true;
  }
  return false;
}

void setupDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void showSelectGameMenu() {
  display.clearDisplay();

  display.setCursor(0, 6);
  display.println("Select A Game");

  for (int i = 0; i < menuOptions.size(); i++) {
    display.setCursor(0, MENU_ITEM_START_ROW + (i * MENU_ITEM_SPACE));

    String finalOption = "";
    if (i == selectedMenuItemIndex) {
      finalOption += "[*]";
    } else {
      finalOption += "[ ]";
    }
    finalOption += menuOptions[i];

    display.println(finalOption);
  }

  display.display();
}

void showHackerChallenge() {
  display.clearDisplay();

  display.setCursor(0, 6);
  display.println("What is hacking?");

  display.display();
}

void setupMenuItems() {
  menuOptions.push_back("Hacker Trivia");
  menuOptions.push_back("Placeholder Game");
}

void setup() {
  Serial.begin(115200);

  setupDisplay();
  setupMenuItems();
  showSelectGameMenu();
}

void loop() {
  boolean leftPressed = false;
  boolean leftLongPressed = false;
  boolean rightPressed = false;
  boolean rightLongPressed = false;
  int leftButtonRead = digitalRead(BUTTON_LEFT_GPIO);
  int rightButtonRead = digitalRead(BUTTON_RIGHT_GPIO);
  
  if (leftButtonRead == 1 && buttonPressManager.lastLeftRead == 0) {
    buttonPressManager.lastLeftPressTime = millis();
  }
  if (leftButtonRead == 0 && buttonPressManager.lastLeftRead == 1) {
    float heldTime = millis() - buttonPressManager.lastLeftPressTime;
    if (heldTime > BUTTON_LONG_PRESS) {
      leftLongPressed = true;
    } else {
      leftPressed = true;
    }
    if (heldTime == 0) {
      leftLongPressed = false;
      leftPressed = false;
    }
  }
  
  if (rightButtonRead == 1 && buttonPressManager.lastRightRead == 0) {
    buttonPressManager.lastRightPressTime = millis();
  }
  if (rightButtonRead == 0 && buttonPressManager.lastRightRead == 1) {
    float heldTimeRight = millis() - buttonPressManager.lastRightPressTime;
    if (heldTimeRight > BUTTON_LONG_PRESS) {
      rightLongPressed = true;
    } else {
      rightPressed = true;
    }
    if (heldTimeRight == 0) {
      rightLongPressed = false;
      rightPressed = false;
    }
  }
  
  buttonPressManager.lastLeftRead = leftButtonRead;
  buttonPressManager.lastRightRead = rightButtonRead;

  switch (currentGameState) {
    case main_menu:
      if (rightLongPressed || leftLongPressed) {
        if (selectedMenuItemIndex == 0) {
          currentGameState = hacker_trivia;
          showHackerChallenge();
        }
      } else if (rightPressed) {
        if (selectedMenuItemIndex < menuOptions.size() - 1) {
          selectedMenuItemIndex++;
          showSelectGameMenu();
        }
      } else if (leftPressed) {
        if (selectedMenuItemIndex > 0) {
          selectedMenuItemIndex--;
          showSelectGameMenu();
        }
      }
      break;
    case hacker_trivia:
      break;
  }
  
  if (leftLongPressed) {
    buttonPressManager.lastLeftLongPressRelease = millis();
  }
  if (rightLongPressed) {
    buttonPressManager.lastRightLongPressRelease = millis();
  }
  if (millis() - buttonPressManager.lastLeftLongPressRelease > 50) {
    buttonPressManager.lastLeftLongPressRelease = 0;
  }
  if (millis() - buttonPressManager.lastRightLongPressRelease > 50) {
    buttonPressManager.lastRightLongPressRelease = 0ll;
  }
  if (buttonPressManager.lastRightLongPressRelease != 0 && buttonPressManager.lastLeftLongPressRelease != 0) {
    buttonPressManager.lastRightLongPressRelease = 0;
    buttonPressManager.lastLeftLongPressRelease = 0;

    currentGameState = main_menu;
    showSelectGameMenu();
  }
  
  lastRightButtonRead = rightButtonRead;
  lastLeftButtonRead = leftButtonRead;
}
