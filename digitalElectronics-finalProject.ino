//GOOD MORNING GAMERS
/* THIS IS ANOTHER COMMENT */
//I AM MAKING CHANGES TO MY PROGRAM AAAAHHHHHHHH

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Encoder.h>

#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Encoder enc(27, 26);
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(4, 32, NEO_RGB);

int radius = 3;
int xPos = radius;
int yPos = radius;

int frameRate = 5;
unsigned long lastFrame = 0;

int directionButtonPins[4];     //0 = left, 1 = up, 2 = down, 3 = right;
bool directionButtonStates[4];  //0 = left, 1 = up, 2 = down, 3 = right;

int rightButtonPin = 32;
bool lastRightButtonState;
bool rightButtonState;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    directionButtonPins[i] = 24 + i;
    pinMode(directionButtonPins[i], INPUT);
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  pinMode(rightButtonPin, INPUT);

  display.display();
  display.clearDisplay();
  helloWorld();
  display.display();
  delay(1000);
  display.clearDisplay();
  // drawCharacter(xPos, yPos);
  // display.display();
}

void helloWorld() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("look steve i did it");
}

// void drawCharacter() {
//   display.fillCircle(3, 3, 3, SSD1306_WHITE);
// }

void drawCharacter(int xPos, int yPos, int radius) {
  display.clearDisplay();
  display.fillCircle(xPos, yPos, radius, SSD1306_WHITE);
  display.display();
}

void loop() {
  if (millis() > lastFrame + frameRate) {
    display.clearDisplay();
    lastFrame = millis();
    // Serial.println(lastFrame);
    moveRight();
    moveLeft();
    moveUp();
    moveDown();
    drawCharacter(xPos, yPos, radius);
    display.display();
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

void moveRight() {
  directionButtonStates[3] = digitalRead(directionButtonPins[3]);
  if (directionButtonStates[3] && xPos <= SCREEN_WIDTH - (2 * radius)) {
    xPos += 3;
    // delay(5);
  }
}
void moveLeft() {
  directionButtonStates[0] = digitalRead(directionButtonPins[0]);
  if(directionButtonStates[0] && xPos >= 0 + (2 * radius)) {
    xPos -= 3;
  }
}
// void moveLeft(bool pressed) {
//   display.clearDisplay();
//   if (pressed && xPos >= 0 + radius) {
//     xPos -= 3;
//     drawCharacter(xPos, yPos, radius);
//     display.display();
//   } else {
//     drawCharacter(xPos, yPos, radius);
//     display.display();
//   }
// }

void moveUp() {
  directionButtonStates[1] = digitalRead(directionButtonPins[1]);
  if (directionButtonStates[1] && yPos >= 0 + (2 * radius)) {
    yPos -= 3;
  }
}

void moveDown() {
  directionButtonStates[2] = digitalRead(directionButtonPins[2]);
  if (directionButtonStates[2] && yPos <= SCREEN_HEIGHT - (2 * radius)) {
    yPos += 3;
  }
}