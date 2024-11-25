//GOOD MORNING GAMERS
/* THIS IS ANOTHER COMMENT */
//I AM MAKING CHANGES TO MY PROGRAM AAAAHHHHHHHH

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <Encoder.h>

#include <Adafruit_NeoPixel.h>

#define SCREEN_HEIGHT 320   // OLED display width, in pixels
#define SCREEN_WIDTH 240  // OLED display height, in pixels

//For Adafruit shield, these are default.
#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

Encoder enc(27, 26);
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(4, 32, NEO_RGB);

//slowly stop:
//current speed and speed to be
//set speed to be to max speed
//set both speed and speed to be to max
//when you lift finger, set speed to be to 0, 
//every frame, that thing will subtract to zero. speed2B

int xPin = A8;
int yPin = A9;

int xPin1 = A7;
int yPin1 = A6;

int joystickPins[4] = { xPin, yPin, xPin1, yPin1 };

int radius = 4;
// int speed = 1;
double speed = 1;
int xPos = radius;
int yPos = radius;

//upDown == 1, leftRight == 0
int directions[4] = { 0 };

int frameRate = 5;
unsigned long lastFrame = 0;

int directionButtonPins[4];     //0 = left, 1 = up, 2 = down, 3 = right;
bool directionButtonStates[4];  //0 = left, 1 = up, 2 = down, 3 = right;

int rightButtonPin = 32;
bool lastRightButtonState;
bool rightButtonState;

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setRotation(4);
  tft.println("LOOK STEVE I DID IT");
  delay(2000);
  tft.println("silas wang digital final project");
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);
  // tft.fillCircle(xPos, yPos, radius, ILI9341_WHITE);

  // for (int i = 0; i < 4; i++) {
  //   directionButtonPins[i] = 25 + i;
  //   pinMode(directionButtonPins[i], INPUT);
  // }

  // // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for (;;)
  //     ;  // Don't proceed, loop forever
  // }

  // pinMode(rightButtonPin, INPUT);

  // display.display();
  // display.clearDisplay();
  // helloWorld();
  // display.display();
  // delay(1000);
  // display.clearDisplay();
  // drawCharacter(xPos, yPos);
  // display.display();
}

void helloWorld() {
}

// void drawCharacter() {
//   display.fillCircle(3, 3, 3, SSD1306_WHITE);
// }

void drawCharacter(int xPos, int yPos, int radius) {
  tft.fillCircle(xPos, yPos, radius, ILI9341_WHITE);
}

void clearCharacter(int xPos, int yPos, int radius) {
  tft.fillCircle(xPos, yPos, radius, ILI9341_BLACK);
}

void loop() {
  if (millis() > lastFrame + frameRate) {

    // display.clearDisplay();
    clearCharacter(xPos, yPos, radius);
    lastFrame = millis();
    Serial.print("yPos: ");
    Serial.print(yPos);
    Serial.print(" ");
    Serial.print(analogRead(yPin));
    Serial.println();

    // // Serial.println(lastFrame);
    moveRight();
    moveLeft();
    moveUp();
    moveDown();
    drawCharacter(xPos, yPos, radius);
    // moveDown();
    // tft.fillScreen(ILI9341_BLACK);
    // drawCharacter(xPos, yPos, radius);
    // display.display();
  }
  // drawCharacter();
  // display.clearDisplay();
  // put your main code here, to run repeatedly:
}

// moveRight(checkButtons(3));
//     moveLeft(checkButtons(0));
//     moveUp(checkButtons(1));
//     moveDown(checkButtons(2));
//build a frame rate: timePerFrame = 5;
//

int constrainValues(int pin, int value, int from, int to, int low, int high) {
  value = map(analogRead(pin), from, to, low, high);
  value = constrain(value, low, high);
  Serial.print("constrained: ");
  Serial.print(value);
  Serial.println();
  return value;
}

