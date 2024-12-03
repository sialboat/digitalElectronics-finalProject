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

//LEFT JOYSTICK PINS
int xPin = A8;
int yPin = A9;
//RIGHT JOYSTICK PINS
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
bool xMoving = false;
bool yMoving = false;

float xPos = radius + SCREEN_WIDTH / 2;
float yPos = radius + SCREEN_HEIGHT / 2;

int colorButtonPin = 29;
int color = 65535;
int colorMode = 0;

//upDown == 1, leftRight == 0
int directions[2] = { 0 };

int frameRate = 30;
unsigned long lastFrame = 0;

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

bool deadZone(int value) {
  return value >= 51 && value <= 54;
}
bool stillZone(int value) {
  return value >= -acceleration && value <= acceleration;
}

void checkY() {

  directions[1] = constrainValues(yPin, directions[1], 50, 920, 0, 100);
//&& yPos <= SCREEN_HEIGHT - (2 * radius)  && yPos >= 0 + (2 * radius)

  if (!deadZone(directions[1]) && directions[1] < 52) {
    ySpeed2B = maxSpeed;
    yMoving = true;
  } else if (!deadZone(directions[1]) && directions[1] > 52) {
    ySpeed2B = -maxSpeed;
    yMoving = true;
  } else {
    ySpeed2B = 0.0;
    Serial.println("DEADZONE");
    yMoving = false;
  }
}

void moveY() {

  Serial.print("ySpeed: ");
  Serial.print(ySpeed);
  Serial.println();

  Serial.print("ySpeed2B: ");
  Serial.print(ySpeed2B);
  Serial.println();

  Serial.print("moving: ");
  Serial.println(yMoving);
 
  if(!yMoving && abs(ySpeed) <= acceleration) {
    Serial.println("Bingus");
    ySpeed = 0;
  } else if (ySpeed < ySpeed2B) { //We move down
    ySpeed += acceleration;
  } else if (ySpeed > ySpeed2B) { //We move up
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
  else if (xSpeed < xSpeed2B) xSpeed += acceleration; //we move right
  else if (xSpeed > xSpeed2B) xSpeed -= acceleration; //we move left
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