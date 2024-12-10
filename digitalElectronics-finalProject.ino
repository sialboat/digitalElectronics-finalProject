//This document contains the code for my F24 Digital Electronics Final Project.

/*
 * This code is all of the functionality for simulating an isosceles triangle with angular velocity and momentum-based movement.
 * It stores the positional and angular information of said isosceles triangle for conversion to CV, effectively making this
 * a video game-to-CV device.
 * 
 * @author Silas Wang
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <math.h>
#include <Encoder.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MCP4728.h>

// DEFAULTS
//For Adafruit shield, these are default.
#define TFT_DC 9
#define TFT_CS 10

//SCREEN DIMENSIONS
#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 240

#define _USE_MATH_DEFINES

//TRIANGLE DIMENSIONS
#define TRIANGLE_HEIGHT 10
#define TRIANGLE_BASE 10

//OBJECTS
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC); //creates the display object allowing us to draw things on the display
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(4, 33, NEO_RGB);  //unused neopixel variable that I did not have time to implement.
Adafruit_MCP4728 mcp;

//LEFT JOYSTICK PINS
int xPin = A8;
int yPin = A9;

//RIGHT JOYSTICK PINS
int xPin1 = A7;
int yPin1 = A6;

//LEFT JOYSTICK VALUES
//upDown == 1, leftRight == 0
int directions[2] = { 0 };

//RIGHT JOYSTICK VALUES
int xJoy = 0;
int yJoy = 0;

//TRIANGLE VALUES:
//center vertex; used to calculate the other parts of the triangle and for positioanl movement.
int cx = SCREEN_HEIGHT / 2;
int cy = SCREEN_WIDTH / 2;

//triangle vertex values (X & Y) - pointy part of the triangle
uint16_t vx = 0;
uint16_t vy = 0;

//base vertices values (X & Y) - not the point part of the triangle.
uint16_t b1x = 0;
uint16_t b1y = 0;
uint16_t b2x = 0;
uint16_t b2y = 0;

//POSITIONAL MOVEMENT
float xSpeed = 0.0;    //current speed
float xSpeed2B = 0.0;  //speed to be
float ySpeed = 0.0;
float ySpeed2B = 0.0;
float maxSpeed = 4.5;
float acceleration = 0.15;
bool xMoving = false;
bool yMoving = false;

//ANGULAR MOVEMENT:
const float maxRotationSpeed = 0.25;  // max. rotation speed
const float maxAcceleration = 0.15;   // rotation acceleration
const float dampingFactor = 0.95;     // Damping factor to slow down the speed gradually

float angularSpeed = 0;    // current angular speed
float angularSpeed2B = 0;  // angular speed to be
float theta = -PI / 2;     // Initial angle (pointing up)
float angularAcceleration = 0;

//values for triangle calculations
float xDir = 0;
float yDir = 0;

//default frame rate of the program
int frameRate = 30;
unsigned long lastFrame = 0;

//Output values for CV and CC:
float cxOut;
float cyOut;
float xDirOut;
float yDirOut;

float cxCCOut;
float cyCCOut;
float xDirCCOut;
float yDirCCOut;


//unimplemented color changing values
int colorButtonPin = 29;
int color = 65535;
int colorMode = 0;

/**
 * This function contains code that runs once, namely the startup text code and the proper rotation
 * for orientating the ILI9341 display.
 */
void setup() {
  Serial.begin(9600);
  if(!tft.begin()) {
    Serial.println("Failed to find ILI9341 display");
    while(1) {
      delay(10);
    }
  }

  // Try to initialize the DAC
  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip");
    while (1) {
      delay(10);
    }
  }

  //for interfacing with the DAC
  mcp.setChannelValue(MCP4728_CHANNEL_A, 4095);
  // mcp.setChannelValue(MCP4728_CHANNEL_B, 2048);
  // mcp.setChannelValue(MCP4728_CHANNEL_C, 1024);
  // mcp.setChannelValue(MCP4728_CHANNEL_D, 0);

  pinMode(colorButtonPin, INPUT);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setRotation(4);
  tft.println("GAME-BASED CV");
  delay(2000);
  tft.setTextSize(2);
  tft.println("silas digital project");
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);
}

/**
 * This function runs indefinitely until the program is shut down. This function contains code for 
 * constantly updating values and redrawing the character based on the given values.
 */
