#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks 
void Relay1On();
void Relay1Off();


// Change this before you flash
const char* ssid = "your_ssid";
const char* password = "your_pass";
const char* RelayName = "upstairs fire starter";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *relay1 = NULL;


void setup()
{
 pinMode (D1, OUTPUT);
 digitalWrite (D1,LOW);
 Serial.begin(9600);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switch
    // Format: Alexa invocation name, local port no, on callback, off callback
    relay1 = new Switch(RelayName, 80, Relay1On, Relay1Off);


    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*relay1);

  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      relay1->serverLoop();

	 }
}

void Relay1On() {
    Serial.print("Switch Relay 1 turn on ...");
    digitalWrite (D1,HIGH); 
}

void Relay1Off() {
    Serial.print("Switch Relay 1 turn off ...");
    digitalWrite (D1,LOW); 
}



// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
