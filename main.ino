#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

SoftwareSerial BTSerial(A4, A5);

#define TRIGGER_PIN A0
#define ECHO_PIN A1

NewPing sonar(TRIGGER_PIN, ECHO_PIN);

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor3.setSpeed(100);
  motor4.setSpeed(100);
}

void forward(){
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
}

void stop(){
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
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
    Serial.print("Obstacle detected at ");
    Serial.print(distance);
    Serial.println("cm!! Stopping...");
  }
}

void loop() {
  if (BTSerial.available()) {
    data = BTSerial.read();
    Serial.println(data);
  }
  if (checkValid(data)){
    if (data == 'F'){
      forward();
      Serial.println("Forward!");
    }
    else if (data == 'S'){
      stop();
      Serial.println("Stopping!");
    }
    else{
      Serial.println("Invalid Command!");
    }
  }
  checkObstacle();
  Serial.flush();
  BTSerial.flush();
  }
