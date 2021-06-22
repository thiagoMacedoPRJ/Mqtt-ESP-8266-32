#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#define LED LED_BUILTIN

bool ws;
bool wu;

//WiFi
const char* SSID = "Dois irmãos";     // SSID / nome da rede WiFi que deseja se conectar
const char* PASSWORD = "gdeimvwg";   // Senha da rede WiFi que deseja se conectar
WiFiClient wifiClient;                        


 
//MQTT Server
const char* BROKER_MQTT = "broker.mqtt-dashboard.com"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883;                      // Porta do Broker MQTT




#define ID_MQTT  "ROUVERmk1"             //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define TOPIC_SUBSCRIBE "Rmk1"   //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
PubSubClient MQTT(wifiClient);        // Instancia o Cliente MQTT passando o objeto espClient


//Declaração das Funções
void mantemConexoes();  //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     //Faz conexão com WiFi
void conectaMQTT();     //Faz conexão com Broker MQTT


void setup() {   
  
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
  MQTT.setCallback(recebePacote); 

}


// Manter a Conexão com a Internet Dentro do loop
void loop() 
{
  mantemConexoes();
  MQTT.loop();
}


// Manter a Conexão com a Internet
void mantemConexoes() {
    if (!MQTT.connected()) 
    {
      conectaMQTT(); 
    }
    
    conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}


void conectaWiFi() 
{  
  if (WiFi.status() == WL_CONNECTED) 
  {
     return;
  }
        
  Serial.print("Conectando-se na rede: ");
  Serial.print(SSID);
  Serial.println(" Aguarde!");

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI  
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(SSID);  
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP()); 
}



void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
            MQTT.subscribe(TOPIC_SUBSCRIBE);
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}



void recebePacote(char* topic, byte* payload, unsigned int length) 
{
    String msg;

    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }

    if (msg == "1")
  {
    if (ws == true)
    {}
    else
    {
      ws = true;
      wu = false;
      digitalWrite(LED, HIGH);
    }
  }
  
  
  if (msg == "0")
  {
    if (wu == true)
    {}
    else
    {
      wu = true;
      ws = false;
      digitalWrite(LED, LOW);
    }
  }
}
