#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int selectedMenuItemIndex = 1;

void menuCursorMoveUp() {
  if (selectedMenuItemIndex + 1 < textOnScreen.size()) {
    selectedMenuItemIndex++;
  }
}

void hackerTriviaCursorMoveUp() {
  if (selectedMenuItemIndex < currentQuestion.answers.size()) {
    selectedMenuItemIndex++;
  }
}

void hackerTriviaCursorMoveDown() {
  if (selectedMenuItemIndex > 0) {
    selectedMenuItemIndex--;
  }
}

void menuCursorMoveDown() {
  if (selectedMenuItemIndex > 1) {
    selectedMenuItemIndex--;
  }
}

void setSelectedMenuIndex(int index) {
  selectedMenuItemIndex = index; 
}

int getSelectedMenuIndex() {
  return selectedMenuItemIndex;
}

void setupDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void drawMenuDisplay() {
  display.setCursor(textOnScreen[0].x, textOnScreen[0].y);
  display.println(textOnScreen[0].text);

  for (int i = 1; i < textOnScreen.size(); i++) {
    display.setCursor(0, MENU_ITEM_START_ROW + ((i - 1) * MENU_ITEM_SPACE));

    String finalOption = "[ ]";
    if (i == selectedMenuItemIndex) {
      finalOption = "[*]";
    }
    finalOption += textOnScreen[i].text;

    display.println(finalOption);
  }
}

void drawHackerTrivia() {
  display.setCursor(0, 6);
  display.println(currentQuestion.question.substring(0, 21));

  for (int i = 0; i < currentQuestion.answers.size(); i++) {
    display.setCursor(0, MENU_ITEM_START_ROW + (i * MENU_ITEM_SPACE));
    
    String finalOption = "[ ]";
    if (i == selectedMenuItemIndex) {
      finalOption = "[*]";
    }
    finalOption += currentQuestion.answers[i].substring(0, 18);
    
    display.println(finalOption);
  }
}

void draw() {
  display.clearDisplay();

  switch (currentGameState) {
    case main_menu: drawMenuDisplay(); break;
    case hacker_trivia: drawHackerTrivia(); break;
  }
   
  display.display();
}
