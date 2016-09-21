//prototyping
double salinityToOutput(double);
double outputToSalinity(double);
double tempToOutput(double);
double outputToTemp(double);
//set up variables
double G = 0.8; //gain
double OF = 0.20; //fraction of overflow water coming directly from DI tank
double m =  65.9; //in grams //pi * .8^2 * depth(2.5) * 2.54^3 * 1g/cm^3 //65.9 given //we calc 82.3704
double FR = 7.89; //(g/s)
int deadtime = 7500; //milliseconds //was 3000
double c1; //current NaCl concentration
double c1Last; //last tested NaCl concentration
double c2; //target NaCl concentration
double csp = 0.0010; //concentration setpoint
double setpoint = salinityToOutput(csp); //setpoint in terms of analogOutput //this and other 3 were ints
double UCL;
double LCL;
double sDev = 3.1772; //standard deviation of 2.1772 
int limitMargin = ceil(3*sDev);
double output; //conductivity output
double x; //mass of correction water
long tlast; //time last valve was opened in milliseconds
int topen; //computed time for valve to stay open in milliseconds
int valveToOpen; //holds value for what valve should be opened
int valveStateNumber; //holds value for what index of the LCD should be changed

int tempTime;
double tsDev = 1;
double tlimitMargin = ceil(3*tsDev); //how much to vary control limits
//double temp;
double tsp = 28;
double t1; //current temperature
double t2; //target temp
double tUCL;
double tLCL;
double tgain = .75;
double tempStop;


void setup() {
//set up pins
pinMode(1, OUTPUT); //LCD screen
pinMode(3, OUTPUT); //Conductivity Sensor
pinMode(4, OUTPUT); //DI solenoid valve
pinMode(5, OUTPUT); //salty solenoid valve
pinMode(6, OUTPUT); //heater

delay(7500);
setControlLimits();

Serial.begin(9600);
Serial.write(12); //the following initializes LCD monitor
Serial.write(132);Serial.write("LCL    SP    UCL");
Serial.write(148);Serial.write("S: ");Serial.print(LCL*100, 3);Serial.write(" ");Serial.print(csp*100, 3);Serial.write(" ");Serial.print(UCL*100, 3);
Serial.write(168);Serial.write("T:  ");Serial.print(tLCL, 1);Serial.write("  ");Serial.print(tsp, 1);Serial.write("  ");Serial.print(tUCL, 1);
Serial.write(188);Serial.write("S=");Serial.write(196);Serial.write("T=");Serial.write(203);Serial.write("H=");
}

void loop() {
  testSalinity(); //gets analog output then converts to salinity
  getTargetConcentration(); //material balance

  heatBalance(); //does all heater stuff
}


//--------------------------------------------------------------

void setControlLimits() { //sets UCL and LCL and converts to wt%
  UCL = outputToSalinity(setpoint+limitMargin);   //setpoint + limitMargin
  LCL = outputToSalinity(setpoint-limitMargin);   //setpoint - limitMargin
  tUCL = outputToTemp(tempToOutput(tsp)+tlimitMargin);
  tLCL = outputToTemp(tempToOutput(tsp)-tlimitMargin);
}

void testSalinity() {
  digitalWrite(3, HIGH); //power on conductivity sensor
  delay(100);
  output = analogRead(0);
  digitalWrite(3, LOW);
  //Serial.println(output);
  delay(1000);
  c1Last = c1;
  c1 = outputToSalinity(output);  //salt fraction
  Serial.write(190);Serial.print(c1*100, 3); //print current salinity
}

void getTargetConcentration() {
  if (abs(c1Last-c1)  < outputToSalinity(sDev*2)) { //makes sure the number isn't ridiculous
    if (c1 > UCL) { //if salinity is above UCL
      c2 = c1 - ((c1 - csp) * G); //calculate target
      getDIAmount(); //go to material balance 
    } else if (c1 < LCL) { //if salinity is below LCL
      c2 = c1 + ((csp - c1) * G); //calculate target
      getSaltyAmount(); //go to material balance
    }
  }
}

void getDIAmount() {
  x = (m*(c1-c2))/((1-OF)*c1); //find DI mass to add
  valveToOpen = 5; //prepare to open DI valve
  
  if (millis() - tlast > deadtime) { //if valve hasn't been opened since deadtime
    triggerValves(); //fix tank salinity
  }  
  
}

void getSaltyAmount() {
  x =  (m*(c2-c1))/((1-OF)*(0.01-c1)); //find salty mass to add
  valveToOpen = 4; //prepare to open salty valve
  
  if (millis() - tlast > deadtime) { //if valve hasn't been opened since deadtime
    triggerValves(); //fix tank salinity
  }  
  
}

void triggerValves() {
  topen = ceil(x / FR * 1000); //x grams * sec/gram * 1000ms/sec, ceil makes it int
  digitalWrite(valveToOpen, HIGH); //turn on the valve for computed time
  delay(topen);
  digitalWrite(valveToOpen, LOW);
  tlast = millis(); //record time we opened valve
}

//--------------------------------------------------------

void heatBalance() {
  delay(200);
  t1 = outputToTemp(analogRead(2)); //returns a temp value
  Serial.write(198);Serial.print(t1, 1);
  if(t1 < tLCL && tempStop < millis()) { //temperature gets too low and current time is past last calculated heat time 
    triggerHeater();
    Serial.write(205);Serial.write("on ");
  } else if (tempStop < millis()) { //temperature is normal and current time is past last calculated heat time
    digitalWrite(6, LOW);
    Serial.write(205);Serial.write("off");
  }
}

void triggerHeater() {
  t2 = t1 + tgain*(tsp-t1);
  //m * cp * delta t / V / I = time
  tempTime = ceil(m*4180*(t2-t1)/8); //8 = watts
  tempStop = millis() + tempTime*1000;
  digitalWrite(6, HIGH);
}

//----------------------------------------------------------

double salinityToOutput(double salinity) {
  return 4781.2*pow(salinity, 0.3319);
}

double outputToSalinity(double output) {
  return 8.197897E-12 * pow(output, 3.01296);
}

double tempToOutput(double temp) {
  return 10.087*temp + 243.69;
}

double outputToTemp(double output) {
  return (output-243.69)/10.087;
}

