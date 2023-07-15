#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change the address (0x27) to match your LCD module

const int button1Pin = 6;  // Connect button 1 to digital pin 6
const int button2Pin = 7;  // Connect button 2 to digital pin 7

const int servo1Pin = 8;  // Connect servo 1 to digital pin 8
const int servo2Pin = 9;  // Connect servo 2 to digital pin 9

const int ultrasonic1TrigPin = 10;  // Connect ultrasonic 1 trigger pin to digital pin 10
const int ultrasonic1EchoPin = 11;  // Connect ultrasonic 1 echo pin to digital pin 11

const int ultrasonic2TrigPin = 12;  // Connect ultrasonic 2 trigger pin to digital pin 12
const int ultrasonic2EchoPin = 13;  // Connect ultrasonic 2 echo pin to digital pin 13

Servo servo1;
Servo servo2;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);  // Initialize the LCD
  lcd.print("Press a button");  // Initial text on LCD
  delay(5000);

  pinMode(button1Pin, INPUT_PULLUP);  // Set button 1 pin as input with internal pull-up resistor
  pinMode(button2Pin, INPUT_PULLUP);  // Set button 2 pin as input with internal pull-up resistor

  pinMode(ultrasonic1TrigPin, OUTPUT);  // Set ultrasonic 1 trigger pin as output
  pinMode(ultrasonic1EchoPin, INPUT);   // Set ultrasonic 1 echo pin as input

  pinMode(ultrasonic2TrigPin, OUTPUT);  // Set ultrasonic 2 trigger pin as output
  pinMode(ultrasonic2EchoPin, INPUT);   // Set ultrasonic 2 echo pin as input

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
}

void loop() {
  if (digitalRead(button1Pin) == LOW) {  // Button 1 is pressed
    float distance1 = measureDistance(ultrasonic1TrigPin, ultrasonic1EchoPin);
    if (distance1 > 5) {
      servo1.write(180);  // Rotate servo 1 to 180 degrees
      delay(2000);
      servo1.write(0);  // Return servo 1 to 0 degrees
    }
    displayResult(distance1, "compartment 1");
    delay(2000);
  }

  if (digitalRead(button2Pin) == LOW) {  // Button 2 is pressed
    float distance2 = measureDistance(ultrasonic2TrigPin, ultrasonic2EchoPin);
    if (distance2 > 5) {
      servo2.write(180);  // Rotate servo 2 to 180 degrees
      delay(2000);
      servo2.write(0);  // Return servo 2 to 0 degrees
    }
    displayResult(distance2, "compartment 2");
    delay(2000);
  }
}

void displayResult(float distance, const char* compartment) {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (distance > 5) {
    lcd.print("Welcome!");
  } else {
    lcd.print(compartment);
    lcd.print(" full :(");
  }
}

float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long pulseDuration = pulseIn(echoPin, HIGH);
  float distance = pulseDuration * 0.034 / 2;

  return distance;
}