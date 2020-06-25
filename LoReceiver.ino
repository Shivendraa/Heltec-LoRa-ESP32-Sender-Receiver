//Code by: Shivendra K.
//File Description: This is the receiver file which will receive request from sender node and will perform input output opeations such as turning on/off LED or Relay.

#include "heltec.h" 
#include "images.h"

#define BAND    433E6 

//PIN1-PIN5 are assigned with GPIO(General urpose input output) pins of Heltec LoRa ESP-32(V2)
#define PIN1 22
#define PIN2 17
#define PIN3 25
#define PIN4 32
#define PIN5 33

String packSize = "--";
String packet ;
String Str;

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}
void process(String packet)
{
  if (packet == "LON") {
      digitalWrite(PIN1,HIGH);
    }
  if (packet == "LOF") {
      digitalWrite(PIN1,LOW);
    }
  if (packet == "PON") {
      digitalWrite(PIN2,HIGH);
    }
  if (packet == "POF") {
      digitalWrite(PIN2,LOW);
    } 
  if (packet == "AON") {
      digitalWrite(PIN3,HIGH);
    }
  if (packet == "AOF") {
      digitalWrite(PIN3,LOW);
    }
    if (packet == "BON") {
      digitalWrite(PIN4,HIGH);
    }
  if (packet == "BOF") {
      digitalWrite(PIN4,LOW);
    }
    if (packet == "CON") {
      digitalWrite(PIN5,HIGH);
    }
  if (packet == "COF") {
      digitalWrite(PIN5,LOW);
    }
}
void LoRaData(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  process(packet);
  packet="";
//  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
  
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++)
  { packet += (char) LoRa.read(); }
  LoRaData();
}

void setup() { 
    pinMode(PIN1, OUTPUT);
    digitalWrite(PIN1,LOW);
    pinMode(PIN2, OUTPUT);
    digitalWrite(PIN2,LOW);
    pinMode(PIN3, OUTPUT);
    digitalWrite(PIN3,LOW);
    pinMode(PIN4, OUTPUT);
    digitalWrite(PIN4,LOW);
    pinMode(PIN5, OUTPUT);
    digitalWrite(PIN5,LOW);
    
  Heltec.begin(true, true, true, true , BAND);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1000);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  LoRa.receive();
}
void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  } 
}