void loop() {
  directions[0] = constrainValues(xPin, directions[0], 50, 920, 0, 100);  //x-Axis values
  directions[1] = constrainValues(yPin, directions[1], 50, 920, 0, 100);  //y-axis values
  xDir = constrainValues(xPin1, xJoy, 0, 1023, -100.0, 100) / 100.0;      //x-axis directional values
  yDir = constrainValues(yPin1, yJoy, 0, 1023, -100.0, 100) / 100.0;      //y-axis directional values

  //temporary code for constraining values to be sent out as CC / CV.
  cxOut = constrain(map(cx,0, 240, 0, 4095),0, 4095);
  cyOut = constrain(map(cy, 0, 320, 0, 4095), 0, 4095);
  xDirOut = constrain(map(xDir, -0.9, 0.77, 0, 4095), 0, 4095);
  yDirOut = constrain(map(yDir, -0.78, 0.84, 0, 4095), 0, 4095);

  cxCCOut = constrain(map(cx, 0, 240, 0, 127), 0, 127);
  cyCCOut = constrain(map(cy, 0, 320, 0, 127), 0, 127);
  xDirCCOut = constrain(map(xDir, -0.9, 0.77, 0, 127), 0, 127);
  yDirCCOut = constrain(map(yDir, -0.78, 0.84, 0, 127), 0, 127);
  float thetaCCOut = constrain(map(theta, 0, 2 * PI, 0, 127), 0, 127);

  if(!deadZone(directions[0])) {
    usbMIDI.sendControlChange(1, cxCCOut, 1);
  }
  if(!deadZone(directions[1])) {
    usbMIDI.sendControlChange(2, cyCCOut, 1);
  }
  if(!angularDeadZone(xDir) || !angularDeadZone(yDir)) {
    usbMIDI.sendControlChange(3, thetaCCOut, 1);
  }

  mcp.setChannelValue(MCP4728_CHANNEL_A, cxOut);
  if (millis() > lastFrame + frameRate) {
    lastFrame = millis();
    clearCharacter(vx, vy, b1x, b1y, b2x, b2y);
    checkY(); 
    moveY();
    checkX();
    moveX();
    checkDirection();
    pointDirection();
    triangleMath();
    drawCharacter(vx, vy, b1x, b1y, b2x, b2y);
  }
}

/**
 * Function that determines the direction of the right joystick and setting angularSpeed2B to 
 * this given value using some math. This is the function that determines where the triangle
 * is pointing towards.
 */
void checkDirection() {
  if (!angularDeadZone(xDir) || !angularDeadZone(yDir)) {
    //calculate target angle from joystick direction
    float targetAngle = atan2(-xDir, -yDir);

    //calculate angular difference between current angle and target angle
    float angleDiff = targetAngle - theta;

    //constrain the angle difference to [-PI, PI]
    if (angleDiff > PI) angleDiff -= 2 * PI;
    if (angleDiff < -PI) angleDiff += 2 * PI;

    //set target speed to angular difference, limit it to max speed
    angularSpeed2B = constrain(angleDiff, -maxRotationSpeed, maxRotationSpeed);
  } else {
    //deadzone; stop rotation smoothly.
    angularSpeed2B = 0;
  }
}

/**
 * Function that physically points the triangle character to the direction that the right joystick
 * is pointing to, whilst considering angular momentum. This is the function that actually does
 * the movement.
 */
void pointDirection() {
  float angleDiff = angularSpeed2B - angularSpeed;

  angularAcceleration = maxAcceleration * (abs(angleDiff) / PI);
  if (acceleration < 0.001) acceleration = 0;

  // set angularSpeed to the respective value based on the direction that the potentiometer is pointing to.
  if (angularSpeed < angularSpeed2B) {
    angularSpeed += angularAcceleration;
    if (angularSpeed > angularSpeed2B) angularSpeed = angularSpeed2B;
  } else if (angularSpeed > angularSpeed2B) {
    angularSpeed -= angularAcceleration;
    if (angularSpeed < angularSpeed2B) angularSpeed = angularSpeed2B;
  }

  // gradually decrease if in the dead zone.
  if (angularDeadZone(xDir) && angularDeadZone(yDir)) {
    angularSpeed *= dampingFactor;
    if (abs(angularSpeed) < 0.001) angularSpeed = 0;
  }

  //actually change triangle's angle
  theta += angularSpeed;

  // restricts range of values to [0, 2*PI]
  if (theta > 2 * PI) theta -= 2 * PI;
  if (theta < 0) theta += 2 * PI;
}

/**
 * Function that contains the trigonometry required to create an isosceles triangle. I still don't quite
 * understand how this works I sorta just looked at the math nodded my head like I knew what was going on.
 * When I get around to it I will probably try my hand at understanding how this works.
 */
void triangleMath() {
  //vertex points
  vx = cx + (TRIANGLE_HEIGHT * cos(theta));
  vy = cy + (TRIANGLE_HEIGHT * sin(theta));

  //base vertices
  b1x = cx - (TRIANGLE_HEIGHT / 2) * cos(theta)
        + (TRIANGLE_BASE / 2) * cos(theta + PI / 2);
  b1y = cy - (TRIANGLE_HEIGHT / 2) * sin(theta)
        + (TRIANGLE_BASE / 2) * sin(theta + PI / 2);
  b2x = cx - (TRIANGLE_HEIGHT / 2) * cos(theta)
        - (TRIANGLE_BASE / 2) * cos(theta + PI / 2);
  b2y = cy - (TRIANGLE_HEIGHT / 2) * sin(theta)
        - (TRIANGLE_BASE / 2) * sin(theta + PI / 2);
}

