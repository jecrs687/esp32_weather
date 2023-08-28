/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#define DHTPIN 12
#define SenMolhado 13
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "emanuel687";
const char* password = "15142000";
const char* serverName = "https://top-redes-40e9a5ec3e40.herokuapp.com";


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 1000;

void setup() {
  dht.begin();
  Serial.begin(115200);
  pinMode(SenMolhado, INPUT);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}
int runs;
float temp, hum, molhado;
void loop() {
  delay(500);
  runs+=1;
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  molhado = digitalRead(SenMolhado);
  if (isnan(hum) || isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      runs=0;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");
      String data="{\"humidate\":"+String(hum)+",\"temperature\":"+String(temp)+",\"molhado\":"+String(molhado)+"}";
      int httpResponseCode = http.POST(data);
      Serial.println(data);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
