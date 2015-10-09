#include <Servo.h>
//constants declared below
int cw = 1300;
int ccw = 1700;
int nw = 1500;
int center_pr = 9;
int left_whisker = 6;
int right_whisker = 7;
int counter = 0;

Servo leftServo;
Servo rightServo;


void setup() {
  leftServo.attach(2);
  rightServo.attach(3);
  
  pinMode(left_whisker, INPUT);
  pinMode(right_whisker, INPUT);
  pinMode(13, OUTPUT);               //13 is the LED
  pinMode(center_pr, INPUT);
  Serial.begin(9600);
}

void loop() {

  stopServos();
  standby();
  
  if (checkWhiskerState(left_whisker)) { //start dancing!
    flashLED(250, 2);
    if (checkWhiskerState(left_whisker)) {
      flashLED(100, 5);
      variableSpeedTest();
    } else {
      forward(1000);
      leftTurn(1000);
      rightTurn(1000);
      backward(1000);        
    }   
  }

if (checkWhiskerState(right_whisker)) {
    flashLED(50, 10);
    forward(500);
    leftTurn(500);
    forward(500);
    leftTurn(500);
    
  }
  
  
}

//----------------------------------------------------------------------------

void variableSpeedTest() {
  for (int i = 0; i < 401; i++) {
    leftServo.writeMicroseconds(ccw - i);
    rightServo.writeMicroseconds(cw + i);
    delay (5);
  }
}

void standby() {
  counter++;
  delay (1);
  if (counter == 2000) {
    flashLED(50, 1);
    counter = 0;
  }
}

boolean checkWhiskerState(int whiskerPin) {
  if (digitalRead(whiskerPin) == HIGH) {
    return 1;
  } else {
    return 0;
  }
}

void forward(int milliseconds) {
  leftServo.writeMicroseconds(ccw);
  rightServo.writeMicroseconds(cw);
  delay (milliseconds);
}

void backward(int milliseconds) {
  leftServo.writeMicroseconds(cw);
  rightServo.writeMicroseconds(ccw);
  delay (milliseconds);
}

void leftTurn(int milliseconds) {
  leftServo.writeMicroseconds(cw);
  rightServo.writeMicroseconds(cw);
  delay (milliseconds);
}


void rightTurn(int milliseconds) {
  leftServo.writeMicroseconds(ccw);
  rightServo.writeMicroseconds(ccw);
  delay (milliseconds);
}

void stopServos(){
  leftServo.writeMicroseconds(nw);
  rightServo.writeMicroseconds(nw);
}

void flashLED(int milliseconds, int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(13, HIGH);
    delay(milliseconds);
    digitalWrite(13, LOW);
    delay(milliseconds);
  }
}

