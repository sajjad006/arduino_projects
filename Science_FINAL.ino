#include <Servo.h>
int s1=15;
int s2=14;
int s3=2;
int s4=3;
int s5=4;
int s6=12;
int s7=13;
int s8=16;
int m1=8;
int m2=9;
int m3=11;
int m4=10;
int trigPin=6;
int echoPin=7;
int se=5;
Servo servo1;


void setup(){
  pinMode(s1,INPUT);
  pinMode(s2,INPUT);
  pinMode(s3,INPUT);
  pinMode(s4,INPUT);
  pinMode(s5,INPUT);
  pinMode(s6,INPUT);
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  servo1.attach(se);
  Serial.begin(9600);
}

void loop(){
line(); 
 long x=dist();
 if(x<=7){
    analogWrite(m1, LOW);
    analogWrite(m2, LOW);
    analogWrite(m3, LOW);
    analogWrite(m4, LOW);
    ser();
 }
    Serial.print(digitalRead(s1));
    Serial.print(digitalRead(s2));
    Serial.print(digitalRead(s3));
    Serial.print(digitalRead(s4));
    Serial.print(digitalRead(s5));
    Serial.print(digitalRead(s6));
    Serial.print(digitalRead(s7));
    Serial.print(digitalRead(s8));
    Serial.println();
}

long dist(){
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration=pulseIn(echoPin, HIGH);
  distance=duration * 0.034 / 2;
//  Serial.print("Distance = ");
//  Serial.println(distance);
  delay(50);
  return distance;
}

void ser(){
  servo1.write(150);
  delay(1600);
  servo1.write(0);
  delay(1600);
}

void line(){
  if(digitalRead(s4)==0 || digitalRead(s5)==0){
    analogWrite(m1,160);
    analogWrite(m2,0);
    analogWrite(m3,160);
    analogWrite(m4,0);  
  }

  else if(digitalRead(s1)==0 || digitalRead(s2)==0 || digitalRead(s3)==0){
    analogWrite(m1,160); 
    analogWrite(m2,0);    
    analogWrite(m3,0);
    analogWrite(m4,0);
  }

  else if(digitalRead(s6)==0 || digitalRead(s7)==0 || digitalRead(s8)==0){
    analogWrite(m1,0);
    analogWrite(m2,0);    
    analogWrite(m3,160);
    analogWrite(m4,0);
  }

  else if(digitalRead(s1)==0 && digitalRead(s2)==0 && digitalRead(s3)==0 && digitalRead(s4)==0 && digitalRead(s5)==0 && digitalRead(s6)==0 && digitalRead(s7)==0 && digitalRead(s8)==0)
 {
    analogWrite(m1,0);
    analogWrite(m2,0);    
    analogWrite(m3,0);
    analogWrite(m4,0);
   
 }

 else if(digitalRead(s1)==1 && digitalRead(s2)==1 && digitalRead(s3)==1 && digitalRead(s4)==1 && digitalRead(s5)==1 && digitalRead(s6)==1 && digitalRead(s7)==1 && digitalRead(s8)==1){
    analogWrite(m1,0);
    analogWrite(m2,0);    
    analogWrite(m3,0);
    analogWrite(m4,0);
   
 }
 
}
