// constants won't change. Used here to set a pin number:
const int motorPin1 =  4;
const int motorPin2 =  5;
const int motorPin3 =  6;
const int motorPin4 =  7;

const int buzzerPin1 = 12;
const int buzzerPin2 = 13; 

// HIGH for the motors means they are OFF
int motorState1 = HIGH;
int motorState2 = HIGH;
int motorState3 = HIGH;
int motorState4 = HIGH;

unsigned long previousMillis = 0;
const long interval = 1000;
int counter = 0;

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // just set the buzzers always on
  pinMode(buzzerPin1, OUTPUT);
  pinMode(buzzerPin2, OUTPUT);
  digitalWrite(buzzerPin1, HIGH);
  digitalWrite(buzzerPin2, HIGH);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (counter == 0) {
      if (motorState1 == LOW) {
        motorState1 = HIGH;
      } else {
        motorState1 = LOW;
      }
  
      digitalWrite(motorPin1, motorState1);
    }
    
    if (counter == 1) {
      if (motorState2 == LOW) {
        motorState2 = HIGH;
      } else {
        motorState2 = LOW;
      }
  
      digitalWrite(motorPin2, motorState2);
    }

    if (counter == 2) {
      if (motorState3 == LOW) {
        motorState3 = HIGH;
      } else {
        motorState3 = LOW;
      }
  
      digitalWrite(motorPin3, motorState3);
    }

    if (counter == 3) {
      if (motorState4 == LOW) {
        motorState4 = HIGH;
      } else {
        motorState4 = LOW;
      }
  
      digitalWrite(motorPin4, motorState4);
    }

    counter = counter + 1;
    if(counter == 4)
      counter = 0;
  }
}
