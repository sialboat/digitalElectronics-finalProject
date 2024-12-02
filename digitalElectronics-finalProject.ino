//GOOD MORNING GAMERS
/* THIS IS ANOTHER COMMENT */
//I AM MAKING CHANGES TO MY PROGRAM AAAAHHHHHHHH

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <Encoder.h>

#include <Adafruit_NeoPixel.h>

#define SCREEN_HEIGHT 320  // OLED display width, in pixels
#define SCREEN_WIDTH 240   // OLED display height, in pixels

//For Adafruit shield, these are default.
#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

Encoder enc(27, 26);
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(4, 33, NEO_RGB);

//slowly stop:
//current speed and speed to be
//set speed to be to max speed
//set both speed and speed to be to max
//when you lift finger, set speed to be to 0,
//every frame, that thing will subtract to zero. speed2B

//LEFT JOYSTICK MAPS
int xPin = A8;
int yPin = A9;
//RIGHT JOYSTICK MAPS
int xPin1 = A7;
int yPin1 = A6;

//CHARACTER VALUES
int radius = 4;
// int speed = 1;
float xSpeed = 0.0;    //current speed
float xSpeed2B = 0.0;  //speed to be
float ySpeed = 0.0;
float ySpeed2B = 0.0;
float maxSpeed = 4.5;
float acceleration = 0.10;

float xPos = radius + SCREEN_WIDTH / 2;
float yPos = radius + SCREEN_HEIGHT / 2;
int colorButtonPin = 29;
int color = 65535;
int colorMode = 0;

bool xMoving = false;
bool yMoving = false;

//upDown == 1, leftRight == 0
int directions[2] = { 0 };

int frameRate = 30;
unsigned long lastFrame = 0;

int directionButtonPins[4];     //0 = left, 1 = up, 2 = down, 3 = right;
bool directionButtonStates[4];  //0 = left, 1 = up, 2 = down, 3 = right;

int rightButtonPin = 32;
bool lastRightButtonState;
bool rightButtonState;

void setup() {
  Serial.begin(9600);
  tft.begin();

  pinMode(colorButtonPin, INPUT);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setRotation(4);
  tft.println("LOOK STEVE I DID IT");
  delay(2000);
  tft.println("silas wang video game cv spaceship");
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);
  // drawColorCharacter();
}

void helloWorld() {
}

// void drawCharacter() {
//   display.fillCircle(3, 3, 3, SSD1306_WHITE);
// }

void drawCharacter(int xPos, int yPos, int radius, int color) {
  tft.fillCircle(xPos, yPos, radius, color);
}

void clearCharacter(int xPos, int yPos, int radius) {
  tft.fillCircle(xPos, yPos, radius, ILI9341_BLACK);
}

