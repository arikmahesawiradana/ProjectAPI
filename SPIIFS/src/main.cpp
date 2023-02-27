#include <Arduino.h>
#include "SPIFFS.h"

using namespace std;

void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File file = SPIFFS.open("/test.txt", FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  file.print("hello world");
  file.close();

  File readFile = SPIFFS.open("/test.txt");
  Serial.println("File Content:");
  while(readFile.available()){
    Serial.println(readFile.readString());
    delay(1000);
  }
  readFile.close();
}
 
void loop() {

}
