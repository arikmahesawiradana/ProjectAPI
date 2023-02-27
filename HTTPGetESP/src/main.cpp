/*
  Complete project details: https://RandomNerdTutorials.com/esp32-https-requests/
  Based on the BasicHTTPSClient.ino example found at Examples > BasicHttpsClient
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char* ssid = "rumah elite";
const char* password = "senyumdululah";
const char* myServer = "https://myslishlearnenglish.000webhostapp.com/asset/datacut.json";
String payload;
String Result;

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



void loop() {
  Result = downloadData(myServer);
  Serial.print(Result);
}