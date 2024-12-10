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
#define TRIANGLE_BASE 10

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

// Constants
const float maxRotationSpeed = 0.25;  // Maximum speed of rotation
const float maxAcceleration = 0.15;    // Acceleration toward target speed
const float dampingFactor = 0.95;    // Damping factor to slow down the speed gradually

// Variables for angular momentum
float angularSpeed = 0;      // Current angular speed
float angularSpeed2B = 0;       // Desired speed based on joystick input
float theta = -PI / 2;        // Initial angle (pointing up)
float angularAcceleration = 0;      // Variable for acceleration

unsigned long lastTime = 0;  // For time-based updates

//triangle vertices
uint16_t vx = 0;
uint16_t vy = 0;

//base vertices
uint16_t b1X = 0;
uint16_t b1Y = 0;
uint16_t b2X = 0;
uint16_t b2Y = 0;

int frameRate = 30;
unsigned long lastFrame = 0;

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
  if (millis() > lastFrame + frameRate) {
    unsigned long currentFrame = millis();
    // float deltaTime = (currentFrame - lastFrame) / 1000.0; // Convert ms to seconds
    lastFrame = currentFrame;
    tft.fillTriangle(vx, vy, b1X, b1Y, b2X, b2Y, ILI9341_BLACK);
    checkDirection();
    moveDirection();
    triangleMath();
    tft.fillTriangle(vx, vy, b1X, b1Y, b2X, b2Y, ILI9341_WHITE);
  }
  Serial.println(xDir);
  Serial.println(yDir);

  // tft.fillScreen(ILI9341_BLACK);
}

// Function to check joystick direction and set target speed
void checkDirection() {
  if (!deadZone(xDir) || !deadZone(yDir)) {
    // Calculate target angle from joystick direction
    float targetAngle = atan2(-xDir, -yDir);
    
    // Calculate the angular difference between current angle and target angle
    float angleDiff = targetAngle - theta;
    
    // Normalize the angle difference to the range [-PI, PI]
    if (angleDiff > PI) angleDiff -= 2 * PI;
    if (angleDiff < -PI) angleDiff += 2 * PI;

    // Set the target speed to the angular difference, but limit it to maxSpeed
    angularSpeed2B = constrain(angleDiff, -maxRotationSpeed, maxRotationSpeed);
  } else {
    // If joystick is in the dead zone, stop rotation smoothly
    angularSpeed2B = 0;
  }
}

// Function to apply angular momentum and update current angle smoothly
void moveDirection() {
  float angleDiff = angularSpeed2B - angularSpeed;

  angularAcceleration = maxAcceleration * (abs(angleDiff) / PI);
  if(angularAcceleration < 0.001) angularAcceleration = 0;

  // Gradually ease the current speed toward the target speed to smooth out the rotation
  if (angularSpeed < angularSpeed2B) {
    angularSpeed += angularAcceleration;
    if (angularSpeed > angularSpeed2B) angularSpeed = angularSpeed2B;
  } else if (angularSpeed > angularSpeed2B) {
    angularSpeed -= angularAcceleration;
    if (angularSpeed < angularSpeed2B) angularSpeed = angularSpeed2B;
  }

  // Apply damping when joystick is in the dead zone
  if (deadZone(xDir) && deadZone(yDir)) {
    angularSpeed *= dampingFactor;
    if (abs(angularSpeed) < 0.001) angularSpeed = 0; // Stop if speed is negligible
  }

  // Update the current angle based on the current speed
  theta += angularSpeed;

  // Ensure the angle stays within the range [0, 2*PI]
  if (theta > 2 * PI) theta -= 2 * PI;
  if (theta < 0) theta += 2 * PI;
}


void triangleMath() {

  // if (!deadZone(-xDir) || !deadZone(-yDir)) {
  //   theta = atan2(-xDir, -yDir);
  //   // theta = lastTheta;
  // }
  // float deltaTime = (millis() - lastTime / 1000.0)

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