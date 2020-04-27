
//This example shows how to set stream event callback functions.
//Two events are available from Firebase's Real Time database HTTP stream connection, dataAvailable and streamTimeout.

//Required HTTPClientESP32Ex library to be installed  https://github.com/mobizt/HTTPClientESP32Ex

#include <WiFi.h>
#include "FirebaseESP32.h"

#define WIFI_SSID "YOUR_WIFI_AP"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "YOUR_FIREBASE_DATABASE_SECRET"

//Define Firebase Data objects
FirebaseData firebaseData1;
FirebaseData firebaseData2;

unsigned long sendDataPrevMillis1;
unsigned long sendDataPrevMillis2;

uint16_t count1;
uint16_t count2;

String json = "";

String path = "/ESP32_Test";

void streamCallback1(StreamData data)
{

  Serial.println("> Stream Data1 available...");
  Serial.println("> STREAM PATH: " + data.streamPath());
  Serial.println("> PATH: " + data.dataPath());
  Serial.println("> TYPE: " + data.dataType());
  Serial.print("> VALUE: ");
  if (data.dataType() == "int")
    Serial.println(data.intData());
  else if (data.dataType() == "float")
    Serial.println(data.floatData());
  else if (data.dataType() == "string")
    Serial.println(data.stringData());
  else if (data.dataType() == "json")
    Serial.println(data.jsonData());
  Serial.println();
}

void streamTimeoutCallback1(bool timeout)
{
  if (timeout)
  {
    Serial.println();
    Serial.println("> Stream 1 timeout, resume streaming...");
    Serial.println();
  }
}

void streamCallback2(StreamData data)
{

  Serial.println(">> Stream Data2 available...");
  Serial.println(">> STREAM PATH: " + data.streamPath());
  Serial.println(">> PATH: " + data.dataPath());
  Serial.println(">> TYPE: " + data.dataType());
  Serial.print(">> VALUE: ");
  if (data.dataType() == "int")
    Serial.println(data.intData());
  else if (data.dataType() == "float")
    Serial.println(data.floatData());
  else if (data.dataType() == "string")
    Serial.println(data.stringData());
  else if (data.dataType() == "json")
    Serial.println(data.jsonData());
  Serial.println();
}

void streamTimeoutCallback2(bool timeout)
{
  if (timeout)
  {
    Serial.println();
    Serial.println(">> Stream 2 timeout, resume streaming...");
    Serial.println();
  }
}

void setup()
{

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  Serial.println("------------------------------------");
  Serial.println("> Begin stream 1...");

  if (!Firebase.beginStream(firebaseData1, path + "/Stream/data1"))
  {
    Serial.println("> FAILED");
    Serial.println("> REASON: " + firebaseData1.errorReason());
    Serial.println();
  }
  else
  {
    Serial.println("> PASSED");
    Serial.println("------------------------------------");
    Serial.println();
  }

  Firebase.setStreamCallback(firebaseData1, streamCallback1, streamTimeoutCallback1);

  //When stream timeou callback function is omitted, use Firebase.setStreamCallback(firebaseData1, streamCallback1);

  Serial.println("------------------------------------");
  Serial.println(">> Begin stream 2...");

  if (!Firebase.beginStream(firebaseData2, path + "/Stream/data2"))
  {
    Serial.println(">> FAILED");
    Serial.println(">> REASON: " + firebaseData2.errorReason());
    Serial.println();
  }
  else
  {
    Serial.println(">> PASSED");
    Serial.println("------------------------------------");
    Serial.println();
  }

  Firebase.setStreamCallback(firebaseData2, streamCallback2, streamTimeoutCallback2);

  //When stream timeou callback function is omitted, use Firebase.setStreamCallback(firebaseData2, streamCallback2);
}

void loop()
{

  if (millis() - sendDataPrevMillis1 > 28000)
  {
    sendDataPrevMillis1 = millis();
    count1++;

    json = "{\"data1-1\":" + String(count1) + ",\"data1-2\":" + String(count1 + 1) + ",\"data1-3\":" + String(count1 + 2) + "}";

    Serial.println("------------------------------------");
    Serial.println("> Update Data 1...");
    if (Firebase.updateNode(firebaseData1, path + "/Stream/data1", json))
    {
      Serial.println("> PASSED");
      Serial.println("> PATH: " + firebaseData1.dataPath());
      Serial.println("> TYPE: " + firebaseData1.dataType());
      Serial.print("> VALUE: ");
      if (firebaseData1.dataType() == "int")
        Serial.println(firebaseData1.intData());
      else if (firebaseData1.dataType() == "float")
        Serial.println(firebaseData1.floatData());
      else if (firebaseData1.dataType() == "string")
        Serial.println(firebaseData1.stringData());
      else if (firebaseData1.dataType() == "json")
        Serial.println(firebaseData1.jsonData());
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("> FAILED");
      Serial.println("> REASON: " + firebaseData1.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
  }

  if (millis() - sendDataPrevMillis2 > 20000)
  {
    sendDataPrevMillis2 = millis();
    count2++;

    json = "{\"data2-1\":" + String(count2) + ",\"data2-2\":" + String(count2 + 1) + ",\"data2-3\":" + String(count2 + 2) + "}";

    Serial.println("------------------------------------");
    Serial.println(">> Update Data 2...");

    if (Firebase.updateNode(firebaseData2, path + "/Stream/data2", json))
    {
      Serial.println(">> PASSED");
      Serial.println(">> PATH: " + firebaseData2.dataPath());
      Serial.println(">> TYPE: " + firebaseData2.dataType());
      Serial.print(">> VALUE: ");
      if (firebaseData2.dataType() == "int")
        Serial.println(firebaseData2.intData());
      else if (firebaseData2.dataType() == "float")
        Serial.println(firebaseData2.floatData());
      else if (firebaseData2.dataType() == "string")
        Serial.println(firebaseData2.stringData());
      else if (firebaseData2.dataType() == "json")
        Serial.println(firebaseData2.jsonData());
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println(">> FAILED");
      Serial.println(">> REASON: " + firebaseData2.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
  }
}
