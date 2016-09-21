#include <Servo.h>
//constants and variables declared below
int cw = 1300;  //constants for motor control
int ccw = 1700;  
int nw = 1500;
int left_pr = 2;  //pins where the photoresistors are plugged in
int right_pr = 3;
int left_pr_val = 0;  //initializing photoresistor light measurements
int right_pr_val = 0;
int light_threshold = 425; //base average of light and dark, may be changed later
int left_whisker = 6; //pins where the whiskers are plugged in
int right_whisker = 7;
int speaker = 11;  //pin where speaker is plugged in
int LEDPin = 13;  //pin where LED is plugged in
int counter = 0;  //initialize variable to count iterations in cyber challenge, standby code, and elsewhere.
float frequency = 500;  //initialize variable for frequency for speaker
int half_period = 1000;  //initialize variable for half period for speaker
boolean success = 0;  //true-false value that determines whether to stop cyber challenge; may go unused
int crawlMod = 172; //value that adjusts speed for cyber challenge
int gradientThreshold = 48; //value for 2nd attempt at cyber challenge, measure this against difference between photoresistors to see if they're on opposite sides of a line.
int gradient = 0; //current difference between PRs, right minus left

Servo leftServo;
Servo rightServo;


void setup() {  //set what pins do what and where the motors are.
  leftServo.attach(3);
  rightServo.attach(4); 
  pinMode(left_whisker, INPUT);
  pinMode(right_whisker, INPUT);
  pinMode(LEDPin, OUTPUT);               
  pinMode(left_pr, INPUT);
  pinMode(right_pr, INPUT);
  pinMode(speaker, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  standby();  
  if (checkWhiskerState(left_whisker)) {  
    flashLED(250, 2);
    if (checkWhiskerState(left_whisker) && checkWhiskerState(right_whisker)) { //PRESS LEFT HOLD BOTH: BIOMED CHALLENGE
      biomedChallenge();
    } else if (checkWhiskerState(left_whisker)) { //HOLD LEFT WHISKER: CYBER CHALLENGE
      cyberChallenge();
    } else if (checkWhiskerState(right_whisker)) { //PRESS LEFT, HOLD RIGHT: MECHANICAL CHALLENGE
      mechanicalChallenge();      
    } else { //PRESS LEFT WHISKER ONCE: CHEMICAL CHALLENGE
      chemicalChallenge();
    }   
  }

if (checkWhiskerState(right_whisker)) { 
    flashLED(50, 10);
    if (checkWhiskerState(left_whisker) && checkWhiskerState(right_whisker)) { //PRESS RIGHT HOLD BOTH: ELECTRICAL CHALLENGE
      electricalChallenge();
    } else if (checkWhiskerState(right_whisker)) { //HOLD RIGHT WHISKER: CIVIL CHALLENGE
      civilChallenge();
    } else if (checkWhiskerState(left_whisker)) { //PRESS RIGHT HOLD LEFT: SET PHOTORESISTORS
      testPhotos();
    } else { //PRESS RIGHT WHISKER ONCE: NANO CHALLENGE    
      nanoChallenge();
    }
  }
}

//----------------------------------------------------------------------------

void cyberChallenge() {
  flashLED(100, 5);
  
  forward(1000);
  while (success == 0) {
    checkPRs();

    if ((left_pr_val <= light_threshold) && (right_pr_val >= light_threshold)) { //If left sees black, right sees white
      crawl(1);
      counter++;
    } else if ((left_pr_val < light_threshold) && (right_pr_val < light_threshold)) { //if both see black; its too far left
      rightTurn(3);
    } else if ((left_pr_val > light_threshold) && (right_pr_val > light_threshold)) { //if both see white; its too far right OR there is a star
      leftCrawl(1);
      //you know, it could have just reached the end, too.
    } else if ((left_pr_val > light_threshold) && (right_pr_val < light_threshold)) { //if left sees white and right sees black, way too far left 
      rightTurn(5);
    }

    if (counter == 50) { //left and right are pretty aligned
      alignPRs(20);
      counter = 0;
    }
  }
  returnToBase();
}

void cyberChallenge2() { //attempt 2 at cyber challenge
  flashLED(100, 5);

  forward(1000);
  while (success == 0) {
    checkPRs();
    gradient = right_pr_val - left_pr_val;

    if (gradient > gradientThreshold) { //if right is much brighter than left
      crawl(1);
      counter++;
        if (counter == 35) {
          alignPRs(0);
          counter = 0;
        }
    } else if (gradient < (gradientThreshold * -1)) { //if right is much darker than left {
      rightTurn(35);
      counter = 0;
    } else if (gradient <= (gradientThreshold)) { //they're close, so it's either both white or both dark
      counter--; //delays the light adjustment a teeny bit
        if ((left_pr_val > light_threshold) && (right_pr_val > light_threshold)) { //both white, too far right
          leftCrawl(1);
        } else if ((left_pr_val < light_threshold) && (right_pr_val < light_threshold)) {//both black, too far left
          rightTurn(5);
        } else { //this bit will only run if there is a small difference between the photoresistors and the values straddle the light threshold, which is where I decide white turns to black
                 //therefore, both are seeing grey. Best option may be to back up and try again.
          backward(10);
        }        
    }    
  }
}

void testPhotos(){  //returns photoresistor values to serial monitor for debugging and testing ambient light levels for cyber challenge. Also can reset value of light_threshold
  int keepTesting = 1;
      for (int i = 0; i < 3000; i++){
      speak(freq_to_half_period(5000));
      }
      while(keepTesting == 1){ //PRESS LEFT WHISKER TO FINISH
      checkPRs();
      Serial.print("Left PR: ");
      Serial.println(left_pr_val);
      
      Serial.print("Right PR: ");
      Serial.println(right_pr_val);

      if (checkWhiskerState(left_whisker)) {
        keepTesting = 0;
        delay(1000);  //to make sure I don't accidentally start another challenge as the function immediately exits.
        alignPRs(10);
      } else if (checkWhiskerState(right_whisker)) {
        keepTesting = 0; //quits without changing light threshold
        delay(1000); //to make sure a different program doesn't instantly start
      }
      }
}

void chemicalChallenge() {
  flashLED(250, 2);
  
  forward(1000);
  leftSwoop(1800);
  forward(1200);
}

void civilChallenge() {
  flashLED(1, 1000);
  
  forward(1500);
  backward(1600);
}

void nanoChallenge() {
  flashLED(500, 1);
  
  forward(1150); //1000
  rightTurn(180);  //a slight adjustment since the challenge isn't a straight line
  forward(900); //800
  backward(1000);
  leftTurn(180);
  backward(1200); //1000
}

void mechanicalChallenge() {
  flashLED(150, 3);
  
  forward(1800); //1600
  leftSwoop(600);
  forward(1900); //1650
  leftTurn(440); //rounding the final corner
  forward(2100);
  rightSwoop(1950); //snags car and begins return
  forward(1850);
  rightSwoop(750);
  forward(2200); //2000
  rightSwoop(500); //rounding the corner for the home stretch
  forward(2300); //2000
}

void biomedChallenge() {
  flashLED(20, 20);
  
  forward(1800);
  leftSwoop(600);
  forward(1800); //1650
  leftSwoop(1300);
  forward(200);//drops off the duck
  //mirror above, except going the other direction to return to exact location
  backward(780);
  rightTurn(700);
  backward(1900);
  rightTurn(270);
  backward(3000); //makes sure it's back in base
}

void electricalChallenge() {
  flashLED(350, 2);
  
  forward(1850);
  leftSwoop(600);
  forward(1950);
  leftSwoop(780);
  forward(3200);  //drops off the duck
  //mirror above, except going the other direction to return to exact location
  backward(1600);
  leftTurn(60);  //poor alignment requires adjustment
  backward(1600); 
  rightTurn(380);
  backward(1700);
  rightTurn(350);
  backward(2900); //makes sure it's back in base
}

void returnToBase() { //called at the end of cyber challenge to attempt to get it back to base. May go unused.
  forward(4000);
  rightTurn(450);
  forward(2000);
  leftTurn(225);
  forward(3000);
}
//----------------------------------------------------------------------------

void standby() { //flashes LED once every two seconds. This function is called when other code isn't running, and counter increases every time the code loops through.
  stopServos();  //It indicates that the robot is on and ready for input. It also looks cool.
  counter++;
  delay (1);
  if (counter == 2000) {
    flashLED(50, 1);
    counter = 0;
  }
}

boolean checkWhiskerState(int whiskerPin) {  //function that simplifies process for getting whisker input; instead of evaluating an expression that contains a function, I call this function.
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

void crawl(int milliseconds) {  //a slow forward
  leftServo.writeMicroseconds(ccw - crawlMod);
  rightServo.writeMicroseconds(cw + crawlMod);
  delay (milliseconds);
}

void backward(int milliseconds) {
  leftServo.writeMicroseconds(cw);
  rightServo.writeMicroseconds(ccw);
  delay (milliseconds);
}

void leftTurn(int milliseconds) {  //spins
  leftServo.writeMicroseconds(cw);
  rightServo.writeMicroseconds(cw);
  delay (milliseconds);
}


void rightTurn(int milliseconds) {  //spins
  leftServo.writeMicroseconds(ccw);
  rightServo.writeMicroseconds(ccw);
  delay (milliseconds);
}

void leftSwoop(int milliseconds) {  //pivots on left wheel
  leftServo.writeMicroseconds(nw);
  rightServo.writeMicroseconds(cw);
  delay (milliseconds);
}

void leftCrawl(int milliseconds) {  //pivots slowly on left wheel
  leftServo.writeMicroseconds(nw);
  rightServo.writeMicroseconds(cw + crawlMod);
}

void rightSwoop(int milliseconds) {  //pivots on right wheel
  leftServo.writeMicroseconds(ccw);
  rightServo.writeMicroseconds(nw);
  delay (milliseconds);
}

void rightCrawl(int milliseconds) {  //pivots more quickly on right wheel for cyber challenge; helps robot from going too far to the left.
  leftServo.writeMicroseconds(ccw - crawlMod + 33);  //however this function may be unused
  rightServo.writeMicroseconds(nw);
  delay (milliseconds);
}

void stopServos(){
  leftServo.writeMicroseconds(nw);
  rightServo.writeMicroseconds(nw);
}

void flashLED(int milliseconds, int count) {  //flash function that takes a flash duration and flash number. Mostly looks cool; also helps indicate which code is running and 
  for (int i = 0; i < count; i++) {           //helps make sure I don't turn on the wrong code accidentally.
    digitalWrite(LEDPin, HIGH);
    delay(milliseconds);
    digitalWrite(LEDPin, LOW);
    delay(milliseconds);
  }
}

int freq_to_half_period(float freq) {  //converts frequency requested to a half period that the piezo can read; used for speaker (hz to microseconds)
  half_period = (1/(freq / 1000000))/2;
  return int(half_period);
}

void speak(int pitch) {  //give it a pitch (in hz, using above function) and it will play the sound if you loop it.
  digitalWrite(speaker, HIGH);
  delayMicroseconds(pitch);
  digitalWrite(speaker, LOW);
  delayMicroseconds(pitch);
}

void checkPRs() {  //reads the voltage coming in for both photoresistors and stores the value in each respective variable
  left_pr_val = analogRead(left_pr);
  right_pr_val = analogRead(right_pr);
}

void alignPRs(int yellow) { //sets light threshold based on current values
  light_threshold = (right_pr_val + left_pr_val)/2 + yellow;  //resets light_threshold for any given ambient light. May or may not be used. Leans to bright to compensate for yellow stars.
        Serial.print("New threshold is: ");
        Serial.println(light_threshold);
}

void drint(String string) {  //debug function; easier to write than Serial.println and also makes it easier to identify and remove ugly debugging code.
  Serial.println(string);
}


