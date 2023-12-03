#include <AFMotor.h>
#include <Servo.h>


AF_DCMotor motor1(1); // top right motor
AF_DCMotor motor2(2); // top left motor
AF_DCMotor motor3(3); // bottom right motor
AF_DCMotor motor4(4); // bottom left motor

const int servoPin = 9;    // Pin for the servo motor
const int triggerPin = A0;   // Trigger pin for the ultrasonic sensor
const int echoPin = A1;      // Echo pin for the ultrasonic sensor
const int detectionDistance = 30;  // Distance threshold for object detection in centimeters
const int yellowPin = A3; // Pin for yellow LED
const int redPin = A4;    // Pin for red LED
const int greenPin = A5;   // Pin for green LED
const int degreeTurn = 3;  // degree for each turn of servo motor
const int buzzerPin = A2;    // Pin for the buzzer
const int frequency = 300;   // frequency of buzzer

int loopDelay = 500;
char data;  // variable for incoming bluetooth data
int oneFullTurnTime = 4000;

const int cluster = 8;
Servo myServo;


void setup(){
  Serial.begin(9600);

  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor3.setSpeed(100);
  motor4.setSpeed(100);

  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(servoPin);
  myServo.write(90);
}

void ledState(char color){

  if (color == 'R'){
    digitalWrite(A3, LOW);
    digitalWrite(A4, HIGH); 
    digitalWrite(A5, LOW); 
  }

  else if (color == 'G'){
    digitalWrite(A3, LOW);
    digitalWrite(A4, LOW);
    digitalWrite(A5, HIGH); 
  }

  else if (color == 'Y'){
    digitalWrite(A3, HIGH); 
    digitalWrite(A4, LOW); 
    digitalWrite(A5, LOW); 

  }
  else{

  }
}

void forward(){
    myServo.write(90);
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
  ledState('Y');

  motor1.setSpeed(180);
  motor2.setSpeed(80);
  motor3.setSpeed(180);
  motor4.setSpeed(80);

  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}
void right(){
  ledState('Y');

  motor1.setSpeed(80);
  motor2.setSpeed(180);
  motor3.setSpeed(80);
  motor4.setSpeed(180);

  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void back(){
    ledState('R');

    motor1.setSpeed(80);
    motor2.setSpeed(80);
    motor3.setSpeed(80);
    motor4.setSpeed(80);

    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
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

bool primaryCheck() {
  long distance = calculateDistance();
  if (distance < detectionDistance){
    long distance1 = calculateDistance();
    delay(100);
    long distance2 = calculateDistance();
    delay(100);
    long avgDistance = (distance1 + distance2) / 2;
    if (avgDistance < detectionDistance){
      return true;
    }
    else{
      return false;
    }
  }
}

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




long calculateDistance(){
  long duration, distance;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Convert duration to distance in cm
  return distance;

}


void playBuzzer(int duration, int frequency,long start){
  if (millis()-start  <= duration){
    tone(buzzerPin, frequency);
  }
  else{
    noTone(buzzerPin);
  }
}





void scan(long* distanceArr){
    for (int angle = 0; angle <= 180; angle += degreeTurn) {
      myServo.write(angle);
      long distance = calculateDistance();
      distanceArr[angle/degreeTurn] = distance;
      delay(50);
    }
}

int analyse(){
  int arraySize = (180 / degreeTurn) + 1;
  long distanceArr[arraySize];
  long averageArr[arraySize];
  scan(distanceArr);
  for(int i = 0;i <= (arraySize-1);i++){
    int sum = 0;
    for(int j = 1;j <= (arraySize-1);j++){
      if((j-i) == cluster){
        break;
      }
      sum += distanceArr[i];
    }
    int average = sum/cluster;
    averageArr[i] = average;
  }
  int max = averageArr[0];
  int index = 0;
  for(int i = 0; i < (arraySize-1);i++){
    if(averageArr[i] > max){
      max = averageArr[i];
      index = i;
    }
  }
  return index;
}

char turnLeftRight(int index){
  float tempTime = (index / 120.0) * oneFullTurnTime;
  int degree = 3*index;
  if (degree < 90){
    holdFunction(2,tempTime);
    return 'r';
  }
  else if (degree > 90){
    holdFunction(3,tempTime);
    return 'l';
  }
  else{
    return 'n';
  }
}


void holdFunction(int identifier,int duration){
  long start = millis();
  while (millis()-start < duration){
    if (identifier == 0){
      stop();  
    }
    else if (identifier == 1){
      back();
    }
    else if (identifier == 2){
      right();
    }
    else if (identifier == 3){
      left();
    }
    else{
    }
  }
}


void loop() {
  if (primaryCheck()) {
    //  long start = millis();
    // playBuzzer(1000,frequency,start);
    holdFunction(1,1000);
    int index = analyse();
    turnLeftRight(index);
    } 
  else {
    if (Serial.available()) {
      data = Serial.read();
      delay(loopDelay);
      execute(data);
    }
    execute(data);
  }
}
