#include <Adafruit_NeoPixel.h>

int xPin = A9;
int yPin = A8;

int xPin1 = A7;
int yPin1 = A6;

String upDown("up/down");
String leftRight("left/right");

int blueValue = 0;
int redValue = 0;
int greenValue = 0;
int brightnessValue = 0;

Adafruit_NeoPixel neopixel(1, 33, NEO_RGB);

void setup() {
  Serial.begin(9600);
  neopixel.begin();
  neopixel.clear();
  neopixel.show();
}

void loop() {
  blueValue = constrain(map(analogRead(xPin), 150, 950, 0, 255), 0, 255);
  double blueValueCopy = constrain(map(analogRead(xPin), 150.0, 950, 0, 255), 0, 255.0);
  redValue = constrain(map(analogRead(yPin), 150, 950, 0, 255), 0, 255);
  greenValue = constrain(map(analogRead(xPin1), 150, 950, 0, 255), 0, 255);
  brightnessValue = constrain(map(analogRead(yPin1), 150, 950, 0, 255), 0, 255);

  neopixel.setPixelColor(0, redValue, greenValue, blueValue);
  neopixel.setBrightness(brightnessValue);
  neopixel.show();
  delay(10);

  Serial.println(upDown);
  Serial.print(" ");
  Serial.print(analogRead(xPin));
  Serial.print(" ");
  Serial.print(blueValue);
  Serial.print(" ");
  Serial.print(analogRead(xPin));

  // Serial.println(leftRight);
  // Serial.print(" ");
  // Serial.print(analogRead(yPin));
  // delay(50);
  // put your main code here, to run repeatedly:
}
