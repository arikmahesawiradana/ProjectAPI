/*
======================= THANK YOU =====================
Thank you for ordering our service. if you really
satisfied with our work let me know in the further.
regards, TRIRAYA.

Our Time:
1. Arik kM. Wiradana
2. Chusnul A. Putri
3. Nurul Q. D. Yulistia
4. Raihan G. Parmanaufal
=======================================================
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"

// the number of the LED pin
const int redPin = 2;  // 16 corresponds to GPIO16
const int greenPin = 19;
const int bluePin = 18;

// setting PWM properties
const int freq = 5000;
const int resolution = 8;

const char* ssid = "rumah elite";
const char* password = "senyumdululah";
const char* myServer = "https://myslishlearnenglish.000webhostapp.com/asset/datacut.json";
String payload;
String Result;
String recentData;

int pos1;
int pos2;
int pos3;
String exap;
String exap2;
String red;
String green;
String blue;

void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  if(!SPIFFS.begin(true)){
    Serial.println("spiff error");
  }
  ledcSetup(0, freq, resolution);
  ledcAttachPin(redPin, 0);
  ledcAttachPin(greenPin, 1);
  ledcAttachPin(bluePin, 2);
}

String downloadData(const char* servername){
  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    client->setInsecure();
    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, servername)) {  // HTTPS
      Serial.print("[HTTPS] GET...\n");
      int httpCode = https.GET();
      if (httpCode > 0) {
       Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          payload = https.getString();
          return payload;
        }
      }
      else {
        payload = "[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str();
        return payload;
      }
      https.end();
    }
  }
  else {
    payload = "[HTTPS] Unable to connect\n";
    return payload;
  }
  Serial.println();
  Serial.println("Waiting 10 secs before the next round...");
  delay(10000);
}

void addNewFile(String contain){
  File newFile = SPIFFS.open("/myAPI.txt", FILE_WRITE);
  if(!newFile){
    Serial.println("failed created file");
  }
  newFile.print(contain);
  newFile.close();
}

String takeFile(){
  File myFile = SPIFFS.open("/myAPI.txt");
  String contains = myFile.readString();
  myFile.close();
  return contains;
}

void LEDON(String gparam)
{
  exap = gparam;
  pos1 = exap.indexOf(',');
  red = exap.substring(0, pos1);
  exap2 = exap.substring(pos1+1, exap.length());
  pos2 = exap2.indexOf(',');
  green = exap2.substring(0, pos2);
  blue = exap2.substring(pos2+1, exap2.length());
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
  ledcWrite(0, red.toInt());
  ledcWrite(1, green.toInt());
  ledcWrite(2, blue.toInt());
}

void playLED(String Time, String Data){
  recentData = takeFile();
  StaticJsonDocument<1536> doc2;
  DeserializationError error2 = deserializeJson(doc2, recentData);
  if(error2){
    Serial.print("error: ");
    Serial.println(error2.f_str());
    return;
  }
  String jikan = doc2[0]["timestamp"];
  if (Time.equals(jikan))
  {
    JsonObject root_1_slide_0 = doc2[1]["slide"][0];
    String delayParam = root_1_slide_0["time"].as<String>();
    for (int i = 800; i < 820; i++)
    {
      String keys = "led" + String(i);
      const char* ledParam = root_1_slide_0[keys];
      LEDON(ledParam);
      delay(delayParam.toInt());
    }
  }
  else
  {
    addNewFile(Data);
    recentData = takeFile();
    StaticJsonDocument<1536> doc2;
    DeserializationError error2 = deserializeJson(doc2, recentData);
    if(error2){
      Serial.print("error: ");
      Serial.println(error2.f_str());
      return;
    }
    String jikan = doc2[0]["timestamp"];
    if (Time.equals(jikan))
    {
      JsonObject root_1_slide_0 = doc2[1]["slide"][0];
      const char* delayParam = root_1_slide_0["time"];
      for (int i = 800; i < 820; i++)
      {
        String keys = "led" + String(i);
        const char* ledParam = root_1_slide_0[keys];
        Serial.print(ledParam);
      }
    }
  }
}


void loop() {
  Result = downloadData(myServer);
  StaticJsonDocument<1536> doc;
  DeserializationError error = deserializeJson(doc, Result);
  if (error)
  {
    Serial.print("error: ");
    Serial.println(error.f_str());
    return;
  }
  String time = doc[0]["timestamp"].as<String>();
  Serial.print(time);
  File readFile = SPIFFS.open("/myAPI.txt");
  if (readFile.available())
  {
    readFile.close();
    addNewFile(Result);
    playLED(time, Result);
  }
  else
  {
    readFile.close();
    playLED(time, Result);
  }

}