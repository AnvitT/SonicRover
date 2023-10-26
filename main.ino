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

#define FORWARD_COMMAND 'F'
#define STOP_COMMAND 'S'
#define OBSTACLE_DISTANCE 12
#define MAX_SPEED 100

void setupMotorsSpeed() {
  motor1.setSpeed(MAX_SPEED);
  motor2.setSpeed(MAX_SPEED);
  motor3.setSpeed(MAX_SPEED);
  motor4.setSpeed(MAX_SPEED);
}

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  setupMotorsSpeed();
}

void debounce() {
  delay(10);
}

void forward(){
    if (sonar.ping_cm() < OBSTACLE_DISTANCE)
        stop();
    else {
        motor1.run(FORWARD);
        motor2.run(FORWARD);
        motor3.run(FORWARD);
        motor4.run(FORWARD);
    }
}

void stop(){
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
}

bool checkValid(char command) {
  return command == FORWARD_COMMAND || command == STOP_COMMAND;
}

void loop() {
  char command;
  if (BTSerial.available()) {
    command = BTSerial.read();
    debounce();
    if(checkValid(command)) {
      switch(command) {
        case FORWARD_COMMAND:
            forward();
            Serial.println("Forward!");
            break;
            
        case STOP_COMMAND:
            stop();
            Serial.println("Stopping!");
            break;
        }
    }
    else {
      Serial.println("Invalid Command!");
    }
  }

  while(BTSerial.available() > 0) BTSerial.read(); 
  while(Serial.available() > 0) Serial.read(); 
}
