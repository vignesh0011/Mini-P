#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h> 
String apiKey = "3G5JATLCJ7EIK6N5"; // Enter your Write API key from ThingSpeak
const char *ssid = "vignesh";     // replace with your wifi ssid and wpa2 key
const char *pass = "vignesh11011";
const char* server = "api.thingspeak.com";
const int sensor_pin = A0;  // Connect Soil moisture analog sensor pin to A0 of NodeMCU 
const int  relay = D4;

WiFiClient client;
 
 
void setup() {
  Serial.begin(9600);
   pinMode(A0,INPUT);
   pinMode(D4,OUTPUT);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    delay(4000);
}
 
void loop() 
{
  int moisture_percentage;
 
  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
 
    Serial.print("Soil Moisture(in Percentage) = ");
    Serial.print(moisture_percentage);
    Serial.println("%");
 
 if( moisture_percentage >90)
 {
   digitalWrite(D4,LOW);
   delay(5000);
  
 }
 else
 {
    digitalWrite(D4,HIGH);
   delay(5000);
 }
 
    if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(moisture_percentage);
    postStr += "r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.println("Data Send to Thingspeak");
  }
    client.stop();
    Serial.println("Waiting...");
    delay(2000);      // thingspeak needs minimum 15 sec delay between updates.
}