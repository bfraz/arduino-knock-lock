#include <Servo.h>

Servo myServo;

const int piezoPin = A0;
const int servoPin = 9;
const int lockedLEDPin = 5;
const int unlockedLEDPin = 4;
const int validKnockLEDPin = 3; 
const int switchPin = 2;
const int loudKnockThreshold = 1000;
const int softKnockThreshold = 100;
const int whiteNoiseThreshold = 25;
const int numberOfKnocksThreshold = 3;
boolean isUnlocked = true;
int knockVal = 0;
int switchVal = 0;
int numberOfKnocks = 0;

void setup(){
  myServo.attach(9);
  Serial.begin(9600);
  pinMode(piezoPin, INPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(lockedLEDPin, OUTPUT); 
  pinMode(unlockedLEDPin, OUTPUT);
  pinMode(validKnockLEDPin, OUTPUT);
  pinMode(switchPin, INPUT);
  digitalWrite(servoPin, LOW);
  myServo.attach(9);
  unlockBox();
}

void lockBox(){
  digitalWrite(lockedLEDPin, HIGH);
  digitalWrite(validKnockLEDPin, LOW);
  digitalWrite(unlockedLEDPin, LOW);
  myServo.write(90);
  isUnlocked = false;
  Serial.println("Box locked");
  delay(500);
  Serial.println("Ready for password");
}

void unlockBox(){
  digitalWrite(lockedLEDPin, LOW);
  digitalWrite(validKnockLEDPin, LOW);
  digitalWrite(unlockedLEDPin, HIGH);
  myServo.write(0);
  numberOfKnocks = 0;
  isUnlocked = true;
  Serial.println("Box unlocked");
  delay(500);
  Serial.println("Ready to lock");
}

void checkIfValidKnock(){
  
  if(knockVal >= softKnockThreshold && knockVal <= loudKnockThreshold){
    numberOfKnocks++;
    Serial.print("Valid knock: ");
    Serial.println(knockVal);    
  } 
  else{
    Serial.print("NOT Valid knock: ");
    Serial.println(knockVal);
  }
  Serial.print(numberOfKnocksThreshold-numberOfKnocks);
  Serial.println(" knocks left");
}

boolean canUnlock(){
  boolean unlock = false;
  if(numberOfKnocks >= numberOfKnocksThreshold && !isUnlocked){
    unlock = true;
  }
  return unlock;
}

void loop(){
  if(isUnlocked){
    switchVal = digitalRead(switchPin);
    if(switchVal == HIGH){
      lockBox();
    }
  }
  else{
    knockVal = analogRead(piezoPin);
    if(numberOfKnocks < numberOfKnocksThreshold && knockVal > whiteNoiseThreshold){
      checkIfValidKnock();
    }
    if(canUnlock()){
      unlockBox();
    }
  }
}