/**
 * Helper function that combines the map() and constrain() function into one, guaranteeing that a value from
 * the PIN potentiometer's VALUE is constrained from the values "LOW" to the values "HIGH."
 * @param pin the potentiometer's pin value
 * @param value the variable storing the potentiometer's value
 * @param from the lower bound of value's current range
 * @param to the upper bound of value's upper range
 * @param low the lower bound of the value's target range
 * @param high the upper bound of the value's target range
 * @return a constrained float value between the values LOW and HIGH (which is prolly an int because the method
 * returns a long by default lmao)
 */
float constrainValues(int pin, float value, float from, float to, float low, float high) {
  value = map(analogRead(pin), from, to, low, high);
  value = constrain(value, low, high);
  return value;
}

/**
 * Helper function that determines the vertical direction that which the triangle character should be moving
 * whilst accounting for momentum-based acceleration.
 */
void checkY() {
  if (!deadZone(directions[1]) && directions[1] < 52) {
    ySpeed2B = maxSpeed;
    yMoving = true;
  } else if (!deadZone(directions[1]) && directions[1] > 52) {
    ySpeed2B = -maxSpeed;
    yMoving = true;
  } else {
    ySpeed2B = 0.0;
    yMoving = false;
  }
}

/**
 * Helper function that actually moves the triangle character with respect to the Y-Axis, accounting for
 * momentum-based acceleration.
 */
void moveY() {
  if (!yMoving && abs(ySpeed) <= acceleration) {
    ySpeed = 0;
  } else if (ySpeed < ySpeed2B) {  //We move down
    ySpeed += acceleration;
  } else if (ySpeed > ySpeed2B) {  //We move up
    ySpeed -= acceleration;
  }
  cy += ySpeed;
}

/**
 * Helper function that determines the horizontal direction that which the triangle character should be moving
 * whilst accounting for momentum-based acceleration.
 */
void checkX() {
  if (!deadZone(directions[0]) && directions[0] > 52) {  //we move left
    xSpeed2B = -maxSpeed;
    xMoving = true;
  } else if (!deadZone(directions[0]) && directions[0] < 52) {  //we move right
    xSpeed2B = maxSpeed;
    xMoving = true;
  } else {
    xSpeed2B = 0.0;
    xMoving = false;
  }
}

/**
 * Helper function that actually moves the triangle character with respect to the X-Axis, accounting for
 * momentum-based acceleration.
 */
void moveX() {
  if (!xMoving && abs(xSpeed) < acceleration) xSpeed = 0;
  else if (xSpeed < xSpeed2B) xSpeed += acceleration;  //we move right
  else if (xSpeed > xSpeed2B) xSpeed -= acceleration;  //we move left
  cx += xSpeed;
}

/**
 * Helper function that returns TRUE or FALSE whether or not the right joystick is in the "dead zone,"
 * the resting spot / value of the potentiometer when nothing is touching it.
 * @param value the value of the potentiometer (restrained to values between -1 and 1)
 * @return true if the given value is in the dead zone, false if otherwise.
 */
bool angularDeadZone(float value) {
  return value > -0.1 && value < 0.1;
}

/**
 * Helper function that returns TRUE or FALSE whether or not the left joystick is in the "dead zone,"
 * the resting spot / value of the potentiometer when nothing is touching it.
 * @param value the value of the potentiometer (restrained to values between -1 and 1)
 * @return true if the given value is in the dead zone, false if otherwise.
 */
bool deadZone(int value) {
  return value >= 51 && value <= 54;
}

/**
 * Function that draws a character by simply calling the fillTriangle method from Adafruit's Native GFX Library.
 * Consult the Adafruit GFX Library for more information on fillTriangle();
 */
void drawCharacter(uint16_t vx, uint16_t vy, uint16_t b1x, uint16_t b1y, uint16_t b2x, uint16_t b2y) {
  tft.fillTriangle(vx, vy, b1x, b1y, b2x, b2y, ILI9341_WHITE);
}

/**
 * Function that removes a character by simply calling the fillTriangle method from Adafruit's Native GFX Library.
 * The main difference between this function and the drawCharacter function is that this one sets the triangle's color
 * as black, the default display background color (effectively giving the illusion that the character has been cleared).
 * Consult the Adafruit GFX Library for more information regarding fillTriangle();
 */
void clearCharacter(uint16_t vx, uint16_t vy, uint16_t b1x, uint16_t b1y, uint16_t b2x, uint16_t b2y) {
  tft.fillTriangle(vx, vy, b1x, b1y, b2x, b2y, ILI9341_BLACK);
}