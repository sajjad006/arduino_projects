#include <Servo.h>

Servo garageDoor;

const int trigPin=8;
const int echoPin=9;

void setup() {
  Serial.begin(9600);
  garageDoor.attach(7);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
    
  garageDoor.write(360);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration=pulseIn(echoPin, HIGH);
  distance=duration * 0.034 / 2;
  Serial.print("Distance = ");
  Serial.println(distance);

  if(distance<=15 && distance!=0){
    garageDoor.write(70);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    delay(3000);
  }else{
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);  
  }
  
  garageDoor.write(360);
}

//void setup(){
//  Serial.begin(9600);
//  pinMode(13, OUTPUT);  
//}
//void loop(){
//  digitalWrite(13, HIGH);
//}