void moveRight() {
  //change in xPin
  lastDirections[0] = directions[0];
  directions[0] = constrainValues(xPin, directions[0], 50, 920, 0, 100);
  // directionButtonStates[3] = digitalRead(directionButtonPins[3]);
  if (!deadZone(directions[0]) && directions[0] < 52 && xPos <= SCREEN_WIDTH - (2 * radius)) {
    xPos += speed;
  //   // delay(5);
  }
}
void moveLeft() {
  lastDirections[0] = directions[0];
  directions[0] = constrainValues(xPin, directions[0], 50, 920, 0, 100);
  if(!deadZone(directions[0]) && directions[0] > 52 && xPos >= 0 + (2 * radius)) {
    xPos -= speed;
  }
  // directionButtonStates[0] = digitalRead(directionButtonPins[0]);
  // if(directionButtonStates[0] && xPos >= 0 + (2 * radius)) {
  //   xPos -= 3;
  // }
}

bool deadZone(int value) {
  return value >= 51 && value <= 54;
}

void moveUp() {
  //if there's a change in direction
  lastDirections[1] = directions[1];
  directions[1] = constrainValues(yPin, directions[1], 50, 920, 0, 100);
  // bool deadZone = (lastDirections[1] == 53 || lastDirections[1] == 54) && (directions[1] == 53 || directions[1] == 54);
  // lastDirections[1] = directions[1];
  // if (deadZone && directions[1] < lastDirections[1] && yPos >= 0 + (2 * radius)) {
  //   yPos -= 3;
  // }

  if(!deadZone(directions[1]) && directions[1] > 52 && yPos >= 0 + (2 * radius)) {
    yPos -= speed;
  }
  // directionButtonStates[1] = digitalRead(directionButtonPins[1]);
  // if (directionButtonStates[1] && yPos >= 0 + (2 * radius)) {
  //   yPos -= 3;
  // }
}

void moveDown() {

  //if there's a change in direction
  lastDirections[1] = directions[1];
  directions[1] = constrainValues(yPin, directions[1], 50, 920, 0, 100);
  // bool deadZone = (lastDirections[2] == 53 || lastDirections[2] == 54);
  // lastDirections[1] = directions[1];
  // if (deadZone && directions[3] > lastDirections[3] && yPos <= SCREEN_HEIGHT + (2 * radius)) {
  //   yPos += 3;
  // }

  if (!deadZone(directions[1]) && directions[1] < 52 && yPos <= SCREEN_HEIGHT - (2 * radius)) {
    yPos += speed;
  }
  // directionButtonStates[2] = digitalRead(directionButtonPins[2]);
  // if (directionButtonStates[2] && yPos <= SCREEN_HEIGHT - (2 * radius)) {
  //   yPos += 3;
  // }
}

int checkDirections(int index) {
  switch(index) {
    case 0:
      return directions[0] >= 55 && directions[0] < 70
      break;
    case 1:
      break;
  }
}

void modifySpeed() {
  //max speed: 3;
  //min speed: 0.1;

  //from slow to fast
  //(52) up/down speed: (DOWN) 51-42, 41-16, 0-15|| (UP) 53-68, 69-90, 91-99
  //(54) left/right speed: (LEFT) 55-70, 71-91, 92-100  || (RIGHT) 53-38, 37-16, 17-10

  //change speed if joystick moves slightly out of the dead zone.
  //slowly add to speed if the joystick is within certain range X
  //moderately add to speed if joystick is within certain range Y
  //rapidly add to speed if joystick is within certain range Z

  //0 is left/Right
  //slowly add: if directions[0] is between 55-70 || directions[0] is between 53-38
  if((directions[0] >= 55 && directions[0] < 70) || (directions[0] <= 53 && directions[0] > 38)) {
    speed += 0.1;
  }
  // if(!deadZone(directions[0]) || !deadZone(directions[1]) && directions[0] >)
  if(directions[0] >= 71 && directions[0] < 91 || directions[0] <= 37 && directions[0] > 16) {

  }

  //mdoerately add

  //rapidly add
  //locks speed to max speed
  if(speed >= 3) {
    speed = 3;
  }
}