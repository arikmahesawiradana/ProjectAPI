#include <Arduino.h>

int pos1;
int pos2;
int pos3;
String exap;
String exap2;
String red;
String green;
String blue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  exap = "0,0,0";
  pos1 = exap.indexOf(',');
  red = exap.substring(0, pos1);
  exap2 = exap.substring(pos1+1, exap.length());
  pos2 = exap2.indexOf(',');
  green = exap2.substring(0, pos2);
  blue = exap2.substring(pos2+1, exap2.length());
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}