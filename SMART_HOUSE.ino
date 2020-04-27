#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>

#define FIREBASE_HOST "android-login-system.firebaseio.com"
#define FIREBASE_AUTH "zkFkV0bY6gdWMt9J7K76QXvc6M3uW6bYZWmykf6U"

#define WIFI_SSID "Zayed"
#define WIFI_PASSWORD "shaaz502"

Servo doorServo, fanServo;
int led=D7, motionSensor = D6, ac=D8;

void setup() {
  Serial.begin(115200);
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("connecting");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.println("connected: ");
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  doorServo.attach(D1);
  fanServo.attach(D0);

  doorServo.write(100);
  fanServo.write(0);
  
  pinMode(led, OUTPUT);
  pinMode(ac, OUTPUT);
  pinMode(motionSensor, INPUT);
}

void loop() {  
  String lightStatus  = Firebase.getString("light");
  String doorStatus   = Firebase.getString("door");
  String fanStatus    = Firebase.getString("fan");
  String acStatus     = Firebase.getString("ac");
  int motion = digitalRead(motionSensor);
  
  Firebase.setInt("motion", motion);
    
  Serial.println("Door = "+doorStatus+" Light = "+lightStatus+" Fan = "+fanStatus+" motion = "+motion);
  
  if(lightStatus=="1")
    digitalWrite(led, HIGH);
  else if(lightStatus == "0")
    digitalWrite(led, LOW);

  if(doorStatus=="1")
    doorServo.write(180);
  else if(doorStatus=="0")
    doorServo.write(100);

  if(fanStatus=="1")
    turnFanOn();
  else if(fanStatus=="0")
    fanServo.write(0);

  if(acStatus=="1")
    digitalWrite(ac, HIGH);
  else if(acStatus=="0")
    digitalWrite(ac, LOW);
}

void turnFanOn(){
    for (int i = 0; i <= 180; i++) { 
      fanServo.write(i);
      delay(5);
    }
    
    for (int i = 180; i >= 0; i--) { 
      fanServo.write(i);                                     
      delay(5);
    }
}
