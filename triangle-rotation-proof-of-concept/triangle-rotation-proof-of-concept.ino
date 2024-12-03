#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <math.h>

#define SCREEN_HEIGHT 320  // OLED display width, in pixels
#define SCREEN_WIDTH 240   // OLED display height, in pixels
#define _USE_MATH_DEFINES

//For Adafruit shield, these are default.
#define TFT_DC 9
#define TFT_CS 10

#define TRIANGLE_HEIGHT 10
#define TRIANGLE_BASE 8

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);




//RIGHT JOYSTICK PINS
int xPin1 = A7;
int yPin1 = A6;

//JOYSTICK INPUTS
int xJoy = 0;
int yJoy = 0;

float xDir = 0;
float yDir = 0;

//center of the triangle on TFT display
int cx = SCREEN_HEIGHT / 2;
int cy = SCREEN_WIDTH / 2;
float theta = 0;
float lastTheta = 0;
int triangleL = 0;
int triangleR = 0;

//triangle vertices
uint16_t vx = 0;
uint16_t vy = 0;

//midpoint vertices
uint16_t mx = 0;
uint16_t my = 0;

//base vertices
uint16_t b1X = 0;
uint16_t b1Y = 0;
uint16_t b2X = 0;
uint16_t b2Y = 0;

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  // tft.setRotation(4);

  tft.println("HELLO WORLD");
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);
  // put your setup code here, to run once:
}

float constrainValues(int pin, float value, float from, float to, float low, float high) {
  value = map(analogRead(pin), from, to, low, high);
  value = constrain(value, low, high);
  // Serial.print("constrained: ");
  // Serial.print(value);
  // Serial.println();
  return value;
}

bool deadZone(float value) {
  // return value > -0.03 && value < -0.02;
  return value > -0.1 && value < 0.1;
}

void loop() {
  // put your main code here, to run repeatedly:

  xDir = constrainValues(xPin1, xJoy, 0, 1023, -100.0, 100) / 100.0;
  yDir = constrainValues(yPin1, yJoy, 0, 1023, -100.0, 100) / 100.0;
  Serial.println(xDir);
  Serial.println(yDir);

  triangleMath();
  tft.fillTriangle(vx, vy, b1X, b1Y, b2X, b2Y, ILI9341_WHITE);
  delay(50);
  tft.fillTriangle(vx, vy, b1X, b1Y, b2X, b2Y, ILI9341_BLACK);

  // tft.fillScreen(ILI9341_BLACK);
}

void triangleMath() {
  if(!deadZone(-xDir) || !deadZone(-yDir)) {
    theta = atan2(-xDir, -yDir);
    // theta = lastTheta;
  }

  //vertex points
  vx = cx + (TRIANGLE_HEIGHT * cos(theta));
  vy = cy + (TRIANGLE_HEIGHT * sin(theta));

  //base vertices
  b1X = cx - (TRIANGLE_HEIGHT / 2) * cos(theta)
        + (TRIANGLE_BASE / 2) * cos(theta + PI / 2);
  b1Y = cy - (TRIANGLE_HEIGHT / 2) * sin(theta)
        + (TRIANGLE_BASE / 2) * sin(theta + PI / 2);
  b2X = cx - (TRIANGLE_HEIGHT / 2) * cos(theta)
        - (TRIANGLE_BASE / 2) * cos(theta + PI / 2);
  b2Y = cy - (TRIANGLE_HEIGHT / 2) * sin(theta)
        - (TRIANGLE_BASE / 2) * sin(theta + PI / 2);
}