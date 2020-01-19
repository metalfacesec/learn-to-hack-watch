#define BUTTON_LEFT_GPIO 15
#define BUTTON_RIGHT_GPIO 2
#define BUTTON_LONG_PRESS 400

typedef struct {
  int leftPressed;
  int rightPressed;
  int lastLeftValue;
  int lastRightValue;
  float lastLeftDownTime;
  float lastRightDownTime;
  float lastRightButtonPressedTime;
  float lastLeftButtonPressedTime;
} ButtonManager;

float lastButtonPressCheck = 0;
ButtonManager buttonManager = { 0, 0, 0, 0, 0, 0, 0, 0 };

void updateButtonPresses() {
  if (millis() < 500) {
    return;
  }
  int leftButtonRead = digitalRead(BUTTON_LEFT_GPIO);
  int rightButtonRead = digitalRead(BUTTON_RIGHT_GPIO);

  if (leftButtonRead && !buttonManager.lastLeftValue) {
    buttonManager.lastLeftDownTime = millis();
  } else if (!leftButtonRead && buttonManager.lastLeftValue) {
    buttonManager.lastLeftButtonPressedTime = millis();
  }
  buttonManager.leftPressed = 0;
  if (millis() - buttonManager.lastLeftButtonPressedTime < 510) {
    buttonManager.leftPressed = 1;
  }

  if (rightButtonRead && !buttonManager.lastRightValue) {
    buttonManager.lastRightDownTime = millis();
  } else if (!rightButtonRead && buttonManager.lastRightValue) {
    buttonManager.lastRightButtonPressedTime = millis();
  }
  buttonManager.rightPressed = 0;
  if (millis() - buttonManager.lastRightButtonPressedTime < 150) {
    buttonManager.rightPressed = 1;
  }

  buttonManager.lastLeftValue = leftButtonRead;
  buttonManager.lastRightValue = rightButtonRead;
}
