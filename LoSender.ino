//Code by: Shivendra K.
//File Description: This is the sender file which will take request from user via URL in the browser and send the following request to the receiver node.


//Include all necessary files
#include <WiFi.h>
#include "heltec.h" 
#include "images.h"

//declare these variables as follows 
#define BAND    433E6 
String myresultat;
String  ClientRequest;
WiFiServer server(80);
WiFiClient client;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}
//Below function will read incoming request from client (Via Browser or App)
String ReadIncomingRequest()
{
  while(client.available()) 
  { 
    ClientRequest = (client.readStringUntil('\r'));
    if ((ClientRequest.indexOf("HTTP/1.1")>0)&&(ClientRequest.indexOf("/favicon.ico")<0))
    {
      myresultat = ClientRequest;
    }
  }
  return myresultat;
}

void setup()
{
ClientRequest = "";
Serial.begin(9600);
//pinMode(27, OUTPUT);
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("YouWifiSSID","YourWifiPassword"); //Kindly Update SSID and Password before use
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");
  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));
  server.begin();
  
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);
}


void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
    client = server.available();
    if (!client) { return; }
    while(!client.available()){  delay(1); }
    ClientRequest = (ReadIncomingRequest());
    ClientRequest.remove(0, 5);
    ClientRequest.remove(ClientRequest.length()-9,9);
    
      Heltec.display->drawString(0, 0, "Sending Command: ");
      Heltec.display->drawString(90, 0, ClientRequest);
      Heltec.display->display();

    LoRa.beginPacket();
    LoRa.print(ClientRequest);
    LoRa.endPacket();

    //Below Lines will generate webpage to access operation via browser
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("OK");
    client.println("</html>");
    client.stop();
    delay(1);
    client.flush();

}
