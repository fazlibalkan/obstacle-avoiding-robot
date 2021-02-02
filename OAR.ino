#include <AFMotor.h>
#include <Servo.h>

#define echoPin A0
#define pingPin A1
#define speedVar 100

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

AF_DCMotor* motors[4];

Servo servo;

long duration; // duration of signal
int distance = 0, distanceLeft, distanceRight;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(10);  
  servo.write(70);

  motors[0] = &motor1;
  motors[1] = &motor2;
  motors[2] = &motor3;
  motors[3] = &motor4;

  for (int i = 0; i < 4; i++) {
    motors[i]->setSpeed(0);
    motors[i]->run(RELEASE);
  }
  
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  MoveFORWARD();

}

void loop() {
  // put your main code here, to run repeatedly:
  distance = ReadUltraSonicInCm();
  if (distance <= 20) {
    Stop();
    MoveBACWARD();
    Stop();
    distanceLeft = Look(-70);
    delay(200);
    distanceRight = Look(70);

    if (distanceLeft > distanceRight) {
      TurnLeft();
      Stop();
      MoveFORWARD();
    } else {
      TurnRight();
      Stop();
      MoveFORWARD();
    }
  }
  
  
}

void MoveFORWARD() {
  for (int i = 0; i < 4; i++){
    motors[i]->run(FORWARD);
  }
  
  for (int i = 0; i < speedVar; i++) {
    for (int j = 0; j < 4; j++){
      motors[j]->setSpeed(i);
    }
    delay(5);
  }
  
}

void MoveBACWARD() {
  for (int i = 0; i < 4; i++){
    motors[i]->run(BACKWARD);
  }
  
  for (int i = 0; i < speedVar; i++) {
    for (int j = 0; j < 4; j++){
      motors[j]->setSpeed(i);
    }
    delay(5);
  }
  
}

void Stop() {
  for (int i = 0; i < 4; i++){
    motors[i]->run(RELEASE);
  }
  delay(300);
}

int Look(int angle) {
  servo.write(angle + 90);
  delay(1000);
  int lookDistance = ReadUltraSonicInCm();
  delay(100);

  servo.write(90);
  return lookDistance;
}

void TurnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(500);
}

void TurnLeft() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(500);
}

int ReadUltraSonicInCm(){
  
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 25000);

  int DistanceInCm = duration * 0.034 / 2;
  Serial.println(DistanceInCm);
  
  return DistanceInCm;
}