void loop() {
  // changeColor();
  if (millis() > lastFrame + frameRate) {
    lastFrame = millis();

    // display.clearDisplay();
    clearCharacter(xPos, yPos, radius);
    // Serial.print("yPos: ");
    // Serial.print(yPos);
    // Serial.print(" ");
    // Serial.print(analogRead(yPin));
    // Serial.println();

    // // Serial.println(lastFrame);
    // moveRight();
    // moveLeft();
    checkY();
    moveY();
    checkX();
    moveX();
    // moveUp();
    // moveDown();
    // modifySpeed();
    drawCharacter(xPos, yPos, radius, color);
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
  // lastDirections[0] = directions[0];
  directions[0] = constrainValues(xPin, directions[0], 50, 920, 0, 100);
  // directionButtonStates[3] = digitalRead(directionButtonPins[3]);
  if (!deadZone(directions[0]) && directions[0] < 52 && xPos <= SCREEN_WIDTH - (2 * radius)) {
    xPos += xSpeed;
    // return true;
    //   // delay(5);
  }
  // return false;
}
void moveLeft() {
  // lastDirections[0] = directions[0];
  directions[0] = constrainValues(xPin, directions[0], 50, 920, 0, 100);
  if (!deadZone(directions[0]) && directions[0] > 52 && xPos >= 0 + (2 * radius)) {
    xPos -= xSpeed;
    // return true;
  }
  // directionButtonStates[0] = digitalRead(directionButtonPins[0]);
  // if(directionButtonStates[0] && xPos >= 0 + (2 * radius)) {
  //   xPos -= 3;
  // }
  // return false;
}

bool deadZone(int value) {
  return value >= 51 && value <= 54;
}
bool stillZone(int value) {
  return value >= -acceleration && value <= acceleration;
}

void checkY() {

  directions[1] = constrainValues(yPin, directions[1], 50, 920, 0, 100);


  if (!deadZone(directions[1]) && directions[1] < 52 && yPos <= SCREEN_HEIGHT - (2 * radius)) {
    ySpeed2B = maxSpeed;
    yMoving = true;
  } else if (!deadZone(directions[1]) && directions[1] > 52 && yPos >= 0 + (2 * radius)) {
    ySpeed2B = -maxSpeed;
    yMoving = true;
  } else {
    ySpeed2B = 0.0;
    Serial.println("DEADZONE");
    xMoving = false;
  }
}

void moveY() {

  Serial.print("ySpeed: ");
  Serial.print(ySpeed);
  Serial.println();


  Serial.print("ySpeed2B: ");
  Serial.print(ySpeed2B);
  Serial.println();
  // int upOrDown = 0;  //if this is set to 1, we moved up, -1 if down.

  //MOVING UP
  // if(ySpeed == 0.20 || ySpeed == -0.20) ySpeed = 0;

  if(!yMoving and abs(ySpeed) < acceleration) {
    ySpeed = 0;
  } else if (ySpeed < ySpeed2B) {
    ySpeed += acceleration;
  } else if (ySpeed > ySpeed2B) {
    ySpeed -= acceleration;
  }

  yPos += ySpeed;
}

void checkX() {

  Serial.print("xSpeed: ");
  Serial.print(xSpeed);
  Serial.println();

  directions[0] = constrainValues(xPin, directions[0], 50, 920, 0, 100);
  
  //MOVING TO THE LEFT
  if (!deadZone(directions[0]) && directions[0] > 52) {
    // && xPos >= 0 + (2 * radius)
    xSpeed2B = -maxSpeed;
    xMoving = true;
    // return true;
  } else if (!deadZone(directions[0]) && directions[0] < 52) {
    // && xPos <= SCREEN_WIDTH - (2 * radius)
    xSpeed2B = maxSpeed;
    xMoving = true;

  } else {
    xSpeed2B = 0.0;
    xMoving = false;
  }
}

void moveX() {

  if(!xMoving && abs(xSpeed) < acceleration) xSpeed = 0;
  else if (xSpeed < xSpeed2B) xSpeed += acceleration;
  else if (xSpeed > xSpeed2B) xSpeed -= acceleration;
  xPos += xSpeed;
}
void drawColorCharacter() {
  for (int i = 0; i < color; i++) {
    tft.fillCircle(xPos, yPos, radius, i);
  }
  for (int g = color; g > 0; g--) {
    tft.fillCircle(xPos, yPos, radius, g);
  }
}

void changeColor() {
  if (digitalRead(colorButtonPin) == HIGH) {
    if (colorMode == 3) {
      colorMode = 3;
    } else {
      colorMode++;
    }
  }
}

// void moveUp() {
//   //if there's a change in direction
//   // lastDirections[1] = directions[1];
//   directions[1] = constrainValues(yPin, directions[1], 50, 920, 0, 100);
//   // bool deadZone = (lastDirections[1] == 53 || lastDirections[1] == 54) && (directions[1] == 53 || directions[1] == 54);
//   // lastDirections[1] = directions[1];
//   // if (deadZone && directions[1] < lastDirections[1] && yPos >= 0 + (2 * radius)) {
//   //   yPos -= 3;
//   // }

//   if (!deadZone(directions[1]) && directions[1] > 52 && yPos >= 0 + (2 * radius)) {
//     yPos -= ySpeed;
//     // return true;
//   }
//   // directionButtonStates[1] = digitalRead(directionButtonPins[1]);
//   // if (directionButtonStates[1] && yPos >= 0 + (2 * radius)) {
//   //   yPos -= 3;
//   // }
//   // return false;
// }

// void moveDown() {

//   //if there's a change in direction
//   // lastDirections[1] = directions[1];
//   directions[1] = constrainValues(yPin, directions[1], 50, 920, 0, 100);
//   // bool deadZone = (lastDirections[2] == 53 || lastDirections[2] == 54);
//   // lastDirections[1] = directions[1];
//   // if (deadZone && directions[3] > lastDirections[3] && yPos <= SCREEN_HEIGHT + (2 * radius)) {
//   //   yPos += 3;
//   // }

//   if (!deadZone(directions[1]) && directions[1] < 52 && yPos <= SCREEN_HEIGHT - (2 * radius)) {
//     yPos += ySpeed;
//     // return true;
//   }
//   // directionButtonStates[2] = digitalRead(directionButtonPins[2]);
//   // if (directionButtonStates[2] && yPos <= SCREEN_HEIGHT - (2 * radius)) {
//   //   yPos += 3;
//   // }
//   // return false;
// }

// int checkDirections(int index) {
//   switch(index) {
//     case 0:
//       return directions[0] >= 55 && directions[0] < 70
//       break;
//     case 1:
//       break;
//   }
// }

void modifySpeed() {}
//max speed: 3;
//min speed: 0.1;

//from slow to fast
//(52) up/down speed: (DOWN) 51-42, 41-16, 0-15|| (UP) 53-68, 69-90, 91-99
//(54) left/right speed: (LEFT) 55-70, 71-91, 92-100  || (RIGHT) 53-38, 37-16, 17-10

//change speed if joystick moves slightly out of the dead zone.
//slowly add to speed if the joystick is within certain range X
//moderately add to speed if joystick is within certain range Y
//rapidly add to speed if joystick is within certain range Z

//acceleration code
//if we are moving, gradually add speed until we reach max speed.
//if we released and are not moving, gradually remove speed until we reach zero.
// if (moveUp() || moveDown() || moveLeft() || moveRight()) {
//   speed2B = 1.5;
//   speed += 0.2;
//   if (speed >= speed2B) {
//     speed = speed2B;
//   }
// } else {
//   speed2B = 0;
//   speed -= 0.2;
//   if (speed <= 0) {
//     speed = 0;
//   }
//   // fade away.
// }

//0 is left/Right
//slowly add: if directions[0] is between 55-70 || directions[0] is between 53-38
// if((directions[0] >= 55 && directions[0] < 70) || (directions[0] <= 53 && directions[0] > 38)) {
//   speed += 0.1;
// }
// // if(!deadZone(directions[0]) || !deadZone(directions[1]) && directions[0] >)
// if(directions[0] >= 71 && directions[0] < 91 || directions[0] <= 37 && directions[0] > 16) {

// }

//mdoerately add

//rapidly add
//locks speed to max speed
//   if(speed >= 1.5) {
//     speed = 1.5;
//   }
// }


// if (!deadZone(directions[1]) && directions[1] > 52 && yPos >= 0 + (2 * radius)) {
//   upOrDown = 1;
//   ySpeed2B = -maxSpeed;
//   ySpeed -= acceleration;

//   if (ySpeed < ySpeed2B) {
//     ySpeed = -maxSpeed;
//   }

//   Serial.println("UP");
//   // yPos += ySpeed;
//   // return true;
// }

// //MOVING DOWN
// else if (!deadZone(directions[1]) && directions[1] < 52 && yPos <= SCREEN_HEIGHT - (2 * radius)) {
//   upOrDown = -1;
//   ySpeed2B = maxSpeed;
//   ySpeed += acceleration;
//   if (ySpeed > ySpeed2B) {
//     ySpeed = maxSpeed;
//   }

//   Serial.println("DOWN");
//   // return true;
// }
// else {
//   ySpeed2B = 0.0;

// }
// yPos += ySpeed;