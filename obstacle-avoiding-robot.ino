#include <AFMotor.h>
#include <Servo.h> 

#define echoPin A0
#define pingPin A1
#define speedVar 200     

//Motors
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

//An array of pointers to the motors
AF_DCMotor* motors[4];

Servo servo;

long duration; // duration of signal
int distance = 0, distanceLeft, distanceRight;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(10);  
  servo.write(80);

  //Making pointers to point to the motors
  motors[0] = &motor1;
  motors[1] = &motor2;
  motors[2] = &motor3;
  motors[3] = &motor4;

  //Initializing the motors. Setting speed as 0 and releasing them.
  for (int i = 0; i < 4; i++) {
    motors[i]->setSpeed(0);
    motors[i]->run(RELEASE);
  }

  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Starting the car
  MoveFORWARD();

}

void loop() {
  // put your main code here, to run repeatedly:
  distance = ReadUltraSonicInCm();
  
  if (distance <= 15 && distance != 0) {
    
    Stop();
    MoveBACWARD();
    Stop();
    
    distanceLeft = Look(-80);
    delay(200);
    
    distanceRight = Look(80);

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
  delay(2);
  
  
}

/*
 *Function for making robot go forward 
 */
void MoveFORWARD() {
  for (int i = 0; i < 4; i++){
    motors[i]->run(FORWARD);
  }
  
  for (int i = 0; i < speedVar; i +=2) {
    for (int j = 0; j < 4; j++){
      motors[j]->setSpeed(i);
    }
    delay(2);
  }
  
}

/*
 *Function for making robot go backwards 
 */
void MoveBACWARD() {
  for (int i = 0; i < 4; i++){
    motors[i]->run(BACKWARD);
  }
  
  for (int i = 0; i < speedVar; i +=2) {
    for (int j = 0; j < 4; j++){
      motors[j]->setSpeed(i);
    }
    delay(2);
  }
  delay(400);
  
}

/*
 *Function for making robot stop 
 */
void Stop() {
  for (int i = 0; i < 4; i++){
    motors[i]->run(RELEASE);
  }
  delay(300);
}

/*
 *Function for making robot look somewhere 
 *It takes an angle to rotate its face
 *negative angles mean left, positive angles mean right
 */
int Look(int angle) {
  servo.write(angle + 80);
  delay(1000);
  int lookDistance = ReadUltraSonicInCm();
  delay(100);

  servo.write(80);
  return lookDistance;
}


/*
 *Functions for making robot turn
 */
void TurnRight() {
  for (int i = 0; i < 2; i++){
    motors[i]->run(BACKWARD);
    motors[i+2]->run(FORWARD);
  }
  
  for (int i = 0; i < speedVar; i +=2) {
    for (int j = 0; j < 2; j++){
      motors[j]->setSpeed(i);
      motors[j+2]->setSpeed(i);
    }
    delay(2);
  }
  delay(700);
}

void TurnLeft() {
    for (int i = 0; i < 2; i++){
    motors[i]->run(FORWARD);
    motors[i+2]->run(BACKWARD);
  }
  
  for (int i = 0; i < speedVar; i +=2) {
    for (int j = 0; j < 2; j++){
      motors[j]->setSpeed(i);
      motors[j+2]->setSpeed(i);
    }
    delay(2);
  }
  delay(700 );
}

/*
 * Function to read data from the pins of ultrasonic sensor in centimetre
 */

int ReadUltraSonicInCm(){

  //reading
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 25000);

  //converting to cm
  int DistanceInCm = duration / 29 /2;
  Serial.println(DistanceInCm);
  
  return DistanceInCm;
}
