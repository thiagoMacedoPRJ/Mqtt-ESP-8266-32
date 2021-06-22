#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <time.h>
#include "secrets.h"

BearSSL::WiFiClientSecure net;
PubSubClient client(net);

// Isto Server para Definir a porta do server web para 80!!
WiFiServer server(80); // Segurança SSL

String header; // Armazenarar os Headers(Dispositivos) das solicitaçoes HTTPS

time_t now;
unsigned long lastMillis = 0;


//Conectar Ao Broker
void mqtt_connect()
{
  while (!client.connected()) {
    
    Serial.print(ctime(&now));
    
    if (client.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) 
    {
      Serial.println("Conectado(a) Com Sucesso ao Broker!!");
      
      client.subscribe(TOPIC_SUB);
    } 
    
    else 
    {
      Serial.print("A Tentativa falhou, Status do Erro == ");
      
      Serial.print(client.state());
      
      Serial.println(" . Será feita uma nova tentativa em 5 segundos.");
      
      Serial.print("");
      
      Serial.println("Tentando Conectar Novamente ao Broker MQTT: Aguarde ...");
            
      delay(5000);
    }
  }
}


// CONFIGURAÇÃO INICIAL !
void setup() 
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  WiFi.hostname(HOSTNAME);
  WiFi.mode(WIFI_STA);

  WiFiManager wifiManager;
  wifiManager.autoConnect("Biocyber.Tech");
  Serial.println("Conectado(a) à rede com sucesso!");
  Serial.println("---*-------------------------*---");
  Serial.println("Conectando-se ao MQTT Broker: Aguarde ...");
  server.begin();   // Inicializar o Servidor



  Serial.print("Configurando a Data/Hora usando SNTP..");
  configTime(-5 * 3600, 0, "br.pool.ntp.org", "time.br.gov");
  now = time(nullptr);
   
  while (now < 1510592825) 
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
   
    
    Serial.println(" Configurado!");
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    
    Serial.print("Data/Horas: ");


      
    /* DEFINIÇÃO DO CERTIFICADO */
      #ifdef CHECK_CA_ROOT
        BearSSL::X509List cert(digicert);
        net.setTrustAnchors(&cert);
      #endif
      
      #if (!defined(CHECK_CA_ROOT))
        net.setInsecure();
      #endif
    
    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setCallback(RecebePayload);
    mqtt_connect();
}



void loop()
{
  // Esta função server para pegar as horas!
  now = time(nullptr);

  // Essa função serve para Estabilizar a Comunicação Entre o Broker!!
  if (!client.connected())
  {
    mqtt_connect();
  }
  else
  {
    client.loop();
  }
}

// RECEBER MENSAGENS!!
void RecebePayload(char* topic, byte* payload, unsigned int length) 
{
  String msg;

  // Exibir payload recebido!
  Serial.println("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  
  //converter o byte em string do payload recebido!
  for(int i = 0; i < length; i++) 
  {
    char c = (char)payload[i];
    msg += c;
  }

  if (msg == "0")
  {
    digitalWrite(LED, HIGH); 
  }

  if (msg == "1")
  {
    digitalWrite(LED, LOW);
  }
 
}
