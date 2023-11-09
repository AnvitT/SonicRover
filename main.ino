#include <AFMotor.h>
#include <NewPing.h>
#include <NewTone.h>

#define TRIGGER_PIN A0
#define ECHO_PIN A1

#define TONE_PIN A3

NewPing sonar(TRIGGER_PIN, ECHO_PIN);

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup() {
  Serial.begin(9600);
  motor1.setSpeed(80);
  motor2.setSpeed(80);
  motor3.setSpeed(80);
  motor4.setSpeed(80);
  pinMode(A4, OUTPUT); // red
  pinMode(A5, OUTPUT); // green


}

void forward(){
    ledState('G');
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
}

void stop(){
    ledState('R');
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
}

void ledState(char color){
  if (color == 'R'){
    digitalWrite(A4, HIGH); 
    digitalWrite(A5, LOW); 
  }
  else if (color == 'G'){
    digitalWrite(A5, HIGH); 
    digitalWrite(A4, LOW);  
  }
  else{

  }
}

bool checkValid(char temp){
  char valid[] = {'F','S'};
  bool isValid = false;
  for (int i = 0; i < sizeof(valid);i++){
    if (temp == valid[i]){
      isValid = true;
      break;
    }
  }
  return isValid;
}

char data;

void checkObstacle(){
  unsigned int distance = sonar.ping_cm();
  if (distance < 12 && data == 'F'){
    stop();
    NewTone(TONE_PIN,300);
    delay(100);
    noNewTone(TONE_PIN);
  }
}

void loop() {
  if (Serial.available()) {
    data = Serial.read();
    delay(500);
  }
  if (checkValid(data)){
    if (data == 'F'){
      forward();
    }
    else if (data == 'S'){
      stop();
    }
    else{
    }
  }
  checkObstacle();

  }

