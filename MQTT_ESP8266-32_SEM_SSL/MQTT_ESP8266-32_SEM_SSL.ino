#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>//https://github.com/tzapu/WiFiManager
#include "secrets.h"


void setup() {
  Serial.begin(SERIAL);

  checkWifi();
  client.setCallback(RecebePayload);
}



void loop()
{
  
  if (WiFi.status() == WL_CONNECTED) // Averigua Se tem Wi-fi Caso Tenha de um Check no broker mqtt caso não de um Check no Wi-Fi
  {
    if (checkMqttConnection())
    {
      client.loop(); // LOOp De conexão com o Broker
    }
  }
  else
  {
    checkWifi();
  }
  
  client.loop(); // Confirmar LOOp De Conexão com o Broker
}
