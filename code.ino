

#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#define DHTPIN 14
#define SenMolhado 33
#define DHTTYPE DHT11 
#define VibraPort 32

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "emanuel687";
const char* password = "15142000";
const char* serverName = "https://top-redes-40e9a5ec3e40.herokuapp.com";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
int runs;
float temp, hum, molhado, shaking, period;
String messageToSent(){
  return 
  "{\"humidate\":"+String(hum)+
  ",\"temperature\":"+String(temp)+
  ",\"shaking\":"+String(shaking)+
  ",\"period\":"+String(period)+
  ",\"captures\":"+String(runs)+
  ",\"initialTime\":"+String(lastTime)+
  ",\"molhado\":"+String(molhado)+
  "}";
}
void sendMessage(){
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");
      String data = messageToSent();
      int httpResponseCode = http.POST(data);
      Serial.println(data);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
}


void setup() {
  dht.begin();
  Serial.begin(115200);
  pinMode(SenMolhado, INPUT);
  pinMode(VibraPort, INPUT);
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

void loop() {
  delay(10);
  runs+=1;
  shaking += digitalRead(VibraPort);
  period = millis() - lastTime;
  if (period > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      hum = dht.readHumidity();
      temp = dht.readTemperature();
      molhado = digitalRead(SenMolhado);
      if (isnan(hum) || isnan(temp)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      sendMessage();
      runs=0;
      shaking = 0;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
