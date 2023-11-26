#include <AFMotor.h>
#include <NewPing.h>
#include <NewTone.h>

#define TRIGGER_PIN A0
#define ECHO_PIN A1

#define TONE_PIN A3

int loopDelay = 500;



NewPing sonar(TRIGGER_PIN, ECHO_PIN);

AF_DCMotor motor1(1); // top right motor
AF_DCMotor motor2(2); // top left motor
AF_DCMotor motor3(3); // bottom right motor
AF_DCMotor motor4(4); // bottom left motor

void setup() {
  Serial.begin(9600);
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor3.setSpeed(100);
  motor4.setSpeed(100);
  pinMode(A4, OUTPUT); // red
  pinMode(A5, OUTPUT); // green
}

void forward(){
    ledState('G');
    motor1.setSpeed(130);
    motor2.setSpeed(130);
    motor3.setSpeed(130);
    motor4.setSpeed(130);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
}

void stop(){
    ledState('R');
    motor1.setSpeed(0);
    motor2.setSpeed(0);
    motor3.setSpeed(0);
    motor4.setSpeed(0);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
}

void left(){
  motor1.setSpeed(180);
  motor2.setSpeed(0);
  motor3.setSpeed(180);
  motor4.setSpeed(0);
  motor1.run(FORWARD);
  motor2.run(RELEASE);
  motor3.run(FORWARD);
  motor4.run(RELEASE);
}

void right(){
  motor1.setSpeed(0);
  motor2.setSpeed(180);
  motor3.setSpeed(0);
  motor4.setSpeed(180);
  motor1.run(RELEASE);
  motor2.run(FORWARD);
  motor3.run(RELEASE);
  motor4.run(FORWARD);
}

void back(){
    motor1.setSpeed(80);
    motor2.setSpeed(80);
    motor3.setSpeed(80);
    motor4.setSpeed(80);
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
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
  char valid[] = {'F','f','S','s','L','l','R','r','B','b'};
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

void execute(char temp){
  if (checkValid(temp)){
    char lower_command = tolower(temp);
    if (lower_command == 'f'){ forward(); }
    else if (lower_command == 's'){ stop(); }
    else if (lower_command == 'l'){ left(); }
    else if (lower_command == 'r'){ right(); }
    else if (lower_command == 'b'){ back(); }
    else{ stop(); }
  }
  else{
    stop();
  }
  primaryCheck();
}

void loop() {
  if (primaryCheck()) {
    stop();
  } 
  else {
    if (Serial.available()) {
      data = Serial.read();
      delay(loopDelay);
      execute(data);
    }
  }
}

bool primaryCheck() {
  unsigned int distance = sonar.ping_cm();
  return (distance > 0 && distance < 20);
}



