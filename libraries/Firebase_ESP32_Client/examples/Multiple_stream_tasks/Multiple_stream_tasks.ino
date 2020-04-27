
//This example shows how to read stream on other task function.

//Required HTTPClientESP32Ex library to be installed  https://github.com/mobizt/HTTPClientESP32Ex

#include <WiFi.h>
#include "FirebaseESP32.h"

#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "YOUR_FIREBASE_DATABASE_SECRET"
#define WIFI_SSID "YOUR_WIFI_AP"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#if CONFIG_FREERTOS_UNICORE
#define CPU_CORE 0
#else
#define CPU_CORE 1
#endif

#define STACK_SIZE_IN_WORD 16384

unsigned long sendPrevMillis = 0;
uint32_t sendCount = 0;

//Define Firebase Data objects
FirebaseData firebaseData1;
FirebaseData firebaseData2;

String path1 = "/ESP32_Test/Data1";
String path2 = "/ESP32_Test/Data2";

void runTask();
double randomDouble(double minf, double maxf);

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
  if (!Firebase.beginStream(firebaseData1, path1))
  {
    Serial.println("> FAILED");
    Serial.println("> REASON: " + firebaseData1.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("> PASSED");
    Serial.println("------------------------------------");
    Serial.println();
  }

  Serial.println("------------------------------------");
  Serial.println(">> Begin stream 2...");
  if (!Firebase.beginStream(firebaseData2, path2))
  {
    Serial.println(">> FAILED");
    Serial.println(">> REASON: " + firebaseData2.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println(">> PASSED");
    Serial.println("------------------------------------");
    Serial.println();
  }

  runTask();
}

void loop()
{

  if (millis() - sendPrevMillis > 30000)
  {
    sendPrevMillis = millis();

    for (int i = 0; i < 3; i++)
    {

      String jsonString1 = "{\"ID\":\"Node 1\", \"Count\":" + String(sendCount + 1) + ", \"Temp\":" + String(randomDouble(25.0, 40.0)) + "}";
      String jsonString2 = "{\"ID\":\"Node 2\", \"Count\":" + String(sendCount + 1) + ", \"Temp\":" + String(randomDouble(25.0, 40.0)) + "}";

      sendCount++;

      Serial.println("------------------------------------");
      Serial.println("> Set Data 1...");

      if (Firebase.setJSON(firebaseData1, path1, jsonString1))
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

      Serial.println("------------------------------------");
      Serial.println(">> Set Data 2...");

      if (Firebase.setJSON(firebaseData2, path2, jsonString2))
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
        Serial.println(">>REASON: " + firebaseData2.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
      }


    }
  }
}

void runTask()
{

  xTaskCreatePinnedToCore([](void *param) {
    for (;;)
    {

      if (!Firebase.readStream(firebaseData1))
      {
        Serial.println("------------------------------------");
        Serial.println("> Read stream 1...");
        Serial.println("> FAILED");
        Serial.println("> REASON: " + firebaseData1.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
      }
      if (firebaseData1.streamTimeout())
        Serial.println("> Stream timeout, resume streaming...");

      if (firebaseData1.streamAvailable())
      {
        Serial.println("> Stream Data1 available...");
        Serial.println("> STREAM PATH: " + firebaseData1.streamPath());
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
        Serial.println();
      }
    }

    vTaskDelete(NULL);
  },
  "streamTask1", STACK_SIZE_IN_WORD, NULL, 3, NULL, CPU_CORE);

  xTaskCreatePinnedToCore([](void *param) {
    for (;;)
    {

      if (!Firebase.readStream(firebaseData2))
      {
        Serial.println("------------------------------------");
        Serial.println(">> Read stream 2...");
        Serial.println(">> FAILED");
        Serial.println(">> REASON: " + firebaseData2.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
      }

      if (firebaseData2.streamTimeout())
        Serial.println(">> Stream timeout, resume streaming...");

      if (firebaseData2.streamAvailable())
      {
        Serial.println(">> Stream Data2 available...");
        Serial.println(">> STREAM PATH: " + firebaseData2.streamPath());
        Serial.println(">> PATH: " + firebaseData2.dataPath());
        Serial.println(">> TYPE: " + firebaseData2.dataType());
        Serial.print(">> VALUE: ");
        if (firebaseData2.dataType() == "int")
          Serial.println(firebaseData2.intData());
        else if (firebaseData2.dataType() == "flaot")
          Serial.println(firebaseData2.floatData());
        else if (firebaseData2.dataType() == "string")
          Serial.println(firebaseData2.stringData());
        else if (firebaseData2.dataType() == "json")
          Serial.println(firebaseData2.jsonData());
        Serial.println();
      }
    }
    vTaskDelete(NULL);
  },
  "streamTask2", STACK_SIZE_IN_WORD, NULL, 3, NULL, CPU_CORE);
}

double randomDouble(double minf, double maxf)
{
  return minf + random(1UL << 31) * (maxf - minf) / (1UL << 31);
}
