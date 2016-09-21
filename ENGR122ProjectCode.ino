int thermistorPin = 1;
int half_period = 1000;
float frequency = 500;
int LEDOut = 3;
int speaker = 2;
int button = 4;
int temperature;
int thermRead;
int BOILING_POINT = 60;
boolean buttonPressed = false;
boolean buttonLastPressed = false;
boolean watchState = true;

void setup() {
  Serial.begin(9600);
  pinMode(LEDOut, OUTPUT);
  pinMode(speaker, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  if (watchState == true)
  {
    thermRead = analogRead(thermistorPin);
    temperature = thermToTemp(thermRead);
    drint(temperature);
    if (temperature > BOILING_POINT)
    {
      digitalWrite(LEDOut, HIGH);
      alert();
    }
    standbyLED();
  }
  else
  {
    drint("Not watching");
  }
  checkButtonState();
}

void standbyLED()
{
  if ((millis()/200)%10 == 0)
  {
    digitalWrite(LEDOut, HIGH);
  }
  else

  {
    digitalWrite(LEDOut, LOW);
  }
}

int thermToTemp(int thermRead)
{
  return (thermRead - 515.14) / 0.6781;
}

//mutates watchState
boolean checkButtonState()
{
  buttonLastPressed = buttonPressed;
  if (digitalRead(button) == HIGH)
  {
    buttonPressed = true;
  }
  else
  {
    buttonPressed = false;
  }

  if (buttonLastPressed == false && buttonPressed == true)
  {
    watchState = !watchState;
  }
}

void alert()
{
  for (int i = 2000; i < 5000; i += 2)
  {
    if ( ((i / 100) % 2) != 0)
    {
      speak(freq_to_half_period(i));
    }
  }


  for (int i = 5000; i > 2000; i -= 2)
  {
    if ( ((i / 100) % 2) != 0)
    {
      speak(freq_to_half_period(i));
    }
  }
}

int freq_to_half_period(float freq) {
  half_period = (1 / (freq / 1000000)) / 2;
  return int(half_period);
}


void speak(int pitch) {
  digitalWrite(speaker, HIGH);
  delayMicroseconds(pitch);
  digitalWrite(speaker, LOW);
  delayMicroseconds(pitch);
}

void drint(String message)
{
  //Serial.println(message);
}

void drint(int number)
{
  Serial.println(number);
}

